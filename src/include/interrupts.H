#ifndef _interrupts_H_                   // include file only once
#define _interrupts_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "machine.H"
#include "exceptions.H"
#include "idt.H"

/*--------------------------------------------------------------------------*/
/* I n t e r r u p t  H a n d l e r  */
/*--------------------------------------------------------------------------*/

class InterruptHandler;

class InterruptManager{

public:

  InterruptManager(IDT& idt);

  void register_handler(unsigned int        _irq_code,
                               InterruptHandler  * _handler);

  void deregister_handler(unsigned int _irq_code);

  void init_dispatcher();

  static void dispatch_interrupt(REGS * _r); 


private:

  const static int IRQ_TABLE_SIZE = 16;
  const static int IRQ_BASE       = 32;
 
  IDT idt;

  static InterruptHandler * handler_table[IRQ_TABLE_SIZE];
  
  static bool generated_by_slave_PIC(unsigned int int_no);
};

class InterruptHandler {

  public: 

   InterruptHandler(){}

   virtual void handle_interrupt(REGS * _regs) {
     assert(false); // sometimes pure virtual functions don't link correctly.
  }
  /* Different interrupt handlers are derived from the base class 
     InterruptHandler, and their functionality is implemented in 
     this function.*/

};

#endif
