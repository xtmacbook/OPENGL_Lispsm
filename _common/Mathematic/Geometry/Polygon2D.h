//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
#ifndef Polygon2DH
#define Polygon2DH
//---------------------------------------------------------------------------
#include <vector>
#include "../Vector2.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

template<class REAL = float>
class Polygon2D {
protected:
	typedef Vector2<REAL> V2;
public:
	typedef std::vector<V2> VecPoint;
	VecPoint points;

	void check() const {
		if(points.size() < 3) {
			throw GeometryException("Polygon2D with less than 3 points");
		}
	}

	bool inside(const V2& p) {
		return false;
	}
};

//namespace
}
//namespace
}
#endif
