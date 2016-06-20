#ifndef __LOG_H__
#define __LOG_H__
#include <fstream>
#include <string>
#include "zm/thread.h"

class Log{
public:
  Log();
  ~Log();
  void eventLog(std::string description);
  void playerLog(int id, std::string action);
  void enemyLog(std::string type, int id, std::string action);
  void bulletLog(std::string type, int id, std::string action);
  void levelLog(std::string description);
private:
  Mutex mutexLogFile;
  std::ofstream logFile;
};
#endif
