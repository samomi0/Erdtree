#include <iostream>
#include <unistd.h>
#include "LogAdapter.hpp"

int main(int argc, char* argv[]) {
  pid_t pid = getpid();

  initLumosLog();
  
  LumosLog::log(LumosLog::LogLevel::INFO, "DummyFunc", "{} >>> MODULE PROCESS", pid);
}