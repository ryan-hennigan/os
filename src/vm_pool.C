/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "vm_pool.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "page_table.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   V M P o o l */
/*--------------------------------------------------------------------------*/

VMPool::VMPool(unsigned long  _base_address, unsigned long  _size, FramePool * _frame_pool, PageTable  * _page_table) :
	base_address(_base_address),
	size(_size),
	frame_pool(_frame_pool),
	page_table(_page_table),
	next(0)
{
	//TODO setup buddy alloc	

	Console::puts("Constructed VMPool object.\n");
}

unsigned long VMPool::allocate(unsigned long _size) {

	//TODO use buddy alloc

	return 0L;
}

void VMPool::release(unsigned long _start_address) {

	//TODO use buddy alloc

	Console::puts("Released region of memory.\n");
}

bool VMPool::is_legitimate(unsigned long _address) {
	
	//TODO use buddy alloc

	Console::puts("Checked whether address is part of an allocated region.\n");
	
	return false;
}

uint32_t VMPool::combineBlock(){

	//TODO 	

}

uint32_t VMPool::segmentBlock()
{

	//TODO

}

uint32_t VMPool::getFlLevel(uint32_t addr)
{

	//TODO
	return 33;
}
