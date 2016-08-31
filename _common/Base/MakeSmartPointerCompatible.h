#ifndef MakeSmartPointerCompatibleH
#define MakeSmartPointerCompatibleH
//---------------------------------------------------------------------------
#include "Base.h"
#include "SmartPointer.h"
//---------------------------------------------------------------------------
template<class T> class MakeSmartPointerCompatible : public Base {
public:
    T object;
	MakeSmartPointerCompatible(const T& obj): object(obj) { }
};

#endif
