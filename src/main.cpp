#include <iostream>
#include <string>
#include <vector>

#include "Erdtree/DaemonController.hpp"

#include "LogAdapter.hpp"

int main(int argc, char* argv[]) {
  initLumosLog();

  if (!Erdtree::DaemonController::instance().initialize("config/config.yaml")) {
    LumosLog::log(LumosLog::LogLevel::FATAL, "Erdtree", "init DaemonController failed.");
    return 1;
  }

  Erdtree::DaemonController::instance().start();

  return 0;
}
