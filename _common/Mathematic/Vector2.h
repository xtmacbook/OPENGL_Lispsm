//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Vector2H
#define Vector2H
//---------------------------------------------------------------------------
#include "Vector.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL = float>
class Vector2: public Vector<REAL,2> {
protected:
	typedef Vector<REAL,2> FATHER;
	typedef Vector2<REAL> V2;
public:

	Vector2() { /* no initialization -> performance */ }
	Vector2(const REAL p[2]): FATHER(p) { } 
	Vector2(const FATHER::FATHER& v): FATHER(v) { }
	Vector2(const V2& v): FATHER(v) { }
	Vector2(const REAL& x, const REAL& y) { 
		array[0] = x;
		array[1] = y;
	}

	REAL& x() { return array[0]; }
	const REAL& x() const { return array[0]; }
	REAL& y() { return array[1]; }
	const REAL& y() const { return array[1]; }

	const V2 getRightNormal() const { return V2(y(),-x()); }
	const V2 getLeftNormal() const { return V2(-y(),x()); }
	
	const REAL perpDot(const Vector2& v) const {
		const V2 normal = getLeftNormal();
		return normal.dotNorm(v);
	}

};

/*	void rotate(const V2& v) { 
		const REAL temp = dot(v.getRightNormal());
		y() = dot(vec);
		x() = temp;
	}
	REAL Vector2::angle(const Vector2& vec) const {
		REAL tmp = dot(vec);
		if(0.0 == tmp) {
			return Math::TReal::pi_2();//Acos(0)
		}
		tmp /= length()*vec.length();
		return Math::aCos(tmp);
	}
*/

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

//namespace
}
#endif