//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#ifndef FrameStateH
#define FrameStateH

#include <fstream>
#include <map>
#include <vector>
#include <Mathematic/HistoryList.h>
#include <Engine/Renderer/State.h>
#include <Engine/EngineBase/Action.h>
#include <Engine/GLInterface/BaseGL.h>
#include <Engine/GLInterface/TextureObjectGL.h>
#include <Engine/EngineLevel1/ShaderManager.h>
#include "DataTypes.h"
#include <Engine/Algo/LispSM.h>
#include "ActionStateHandler.h"

class Scene;
class FBODepth;

struct ApplicationException : public BaseException {
	ApplicationException(const std::string& vMsg): BaseException(vMsg) { };
};

typedef LispSM<Real> ShadowAlgo;

class FrameState : public ShadowAlgo::ILispSMSSD, public IActionStateNotificationReceiver {
	unsigned frame;
	int winWidth, winHeight;
	Real nearDist; // eye near plane distance (often 1)
	Real farDist;

	V3 eyePos;  // eye position 
	V3 eyeDir;  // eye view dir 
	V3 spotLightPos;  // light position 
	V3 lightDir;  // light dir 
	float halfAngleDeg;

	M4 eyeView; // eye view matrix
	M4 eyeProj; // eye projection matrix
	M4 eyeSpace; //= eyeProj*eyeView
	M4 invEyeSpace; //= (eyeProj*eyeView)^(-1)

	M4 lightView; // light view matrix
	M4 lightProjection; // light projection matrix
	M4 lightSpace; // light space matrix (lightProj*lightView)

	Math::Vector3f posF;
	void updateLightMtx();
	void updateEyeMtx();

	typedef std::vector<SmpState> VecState;
	VecState vecState;

	typedef std::map<std::string,unsigned> MapIDs;

	typedef std::vector<SmpTextureObjectGL> VecTexture;
	MapIDs texIds;
	VecTexture vecTexture;

	//constants for polygon offset
	const float po_factor;
	const float po_units;

	const C4 fogColor;
	const C3 lightColor;
	const C3 depthTexColor;
	const C4 viewFrustColor;
	const C4 textColor;

	FBODepth* fboDepth;

	typedef Math::HistoryList<unsigned> HistoryListu;
	HistoryListu frames;

	//handels drawing and contains the scene objects
	Scene* scene;

	void setLightPos();
	void drawPolyhedron(const C4&, const Polyhedron&);
	void drawClippedViewFrustum();
	void drawSphere(const V3& pos, const C3& col = C3::WHITE(), const Real size = 0.1);
	void drawLight();

	void drawTexture(const TextureObjectGL& tex);
	void drawTexture(const TextureObjectGL& tex, const float& lod, const float& alpha);
	void begin2D();
	void end2D();
	void generateDepthTexture();
	void prepareShadowMapping();
	void unprepareShadowMapping();
	void setupEyeView();
	void setupLightView();
	void drawEyeView();
	void drawLightView();

	void initExtensions();
	void initGLstate();
	bool loadShaders();
	void loadResources();

	void setVisibleFrustum(const Math::Matrix4<float>&); 

	void setWindowsExtents(const int width, const int height);
	void setWindowsExtentsProj(const int width, const int height, const Real& near, const Real& far);
	void updateEyeViewMtx();

public:

	const static GLenum texNoiseUnit = GL_TEXTURE3; 
	const static GLenum texShadowUnit = GL_TEXTURE1; 

	void bufferChange(const unsigned size);

	unsigned addTexture(const std::string& name, SmpTextureObjectGL tex);
	TextureObjectGL& getTexture(const unsigned id) { return *vecTexture[id]; }
	TextureObjectGL& getTexture(const std::string& name);

	std::ostream& debug() const { return std::cout; }
	
	Frustum frustum;

	ShaderManager shaderManager;
	//Shader objects:
	ShaderObject* shdDepthOnly;  
	ShaderObject* shdSky;  
	ShaderObject* shdShadowNdiffuse;  
	
	StateManager stateManager;
	
	const V3 up;

	ActionStateHandler* mActionStateHandler;

	FrameState(const unsigned frameID, const Real vNearDist, const Real vFarDist,
					const V3& ePos, const V3& eDir, const V3& spotPos, const V3& lDir, 
					const C4& vFogColor);
	
	virtual ~FrameState();

	bool isLightDirectional() const { return mActionStateHandler->isLightDirectional(); }
	bool useLiSPSM() const { return mActionStateHandler->useLiSPSM(); }

	bool useFormula() const { return mActionStateHandler->useFormula(); }
	void setN(const Real& n_) { mActionStateHandler->setN(n_); }
	const Real getN() const { return mActionStateHandler->getN(); }
	const Real getFovY() const { return mActionStateHandler->getFovY(); }

	void nextFrame() { frame++; }
	const unsigned getFrame() const { return frame; }
	const HistoryListu getFrames() const { return frames; }
	
	const Real getNearDist() const { return nearDist; }
	const Real getFarDist() const { return farDist; }

	void setEyePos(const V3& v) {
		eyePos = v;
		posF = eyePos.convert2<float>();
	}
	const Math::Vector3f& getPosF() const { return posF; }
	const V3 getEyePos() const { return eyePos; }

	void setEyeDir(const V3& v) {
		eyeDir = v;
		eyeDir.normalize();
	}
	const V3 getEyeDir() const { return eyeDir; }


	void setSpotLightPos(const V3& v) {	spotLightPos = v; }
	const V3 getSpotLightPos() const { return spotLightPos; }

	void setLightDir(const V3& v) {
		lightDir = v;
		lightDir.normalize();
	}
	const V3 getLightDir() const { return lightDir; }

	const Real getHalfAngleDeg() const { return halfAngleDeg; }

	const AABox& getSceneAABox() const;

	const unsigned getDepthMapSize() const;

	unsigned addMaterial(SmpState state);
	void setMaterial(const unsigned id);

	void render();
	
	const int getWidth() const { return winWidth; }
	const int getHeight() const { return winHeight; }
	const Real getAspect() const { return (double)getWidth() / (double)getHeight(); }
	void onResize(const int w, const int h);

	const M4& getEyeView() const { return eyeView; }
	const M4& getEyeProj() const { return eyeProj; }
	const M4& getEyeSpace() const { return eyeSpace; }
	const M4& getInvEyeSpace() const { return invEyeSpace; }

	const M4& getLightView() const { return lightView; }
	const M4& getLightProj() const { return lightProjection; }
	const M4& getLightSpace() const { return lightSpace; }
};

#endif
