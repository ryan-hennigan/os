#ifndef __rr_scheduler_H__
#define __rr_scheduler_H__

#include "scheduler.H"

class RRScheduler : public Scheduler{

public:
	RRScheduler();
	
	void yield();

	void resume(Thread* t);

	void add(Thread* t);

	void terminate(Thread* t);

private:
	//TODO add run queue

};

#endif
