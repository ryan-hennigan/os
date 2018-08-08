#include <types.H>
#include <gmock/gmock.h>

#include <vm_pool.H>
#include <vm_pool.C>

void Console::puts(const char *c){}



TEST(vmpool,poolConstIsLegitimate)
{
	char* testc = new char[4096*4];
	
	VMPool vp((uint64_t)testc,4096*4,0,0);

	uint64_t flhpStart = (uint64_t)((char*)testc+Machine::PAGE_SIZE + 24);

	ASSERT_EQ(vp.is_legitimate(flhpStart),true);
	free(testc);
}

TEST(vmpool, infoTableCorrect)
{
	uint64_t mem = Machine::PAGE_SIZE*4;
	uint64_t info = Machine::PAGE_SIZE;
	char* testc = new char[mem+info];
	
	VMPool::fl_header** flt = (VMPool::fl_header**)(testc);	
	
	VMPool vp((uint64_t)testc,mem+info,0,0);

	uint32_t sz = vp.getFlLevel(Machine::PAGE_SIZE*4)+1;

//	for(int i = 0; i < sz; i++){
//
//		std::cout << "FL index " << i << " :" << flt[i] <<std::endl;
//
//	}

	VMPool::fl_header* flhpStart = (VMPool::fl_header*)((char*)testc+info);



	ASSERT_EQ(flhpStart->length,mem-sizeof(VMPool::fl_header));

	vp.segmentBlock(flhpStart);
//	for(int i = 0; i < sz; i++){
//
//		std::cout << "FL index " << i << " :" << flt[i] <<std::endl;
//
//	}


	ASSERT_EQ(flhpStart->cookie,0);

	VMPool::fl_header* flhpFree = (VMPool::fl_header*)((char*)testc+info+mem/2);
	ASSERT_EQ(flhpFree->cookie, (uint64_t)vCOOKIE);
	ASSERT_EQ(flhpFree->length,(mem/2-sizeof(VMPool::fl_header)));



	free(testc);

}

TEST(vmpool, startCookie_firstSegment)
{
	uint64_t mem = Machine::PAGE_SIZE*4;
	uint64_t info = Machine::PAGE_SIZE;
	char* testc = new char[mem+info];
	
	VMPool vp((uint64_t)testc,mem+info,0,0);

	VMPool::fl_header* flhpStart = (VMPool::fl_header*)((char*)testc+info);

	ASSERT_EQ(flhpStart->length,mem-sizeof(VMPool::fl_header));

	vp.segmentBlock(flhpStart);

	ASSERT_EQ(flhpStart->cookie,0);

	flhpStart = (VMPool::fl_header*)((char*)testc+info+mem/2);
	ASSERT_EQ(flhpStart->cookie, (uint64_t)vCOOKIE);
	ASSERT_EQ(flhpStart->length,(mem/2-sizeof(VMPool::fl_header)));
	free(testc);
}

TEST(vmpool, combineSegment)
{
}


TEST(vmpool, allocate)
{
	uint64_t mem = Machine::PAGE_SIZE*4;
	uint64_t info = Machine::PAGE_SIZE;
	char* testc = new char[mem+info];
	
	VMPool::fl_header** flt = (VMPool::fl_header**)(testc);	
	
	VMPool vp((uint64_t)testc,mem+info,0,0);


	VMPool::fl_header* flhpStart = (VMPool::fl_header*)((char*)testc+info);

	ASSERT_EQ(flhpStart->length,mem-sizeof(VMPool::fl_header));

//	for(int j = 0; j < 10; j++){
//		uint64_t* tmp = (uint64_t*)vp.allocate(64*sizeof(uint64_t));
//		for(int i = 0; i < 64; i++){
//			tmp[i] = i*j+j;
//
//		}
//		for(int i = 0; i<64; i++){
//			ASSERT_EQ(tmp[i],i*j+j);
//		}
//		vp.release((uint64_t)tmp);
//	}
//
//	uint32_t sz = vp.getFlLevel(Machine::PAGE_SIZE*4)+1;
//	for(int i = 0; i < sz-1; i++){
//		if(flt[i]){
//			ASSERT_EQ(1,2);
//		}
//	}
   int size1 = 50;
   int size2 = 10;
   for(int i=1; i<size1; i++) {
      int *arr = (int*)vp.allocate(size2*i*sizeof(int));//new int[size2 * i];
      if(vp.is_legitimate((unsigned long)arr) == false) {
	ASSERT_EQ(1,2);
      }
      for(int j=0; j<size2*i; j++) {
         arr[j] = j;
      }
      for(int j=size2*i - 1; j>=0; j--) {
         if(arr[j] != j) {
            ASSERT_EQ(2,3);
         }
      }
      vp.release((uint64_t)arr);
   }

	uint32_t sz = vp.getFlLevel(Machine::PAGE_SIZE*4)+1;
	for(int i = 0; i < sz-1; i++){
		if(flt[i]){
			ASSERT_EQ(1,2);
		}
	}


	free(testc);
}
