#ifndef __list_H__
#define __list_H__

template<class T>
class List {

public:
	List() : next(0){}

	T* getNext(){
		return next;
	}

	void setNext(T* _next){
		next = _next;
	}

private:
	T* next;
};

#endif
