#include <types.H>
#include <console.H>
#include <utils.C>


TEST(strlen, EmptyString)
{
	ASSERT_EQ(0, kutils::strlen(""));
}

TEST(strlen, SmallString)
{
	ASSERT_EQ(3,kutils::strlen("aaa"));
}

TEST(strlen, LargeString)
{
	char tmp[256];

	for(uint32_t i; i < 256; i++)
	{
		tmp[i] = 'a';
	}
	tmp[256] = '\x00';

	ASSERT_EQ(256,kutils::strlen((const char*)tmp));
}


TEST(memset, SetNull)
{
	char tmp[64];

	kutils::memset((void*)tmp,'\x00',64);
	for(uint32_t i; i < 64; i++)
	{
		ASSERT_EQ('\x00',tmp[i]);
	}
}




