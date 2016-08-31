//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#ifndef Vector3XformH
#define Vector3XformH
//---------------------------------------------------------------------------
#include <Mathematic/Vector2.h>
#include <Mathematic/Vector3.h>
//---------------------------------------------------------------------------
namespace Math {

template<class REAL, unsigned AXE>
inline void rotate(Vector3<REAL>& output, const Vector3<REAL>& input, const REAL& cosR, const REAL& sinR) {
	//possible input == output so exact order needed
	const unsigned next = (AXE+1) % 3;
	const unsigned prior = (AXE+2) % 3; //-1 is possibly dangerous because of unsigned
	const REAL tempNext = input[next]*cosR-input[prior]*sinR;
	output[AXE] = input[AXE];
	output[prior] = input[next]*sinR+input[prior]*cosR;
	output[next] = tempNext;
}

template<class REAL, unsigned AXE>
inline void rotate(Vector3<REAL>& v, const REAL& angle) {
	const REAL rad = Math::deg2Rad(angle);
	const REAL cosR = Math::cos(rad);
	const REAL sinR = Math::sin(rad);
	rotate<REAL,AXE>(v,v,cosR,sinR);
}

//todo*********: dirIn calc one time use coord system for repetitive computations
template <class REAL>
inline Vector2<REAL> make2D(const Vector3<REAL>& dirIn, const Vector3<REAL>& p) {
	typedef Vector3<REAL> V3;
	V3 dir(dirIn);
	dir.unitize();
	V3 up;
	if(dir.z() == 0) {
		up = V3(dir.y(),-dir.x(),0);
	}
	else {
		up = V3(0,dir.z(),-dir.y());
	}
	V3 l;
	l.unitCross(dir,up);
	return Vector2<REAL>(l.dot(p),up.dot(p));
}

//namespace
}
#endif
