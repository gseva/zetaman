#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
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

#endif
