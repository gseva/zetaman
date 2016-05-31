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

namespace zm {


class ConditionVariable {
    private:
        pthread_mutex_t mutex;
        pthread_cond_t cond;

        ConditionVariable& operator=(const ConditionVariable&);
        ConditionVariable(const ConditionVariable&);

    public:
        ConditionVariable();

        void lock();
        void unlock();

        /*
         * Duerme al hilo actual a la espera de recibir una señal.
         * De forma automatica libera el mutex; cuando se despierte el hilo
         * se captura nuevamente (lock).
         *
         * Este metodo debe llamarse solo si el mutex ya fue adquirido
         * (se debe llamar al metodo lock() antes).
         * */
        void wait();

        /*
         * Envia una señal y despierta a un unico hilo que este durmiendo
         * en un wait.
         *
         * Si no hay ningun hilo durmiendo, la señal se descarta.
         * */
        void signal();

        /*
         * Envia una señal y despierta a todos los hilos que esten durmiendo
         * en un wait. Cabe aclarar que esos hilos intentaran adquirir el
         * mutex (lock) pero solo uno de ellos lo lograra; el resto quedara
         * despierto y bloqueado en la toma del mutex.
         *
         * Si no hay ningun hilo durmiendo, la señal se descarta.
         * */
        void broadcast();

        ~ConditionVariable();
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

        void push(const T& val);

        T pop();
};

} // zm

#endif
