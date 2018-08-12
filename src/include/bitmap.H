#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "types.H"

class Bitmap{


public:
	Bitmap(); 
	void init(uint64_t _b, uint32_t _sz, uint32_t attribs);
	
	void set(uint32_t index, uint32_t attrib, bool val);
	bool get(uint32_t index, uint32_t attrib);

private:

	char* base;
	uint32_t size;
	uint32_t attribs;
};
#endif
