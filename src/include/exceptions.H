#ifndef _exceptions_H_                   // include file only once
#define _exceptions_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "machine.H"
#include "idt.H"

/*--------------------------------------------------------------------------*/
/* E x c e p t i o n H a n d l e r  */
/*--------------------------------------------------------------------------*/

class ExceptionHandler;

class ExceptionManager{

  public:

  ExceptionManager(IDT& idt);

  void register_handler(unsigned int        _isr_code,
                               ExceptionHandler  * _handler);
  /* This function allows to install an exception handler for the given 
     exception code. The handler is a function pointer defined above. 
     Interrupt handlers are installed as exception handlers as well.
     The 'register_interrupt' function uses irq2isr to map the IRQ 
     number to the code. */

  void deregister_handler(unsigned int _isr_code);

  /* -- DISPATCHER */
  void init_dispatcher();
  /* This function is called to initialize the high-level exception handling. 
     No high level exception handlers are installed yet. If an exception 
     occurs at this point, the system displays an error message and 
     terminates. */

  static void dispatch_exception(REGS * _r);
  /* This is the high-level exception dispatcher. It dispatches the exception
     to the previously registered exception handler. 
     This function is called by the low-level function 
     "lowlevel_dispatch_exception(REGS * _r)".*/


  private:

  const static int EXCEPTION_TABLE_SIZE = 32;
  static ExceptionHandler * handler_table[EXCEPTION_TABLE_SIZE];
  IDT idt;
};

class ExceptionHandler {

  /* The Exception Handler Table */
  public: 

  ExceptionHandler(){}

  /* -- POPULATE DISPATCHER TABLE */
  /* -- MANAGE INSTANCES OF EXCEPTION HANDLERS */

  virtual void handle_exception(REGS * _regs) {
     assert(false); // sometimes pure virtual functions dont link correctly.
  }
  /* Different exception handlers are derived from the base class ExceptionHandler
     and their functionality is implemented in this function.*/

};

#endif
