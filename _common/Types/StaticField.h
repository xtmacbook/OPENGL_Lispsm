//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#ifndef StaticFieldH
#define StaticFieldH
#include "StaticArray.h"
//---------------------------------------------------------------------------
template<class T, const unsigned int X, const unsigned int Y> 
class StaticField : public StaticArray<T,X*Y> {
public:
	const T& operator()(const unsigned x, const unsigned y) const { return data[x+X*y]; }
	T& operator()(const unsigned x, const unsigned y) { return data[x+X*y]; }

	const unsigned int sizeX() const { return X; }
	const unsigned int sizeY() const { return Y; }

};

#endif


