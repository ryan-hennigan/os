#ifndef _BUDDY_VM_H_
#define _BUDDY_VM_H_

#include "types.H"
#include "utils.H"
#include "frame_pool.H"
#include "machine.H"
#include "vm_pool.H"



class PageTable;

class BuddyVm : public VMPool{ 
  
public:

   BuddyVm(unsigned long  _base_address,
          unsigned long  _size,
          FramePool *_frame_pool,
          PageTable     *_page_table);

   virtual uint64_t allocate(uint64_t _size);
   virtual void release(uint64_t _start_address);

   virtual bool is_legitimate(uint64_t _address);


private:

   //TODO remove need for cookie, use size instead
   struct fl_header{
      uint32_t cookie;
      uint32_t length;
      fl_header* next;
      fl_header* prev;
   };

   /*--- vm management ---*/
   unsigned long base_address;
   unsigned long size;
   FramePool * frame_pool;
   PageTable * page_table;
 	
   /*--- free list management ---*/
   fl_header** flTable;
   uint32_t tableSize;
   
   uint32_t getFlLevel(uint32_t a);
   void fl_add(uint32_t index, fl_header* fl);
   void fl_remove(uint32_t index, fl_header* fl);
   fl_header* segmentBlock(fl_header* h);
   fl_header* combineBlock(fl_header* h);

   
   /*--- class variables ---*/
   static const uint32_t vCOOKIE = 0xdeadb33f;
   static const uint32_t blockSize = Machine::PAGE_SIZE-sizeof(fl_header);
 
 };

#endif
