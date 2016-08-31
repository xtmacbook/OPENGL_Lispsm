//---------------------------------------------------------------------------
#ifndef SingeltonH
#define SingeltonH
#include "BaseException.h"
//---------------------------------------------------------------------------
class Singelton {
protected:
	Singelton(const Singelton&) { throw SingeltonException(); }
	Singelton& operator=(const Singelton&) { throw SingeltonException(); }
	static unsigned objectCount;
public:
	Singelton() { 
		objectCount++; 
		if(objectCount > 1)
			throw SingeltonException(); 
	}

	virtual ~Singelton() { objectCount--; }

};

#endif
