#include "bitmap.H"
#include "utils.H"




Bitmap::Bitmap() :
	base(0),
	size(0),
	attribs(0)
{
}

void Bitmap::init(uint64_t _b, uint32_t _sz, uint32_t _attribs)
{
	base = (char*)_b;
	attribs = _attribs;
	size = _sz*attribs;

	if(attribs > 8)
	{
		//ASSERT
	}
	
	uint32_t size_in_bytes = size / 8;

	kutils::memset((void*)base,'\x00',size_in_bytes);
}

void Bitmap::set(uint32_t index, uint32_t attrib, bool val)
{
	if(index > size | attrib >= attribs)
	{
		//ASSERT
	}
	char tmp = base[index/(8/attribs)];

	char mask = 1 << ((index%(8/attribs))*attribs + attrib);	
	

	char z_new = tmp & ~mask;

	char vmask = val << ((index%(8/attribs))*attribs + attrib);

	char final = z_new | vmask;
	base[index/(8/attribs)] = final;

}

bool Bitmap::get(uint32_t index, uint32_t attrib)
{
	if(index > size)
	{
		//ASSERT
	}
	char tmp = base[index/(8/attribs)];
	char mask = 1 << ((index%(8/attribs))*attribs+attrib);
	return (tmp & mask)!=0;
}

