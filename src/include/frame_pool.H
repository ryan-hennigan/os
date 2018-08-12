#ifndef _FRAME_POOL_H_                   // include file only once
#define _FRAME_POOL_H_

#include "types.H"

class FramePool {
    
public:

    virtual uint64_t get_frames(uint32_t _n_frames) = 0;
    
    virtual void mark_inaccessible(uint64_t _base_frame_no, uint64_t _n_frames) = 0;
    
    virtual void release_frames(uint64_t _first_frame_no) = 0;
};
#endif

