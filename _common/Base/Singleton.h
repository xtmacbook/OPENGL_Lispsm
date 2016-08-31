//---------------------------------------------------------------------------
#ifndef SingletonH
#define SingletonH
#include "BaseException.h"
//---------------------------------------------------------------------------
class Singleton {
protected:
	Singleton(const Singleton&) { throw SingeltonException(); }
	Singleton& operator=(const Singleton&) { throw SingeltonException(); }
//	static unsigned objectCount;
public:
	Singleton() { 
//		objectCount++; 
//		if(objectCount > 1)
//			throw SingeltonException(); 
	}

//	virtual ~Singleton() { objectCount--; }

};

#endif
