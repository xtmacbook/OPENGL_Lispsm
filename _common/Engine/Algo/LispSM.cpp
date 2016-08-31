//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#include <Mathematic/MathTools.h>
#include <Mathematic/Matrix4Xform.h>
#include <Mathematic/Geometry/GeometryTools.h>
#include "LispSM.h"

//we search the point in the LVS volume that is nearest to the camera
template<class REAL>
Math::Vector3<REAL> LispSM<REAL>::getNearCameraPointE(const ILispSMSSD& fs) const {
	const M4& eyeView = fs.getEyeView();
	VecVec3 LVS;
	calcLVS(LVS,fs);
	//the LVS volume is always in front of the camera
	LVS *= eyeView;
	//the camera points along the neg z axis.
	//-> so the nearest point is the maximum
	unsigned max = 0;
	for(unsigned i = 1; i < LVS.size(); i++) {
		if(LVS[max].z() < LVS[i].z()) {
			max = i;
		}
	}
	//transform back to world space
	return eyeView.getInverse().mulHomogenPoint(LVS[max]);
}

//z0 is the point that lies on the parallel plane to the near plane through e (A)
//and on the near plane of the C frustum (the plane z = bZmax) and on the line x = e.x
template<class REAL>
Math::Vector3<REAL> LispSM<REAL>::getZ0_ls(const M4& lightSpace, const V3& e, const REAL& b_lsZmax, const V3& eyeDir) const {
	//to calculate the parallel plane to the near plane through e we 
	//calculate the plane A with the three points
	Plane A = Plane(e,eyeDir);
	//to transform plane A into lightSpace calculate transposed inverted lightSpace	
	const M4 invTlightSpace = lightSpace.getInverse().transpose();
	//and transform the plane equation with it
	A.transform(invTlightSpace);
	//get the parameters of A from the plane equation n dot d = 0
	const REAL d = A.getD();
	const V3 n = A.getN();
	//transform to light space
	const V3 e_ls = lightSpace.mulHomogenPoint(e);
	//z_0 has the x coordinate of e, the y coord of B.max() 
	//and the z coord of the plane intersection
	return V3(e_ls.x(),(d-n.z()*b_lsZmax-n.x()*e_ls.x())/n.y(),b_lsZmax);
}

template<class REAL>
REAL LispSM<REAL>::calcNoptGeneral(const M4 lightSpace, const AABox& B_ls, const ILispSMSSD& fs) const {
	const M4& eyeView = fs.getEyeView();
	const M4 invLightSpace(lightSpace.getInverse());

	const V3 z0_ls = getZ0_ls(lightSpace,getNearCameraPointE(fs),B_ls.getMax().z(),fs.getEyeDir());
	const V3 z1_ls = V3(z0_ls.x(),z0_ls.y(),B_ls.getMin().z());
	
	//to world
	const V3 z0_ws = invLightSpace.mulHomogenPoint(z0_ls);
	const V3 z1_ws = invLightSpace.mulHomogenPoint(z1_ls);

	//to eye
	const V3 z0_cs = eyeView.mulHomogenPoint(z0_ws);
	const V3 z1_cs = eyeView.mulHomogenPoint(z1_ws);

	const REAL z0 = z0_cs.z();
	const REAL z1 = z1_cs.z();
	const REAL d = Math::abs(B_ls.getMax().z()-B_ls.getMin().z());

	return d/( Math::sqrt(z1/z0)-REAL(1.0) );
}

template<class REAL>
REAL LispSM<REAL>::calcNoptOld(const ILispSMSSD& fs, const REAL gamma_ = 999) const {
	using namespace Math;
	const REAL& n = fs.getNearDist();
	const REAL& f = fs.getFarDist();
	const REAL d = abs(f-n);
	REAL sinGamma(0);
	if(999 == gamma_) {
		sinGamma = sin(abs(aCos(fs.getEyeDir().dot(fs.getLightDir()))));
	}
	else {
		sinGamma = sin(gamma_);
	}

	return (n+sqrt(n*(n+d*sinGamma)))/sinGamma;
}

template<class REAL>
REAL LispSM<REAL>::getN(const M4 lightSpace, const AABox& B_ls) const {
	if(mLispSMSSD.useFormula()) {
		return calcNoptGeneral(lightSpace,B_ls,mLispSMSSD); 
	}
	else {
		return mLispSMSSD.getN();
	}
}

//this is the algorithm discussed in the article
template<class REAL>
Math::Matrix4<REAL> LispSM<REAL>::getLispSmMtx(const M4& lightSpace, const VecVec3& B, ILispSMSSD& fs) const {
	const AABox B_ls(B,lightSpace);

	const REAL n = getN(lightSpace,B_ls);
	fs.setN(n);

	//get the coordinates of the near camera point in light space
	const V3 e_ls = lightSpace.mulHomogenPoint(getNearCameraPointE(fs));
	//c start has the x and y coordinate of e, the z coord of B.min() 
	const V3 Cstart_lp(e_ls.x(),e_ls.y(),B_ls.getMax().z());

	if(n >= Math::Const<REAL>::infinity()) {
		//if n is inf. than we should do uniform shadow mapping
		return M4::IDENTITY();
	}
	//calc C the projection center
	//new projection center C, n behind the near plane of P
	//we work along a negative axis so we transform +n*<the positive axis> == -n*<neg axis>
	const V3 C(Cstart_lp+n*V3::UNIT_Z());
	//construct a translation that moves to the projection center
	const M4 projectionCenter = Math::makeTranslationMtx<REAL>(M4(),-C);

	//calc d the perspective transform depth or light space y extents
	const REAL d = Math::abs(B_ls.getMax().z()-B_ls.getMin().z());

	//the lispsm perspective transformation
	M4 P;
	//here done with a standard frustum call that maps P onto the unit cube with
	//corner points [-1,-1,-1] and [1,1,1].
	//in directX you can use the same mapping and do a mapping to the directX post-perspective cube
	//with corner points [-1,-1,0] and [1,1,1] as the final step after all the shadow mapping.
	Math::frustumGL<REAL>(P,-1.0,1.0,-1.0,1.0,n,n+d);
	//invert the transform from right handed into left handed coordinate system for the ndc
	//done by the openGL style frustumGL call
	//so we stay in a right handed system
	P = Math::makeScaleMtx<REAL>(M4(),1.0,1.0,-1.0)*P;
	//return the lispsm frustum with the projection center
	return P*projectionCenter;
}

template<class REAL>
Math::Vector3<REAL> LispSM<REAL>::getProjViewDir_ls(const M4& lightSpace, const ILispSMSSD& fs) const {
	//get the point in the LVS volume that is nearest to the camera
	const V3 e = getNearCameraPointE(fs);
	//construct edge to transform into light-space
	const V3 b = e+fs.getEyeDir();
	//transform to light-space
	const V3 e_lp = lightSpace.mulHomogenPoint(e);
	const V3 b_lp = lightSpace.mulHomogenPoint(b);
	V3 projDir(b_lp-e_lp);
	//project the view direction into the shadow map plane
	projDir.y() = 0.0;
	return projDir;
}

template<class REAL>
void LispSM<REAL>::updateLightMtx(M4& lightView, M4& lightProj, const VecVec3& B, ILispSMSSD& fs) const {
	//calculate standard light space for spot or directional lights
	//this routine returns two matrices: 
	//lightview contains the rotated translated frame
	//lightproj contains in the case of a spot light the spot light perspective transformation
	//in the case of a directional light a identity matrix
	calcLightSpace(lightView,lightProj,fs);

	if(0 == B.size()) {
		//fs.debug() << "empty intersection body -> completely inside shadow\n";//debug output
		return;
	}
	//coordinate system change for calculations in the article
	M4 switchToArticle(M4::ZERO());
	switchToArticle(1,1) = 1.0;
	switchToArticle(2,3) = -1.0; // y -> -z
	switchToArticle(3,2) = 1.0;  // z -> y
	switchToArticle(4,4) = 1.0;
	//switch to the lightspace used in the article
	lightProj = switchToArticle*lightProj;
//Math::fineOut(fs.debug(),M4(lightProj*lightView));
	const M4 L = lightProj*lightView; 
//fs.debug() << '\n';
	V3 projViewDir = getProjViewDir_ls(L,fs); 
//	frameState.debug() << std::fixed << projViewDir.normalize() << std::endl;
	if(//projViewDir.z() < 0 &&
	fs.useLiSPSM()) {
		//do Light Space Perspective shadow mapping
		//rotate the lightspace so that the proj light view always points upwards
		//calculate a frame matrix that uses the projViewDir[light-space] as up vector
		//look(from position, into the direction of the projected direction, with unchanged up-vector)
		lightProj = Math::look<REAL>(M4(),V3::ZERO(),projViewDir,V3::UNIT_Y())*lightProj;
		M4 lispsm = getLispSmMtx(lightProj*lightView,B,fs);
		lightProj = lispsm*lightProj;
	}
	const M4 PL = lightProj*lightView; 
	//map to unit cube
	lightProj = scaleTranslateShadowMtx(PL,B)*lightProj;

	//coordinate system change for calculations in the article
	M4 switchToGL(M4::ZERO());
	switchToGL(1,1) = 1.0;
	switchToGL(2,3) = 1.0; // y -> z
	switchToGL(3,2) = -1.0;  // z -> -y
	switchToGL(4,4) = 1.0;

	//back to open gl coordinate system y <-> z
	lightProj = switchToGL*lightProj;
	//transform from right handed system into left handed ndc
	lightProj = Math::makeScaleMtx<REAL>(M4(),1.0,1.0,-1.0)*lightProj;
}

template<class REAL>
void LispSM<REAL>::updateLightMtx(M4& lightView, M4& lightProj, ILispSMSSD& fs) const {
	//calc the intersection Body B
	VecVec3 B;
	calcB(B,fs);
	updateLightMtx(lightView,lightProj,B,fs);
}

template struct LispSM<float>;
template struct LispSM<double>;
