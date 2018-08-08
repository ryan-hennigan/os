#ifndef _GDT_H_                   // include file only once
#define _GDT_H_

/*--------------------------------------------------------------------------*/
/* GDT */ 
/*--------------------------------------------------------------------------*/

class GDT {

private:

  /* Use this function to set up an entry in the GDT. */
  void set_gate(int num, 
                       unsigned long base, unsigned long limit, 
                       unsigned char access, unsigned char gran);

public:

  /* 
     Initialize the GDT with a null segment, a code segment, 
     and one data segment.
  */

  void init();

  static const unsigned int SIZE = 3;
};

#endif
