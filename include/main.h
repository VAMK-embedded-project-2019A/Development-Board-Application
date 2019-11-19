#ifndef MAIN_H
#define MAIN_H

#include "config.h"

#include <string>
#include <future>

class ServerComm;
class WifiScanner;
class Process;

std::future<void> startProcess(Process &process);

#endif // MAIN_H