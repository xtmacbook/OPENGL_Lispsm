//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef MathIOH
#define MathIOH

#include "Vector.h"
#include <iostream>
#include <Base/StreamTools.h>
#include "Vector4.h"
#include "Matrix4.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL, unsigned SIZE> 
std::ostream& operator<<(std::ostream& s, const Tuppel<REAL,SIZE>& input) {
	return Tools::tuppelOut(s,input);
}

template<class REAL, unsigned SIZE> 
std::istream& operator>>(std::istream& s, Tuppel<REAL,SIZE>& output) {
	Tools::tuppelIn(s,output);
	return s;
}

template<class REAL>
std::ostream& fineOut(std::ostream& s, const Matrix4<REAL>& m) {
	for(unsigned r = 1; r <= m.rowCount(); r++) {
		Vector4<REAL> v(m(r,1),m(r,2),m(r,3),m(r,4));
		s << v << '\n';
	}
	return s;
}

//namespace
};
#endif
