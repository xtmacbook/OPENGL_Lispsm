//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Vector4H
#define Vector4H
//---------------------------------------------------------------------------
#include "Vector.h"

namespace Math {

template<class REAL = float>
class Vector4: public Vector<REAL,4> {
public:
	typedef Vector<REAL,4> FATHER;
	typedef Vector4<REAL> V4;

protected:
	static inline const V4 setUnitX() throw() { return V4(1.0,0.0,0.0,0.0); } 
	static inline const V4 setUnitY() throw() { return V4(0.0,1.0,0.0,0.0); } 
	static inline const V4 setUnitZ() throw() { return V4(0.0,0.0,1.0,0.0); } 
	static inline const V4 setUnitW() throw() { return V4(0.0,0.0,0.0,1.0); } 

	void init(const REAL x, const REAL y, const REAL z, const REAL w) { 
		array[0] = x;
		array[1] = y;
		array[2] = z;
		array[3] = w;
	}

public:
	Vector4() { /* no initialization -> performance */ }
	Vector4(const REAL x, const REAL y, const REAL z, const REAL w) { init(x,y,z,w); }
	Vector4(const REAL p[4]): FATHER(p) { } 
	Vector4(const V4& v): FATHER(v) { }
	Vector4(const Tuppel<REAL,4>& v): FATHER(v) { }
	Vector4(const Vector<REAL,3>& v, const REAL& r) { init(v[0],v[1],v[2],r); }

	Vector4& makeHomogen() { operator/=(array[4]); return *this; }
	
	Vector<REAL,3> toVector3() const { return Vector<REAL,3>(array); }


	REAL& x() { return array[0]; }
	const REAL& x() const { return array[0]; }
	REAL& y() { return array[1]; }
	const REAL& y() const { return array[1]; }
	REAL& z() { return array[2]; }
	const REAL& z() const { return array[2]; }
	REAL& w() { return array[3]; }
	const REAL& w() const { return array[3]; }


	static inline const V4 UNIT_X() throw() { 
		const static V4 t = setUnitX();
		return t;
	}

	static inline const V4 UNIT_Y() throw() { 
		const static V4 t = setUnitY();
		return t;
	}

	static inline const V4 UNIT_Z() throw() { 
		const static V4 t = setUnitZ();
		return t;
	}

	static inline const V4 UNIT_W() throw() { 
		const static V4 t = setUnitW();
		return t;
	}

};

/*inline Vector4 operator*(const Matrix4& m, const Vector4& v) {
	return Vector4().mult(m,v);
}
*/

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

//namespace
};
#endif