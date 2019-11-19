#ifndef MAIN_H
#define MAIN_H

#include <future>

class Process;

std::future<void> startProcess(Process &process);

#endif // MAIN_H