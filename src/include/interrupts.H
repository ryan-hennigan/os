#ifndef _interrupts_H_                   // include file only once
#define _interrupts_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "machine.H"
#include "exceptions.H"

/*--------------------------------------------------------------------------*/
/* I n t e r r u p t  H a n d l e r  */
/*--------------------------------------------------------------------------*/

class InterruptHandler {

  private: 

  /* The Interrupt Handler Table */  
  const static int IRQ_TABLE_SIZE = 16;
  const static int IRQ_BASE       = 32;

  static InterruptHandler * handler_table[IRQ_TABLE_SIZE];
  
  static bool generated_by_slave_PIC(unsigned int int_no);
  /* Has the particular interupt been generated by the Slave PIC? */

  public: 

  /* -- POPULATE INTERRUPT-DISPATCHER TABLE */
  static void register_handler(unsigned int        _irq_code,
                               InterruptHandler  * _handler);
  /* This function allows to install an interrupt handler for the given 
     Interrupt code. The handler is a function pointer defined above. 
     Interrupt handlers are installed as Interrupt handlers as well.
     The 'register_interrupt' function uses irq2isr to map the IRQ 
     number to the code. */

  static void deregister_handler(unsigned int _irq_code);

  /* -- INITIALIZER */
  static void init_dispatcher();
  /* This function is called to initialize the high-level interrupt 
     handling. No high level interrupt handlers are installed yet. 
     If an interrupt occurs at this point, the system displays an error 
     message and terminates. */

  static void dispatch_interrupt(REGS * _r); 
  /* This is the high-level interrupt dispatcher. It dispatches the interrupt
     to the previously registered interrupt handler. 
     This function is called by the low-level function 
     "lowlevel_dispatch_interrupt(REGS * _r)".*/

  /* -- MANAGE INSTANCES OF INTERRUPT HANDLERS */

  virtual void handle_interrupt(REGS * _regs) {
     assert(false); // sometimes pure virtual functions don't link correctly.
  }
  /* Different interrupt handlers are derived from the base class 
     InterruptHandler, and their functionality is implemented in 
     this function.*/

};

#endif