#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <queue>


class Thread {
public:
	Thread();
	void start();
	void join();
	virtual void run() = 0;
	virtual ~Thread();
private:
	pthread_t thread;
 	static void *runner(void *data);
	Thread(const Thread&);
	Thread& operator=(const Thread&);
};

class Mutex {
public:
	Mutex();
	void lock();
	void unlock();
	~Mutex();
private:
	pthread_mutex_t mutex;
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);
};

class Lock {
public:
	explicit Lock(Mutex &m);
	~Lock();
private:
	Mutex &m;
	Lock(const Lock&);
	Lock& operator=(const Lock&);
};

class ConditionVariable {
    private:
        pthread_mutex_t mutex;
        pthread_cond_t cond;

        ConditionVariable& operator=(const ConditionVariable&);
        ConditionVariable(const ConditionVariable&);
    public:
        ConditionVariable() {
            pthread_mutex_init(&mutex, 0);
            pthread_cond_init(&cond, 0);
        }

        void lock() {
            pthread_mutex_lock(&mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&mutex);
        }

        /*
         * Duerme al hilo actual a la espera de recibir una señal.
         * De forma automatica libera el mutex; cuando se despierte el hilo
         * se captura nuevamente (lock).
         *
         * Este metodo debe llamarse solo si el mutex ya fue adquirido
         * (se debe llamar al metodo lock() antes).
         * */
        void wait() {
            pthread_cond_wait(&cond, &mutex);
        }

        /*
         * Envia una señal y despierta a un unico hilo que este durmiendo
         * en un wait.
         *
         * Si no hay ningun hilo durmiendo, la señal se descarta.
         * */
        void signal() {
            pthread_cond_signal(&cond);
        }

        /*
         * Envia una señal y despierta a todos los hilos que esten durmiendo
         * en un wait. Cabe aclarar que esos hilos intentaran adquirir el
         * mutex (lock) pero solo uno de ellos lo lograra; el resto quedara
         * despierto y bloqueado en la toma del mutex.
         *
         * Si no hay ningun hilo durmiendo, la señal se descarta.
         * */
        void broadcast() {
            pthread_cond_broadcast(&cond);
        }

        ~ConditionVariable() {
            pthread_cond_destroy(&cond);
            pthread_mutex_destroy(&mutex);
        }
};

/*
 * Implementacion de una cola protegida (thread safe).
 * No solo los metodos push y pop usan un mutex para evitar
 * race conditions sino que ademas hacen uso de una condition variable
 * para permitir que el metodo pop se bloquee cuando la cola este vacia
 * y se despierte cuando haya algo que sacar (cola no vacia).
 *
 * Se deja al lector las siguientes posibles mejoras:
 *  - implementar una version RAII del objeto ConditionVariable para ser
 *    usado en los metodos push y pop que automaticamente liberen el mutex.
 *  - implementar una especializacion total Queue<void*> y luego
 *    una especializacion parcial Queue<T*> con el fin de evitar el code
 *    bloat por parte del compilador.
 **/
template<class T>
class Queue {
    private:
        std::queue<T> q;

        const Queue& operator=(const Queue&);
        Queue(const Queue&);

        ConditionVariable cond;
    public:
        Queue() { }

        void push(const T& val) {
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

        T pop() {
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
};

#endif
