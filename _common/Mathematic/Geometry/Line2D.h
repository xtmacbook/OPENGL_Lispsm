//---------------------------------------------------------------------------
#ifndef Line2DH
#define Line2DH
#include <Mathematic/Vector2.h>
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

template<class REAL>
struct Line2D {
	typedef Math::Vector2<REAL> V2;
	V2 p;
	V2 dir;

	// construction
	Line2D() { /* no initialization -> performance */ }
	Line2D(const V2& vP, const V2& vDirection): p(vP), dir(vDirection) { }

	bool operator==(const Line2D& l) const { return p == l.p && dir == l.dir; }
	bool operator!= (const Line2D& l) const { return !operator==(l); }


	V2 getPoint(const REAL& t) const { return p+dir*t; }
	V2 getLeftN() const { return V2(-dir.y(),dir.x()); }
	V2 getRightN() const { return V2(dir.y(),-dir.x()); }
	bool leftofLine(const V2& point) const {
		const V2& leftN = getLeftN();
		const V2 vec(point-p);
		if(vec.dot(leftN) > REAL(0.0)) {
			return true;
		}
		return false;
	}

	const static Line2D ZERO;
};


//namespace
}
//namespace
}
#endif