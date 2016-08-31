//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef VectorH
#define VectorH
//---------------------------------------------------------------------------
#include "Tuppel.h"

namespace Math {

template<class REAL = float, const unsigned SIZE = 3>
struct Vector : public Tuppel<REAL,SIZE> {
	typedef Tuppel<REAL,SIZE> FATHER;
	typedef Vector<REAL,SIZE> V;
	// construction
	Vector() { /* no initialization -> performance */ }
	Vector(const FATHER& v) : FATHER(v) { }
	Vector(const V& v): FATHER(v) { }
	Vector(const REAL p[SIZE]): FATHER(p) { }

	V& invert() { 
		for(unsigned i = 0; i < SIZE; i++)
			array[i] = -array[i];
		return *this; 
	}

	REAL dot(const V& v) const {
		REAL result = array[0]*v[0];
		for(unsigned i = 1; i < SIZE; i++)
			result += array[i]*v[i];
		return result;
	}

	REAL squaredLength() const { return dot(*this); }
	REAL length() const { return sqrt(squaredLength()); }
	V& normalize() { operator/=(length()); return *this; }

	REAL unitize(const REAL& rTolerance = 1e-06) {
		REAL rLength = length();
		if(rLength > rTolerance) {
			operator/=(rLength);
		}
		else {
			rLength = 0.0;
		}
		return rLength;
	}

	const REAL dotNorm(const V& v) const {
		REAL tmp = dot(v);
		if(REAL(0.0) != tmp) {
			tmp /= length()*v.length();
		}
		return tmp;
	}

};

//namespace
}
#endif