//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
#ifndef VecVector_DMS_H
#define VecVector_DMS_H
//---------------------------------------------------------------------------
#include "Vector2.h"
#include "Vector3.h"
//---------------------------------------------------------------------------
namespace Math {

//for Boxes:
//     7+------+6
//     /|     /|
//    / |    / |
//   / 4+---/--+5
// 3+------+2 /    y   z
//  | /    | /     |  /
//  |/     |/      |/
// 0+------+1      *---x


template<class TUPPEL, class REAL>
inline void calcRectangularHull(const TUPPEL<Vector2<REAL> >& ps, Vector2<REAL>& min, Vector2<REAL>& max) {
	if(ps.size() > 0) {
		max = min = ps[0];
		for(unsigned i = 1; i < ps.size(); i++) {
			const Vector2<REAL>& p = ps[i];
			for(unsigned j = 0; j < 2; j++) {
				if(p[j] < min[j]) {
					min[j] = p[j];
				}
				else if(p[j] > max[j]) {
					max[j] = p[j];
				}
			}
		}
	}
}

template<class TUPPEL, class REAL>
inline void calcCubicHull(const TUPPEL<Vector3<REAL> >& ps, Vector3<REAL>& min, Vector3<REAL>& max) {
	if(ps.size() > 0) {
		max = min = ps[0];
		for(unsigned i = 1; i < ps.size(); i++) {
			const Vector3<REAL>& p = ps[i];
			for(unsigned j = 0; j < 3; j++) {
				if(p[j] < min[j]) {
					min[j] = p[j];
				}
				else if(p[j] > max[j]) {
					max[j] = p[j];
				}
			}
		}
	}
}

/*template<class TUPPEL, class REAL>
inline bool alike(const TUPPEL<Vector3<REAL> >& a_, const TUPPEL<Vector3<REAL> >& b_) {
	if(a_.size() != b_.size()) {
		return false;
	}

	TUPPEL::const_iterator itA = begin();
	TUPPEL::const_iterator itb = begin();
}*/

//namespace
}
#endif
