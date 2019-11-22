#include "process.h"

bool Process::isError() const
{
	return _error;
}

void Process::setError(bool error)
{
	_error = error;
}