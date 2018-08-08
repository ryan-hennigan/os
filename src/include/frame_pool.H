#ifndef _FRAME_POOL_H_                   // include file only once
#define _FRAME_POOL_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "machine.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/
/* -- (none) -- */

class FramePool {
    
private:
	unsigned long base_frame_no;
	unsigned long n_frames;
	unsigned long info_frame_no;
	unsigned long n_info_frames;

	unsigned char * bitmap;
	FramePool* nextp;
	FramePool* prevp;


	bool contains_frames(unsigned long k);
	void release_frames_inner(unsigned long n);
	void mark_HOS(unsigned long k);
	void mark_USE(unsigned long k);    
	void mark_FREE(unsigned long k);
	bool is_HOS(unsigned long k);
	bool is_USED(unsigned long k);	
	
public:
    static FramePool* fPool;

    static const unsigned int FRAME_SIZE = Machine::PAGE_SIZE; 

    FramePool(unsigned long _base_frame_no,
                  unsigned long _n_frames,
                  unsigned long _info_frame_no,
                  unsigned long _n_info_frames);
    
    unsigned long get_frames(unsigned int _n_frames);
    
    void mark_inaccessible(unsigned long _base_frame_no,
                           unsigned long _n_frames);
    
    static void release_frames(unsigned long _first_frame_no);
    
    static unsigned long needed_info_frames(unsigned long _n_frames);
};
#endif

