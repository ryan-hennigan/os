#include <types.H>
#include <gmock/gmock.h>

#include <buddy_vm.H>
#include <buddy_vm.C>

void Console::puts(const char *c){}


namespace{

	class BuddyVmTest : public ::testing::Test{

	protected:
		BuddyVm* vmp;
		int* mem;
		uint32_t memsz;

		BuddyVmTest():
			memsz(4096*4)
		{
		}

		virtual ~BuddyVmTest(){}

		virtual void SetUp()
		{
			mem = new int[memsz];
			vmp = new BuddyVm((uint64_t)mem,memsz,0,0);
		}

		virtual void TearDown()
		{
			free(mem);
			free(vmp);
		}
	};


	TEST_F(BuddyVmTest,initPoolIsLegitimate)
	{
		ASSERT_EQ(vmp->is_legitimate((uint64_t)mem + Machine::PAGE_SIZE + 14),true);

	}

	TEST_F(BuddyVmTest,allocateMulltipleTimes)
	{	
		uint64_t x = vmp->allocate(1024);
		uint64_t y = vmp->allocate(20);

		ASSERT_NE(x,y);
	}

	TEST_F(BuddyVmTest,allocReleaseAlloc)
	{
		uint64_t x = (uint64_t)vmp->allocate(64);
		vmp->release(x);
		uint64_t y = vmp->allocate(20);
		uint64_t y1 = vmp->allocate(20);
		uint64_t y2 = vmp->allocate(20);
		uint64_t y3 = vmp->allocate(20);
		ASSERT_EQ((uint64_t)x,y);
	}

	TEST_F(BuddyVmTest,smallAlloc)
	{
		//TODO
	}

	TEST_F(BuddyVmTest,largeAlloc)
	{
		//TODO
	}
	
	TEST_F(BuddyVmTest,doubleFree)
	{
		//TODO
	}

	TEST_F(BuddyVmTest,loopTest)
	{
		int sz1 = 70;
		int sz2 = 20;
		for(int i = 1; i < sz1; i++){
			int* arr = (int*)(vmp->allocate(sz2*i*sizeof(int)));
			for(int j = 0; j < sz2*i; j++){
				arr[j] = j;
			}
			for(int j = 0; j < sz2*i; j++){
				ASSERT_EQ(arr[j],j);
			}
			vmp->release((uint64_t)arr);
		}
	}

}//namespace

