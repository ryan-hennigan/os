#ifndef _IRQ_H_                   // include file only once
#define _IRQ_H_


/*--------------------------------------------------------------------------*/
/*  */ 
/*--------------------------------------------------------------------------*/

class IRQ {

public:

  void init();
  /* Initialize the IRQ handlers, i.e. fill 16 entries with pointers to handle
     the PIC generated interrupts. These interrupts are routed to the exception 
     dispatcher (see 'exceptions.H'). At this point, no exception handlers are 
     installed yet.
  */

};

#endif
