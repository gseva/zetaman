#include "log.h"
#include <string>
#include <iostream>
#include <fstream>

Log::Log(){
  logFile.open ("log.txt");
}

Log::~Log(){
    logFile.close();
}

void Log::eventLog(std::string description){
  Lock locker(mutexLogFile);
  logFile << description << "\n";
}

void Log::playerLog(int id, std::string action){
  Lock locker(mutexLogFile);
  logFile << "Jugador " << id << " " << action << "\n";
}

void Log::enemyLog(std::string type, int id, std::string action){
  Lock locker(mutexLogFile);
  logFile << "Enemigo " << type << " " << id << " " << action << "\n";
}

void Log::bulletLog(std::string type, int id, std::string action){
  Lock locker(mutexLogFile);
  logFile << "Bala " << type << " " << id << " " << action << "\n";
}

void Log::levelLog(std::string description){
  Lock locker(mutexLogFile);
  logFile << "Level " << description << "\n";
}
