#ifndef _MEMMAP_H_
#define _MEMMAP_H_

#include "machine.H"
#include "frame_pool.H"
#include "bitmap.H"


class MemMap : public FramePool {
    
private:

	//TODO cache last X freed frames
	unsigned long base_frame_no;
	unsigned long n_frames;
	Bitmap bitmap;

	/*--- helper functions---*/
	bool contains_frames(unsigned long k);

	/*--- meta data info ---*/
	static const uint32_t BITMAP_ATTRIBS = 2;
	static const uint32_t USED = 0x1;
	static const uint32_t HOS = 0x0;

public:

    MemMap(unsigned long _base_frame_no,
                  unsigned long _n_frames,
                  unsigned long _info_frame_no);
    
    uint64_t get_frames(uint32_t _n_frames);
    
    void mark_inaccessible(uint64_t _base_frame_no,
                           uint64_t _n_frames);
    
    void release_frames(uint64_t _first_frame_no);

    static uint64_t needed_info_frames(uint64_t _n_frames);
};
#endif
