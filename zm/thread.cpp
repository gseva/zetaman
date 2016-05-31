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

namespace zm {

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

ConditionVariable::~ConditionVariable() {
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
}

} // zm
