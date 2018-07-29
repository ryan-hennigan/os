#ifndef _machine_low_H_                   // include file only once
#define _machine_low_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

 /* (none) */

/*--------------------------------------------------------------------------*/
/* LOW-LEVER CPU OPERATIONS */
/*--------------------------------------------------------------------------*/

extern "C" unsigned long get_EFLAGS(); 
/* Return value of the EFLAGS status register. */

#endif
