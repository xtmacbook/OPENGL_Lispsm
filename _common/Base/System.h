//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef _DMS_SystemH
#define _DMS_SystemH
//---------------------------------------------------------------------------
namespace DMS {

///hint: if you just would convert to void* the destructor would not be called
template<class T> 
inline void saveDelete(T& p) {
	if(0 != p) {
		delete p;
		p = 0;
	}
}

//namespace
};

#endif
