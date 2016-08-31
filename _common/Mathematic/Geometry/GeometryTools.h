//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
#ifndef GeometryToolsH
#define GeometryToolsH
//---------------------------------------------------------------------------
#include "../Matrix4.h"
#include "../Matrix4Xform.h"
#include "Plane.h"
#include "Frustum.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

//make a scaleTranslate matrix that includes the given AABox
template<class REAL>
inline const Matrix4<REAL>& scaleTranslateToFit(Matrix4<REAL>& output, const AABox<REAL>& box) {
	return scaleTranslateToFit(output,box.getMin(),box.getMax());
}

//needs an OpenGL style proj*camera matrix to work (coordinate change!)
//proj*camera matrix is used to calculate the six planes of this frustum 
//add them to the given Frustum class
template <class REAL>
inline void addGlProjCamPlanes(Frustum<REAL>& f, const Matrix4<REAL>& glProjCam) {
	typedef Plane<REAL> Plane;
	const Matrix4<REAL>& m = glProjCam;
	// the n vectors are pointing inside
	// Extract the NEAR plane 
	f.addClipPlane(Plane(m(4,1)+m(3,1), m(4,2)+m(3,2), m(4,3)+m(3,3), -m(3,4)-m(4,4)));
	// Extract the FAR plane
	f.addClipPlane(Plane(m(4,1)-m(3,1), m(4,2)-m(3,2), m(4,3)-m(3,3), m(3,4)-m(4,4)));
	// Extract the numbers for the LEFT plane
	f.addClipPlane(Plane(m(4,1)+m(1,1), m(4,2)+m(1,2), m(4,3)+m(1,3), -m(1,4)-m(4,4)));
	// Extract the numbers for the RIGHT plane
	f.addClipPlane(Plane(m(4,1)-m(1,1), m(4,2)-m(1,2), m(4,3)-m(1,3), m(1,4)-m(4,4)));
	// Extract the BOTTOM plane
	f.addClipPlane(Plane(m(4,1)+m(2,1), m(4,2)+m(2,2), m(4,3)+m(2,3), -m(2,4)-m(4,4)));
	// Extract the TOP plane
	f.addClipPlane(Plane(m(4,1)-m(2,1), m(4,2)-m(2,2), m(4,3)-m(2,3), m(2,4)-m(4,4)));
}

//proj*camera matrix is used to calculate the six planes of this frustum 
//and add them to the given Frustum class
template <class REAL>
inline void addProjCamPlanes(Frustum<REAL>& f, const Matrix4<REAL>& projCam) {
	typedef Plane<REAL> Plane;
	const Matrix4<REAL>& m = projCam;
	// the n vectors are pointing inside
	// Extract the NEAR plane 
	f.addClipPlane(Plane(m(4,1)-m(3,1), m(4,2)-m(3,2), m(4,3)-m(3,3), m(3,4)-m(4,4)));
	// Extract the FAR plane
	f.addClipPlane(Plane(m(4,1)+m(3,1), m(4,2)+m(3,2), m(4,3)+m(3,3), -m(3,4)-m(4,4)));
	// Extract the numbers for the LEFT plane
	f.addClipPlane(Plane(m(4,1)+m(1,1), m(4,2)+m(1,2), m(4,3)+m(1,3), -m(1,4)-m(4,4)));
	// Extract the numbers for the RIGHT plane
	f.addClipPlane(Plane(m(4,1)-m(1,1), m(4,2)-m(1,2), m(4,3)-m(1,3), m(1,4)-m(4,4)));
	// Extract the BOTTOM plane
	f.addClipPlane(Plane(m(4,1)+m(2,1), m(4,2)+m(2,2), m(4,3)+m(2,3), -m(2,4)-m(4,4)));
	// Extract the TOP plane
	f.addClipPlane(Plane(m(4,1)-m(2,1), m(4,2)-m(2,2), m(4,3)-m(2,3), m(2,4)-m(4,4)));
}

//namespace
}
//namespace
}
#endif
