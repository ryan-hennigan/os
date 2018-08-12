#include "memmap.H"
#include "console.H"
#include "utils.H"
#include "assert.H"



MemMap::MemMap(unsigned long _base_frame_no, unsigned long _n_frames, unsigned long info_frame_no) :
    base_frame_no(_base_frame_no),
    n_frames(_n_frames),
    bitmap()
{
	if(info_frame_no == 0){
		bitmap.init(base_frame_no * Machine::PAGE_SIZE, n_frames, BITMAP_ATTRIBS);
		uint64_t n_info_frames = needed_info_frames(n_frames); 

		//internal storage
		bitmap.set(0,HOS,true);
		for(uint64_t i = 0; i < n_info_frames; i++)
		{
			bitmap.set(i,USED,true);
			assert(bitmap.get(i,USED)==true);
		}
		assert(bitmap.get(0,HOS)==true);
		assert(bitmap.get(0,USED)==true);
	}
	else{
		bitmap.init(info_frame_no * Machine::PAGE_SIZE, n_frames, BITMAP_ATTRIBS);
	}
	
}

unsigned long MemMap::get_frames(unsigned int _n_frames)
{
	unsigned int cnt = 0;
	unsigned int i = 0;
 
	for(; i < n_frames; i++){
		if(bitmap.get(i,USED)){
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

void MemMap::mark_inaccessible(unsigned long _base_frame_no,
                                      unsigned long _n_frames)
{	
	unsigned int index = _base_frame_no - base_frame_no;

	//base is within frame count
	assert(index < n_frames);

	//inaccessible range is within access
	assert((index+_n_frames) < n_frames);

	bitmap.set(index, HOS, true);
	for(unsigned int i = 0; i < _n_frames; i++){
		bitmap.set(index + i, USED,true);
	} 	
}

bool MemMap::contains_frames(unsigned long k){
	return ((k >= base_frame_no)&&(k < (base_frame_no + n_frames)));	
}

void MemMap::release_frames(unsigned long k)
{
	unsigned long kr = k-base_frame_no;
	assert(bitmap.get(kr,HOS));
	bitmap.set(kr,HOS,false);
	bitmap.set(kr,USED,false);
	kr++;

	//stop when reaching new HOS or free page
	while(!bitmap.get(kr,HOS) || !bitmap.get(kr,USED))
	{
		bitmap.set(kr,USED,false);
		kr++;
	}	
}

unsigned long MemMap::needed_info_frames(unsigned long _n_frames)
{
	unsigned long per_page = Machine::PAGE_SIZE * 8/BITMAP_ATTRIBS;
	return (_n_frames/per_page + (_n_frames % per_page > 0 ? 1 : 0));
}
