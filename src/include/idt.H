#ifndef _IDT_H_
#define _IDT_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* Class IDT */ 
/*--------------------------------------------------------------------------*/

class IDT {

  public:

  static const int SIZE = 256;

  static void init();
  /* Initialize the IDT, and fill the 32 first entries with pointers to handle
     the 32 Intel-defined exceptions. After initializing the IDT, these exceptions
     are routed to the exception dispatcher (see 'exceptions.H'). At this point,
     no exception handlers are installed yet.
  */

  static void set_gate(unsigned char  num, unsigned long base, 
                       unsigned short sel, unsigned char flags);
  /* Used to install a low-level exception handler in the IDT. For high-level
     exception handlers, use the exception management framework defined in 
     file 'exceptions.H'.
  */

};

#endif
