#include <stdlib.h>
#include <types.H>
#include <gmock/gmock.h>

#include <assert.H>
#include <assert.C>

#include <memmap.H>
#include <memmap.C>

#define KB * 1<<10
#define MB * 1<<20



namespace{

	class MemMapTest : public ::testing::Test{

	protected:
		MemMap* mmp;
		char* memt;
		char* mem;
		uint32_t memsz;

		MemMapTest():
			memsz(4096*40)
		{
		}

		virtual ~MemMapTest(){}

		virtual void SetUp()
		{
			memt = new char[memsz];
			uint64_t tmp = (uint64_t)memt;
			tmp >>= 12;
			tmp++;
			tmp <<= 12;
			mem = (char*)tmp;
			
			uint32_t szt = memsz-((uint64_t)mem-(uint64_t)memt);
			mmp = new MemMap(tmp>>12,szt/4096,0);
			
		}

		virtual void TearDown()
		{
			free(memt);
			free(mmp);

		}
	};



	TEST_F(MemMapTest, constructor)
	{
		uint64_t frame = mmp->get_frames(1);
		ASSERT_NE(frame,0);
	}

	void test_framing(FramePool * _pool, uint32_t _allocs_to_go) {
	    if (_allocs_to_go > 0) {
		uint32_t n_frames = _allocs_to_go % 4 + 1;
		uint64_t frame = _pool->get_frames(n_frames);
//		std::cout << frame <<std::endl;
		uint32_t * value_array = (uint32_t*)(frame * (4 KB));        
		for (uint32_t i = 0; i < (1 KB) * n_frames; i++) {
		    value_array[i] = _allocs_to_go;
		}
		test_framing(_pool, _allocs_to_go - 1);
		for (uint32_t i = 0; i < (1 KB) * n_frames; i++) {
		    ASSERT_EQ(value_array[i], _allocs_to_go);
		}
		_pool->release_frames(frame); 
	    }
	}

	TEST_F(MemMapTest, loop)
	{
		test_framing(mmp,5);

	}

}//namespace
