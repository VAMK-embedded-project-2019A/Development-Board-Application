#ifndef PROCESS_H
#define PROCESS_H

class Process
{
public:
	Process() = default;
	
	virtual void start() = 0;
	
	bool isError() const;
	void setError(bool error);
	
private:
	bool _error{false};
};

#endif // PROCESS_H