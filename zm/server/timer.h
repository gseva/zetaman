#ifndef __TIMER_H__
#define __TIMER_H__
#include <zm/thread.h>
#include "zm/server/physical/physical.h"
//#include "zm/server_proxy.h"

class ServerProxy;

class Timer : public Thread{
public:
  explicit Timer(Physical& physical, ServerProxy& sp);
  virtual ~Timer();
  virtual void run();
private:
  Physical& physical;
  ServerProxy& sp;
};

#endif
