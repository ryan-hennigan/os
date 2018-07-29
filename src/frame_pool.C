/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define HOS 0x80
#define USE 0x40
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "frame_pool.H"
#include "console.H"
#include "utils.H"
#include "assert.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

FramePool * FramePool::fPool = 0;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   C o n t F r a m e P o o l */
/*--------------------------------------------------------------------------*/

FramePool::FramePool(unsigned long _base_frame_no, unsigned long _n_frames, unsigned long _info_frame_no, unsigned long _n_info_frames) :
    base_frame_no(_base_frame_no),
    info_frame_no(_info_frame_no),
    n_info_frames(_n_info_frames),
    n_frames(_n_frames)
{
	if(info_frame_no == 0){
		bitmap = (unsigned char*)(base_frame_no * FRAME_SIZE);
		n_frames += n_info_frames;
	}
	else{
		bitmap = (unsigned char*)(info_frame_no * FRAME_SIZE);
	}

	nextp = 0;
	prevp = 0;	
	
	if(FramePool::fPool == 0){
		FramePool::fPool = this;
	}
	else{
		nextp = FramePool::fPool;
		FramePool::fPool-> prevp = this;
		FramePool::fPool = this;
	}
	
	//preset the bitmap
	unsigned int frames = n_frames;
	for(unsigned int i = 0; i < frames; i++ ){
		bitmap[i] = 0xFF;
	}

	//internal storage, mark base as used
	if(info_frame_no == 0){
		mark_USE(0); 
	}
}

unsigned long FramePool::get_frames(unsigned int _n_frames)
{
	unsigned int cnt = 0;
	unsigned int i = 0;
 
	for(; i < n_frames; i++){
		if(is_USED(i)){
			cnt = 0;
		}
		else{
			cnt++;
		}
		if(cnt == _n_frames) break;
	}
	if(cnt == _n_frames){
		i -= (cnt-1);	
		mark_inaccessible(base_frame_no + i, _n_frames);
		return base_frame_no+i;
	}
	else{
		assert("get frames failed!\n");	
		return 0;
	}
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
                                      unsigned long _n_frames)
{	
	unsigned int base = _base_frame_no - base_frame_no;

	//base is within frame count
	assert(base < n_frames);

	//inaccessible range is within access
	assert((base+_n_frames) < n_frames);

	mark_HOS(base);
	for(unsigned int i = 0; i < _n_frames; i++){
		mark_USE(base + i);
	} 	
}

bool FramePool::contains_frames(unsigned long k){
	return ((k >= base_frame_no)&&(k < (base_frame_no + n_frames)));	
}

void FramePool::release_frames(unsigned long _first_frame_no)
{
	FramePool* fpn = fPool;
	bool found = false;
	while(fpn != 0){
		if(fpn->contains_frames(_first_frame_no)){
			found = true;
			break;
		}
		fpn = fpn->nextp;
	}
	if(found){
		fpn->release_frames_inner(_first_frame_no);
	}
}

void FramePool::release_frames_inner(unsigned long k){
	unsigned long kr = k-base_frame_no;
	assert(is_HOS(kr));	
	assert(is_USED(kr));
	unsigned long i = kr+1;
	mark_FREE(kr);
	while(is_USED(i) && !is_HOS(i) && (i < n_frames)){
		mark_FREE(i);
		i++;
	}
}

unsigned long FramePool::needed_info_frames(unsigned long _n_frames)
{
	unsigned long per_page = FRAME_SIZE * 4;
	return (_n_frames/per_page + (_n_frames % per_page > 0 ? 1 : 0));
}
void FramePool::mark_HOS(unsigned long k){
	const char mask = (HOS >> (2* ((k % 4))));
	//assert(is_HOS(k) == false);
	bitmap[k/4] ^= mask;
}

void FramePool::mark_USE(unsigned long k){
	const char mask = (USE >> (2* ((k % 4))));
	assert(is_USED(k)==0);
	bitmap[k/4] ^= mask;
}

void FramePool::mark_FREE(unsigned long k){
	char mask_USE = (USE >> (2* ((k % 4))));
	char mask_HOS = (HOS >> (2* ((k % 4))));
	bitmap[k/4] |= (mask_HOS|mask_USE);
}

bool FramePool::is_USED(unsigned long k){
	char mask = (USE >> (2* ((k % 4))));
	return ((bitmap[k/4] & mask) == 0);
}

bool FramePool::is_HOS(unsigned long k){
	char mask = (HOS >> (2* ((k % 4))));
	return ((bitmap[k/4] & mask) == 0);
}
