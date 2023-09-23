#include "daemon.hpp"
#include "UI/tui.hpp"
#include <iostream>
#include <fstream>
#include <cstring> // for strcmp
#include <signal.h>

int main(int argc, char* argv[]) {
  const std::string PID_FILE = "daemon.pid";
  if (argc > 1 && strcmp(argv[1], "start") == 0) {
    DaemonStart();
    std::cout << "Daemon started.";
  } else if (argc > 1 && strcmp(argv[1], "stop") == 0) {
    std::ifstream pidFile(PID_FILE);
    if (pidFile.is_open()) {
      pid_t pid;
      pidFile >> pid;
      pidFile.close();

      if (pid > 0) {
        kill(pid, SIGTERM);
        remove(PID_FILE.c_str());
        std::cout << "Daemon stopped." << std::endl;
      } else {
        std::cout << "No running daemon found." << std::endl;
      }
    } else {
      std::cout << "No running daemon found." << std::endl;
      }
  } else if (argc > 1 && strcmp(argv[1], "status") == 0) {
    std::ifstream pidFile(PID_FILE);
    if (pidFile.is_open()) {
      pid_t pid;
      pidFile >> pid;
      pidFile.close();

      if (pid > 0) {
        std::cout << "Daemon is running with PID: " << pid << std::endl;
      } else {
        std::cout << "No running daemon found." << std::endl;
      }
    } else {
      std::cout << "No running daemon found." << std::endl;
    }
  } else if (argc > 1 && strcmp(argv[1], "history") == 0) {
      std::cout << "history \n";
      DisplayHistory();
  } else {
    std::cout << "Enter correct argument \n";
    DisplayHistory();
  }
  return 0;
}
