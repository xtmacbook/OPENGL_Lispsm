#ifndef AACylinder_DMS_H
#define AACylinder_DMS_H
//---------------------------------------------------------------------------
#include <Mathematic/Vector3.h>
#include <Mathematic/Geometry/AABox.h>
//#include "Triangle2D.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

template<class REAL>
struct AACylinder {
	typedef Math::Vector3<REAL> V3;
	typedef AABox<REAL> AABox;
    REAL r, h_2;
    V3 center;
    //bool intersects(const Triangle2D&);
    bool intersects(const AABox&) const;
    AACylinder(const V3& vCenter, const REAL& vR, const REAL& vh_2):
        center(vCenter), r(vR), h_2(vh_2) { }
};

//namespace
}
//namespace
}
#endif
