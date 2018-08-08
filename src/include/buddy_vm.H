#ifndef _BUDDY_VM_H_                   // include file only once
#define _BUDDY_VM_H_

#include "types.H"
#include "utils.H"
#include "frame_pool.H"
#include "vm_pool.H"



class PageTable;

class BuddyVm : public VMPool{ 
  
public:

   //TODO remove need for cookie
   //TODO move to private
   struct fl_header{
      uint32_t cookie;
      uint32_t length;
      fl_header* next;
      fl_header* prev;
   };

   BuddyVm(unsigned long  _base_address,
          unsigned long  _size,
          FramePool *_frame_pool,
          PageTable     *_page_table);

   virtual uint64_t allocate(uint64_t _size);
   virtual void release(uint64_t _start_address);

   virtual bool is_legitimate(uint64_t _address);


   //TODO declared public for testing 
   uint32_t getFlLevel(uint32_t a);
   void fl_add(uint32_t index, fl_header* fl);
   void fl_remove(uint32_t index, fl_header* fl);
   fl_header* segmentBlock(fl_header* h);
   fl_header* combineBlock(fl_header* h);


   private:
   /* -- DEFINE YOUR VIRTUAL MEMORY POOL DATA STRUCTURE(s) HERE. */
   unsigned long base_address;
   unsigned long size;
   FramePool * frame_pool;
   PageTable * page_table;


   bool inRange(uint64_t a);
 	
   fl_header** flTable;
   
   static const uint32_t blockSize = 64;
   uint32_t tableSize;

   static const uint32_t vCOOKIE = 0xdeadb33f;
 
 };

#endif
