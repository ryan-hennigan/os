#include "assert.H"
#include "exceptions.H"
#include "console.H"
#include "paging_low.H"
#include "page_table.H"

PageTable * PageTable::current_page_table = NULL;
unsigned int PageTable::paging_enabled = 0;
FramePool * PageTable::kernel_mem_pool = NULL;
FramePool * PageTable::process_mem_pool = NULL;
unsigned long PageTable::shared_size = 0;



void PageTable::init_paging(FramePool * _kernel_mem_pool, FramePool * _process_mem_pool, const unsigned long _shared_size)
{

	kernel_mem_pool = _kernel_mem_pool;
	process_mem_pool = _process_mem_pool;
	shared_size = _shared_size;
	Console::puts("Initialized Paging System\n");
}

PageTable::PageTable()
{
	//allocate the PDT and first PT
	unsigned long page_directory_n = process_mem_pool->get_frames(1);
	page_directory = (unsigned long *)(page_directory_n * Machine::PAGE_SIZE);
	unsigned long direct_table_n = process_mem_pool->get_frames(1);
	unsigned long *direct_table = (unsigned long*)(direct_table_n * Machine::PAGE_SIZE);
	
	//populate the PT
	unsigned long address = 0;
	for(unsigned int i= 0;i<Machine::PAGE_SIZE/sizeof(long); i++){
		direct_table[i] = address | (READ_WRITE | VALID);
		address+=Machine::PAGE_SIZE;
	}

	//populate the PDT	
	page_directory[0] = (unsigned long)direct_table;
	page_directory[0] = page_directory[0] | (READ_WRITE | VALID);
		for(unsigned int i=1; i < Machine::PAGE_SIZE/sizeof(long); i++){
		page_directory[i] = 0 | READ_WRITE;
	}
	page_directory[1023] = (unsigned long)page_directory;
	page_directory[1023] = page_directory[1023] | (VALID | READ_WRITE);

	Console::puts("Constructed Page Table object\n");
}


void PageTable::load()
{
	Console::puts("Loaded page table\n");
	current_page_table = this;
	write_cr3((unsigned long)page_directory);
}

void PageTable::enable_paging()
{
	Console::puts("Enabled paging\n");
	write_cr0(read_cr0() | 0x80000000);
}

void PageTable::handle_fault(REGS * _r)
{
	Console::puts("handled page fault\n");

	unsigned long addr = read_cr2();
	unsigned long pde,pte;

	pde = addr >> (PTE+OFFSET);
	pte = (addr << PDE) >> (PTE+OFFSET);

	//make sure reference is in the VMPool or part of the Page Table system
	bool legit = false;
	VMPool* tmp = current_page_table->head;
	while(tmp){
		if(tmp->is_legitimate(addr)) legit = true;
		tmp = tmp->next;
	}
	if(addr >= 0xffc00000) legit = true;
	assert(legit && "INVALID memory reference!");

	//no pde, so make an new one w/ the recursive lookup
	unsigned long * page_directory_r = (unsigned long *)0xfffff000;
	if(!(page_directory_r[pde] & VALID)){
		unsigned long pt_n = process_mem_pool->get_frames(1);

		unsigned long * pt_page = (unsigned long *)(0xffc00000 | (pt_n << OFFSET));

		page_directory_r[pde] = ((pt_n * Machine::PAGE_SIZE) | (READ_WRITE | VALID));	

		for(int i = 0; i < Machine::PAGE_SIZE/sizeof(int); i++){
			pt_page[i] = 0 | READ_WRITE;
		}
	}

	//obtain the pt recursively
	unsigned long * pt_r = (unsigned long*)(0xffc00000 | (pde<<OFFSET));

	//no pte so make one
	if(!(pt_r[pte] & VALID)){
		unsigned long frame = process_mem_pool->get_frames(1) * Machine::PAGE_SIZE;
		pt_r[pte] = (frame | (READ_WRITE | VALID));
	}
}

void PageTable::register_pool(VMPool * _vm_pool)
{
	//use the vm pool as a node for the list
	_vm_pool->next = head;
	head = _vm_pool;	

	Console::puts("registered VM pool\n");
}

void PageTable::free_page(unsigned long _page_no) {
	Console::puts("freed page\n");

	//make pte invalid
	unsigned long* freepage = (unsigned long *)(0xffc00000 | (_page_no >> PDE));
	*freepage = *freepage ^ VALID;	
	load();
}
