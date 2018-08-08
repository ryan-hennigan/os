#ifndef _VM_POOL_H_                   // include file only once
#define _VM_POOL_H_

#include "types.H"
#include "list.H"

class VMPool : public List<VMPool>{ 
  
public:

   VMPool(){}

   virtual ~VMPool(){};

   virtual uint64_t allocate(uint64_t _size) = 0;

   virtual void release(uint64_t _start_address) = 0;

   virtual bool is_legitimate(uint64_t _address) = 0;

};

#endif
