#include "thread.h"
#include <pthread.h>
#include <limits.h>

void *Thread::runner(void *data){
	Thread *self = (Thread*)data;
	self->run();
	return NULL;
}

Thread::Thread(){}

Thread::~Thread(){}

void Thread::start(){
	pthread_attr_t tattr;
  pthread_attr_init(&tattr);
  size_t size = PTHREAD_STACK_MIN + 50 * 1024;
  pthread_attr_setstacksize(&tattr, size);
  pthread_create(&thread, &tattr, Thread::runner, this);
}

void Thread::join(){
	pthread_join(thread, NULL);
}

Mutex::Mutex(){
	pthread_mutex_init(&this->mutex, NULL);
}

void Mutex::lock(){
	pthread_mutex_lock(&this->mutex);
}

void Mutex::unlock(){
	pthread_mutex_unlock(&this->mutex);
}
Mutex::~Mutex(){
	pthread_mutex_destroy(&this->mutex);
}


Lock::Lock(Mutex &m) : m(m){
	m.lock();
}

Lock::~Lock(){
	m.unlock();
}


ConditionVariable::ConditionVariable() {
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&cond, 0);
}

void ConditionVariable::lock() {
  pthread_mutex_lock(&mutex);
}

void ConditionVariable::unlock() {
  pthread_mutex_unlock(&mutex);
}

void ConditionVariable::wait() {
  pthread_cond_wait(&cond, &mutex);
}

void ConditionVariable::signal() {
  pthread_cond_signal(&cond);
}

void ConditionVariable::broadcast() {
  pthread_cond_broadcast(&cond);
}

~ConditionVariable::ConditionVariable() {
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
}


void Queue::push(const T& val) {
  cond.lock();
  if (q.empty()) {
      /*
       * La cola esta vacia por lo que este push hara que la cola
       * tenga un elemento y por lo tanto deje de estar vacia.
       *
       * Como puede haber hilos esperando a que la cola no este
       * vacia, despertamos a todos ellos enviandoles una señal
       * con el metodo broadcast().
       *
       * Como este hilo aun tiene adquirido el mutex (lock), los
       * otros hilos que se despierten no ejecutaran nada hasta
       * que liberemos el mutex lo que nos garantiza que no
       * habra race conditions.
       * */
      cond.broadcast();
  }

  q.push(val);
  cond.unlock();
}


T Queue::pop() {
  cond.lock();
  while (q.empty()) {
      /* Si la cola esta vacia, no podemos hacer un pop.
       * En vez de retornar con un codigo de error esperamos
       * a que la cola deje de estar vacia con el metodo wait().
       *
       * Literalmente este hilo deja de ejecutarse a la espera de
       * recibir una señal.
       * Señal que deberia llegarnos cuando la cola no este vacia
       * (alguien hizo un push). Sin embargo como pueden haber
       * otros hilos tambien haciendo pop, es posible que para
       * que este hilo en particular se despierte la cola vuelva
       * a estar vacia: por eso tenemos un loop y mientras este
       * vacia seguiremos haciendo waits.
       *
       * Por supuesto, el metodo wait() automaticamente libera
       * el mutex asociado. Cuando este hilo se despierte y se
       * retorne de wait(), el mutex es automaticamente obtenido
       * (lock) de forma transparente al desarrollador.
       * */
      cond.wait();
  }

  const T val = q.front();
  q.pop();

  cond.unlock();
  return val;
}
