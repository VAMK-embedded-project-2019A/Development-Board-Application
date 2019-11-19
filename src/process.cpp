#include "process.h"

bool Process::isDone()
{
	std::unique_lock<std::mutex> process_lock(_mutex, std::defer_lock);
	if(process_lock.try_lock())
		return _done;
	else
		return false;
}

bool Process::isError()
{
	std::unique_lock<std::mutex> process_lock(_mutex);
	return _error;
}

void Process::setDone(bool done)
{
	std::unique_lock<std::mutex> process_lock(_mutex);
	_done = done;
	process_lock.unlock();
}

void Process::setError(bool error)
{
	std::unique_lock<std::mutex> process_lock(_mutex);
	_error = error;
	process_lock.unlock();
}