#include "daemon.hpp"
#include "JSONmodel/historyJSON.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

#if __APPLE__
  #include "OSSpecific/darwin.hpp"
#endif

bool running = true;
const std::string history_file = "history.txt";
const std::string PID_FILE = "daemon.pid";


void DaemonLoop() {
  HistoryJSON history;
  std::string pcontent = "";
  while (running) {
    std::string content = GetClipboard();
    if (content != pcontent) {
      pcontent = content;
      history.addHistoryEntry(content);
      history.saveHistory();
      }
    sleep(1);
  }
}

void DaemonStart() {
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "Error forking process." << std::endl;
    exit(EXIT_FAILURE);
  } if (pid > 0) {
    // Write child PID to file
    std::ofstream pidFile(PID_FILE);
    if (pidFile.is_open()) {
        pidFile << pid;
        pidFile.close();
    }
    exit(EXIT_SUCCESS);
  }  
  umask(0);
  setsid();

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  DaemonLoop();
}


