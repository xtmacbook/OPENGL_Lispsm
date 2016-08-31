//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, Vienna University of Technology, 2005.
#ifndef ShadowMtxCalc_DMS_H
#define ShadowMtxCalc_DMS_H

#include <Mathematic/Geometry/GeometryTools.h>
#include <Mathematic/Geometry/Converter.h>


template<class REAL>
struct ShadowMtxCalc {
	typedef REAL Real;
	typedef Math::Vector3<REAL> V3;
	typedef Math::Matrix4<REAL> M4;
	typedef Math::Geometry::AABox<REAL> AABox;
	typedef Math::Geometry::VecVector3<REAL> VecVec3;
	typedef Math::Geometry::Object<REAL> Object;

	struct IShadowSceneData {
		virtual const V3 getEyeDir() const = 0;
		virtual const V3 getEyePos() const = 0;
		virtual const M4& getInvEyeSpace() const = 0;
		virtual const AABox& getSceneAABox() const = 0;
		
		virtual const REAL getHalfAngleDeg() const = 0;
		virtual bool isLightDirectional() const = 0;
		virtual const V3 getSpotLightPos() const = 0;
		virtual const V3 getLightDir() const = 0;

		virtual const M4& getLightView() const = 0;
		virtual const M4& getLightProj() const = 0;
	};

protected:
	static void calcLightProj(M4& proj, const IShadowSceneData& fs) {
		if(fs.isLightDirectional()) {
			proj = M4::IDENTITY();
		}
		else {
			const REAL fovY = fs.getHalfAngleDeg()*REAL(2.0);
			Math::perspectiveDegGL<REAL>(proj,fovY,1.0,1.0,100.0);
		}
	}


public:
	static void calcSpotShadowMtx(M4& view, M4& proj, const V3& up, const IShadowSceneData& fs) {
		//this function calculates a spot shadow view and projection matrix
		//the view matrix origin is the light position; the view direction is 
		//the light direction and the up vector is given as parameter
	//	const V3 invLightDir = -frameState.getLightDir();
		const V3& lightDir = fs.getLightDir();
		const V3& lightPos = fs.getSpotLightPos();
		//look from position(lightPos)
		//into direction(lightDir) 
		//with up vector(up)
	//	Math::look(view,lightPos,invLightDir,up);
		Math::look(view,lightPos,lightDir,up);
		calcLightProj(proj,fs);
		//invert the transform from right handed into left handed coordinate system for the ndc
		//done by the openGL style perspective calls
		//so we stay in a right handed system
		proj = Math::makeScaleMtx<REAL>(M4(),1.0,1.0,-1.0)*proj;
	}

	static void calcLightSpace(M4& lightView, M4& lightProj, const IShadowSceneData& fs) {
		if(fs.isLightDirectional()) {
			//directional light
			//this routine calculates a directional
			const V3& eyePos = fs.getEyePos();
			const V3& lightDir = fs.getLightDir();
			const V3 up = fs.getEyeDir();
			//look from position(eyePos)
			//into direction(lightDir) 
			//with up vector(up)
			lightView = Math::look(M4(),eyePos,lightDir,up);	
			lightProj = M4::IDENTITY();
		}
		else {
			//spot light
			calcSpotShadowMtx(lightView,lightProj,fs.getEyeDir(),fs);
		}
	}

	static const M4 scaleTranslateShadowMtx(const M4& lightSpace, const VecVec3& B) {
		//transform the light volume points from world into light space
		//calculate the cubic hull (an AABB) 
		//of the light space extents of the intersection body B
		const AABox b_ls(B,lightSpace);

		//refit to unit cube
		//this operation calculates a scale translate matrix that
		//maps the two extreme points min and max into (-1,-1,-1) and (1,1,1)
		return Math::Geometry::scaleTranslateToFit(M4(),b_ls);
	}

	static void calcB(VecVec3& B, const IShadowSceneData& fs) {
		//intersection object
		Object temp;
		//calculates the ViewFrustum Object
		temp.createFromFrustMatrixGL(fs.getInvEyeSpace());
		//done: intersect with the scene bb
		temp.clipByAABox(fs.getSceneAABox());

		if(fs.isLightDirectional()) {
			const V3& lightDir = fs.getLightDir();
			//extrudes the clipped view frustum into -lightDir and clippes by the sceneAABox
			//the defining points are returned
			temp.includeObjectLightVolume(B,lightDir,fs.getSceneAABox());
		}
		else {
			//done: calculate the convex hull of the clipped view frustum and the light position
			Math::Geometry::convexHullOfObjectAndPoint(temp,fs.getSpotLightPos());
			//done: intersect with the scene bb
			temp.clipByAABox(fs.getSceneAABox());
			//calculate the light frustum matrices
			M4 lv, lp;
			calcSpotShadowMtx(lv,lp,fs.getEyeDir(),fs);
			const M4 lightSpace = lp*lv;
			//done: intersect with light frustum
			temp.clipByFrustum(lightSpace);
			//calculate the defining points
			calcPoints(B,temp);
		}
	}

	static void calcLVS(VecVec3& LVS, const IShadowSceneData& fs) {
		//intersection object
		Object intersect;
		//calculates the ViewFrustum Object
		intersect.createFromFrustMatrixGL(fs.getInvEyeSpace());
		//done: intersect with the scene bb
		intersect.clipByAABox(fs.getSceneAABox());

		if(!fs.isLightDirectional()) {
			//calculate the light frustum matrices
			M4 lv,lp;
			calcSpotShadowMtx(lv,lp,fs.getEyeDir(),fs);
			//done: intersect with light frustum
			intersect.clipByFrustum(lp*lv);
		}

		calcPoints(LVS,intersect);
	}

};


#endif
