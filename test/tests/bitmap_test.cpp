#include <types.H>
#include <gmock/gmock.h>

#include <bitmap.H>
#include <bitmap.C>


namespace{

	class BitmapTest : public ::testing::Test{

	protected:
		Bitmap* bmp;
		char* mem;
		uint32_t memsz;

		BitmapTest():
			memsz(4096*4)
		{
		}

		virtual ~BitmapTest(){}

		virtual void SetUp()
		{
			mem = new char[memsz];
			bmp = new Bitmap();
			bmp->init((uint64_t)mem,memsz/2,2);
		}

		virtual void TearDown()
		{
			free(mem);
			free(bmp);
		}
	};




	TEST_F(BitmapTest, setAndCheck)
	{
		ASSERT_EQ(bmp->get(0,0),false);	
		bmp->set(0,0,true);
		ASSERT_EQ(bmp->get(0,0),true);	
		ASSERT_EQ(bmp->get(0,1),false);	
	}
	
	TEST_F(BitmapTest, setSetCheck)
	{
		bmp->set(23,1,true);
		ASSERT_EQ(bmp->get(23,1),true);
		ASSERT_EQ(bmp->get(23,0),false);
		bmp->set(23,1,false);
		ASSERT_EQ(bmp->get(23,1),false);
		ASSERT_EQ(bmp->get(23,0),false);
	}

	TEST_F(BitmapTest, setArray)
	{
		for(int i = 0; i < 32; i++){
			bmp->set(i,0x0,true);
		}
		for(int i = 0; i < 32; i++){
			ASSERT_EQ(bmp->get(i,0x0),true);
//			std::cout << i << std::endl;
			ASSERT_EQ(bmp->get(i,0x1),false);
		}
		for(int i = 0; i < 32; i++){
			bmp->set(i,0x0,false);
		}
		for(int i = 0; i < 32; i++){
			ASSERT_EQ(bmp->get(i,0x0),false);
			ASSERT_EQ(bmp->get(i,0x1),false);
		}
	}


	TEST_F(BitmapTest, looping)
	{
		bmp->set(0,1,true);
		for(int i = 0; i < 32; i++){
			bmp->set(i,0,true);


		}
		ASSERT_EQ(bmp->get(0,0),true);
		ASSERT_EQ(bmp->get(0,1),true);



	}



}//namespace


