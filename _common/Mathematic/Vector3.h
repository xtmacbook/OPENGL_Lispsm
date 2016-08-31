//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Vector3H
#define Vector3H
//---------------------------------------------------------------------------
#include "Vector.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL = float>
class Vector3: public Vector<REAL,3> {
protected:
	typedef Vector<REAL,3> FATHER;
	typedef Vector3<REAL> V3;

	static inline const V3 setUnitX() throw() { return V3(1.0,0.0,0.0); }
	static inline const V3 setUnitY() throw() { return V3(0.0,1.0,0.0); }
	static inline const V3 setUnitZ() throw() { return V3(0.0,0.0,1.0); }

	void init(const REAL& x, const REAL& y, const REAL& z) { 
		array[0] = x;
		array[1] = y;
		array[2] = z;
	}

	void cross(V3& v, const V3& a, const V3& b) {
		v[0] =  det2x2(a[1],b[1],a[2],b[2]);
		v[1] = -det2x2(a[0],b[0],a[2],b[2]);
		v[2] =  det2x2(a[0],b[0],a[1],b[1]);
	}
public:

	Vector3() { /* no initialization -> performance */ }
	Vector3(const FATHER::FATHER& v): FATHER(v) { }
	Vector3(const Tuppel<REAL,2>& v, const REAL& vZ) { init(v[0],v[1],vZ); }
	Vector3(const REAL p[3]): FATHER(p) { } 
	Vector3(const V3& v): FATHER(v) { }
	Vector3(const REAL& x, const REAL& y, const REAL& z) { init(x,y,z); }

	V3& cross(const V3& a, const V3& b) {
		cross(*this,a,b);
		return *this;
	}

	const Vector3 cross(const Vector3& b) const { return Vector3().cross(*this,b); }

	V3& unitCross(const V3& a, const V3& b) {
		cross(a,b);
		normalize();
		return *this;
	}


	REAL& x() { return array[0]; }
	const REAL& x() const { return array[0]; }
	REAL& y() { return array[1]; }
	const REAL& y() const { return array[1]; }
	REAL& z() { return array[2]; }
	const REAL& z() const { return array[2]; }


	static inline const V3 UNIT_X() throw() { 
		const static V3 t = setUnitX();
		return t;
	}

	static inline const V3 UNIT_Y() throw() { 
		const static V3 t = setUnitY();
		return t;
	}

	static inline const V3 UNIT_Z() throw() { 
		const static V3 t = setUnitZ();
		return t;
	}

};

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

//namespace
};
#endif