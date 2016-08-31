//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Matrix4XformH
#define Matrix4XformH
#include "Matrix4.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL>
Matrix4<REAL>& makeTranslationMtx(Matrix4<REAL>& output, const Vector3<REAL>& t) {
	output[ 0] = 1.0;
	output[ 1] = 0.0;
	output[ 2] = 0.0;
	output[ 3] = 0.0;

	output[ 4] = 0.0;
	output[ 5] = 1.0;
	output[ 6] = 0.0;
	output[ 7] = 0.0;

	output[ 8] = 0.0;
	output[ 9] = 0.0;
	output[10] = 1.0;
	output[11] = 0.0;

	output[12] = t[0];
	output[13] = t[1];
	output[14] = t[2];
	output[15] = 1.0;
	return output;
}

//[x,0,0,0]
//[0,y,0,0]
//[0,0,z,0]
//[0,0,0,1]
template<class REAL>
Matrix4<REAL>& makeScaleMtx(Matrix4<REAL>& output, const REAL& x, const REAL& y, const REAL& z) {
	output[ 0] = x;
	output[ 1] = 0.0;
	output[ 2] = 0.0;
	output[ 3] = 0.0;

	output[ 4] = 0.0;
	output[ 5] = y;
	output[ 6] = 0.0;
	output[ 7] = 0.0;

	output[ 8] = 0.0;
	output[ 9] = 0.0;
	output[10] = z;
	output[11] = 0.0;

	output[12] = 0.0;
	output[13] = 0.0;
	output[14] = 0.0;
	output[15] = 1.0;
	return output;
}

template<class REAL>
Matrix4<REAL>& makeScaleMtx(Matrix4<REAL>& output, const Vector3<REAL>& v) {
	return makeScaleMtx<REAL>(output,v.x(),v.y(),v.z());
}

template<class REAL>
Matrix4<REAL>& makeScaleTranslateMtx(Matrix4<REAL>& output, const Vector3<REAL>& s, const Vector3<REAL>& t) {
	makeTranslationMtx<REAL>(output,t);
	output(1,1) = s[0];
	output(2,2) = s[1];
	output(3,3) = s[2];
	return output;
}

template<class REAL>
Matrix4<REAL>& makeXRotationMtx(Matrix4<REAL>& output, const REAL& theta) {
	const REAL ct = cos(theta);
	const REAL st = sin(theta);

	output[ 0] = 1.0;
	output[ 1] = 0.0;
	output[ 2] = 0.0;
	output[ 3] = 0.0;

	output[ 4] = 0.0;
	output[ 5] = ct;
	output[ 6] = st;
	output[ 7] = 0.0;

	output[ 8] = 0.0;
	output[ 9] = -st;
	output[10] = ct;
	output[11] = 0.0;

	output[12] = 0;
	output[13] = 0;
	output[14] = 0;
	output[15] = 1.0;

	return output;
}

template<class REAL>
Matrix4<REAL>& makeYRotationMtx(Matrix4<REAL>& output, const REAL& theta) {
	const REAL ct = cos(theta);
	const REAL st = sin(theta);

	output[ 0] = ct;
	output[ 1] = 0.0;
	output[ 2] = -st;
	output[ 3] = 0.0;

	output[ 4] = 0.0;
	output[ 5] = 1.0;
	output[ 6] = 0.0;
	output[ 7] = 0.0;

	output[ 8] = st;
	output[ 9] = 0.0;
	output[10] = ct;
	output[11] = 0.0;

	output[12] = 0;
	output[13] = 0;
	output[14] = 0;
	output[15] = 1.0;

	return output;
}

template<class REAL>
Matrix4<REAL>& makeZRotationMtx(Matrix4<REAL>& output, const REAL& theta) {
	const REAL ct = cos(theta);
	const REAL st = sin(theta);

	output[ 0] = ct;
	output[ 1] = st;
	output[ 2] = 0.0;
	output[ 3] = 0.0;

	output[ 4] = -st;
	output[ 5] = ct;
	output[ 6] = 0.0;
	output[ 7] = 0.0;

	output[ 8] = 0.0;
	output[ 9] = 0.0;
	output[10] = 1.0;
	output[11] = 0.0;

	output[12] = 0.0;
	output[13] = 0.0;
	output[14] = 0.0;
	output[15] = 1.0;

	return output;
}

//make a scaleTranslate matrix that includes the two values vMin and vMax
template<class REAL>
Matrix4<REAL>& scaleTranslateToFit(Matrix4<REAL>& output, const Vector3<REAL>& vMin, const Vector3<REAL>& vMax) {
	const REAL diffX = vMax.x()-vMin.x();
	output[ 0] = 2/diffX;
	output[ 4] = 0;
	output[ 8] = 0;
	output[12] = -(vMax.x()+vMin.x())/diffX;

	const REAL diffY = vMax.y()-vMin.y();
	output[ 1] = 0;
	output[ 5] = 2/diffY;
	output[ 9] = 0;
	output[13] = -(vMax.y()+vMin.y())/diffY;

	const REAL diffZ = vMax.z()-vMin.z();
	output[ 2] = 0;
	output[ 6] = 0;
	output[10] = 2/diffZ;
	output[14] = -(vMax.z()+vMin.z())/diffZ;

	output[ 3] = 0;
	output[ 7] = 0;
	output[11] = 0;
	output[15] = 1;
	return output;
}

//make a scaleTranslate matrix that includes the two values vMin and vMax
//an infinit volume defaults to the identity matrix
template<class REAL>
Matrix4<REAL>& scaleTranslateToFitSave(Matrix4<REAL>& output, const Vector3<REAL>& vMin, const Vector3<REAL>& vMax) {
	const REAL diffX = vMax.x()-vMin.x();
	if(diffX > Math::Const<REAL>::epsilon()) { 
		output[ 0] = 2/diffX;
		output[ 4] = 0;
		output[ 8] = 0;
		output[12] = -(vMax.x()+vMin.x())/diffX;
	}
	else {
		output[ 0] = 1;
		output[ 4] = 0;
		output[ 8] = 0;
		output[12] = 0;
	}

	const REAL diffY = vMax.y()-vMin.y();
	if(diffY > Math::Const<REAL>::epsilon()) { 
		output[ 1] = 0;
		output[ 5] = 2/diffY;
		output[ 9] = 0;
		output[13] = -(vMax.y()+vMin.y())/diffY;
	}
	else {
		output[ 1] = 0;
		output[ 5] = 1;
		output[ 9] = 0;
		output[13] = 0;
	}

	const REAL diffZ = vMax.z()-vMin.z();
	if(diffZ > Math::Const<REAL>::epsilon()) { 
		output[ 2] = 0;
		output[ 6] = 0;
		output[10] = 2/diffZ;
		output[14] = -(vMax.z()+vMin.z())/diffZ;
	}
	else {
		output[ 2] = 0;
		output[ 6] = 0;
		output[10] = 1;
		output[14] = 0;
	}

	output[ 3] = 0;
	output[ 7] = 0;
	output[11] = 0;
	output[15] = 1;
	return output;
}

//output = look from position:pos into direction:dir with up-vector:up
template<class REAL>
Matrix4<REAL>& look(Matrix4<REAL>& output, const Vector3<REAL>& pos, const Vector3<REAL>& dir, const Vector3<REAL>& up) {
	typedef Vector3<REAL> V3;

	V3 dirN = dir;
	dirN.normalize();

	V3 upN = up;
	upN.normalize();

	V3 xN;
	xN.unitCross(dirN,upN);
	upN.unitCross(xN,dirN);

	output(1,1) = xN.x();
	output(2,1) = upN.x();
	output(3,1) = -dirN.x();
	output(4,1) = 0.0;

	output(1,2) = xN.y();
	output(2,2) = upN.y();
	output(3,2) = -dirN.y();
	output(4,2) = 0.0;

	output(1,3) = xN.z();
	output(2,3) = upN.z();
	output(3,3) = -dirN.z();
	output(4,3) = 0.0;

	output(1,4) = -xN.dot(pos);
	output(2,4) = -upN.dot(pos);
	output(3,4) = dirN.dot(pos);
	output(4,4) = 1.0;
	return output;
}


template<class REAL>
//output = look from position:pos onto point center with up-vector:up
Matrix4<REAL>& lookAt(Matrix4<REAL>& output, const Vector3<REAL>& eye, const Vector3<REAL>& center, const Vector3<REAL>& up) {
	return Math::look<REAL>(output,eye,center-eye,up);
}

//output = look from origin into direction:dir with up-vector:up, produces no ortho system
template<class REAL>
Matrix4<REAL>& rotateAt(Matrix4<REAL>& output, const Vector3<REAL>& dir, const Vector3<REAL>& up) {
	V3 lft;
	lft.unitCross(dir,up);

	output(1,1) =  lft.x(); output(1,2) =  lft.y(); output(1,3) =  lft.z(); output(1,4) = 0;
	output(2,1) =   up.x(); output(2,2) =   up.y(); output(2,3) =   up.z(); output(2,4) = 0;
	output(3,1) = -dir.x(); output(3,2) = -dir.y(); output(3,3) = -dir.z(); output(3,4) = 0;
	output(4,1) =        0; output(4,2) =        0; output(4,3) =        0; output(4,4) = 1;

	return output;
}


//output is initialized with the same result as glPerspective vFovy in rad
template<class REAL>
Matrix4<REAL>& perspectiveRadGL(Matrix4<REAL>& output, const REAL& vFovy, const REAL& vAspect,
					const REAL& vNearDis, const REAL& vFarDis) {
	const REAL f = Math::coTan(vFovy/REAL(2.0));
	const REAL dif = REAL(1.0)/(vNearDis-vFarDis);

	output(1,1) = f/vAspect;
	output(1,2) = 0;
	output(1,3) = 0;
	output(1,4) = 0;

	output(2,1) = 0;
	output(2,2) = f;
	output(2,3) = 0;
	output(2,4) = 0;

	output(3,1) = 0;
	output(3,2) = 0;
	output(3,3) = (vNearDis+vFarDis)*dif;
	output(3,4) = 2*vNearDis*vFarDis*dif;

	output(4,1) = 0;
	output(4,2) = 0;
	output(4,3) = -1;
	output(4,4) = 0;
	return output;
}

//output is initialized with the same result as glFrustum
template<class REAL>
void frustumGL(Matrix4<REAL>& output, const REAL& left, const REAL& right,
					const REAL& bottom, const REAL& top,
					const REAL& zNearDis, const REAL& zFarDis) {
	const REAL diffX = REAL(1.0)/(right-left);
	const REAL diffY = REAL(1.0)/(top-bottom);
	const REAL diffZ = REAL(1.0)/(zNearDis-zFarDis);

	output(1,1) = 2*zNearDis*diffX;
	output(1,2) = 0;
	output(1,3) = (right+left)*diffX;
	output(1,4) = 0;

	output(2,1) = 0;
	output(2,2) = 2*zNearDis*diffY;
	output(2,3) = (top+bottom)*diffY;
	output(2,4) = 0;

	output(3,1) = 0;
	output(3,2) = 0;
	output(3,3) = (zFarDis+zNearDis)*diffZ;
	output(3,4) = 2*zNearDis*zFarDis*diffZ;

	output(4,1) = 0;
	output(4,2) = 0;
	output(4,3) = -1;
	output(4,4) = 0;
}

//output is initialized with the same result as glPerspective would have on an identity matrix
//vFovy in degrees
template<class REAL>
Matrix4<REAL>& perspectiveDegGL(Matrix4<REAL>& output, const REAL& vFovy, const REAL& vAspect,
						  const REAL& vNearDis, const REAL& vFarDis) {
	return perspectiveRadGL(output,vFovy*Math::Const<REAL>::pi_180(),vAspect,vNearDis,vFarDis);
}

// v will be normalized // theta in radians
template<class REAL>
Matrix4<REAL>& rotationRad(Matrix4<REAL>& output, const REAL& theta, const Vector3<REAL>& v) {
	const REAL c = Math::cos(theta);
	const REAL c_1 = REAL(1.0)-c;
	const REAL s = Math::sin(theta);

	Vector3<REAL> vN(v);
	vN.normalize();

	const REAL& x = vN.x();
	const REAL& y = vN.y();
	const REAL& z = vN.z();
	
	const REAL xx = x*x;
	const REAL yy = y*y;
	const REAL zz = z*z;
	const REAL xy = x*y;
	const REAL xz = x*z;
	const REAL yz = y*z;

	const REAL xs = x*s;
	const REAL ys = y*s;
	const REAL zs = z*s;

	output[ 0] = xx*(c_1)+c;
	output[ 4] = xy*(c_1)-zs;
	output[ 8] = xz*(c_1)+ys;
	output[12] = 0;

	output[ 1] = xy*(c_1)+zs;
	output[ 5] = yy*(c_1)+c;
	output[ 9] = yz*(c_1)-xs;
	output[13] = 0;

	output[ 2] = xz*(c_1)-ys;
	output[ 6] = yz*(c_1)+xs;
	output[10] = zz*(c_1)+c;
	output[14] = 0;

	output[ 3] = 0;
	output[ 7] = 0;
	output[11] = 0;
	output[15] = 1;
	return output;
}

//output is initialized with the same result as glRotation would have on an identity matrix
template<class REAL>
Matrix4<REAL>& rotationDeg(Matrix4<REAL>& output, const REAL& alpha, const Vector3<REAL>& v) {
	return rotationRad(output,alpha*Math::Const<REAL>::pi_180(),v);
}


//namespace
}
#endif
