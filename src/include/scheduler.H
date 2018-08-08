#ifndef SCHEDULER_H
#include "thread.H"

/*--------------------------------------------------------------------------*/
/* SCHEDULER */
/*--------------------------------------------------------------------------*/

class Scheduler {

public:

   virtual void yield() = 0;

   virtual void resume(Thread * _thread) = 0;

   virtual void add(Thread * _thread) = 0;

   virtual void terminate(Thread * _thread) = 0;


};
	
	

#endif
