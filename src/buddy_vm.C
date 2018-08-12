#include "buddy_vm.H"
//#include "console.H"
#include "utils.H"
#include "assert.H"
#include "page_table.H"


void BuddyVm::fl_add(uint32_t index, fl_header* val)
{
	if(flTable[index]){
 		val->prev = 0;
		val->next = flTable[index];
		flTable[index]->prev = val;
		flTable[index] = val;
	}	
	else{
		val->next = 0;
		val->prev = 0;
		flTable[index] = val;	
	}
}

void BuddyVm::fl_remove(uint32_t index, fl_header* val)
{
	if(val->next){
		val->next->prev = val->prev;
	}

	if(val->prev){
		
		val->prev->next = val->next;
	}
	else{
		flTable[index] = val->next;
	}
}

BuddyVm::BuddyVm(unsigned long  _base_address, unsigned long  _size, FramePool * _frame_pool, PageTable  * _page_table) :
	flTable((fl_header**)_base_address),
	size(_size - Machine::PAGE_SIZE),
	frame_pool(_frame_pool),
	page_table(_page_table),
	tableSize(0)
{
	tableSize = getFlLevel(size) + 1;		

	//start after the fl table
	uint64_t start = ((uint64_t)_base_address + Machine::PAGE_SIZE);

	kutils::memset((void*)flTable,'\x00',Machine::PAGE_SIZE);	

	//init header
	fl_header* flshp = ((fl_header*)start);
	flshp->length = (uint64_t)(size - sizeof(fl_header)); //only length of allocatable blocks
	flshp->cookie = vCOOKIE;

	//add to the fl table at highest point
	fl_add(tableSize-1,flshp);

	//Console::puts("Constructed VMPool object.\n");
}

uint64_t BuddyVm::allocate(uint64_t _size) 
{
	if(_size < blockSize){
		_size = blockSize;
	}
	const uint32_t levelNeeded = getFlLevel((uint64_t)(_size + sizeof(fl_header)));	
	bool space = false;
	uint32_t clevel = levelNeeded;

	fl_header* tmp = 0;

	while(!space && clevel <= tableSize){
		if(flTable[clevel]){
			space = true;
			tmp = flTable[clevel];
			while(clevel != levelNeeded){
				tmp = segmentBlock(tmp);
				clevel--;
			}
			fl_remove(levelNeeded,tmp);
			tmp->cookie = 0;
		}
		clevel++;
	}
	if(tmp){
		return (uint64_t)((char*)tmp+sizeof(fl_header));
	}
	else{
		//assert(false);
		return 0L;
	}
}

void BuddyVm::release(unsigned long _a) {

	fl_header* aBlock = (fl_header*)((char*)_a - sizeof(fl_header));
	uint32_t levelIndex = getFlLevel(aBlock->length + sizeof(fl_header));
	aBlock->cookie = vCOOKIE;
	fl_add(levelIndex,aBlock);
	combineBlock(aBlock);

	//Console::puts("Released region of memory.\n");
}

bool BuddyVm::is_legitimate(uint64_t _address)
{
	//Console::puts("Checked whether address is part of an allocated region.\n");

	//base + info table size
	uint64_t lb = (uint64_t)flTable + Machine::PAGE_SIZE;
	uint64_t ub = lb + size - sizeof(fl_header) - blockSize;
	if(_address >= lb && _address <= ub)
	{
		return true;
	} 
	else{
		return false;
	}

}

BuddyVm::fl_header* BuddyVm::combineBlock(fl_header* head){
	unsigned int bounds = head->length + sizeof(fl_header);
	if(bounds < blockSize){
		return head;
	}
	unsigned int currLevel = getFlLevel(bounds);
	fl_header* left = (fl_header*)((char*)head-bounds);
	fl_header* right = (fl_header*)((char*)head+bounds);

	if(is_legitimate((uint64_t)left)){
		if(left->cookie==vCOOKIE){
			if(left->length == head->length){
				head->cookie = 0;
				fl_remove(currLevel,head);
				fl_remove(currLevel,left);
				left->length = 2*bounds-sizeof(fl_header);
				fl_add(currLevel+1,left);
				combineBlock(left);
			}	
		}
	}
	else if(is_legitimate((uint64_t)head)){
		if(head->cookie == vCOOKIE){
			if(right->length == head->length){
				right->cookie = 0;
				fl_remove(currLevel,head);
				fl_remove(currLevel,right);
				head->length = 2*bounds-sizeof(fl_header);
				fl_add(currLevel+1,head);
				combineBlock(head);
			}	
		}
	}
}

BuddyVm::fl_header* BuddyVm::segmentBlock(fl_header* head)
{
	uint32_t listNeeded = getFlLevel(head->length + sizeof(fl_header));
	if(listNeeded != getFlLevel(blockSize)){
		uint32_t newBlockLvl = listNeeded-1;
		uint32_t newBlockLength = (1 << newBlockLvl);
		head->length = (newBlockLength - sizeof(fl_header));
		fl_header* freeBlock = (fl_header*)((char*)head + newBlockLength);
		freeBlock->length = newBlockLength-sizeof(fl_header);	
		freeBlock->cookie = vCOOKIE;
		head->cookie = 0;
		fl_remove(listNeeded,head);	
		fl_add(newBlockLvl,freeBlock);
		fl_add(newBlockLvl,head);
		combineBlock(freeBlock);
		return head;
	}
	else{
		return head;
	}
}

uint32_t BuddyVm::getFlLevel(uint32_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	uint32_t lvl = 0;
	while(v){
		v = v >> 1;
		lvl++;
	}
	return lvl-1;
}

