#ifndef PROCESS_H
#define PROCESS_H

#include <mutex>

class Process
{
public:
	Process() = default;
	
	virtual void start() = 0;
	
	bool isDone();
	bool isError();
	void setDone(bool done);
	void setError(bool error);
	
protected:
	std::mutex _mutex;
	
private:
	bool _done{true};
	bool _error{false};
};

#endif // PROCESS_H