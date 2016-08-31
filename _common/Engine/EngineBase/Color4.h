//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Color4H
#define Color4H
//---------------------------------------------------------------------------
#include "Color.h"
//---------------------------------------------------------------------------
template<class REAL>
class Color4 : public Color<REAL,4> {
	void init(const REAL& vR, const REAL& vG, const REAL& vB, const REAL& vA) { 
		array[0] = vR;
		array[1] = vG;
		array[2] = vB;
		array[3] = vA;
	}

public:
	typedef Color<REAL,4> FATHER;
	typedef Color4<REAL> C4;
	// components in range [0,1]
	Color4(const REAL& vR, const REAL& vG, const REAL& vB, const REAL& vA) { 
		init(vR,vG,vB,vA);
	}

	Color4(const FATHER& rgba): FATHER(rgba) { }
	Color4(const REAL rgba[4]): FATHER(rgba) { }
	Color4(const Color<REAL,3>& rgb, const REAL& a) { init(rgb[0],rgb[1],rgb[2],a); }

	REAL& r() { return array[0]; }
	REAL& g() { return array[1]; }
	REAL& b() { return array[2]; }
	REAL& a() { return array[3]; }
};

typedef Color4<float> Color4f;
typedef Color4<double> Color4d;

#endif
