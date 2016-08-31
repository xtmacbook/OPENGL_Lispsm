//---------------------------------------------------------------------------
#pragma hdrstop
#include "AACylinder.h"
//#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

/*bool AACylinder::intersects(const Triangle2D& tri) {
    if(tri.contains(Vector2(base.x,base.z))) {
        return true;
    }
    //todo: the rest
    return false;
}*/

template<class REAL>
bool AACylinder<REAL>::intersects(const AABox& box) const {
	const V3& min = box.getMin();
	const V3& max = box.getMax();
	REAL d = 0.0;
	REAL distX = center.x()-min.x();
	if(distX < 0.0) {
		distX *= distX;
		d += distX;
	}
	else {
		distX = center.x()-max.x();
		if(distX > 0.0) {
			distX *= distX;
			d += distX;
		}
	}

	REAL distZ = center.z()-min.z();
	if(distZ < 0.0) {
		distZ *= distZ;
		d += distZ;
	}
	else {
		distZ = center.z()-max.z();
		if(distZ > 0.0) {
			distZ *= distZ;
			d += distZ;
		}
	}
	
	if(d > r*r) {
		return false;
	}

	if(center.y()-h_2 > max.y()) {
		return false;
	}
	if(center.y()+h_2 < min.y()) {
		return false;
	}
	return true;
}

//namespace
}
//namespace
}
