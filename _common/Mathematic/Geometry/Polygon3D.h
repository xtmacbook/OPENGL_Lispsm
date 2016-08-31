//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
#ifndef Polygon3DH
#define Polygon3DH
//---------------------------------------------------------------------------
#include <vector>
#include "../Vector3.h"
#include "../Matrix4.h"
#include "Plane.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

//vector of 3d points
template<class REAL>
class VecVector3 : public std::vector<Vector3<REAL> > {
	typedef Vector3<REAL> V3;
	typedef Matrix4<REAL> M4;
	typedef Plane<REAL> Plane;

	// mulHomogenPoint each point of VecPoint into new VecPoint
	void transform(VecVector3& polyOut, const VecVector3& polyIn, const M4& xForm) {
		polyOut.resize(polyIn.size());
		for(unsigned i = 0; i < polyIn.size(); i++) {
			polyOut[i] = xForm.mulHomogenPoint(polyIn[i]);
		}
	}

public:
	VecVector3() { }

	VecVector3(const VecVector3& vec, const M4& xForm) {
		transform(*this,vec,xForm);
	}

	VecVector3& operator*=(const M4& xForm) {
		for(iterator i = begin(); i != end(); i++) {
			*i = xForm.mulHomogenPoint(*i);
		}
		return *this;
	}

	const int findAlikePointID(const V3& p) {
		for(unsigned i = 0; i < size(); i++) {
			if(operator[](i).alike(p,(REAL)0.001)) {
				return i;
			}
		}
		return -1;
	}

	//the polygon is clipped to the part behind the plane (positive half-space is clip-space)
	void clipAsPolygonByPlane(const Plane& A, VecVector3& polyOut, VecVector3& interPts) const {
		if(size() < 3) {
			return;
		}
		DynamicArray<bool> outside(size());
		//for each point mark the points in front of the plane
		//the points that will be clipped away
		for(unsigned i = 0; i < size(); i++) {
			outside[i] = A.planeBehindPoint(operator[](i));
		}
		for(unsigned i = 0; i < size(); i++) {
			//id of the second point of the edge
			const unsigned idNext = (i+1) % size();
			//both outside -> save none
			if(outside[i] && outside[idNext]) {
				continue;
			}
			//outside to inside -> calc intersection save intersection and save i+1
			if(outside[i]) {
				V3 inter;
				if(A.intersectionWithEdge(inter,operator[](i),operator[](idNext))) {
					polyOut.push_back(inter);
					interPts.push_back(inter);
				}
				polyOut.push_back(operator[](idNext));
				continue;
			}
			//inside to outside -> calc intersection save intersection
			if(outside[idNext]) {
				V3 inter;
				if(A.intersectionWithEdge(inter,operator[](i),operator[](idNext))) {
					polyOut.push_back(inter);
					interPts.push_back(inter);
				}
				continue;
			}
			//both inside -> save point i+1
			polyOut.push_back(operator[](idNext));
		}
	}

};


template<class REAL>
struct Polygon3D {
	typedef Vector3<REAL> V3;
	typedef VecVector3<REAL> VecVec3;
	
	VecVec3 points;
	const V3 getNormal() const {
		if(points.size() < 3) {
			return V3::ZERO();
		}
		const V3 a = points[1]-points[0];
		const V3 b = points[2]-points[1];
		V3 n;
		n.unitCross(a,b);
		return n;
	}


};

//template<class REAL = float>
//class Polygon3D {
//protected:
//	typedef Vector2<REAL> V2;
//public:
//	typedef std::vector<V2> VecPoint;
//	VecPoint points;
//
//	void check() const {
//		if(points.size() < 3) {
//			throw GeometryException("Polygon2D with less than 3 points");
//		}
//	}
//
//	bool inside(const V2& p) {
//		return false;
//	}
//};

//namespace
}
//namespace
}
#endif
