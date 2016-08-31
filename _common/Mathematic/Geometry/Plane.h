//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Plane_DMS_H
#define Plane_DMS_H
#include "../Mathematic.h"
#include "../Vector3.h"
#include "../Vector4.h"
#include "../Matrix4.h"
#include "Line.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

enum PlaneSide { BEFORE, BEHIND, CROSS };

//Plane defined through normal vector n and signed distance to origin d
//we use n dot p = d
template<class REAL = float>
class Plane {
protected:
	typedef Vector3<REAL> V3;
	typedef Vector4<REAL> V4;
	typedef Matrix4<REAL> M4;
	typedef Line<REAL> Line;
	typedef Plane<REAL> P;
	V3 n;
	REAL d;
public:
	// construction
	Plane(const V3& vN, const REAL& vD): 
		n(vN), d(vD) { hessNorm(); }

	Plane(const REAL& vA, const REAL& vB, const REAL& vC, const REAL& vD): 
		n(vA,vB,vC), d(vD) { hessNorm(); }

	Plane(const REAL abcd[4]): 
		n(abcd[0],abcd[1],abcd[2]), d(abcd[3]) { hessNorm(); }

	//assume ccw
	Plane(const V3& a, const V3& b, const V3& c) {
		n.cross(b-a,c-b);
		n.normalize();
		d = n.dot(a);
	}

	Plane(const V3& p, const V3& vN) {	
		n = vN;
		n.normalize();
		d = n.dot(p);
	}

	inline bool operator==(const Plane&) const { return p.n == n && p.d == d; }
	inline bool operator!= (const Plane&) const { return !operator==(p); }

	inline const V3& getN() const { return n; }
	inline const REAL& getD() const { return d; } 
	inline V4 getHomogenEquation() const { return V4(n,-d); }
	// d has different meanings in different math books!!
	// because sometimes a plane is defined as [n dot p + d = 0] or [n dot p = d]
	// so better avoid code which depends on d
	inline void hessNorm() {
		REAL t = n.squaredLength();
		if(t != 1.0f) {
			t = Math::sqrt(t);
			n /= t;
			d /= t;
		}
		//hessNorm is not unique, but argument below switches n vector for certain planes
/*		if(0.0 > d) {
			n.negate();
			d = -d;
		}*/
	}

	inline REAL distance(const V3& p) const {
		return distance(p[0],p[1],p[2]);
	}

	inline REAL distance(const REAL& x, const REAL& y, const REAL& z) const {
		return n[0]*x+n[1]*y+n[2]*z-d;
	}

	//returns the plane with the normal vector pointing in the antiparallel direction
	inline const P getInverted() const {
		return P(-getN(),-getD());
	}

	inline void transform(const M4& invTranSpace) { //move to separate transform class?
		const V4 p = invTranSpace*getHomogenEquation();
		n = p.toVector3();
		//our d is -d of homogen equation
		d = -p.w();
		hessNorm();
	}

	inline bool intersection(const Line& l, REAL& t, const REAL eps = Const<REAL>::near_epsilon()) const {
		const REAL a = getN().dot(l.dir);
		if(abs(a) < eps) { //edge and plane are parallel
			return false;
		}
		t = (getD()-getN().dot(l.p))/a;
		//-> plane and line intersect
		return true;
	}

	inline bool intersectionWithEdge(const Line& l, REAL& t, const REAL eps = Const<REAL>::near_epsilon()) const {
		if(intersection(l,t,eps)) {
			if(t < -eps || 1.0+eps < t)
				return false; //intersection is outside of edge
		}
		else {
			return false;
		}
		//-> plane and edge intersect
		return true;
	}

	inline bool intersectionWithEdge(V3& output, const V3& a, const V3& b) const {
		const Line l(a,b-a);
		REAL t;
		if(intersectionWithEdge(l,t)) {
			output = l.getPoint(t);
			return true;
		}
		else {
			return false;
		}
	}

	inline const bool planeBehindPoint(const V3& p, const REAL eps = Const<REAL>::near_epsilon()) const {	return distance(p) > eps;	} //plane behind point
	inline const bool planeInFrontPoint(const V3& p, const REAL eps = Const<REAL>::near_epsilon()) const { return distance(p) < -eps; } //plane in front of point
	
	inline const PlaneSide getPlaneSide(const V3& p) const {
		const REAL dist = distance(p);
		if(dist < 0.0) {
			return BEHIND;
		}
		else {
			if(dist > 0.0) {
				return BEFORE;
			}
			else {
				return CROSS;
			}
		}
	}

};

//namespace
}
//namespace
}
#endif