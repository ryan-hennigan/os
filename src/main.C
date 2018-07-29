/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */

#include "machine.H"
#include "gdt.H"

#include "utils.H"
#include "console.H"

#include "idt.H"
#include "exceptions.H"

#include "irq.H"
#include "interrupts.H"
#include "timer.H"

#include "frame_pool.H"
#include "paging_low.H"
#include "page_table.H"

/*--------------------------------------------------*/
/*  DEFINES  */
/*--------------------------------------------------*/

#define KB * (0x1 << 10)
#define MB * (0x1 << 20)

//memory layout
/* |-- reserved 2MB --|-- kernel 2MB --|-- free 11MB --|-- reserved 1MB --|-- free --| */  
#define KERNEL_POOL_START_FRAME ((2 MB) / Machine::PAGE_SIZE)
#define KERNEL_POOL_SIZE ((2 MB) / Machine::PAGE_SIZE)
#define PROCESS_POOL_START_FRAME ((4 MB) / Machine::PAGE_SIZE)
#define PROCESS_POOL_SIZE ((28 MB) / Machine::PAGE_SIZE)
/* definition of the kernel and process memory pools */

#define MEM_HOLE_START_FRAME ((15 MB) / Machine::PAGE_SIZE)
#define MEM_HOLE_SIZE ((1 MB) / Machine::PAGE_SIZE)
/* we have a 1 MB hole in physical memory starting at address 15 MB */

typedef unsigned int uint32_t;
typedef unsigned long uint64_t;



/*--------------------------------------------------*/
/*  TESTING  */
/*--------------------------------------------------*/

void test_framing(FramePool * _pool, unsigned int _allocs_to_go);
void test_paging(unsigned long addr, int naccesses);

void loading_message();
void passed();
void failed();

int main()
{
    //init Global Descriptor Table
    GDT::init();

    Console::init();
    
    //init Interrupt Descriptor Table and Exceptions 
    IDT::init();
    ExceptionHandler::init_dispatcher();

    //init Interrupts
    IRQ::init();
    InterruptHandler::init_dispatcher();

    // --- Simple Timer to handle timer interrupts ---//
    Timer timer;
    InterruptHandler::register_handler(0, &timer);

    //need timer before enabling interrupts
    Machine::enable_interrupts();

    // --- Setup Kernel Frame Pool Manager --- //
    FramePool kernelframepool(KERNEL_POOL_START_FRAME, KERNEL_POOL_SIZE, 0, 0);

    // --- Setup User Frame Pool Manager --- //
    uint32_t num_info_frames = FramePool::needed_info_frames(PROCESS_POOL_SIZE);
    uint32_t process_info_pool = kernelframepool.get_frames(num_info_frames);

    FramePool processframepool(PROCESS_POOL_START_FRAME, PROCESS_POOL_SIZE, process_info_pool, num_info_frames);

    //leave space for ISA
    processframepool.mark_inaccessible(MEM_HOLE_START_FRAME, MEM_HOLE_SIZE);

    Console::puts("Testing kernel mempool...");
    test_framing(&kernelframepool, 10);
    passed();

    Console::puts("Testing process mempool...");
    test_framing(&processframepool, 170);
    passed();

   // --- Setup and Enable Paging --- //

    class PageFault_Handler : public ExceptionHandler {
      /* We derive the page fault handler from ExceptionHandler 
	 and overload the method handle_exception. */
      public:
        virtual void handle_exception(REGS * _regs) {
          PageTable::handle_fault(_regs);
        }
    } pagefault_handler;

    ExceptionHandler::register_handler(14, &pagefault_handler);

    PageTable::init_paging(&kernelframepool, &processframepool, 4 MB);

    PageTable pt1;

    pt1.load();

    PageTable::enable_paging();

    test_paging(4 MB, (1 MB)/4);


    loading_message(); 

    for (;;);
}

void loading_message()
{
    Console::settextcolor(LIGHT_GREEN,BLACK);
    Console::puts("Welcome to OS!");
    Console::settextcolor(WHITE,BLACK);
    Console::puts("\n");
}

void test_framing(FramePool * _pool, unsigned int _allocs_to_go) {
    if (_allocs_to_go > 0) {
        int n_frames = _allocs_to_go % 4 + 1;
        unsigned long frame = _pool->get_frames(n_frames);
        int * value_array = (int*)(frame * (4 KB));        
        for (int i = 0; i < (1 KB) * n_frames; i++) {
            value_array[i] = _allocs_to_go;
        }
        test_framing(_pool, _allocs_to_go - 1);
        for (int i = 0; i < (1 KB) * n_frames; i++) {
            if(value_array[i] != _allocs_to_go){
                Console::puts("MEMORY TEST FAILED. ERROR IN FRAME POOL\n");
                Console::puts("i ="); Console::puti(i);
                Console::puts("   v = "); Console::puti(value_array[i]); 
                Console::puts("   n ="); Console::puti(_allocs_to_go);
                Console::puts("\n");
                for(;;); 
            }
        }
        FramePool::release_frames(frame); 
    }
}

void test_paging(unsigned long start_address, int n_references) {
  int *foo = (int *) start_address;
  
  for (int i=0; i<n_references; i++) {
    foo[i] = i;
  }
  
  Console::puts("DONE WRITING TO MEMORY. Now testing...\n");

  for (int i=0; i<n_references; i++) {
    if(foo[i] != i) {
      failed();
    }
  }
}

void passed(){
    Console::settextcolor(LIGHT_GREEN,BLACK);
    Console::puts("PASSED!");
    Console::settextcolor(WHITE,BLACK);
    Console::puts("\n");
}
void failed(){
    Console::settextcolor(RED,BLACK);
    Console::puts("FAILED!");
    Console::settextcolor(WHITE,BLACK);
    Console::puts("\n");
}
