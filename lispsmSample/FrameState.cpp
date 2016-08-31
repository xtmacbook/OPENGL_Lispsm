//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.
#include <Base/System.h>
#include <Base/StreamTools.h>
#include <Base/StringTools.h>
#include <Mathematic/Matrix4Xform.h>
#include <Mathematic/MathIO.h>
#include <Mathematic/Geometry/GeometryTools.h>
#include <Mathematic/Geometry/Converter.h>
#include "Text.h"
#include "FrameState.h"
#include "SceneData.h"
#include "FBODepth.h"
#include <Engine/GLInterface/ToolsGL.h>
#include <Engine/GLInterface/ObjectsGL.h>

FrameState::FrameState(const unsigned frameID, const Real vNearDist, const Real vFarDist,
						const V3& ePos, const V3& eDir, const V3& spotPos, const V3& lDir, 
						const C4& vFogColor): frames(20),
				frame(frameID), nearDist(vNearDist), farDist(vFarDist),
				eyePos(ePos), eyeDir(eDir), spotLightPos(spotPos), up(V3::UNIT_Y()), lightDir(lDir), 
				halfAngleDeg(30.0f), mActionStateHandler(0),
				fogColor(vFogColor), lightColor(C3::WHITE()), textColor(C4::WHITE()),
				depthTexColor(C3::GREEN()),	viewFrustColor(C4(0.1,0.1,1,0.3)), 
				fboDepth(0), scene(0), shdDepthOnly(0), shdSky(0), po_factor(2.0), po_units(4.0) { 
	mActionStateHandler = new ActionStateHandler(*this);
	posF = eyePos.convert2<float>();
	lightDir.normalize();
	using Engine::KeyState;
//	updateEyeMtx();

	initExtensions();
	initGLstate();
		
	if(!loadShaders()) {
		throw ApplicationException("Can't create shader objects. Exiting now.");
	}

	bufferChange(2048);
	loadResources();
}
	
FrameState::~FrameState() {
	delete fboDepth;
	delete scene;
	delete mActionStateHandler;
}

void FrameState::loadResources() {
	Scene::loadResources(*this);
	scene = new Scene(*this);
}

void FrameState::bufferChange(const unsigned size) {
	delete fboDepth;
 	fboDepth = new FBODepth(size);
}

const AABox& FrameState::getSceneAABox() const { 
	return scene->getAABox(); 
}

const unsigned FrameState::getDepthMapSize() const { 
	return fboDepth->size(); 
}

void FrameState::setVisibleFrustum(const Math::Matrix4<float>& m) { 
	frustum.clear();
	Math::Geometry::addGlProjCamPlanes<float>(frustum,m);
}

unsigned FrameState::addTexture(const std::string& name, SmpTextureObjectGL tex) {
	vecTexture.push_back(tex);
	const unsigned id = vecTexture.size()-1;
	texIds[name] = id;
	return id;
}

TextureObjectGL& FrameState::getTexture(const std::string& name) { 
	if(texIds.end() == texIds.find(name)) {
		throw ApplicationException("No texture with name:"+name+" registered");
	}
	return getTexture(texIds[name]); 
}

unsigned FrameState::addMaterial(SmpState state) {
	vecState.push_back(state);
	return vecState.size()-1;
}

void FrameState::setMaterial(const unsigned id) {
	if(id < vecState.size()) {
		stateManager.setState(*vecState[id]);
	}
	else {
		throw ApplicationException("Invalid Material:"+Tools::toString(id));
	}
}

bool FrameState::loadShaders() {
	shdDepthOnly = shaderManager.loadfromFile("depthOnly.vert","depthOnly.frag");
	shdSky = shaderManager.loadfromFile("","sky.frag");
	shdShadowNdiffuse = shaderManager.loadfromFile("shadowNdiffuse.vert","shadowNdiffuse.frag");
			
	const bool error = shdDepthOnly == 0 || shdSky == 0 || 
		shdShadowNdiffuse == 0;

	if(error) {
		return false;
	}

	shdSky->begin();
		shdSky->sendUniform1i("tex2D",0); // Texture Unit 0
	shdSky->end();
	return true;
}

void FrameState::setWindowsExtents(const int width, const int height) {
	winWidth = width;
	winHeight = height;
	updateEyeMtx();
}

void FrameState::setWindowsExtentsProj(const int width, const int height, const Real& vNear, const Real& vFar) {
	nearDist = vNear;
	farDist = vFar;
	setWindowsExtents(width,height);
}

void FrameState::initExtensions() {
	startupGLEW();
	if(!GLEW_ARB_multitexture) {
		throw GLException("I require the ARB_multitexture OpenGL extension to work.");
	}
	if(!GLEW_EXT_framebuffer_object) {
		throw GLException("I require the EXT_framebuffer_object OpenGL extension to work.");
	}
	
	if(!GLEW_EXT_texture_rectangle && !GLEW_ARB_texture_rectangle) {
		debug() << "texture rectangle not supported -> don't resize window!\n";
	}
	debug() << "OpenGL " << getOpenGLVersion() << " supported.\n";
}

void FrameState::initGLstate() {
	GL::clearColor(fogColor);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	glClearDepth(1.0);
	glDepthRange(0.0,1.0);
	//for deferred shading)
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Math::Vector4f::UNIT_W().addr());
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor.addr());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor.addr());

	glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0);

	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,20.0);
	glFogf(GL_FOG_END,getFarDist());
	glFogfv(GL_FOG_COLOR,fogColor.addr());
	glEnable(GL_FOG);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
//	glEnable(GL_SCISSOR_TEST);
	glDisable(GL_SCISSOR_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glEnable(GL_NORMALIZE);

    glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//for each scene other offset values are suitable
	glPolygonOffset(po_factor,po_units);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void FrameState::updateEyeViewMtx() {
	Math::look(eyeView,getEyePos(),getEyeDir(),up);
	eyeSpace = eyeProj*eyeView;
	invEyeSpace.invert(eyeSpace);
}

void FrameState::updateEyeMtx() {
	perspectiveDegGL(eyeProj,mActionStateHandler->getFovY(),getAspect(),nearDist,farDist);
	updateEyeViewMtx();
}

void FrameState::updateLightMtx() {
	ShadowAlgo(*this).updateLightMtx(lightView,lightProjection,*this);
	lightSpace = lightProjection*lightView;
}

void FrameState::prepareShadowMapping() {
	glActiveTextureARB(texShadowUnit);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	
	glTexGendv(GL_S, GL_EYE_PLANE, Math::Vector4d::UNIT_X().addr());
	glTexGendv(GL_T, GL_EYE_PLANE, Math::Vector4d::UNIT_Y().addr());
	glTexGendv(GL_R, GL_EYE_PLANE, Math::Vector4d::UNIT_Z().addr());
	glTexGendv(GL_Q, GL_EYE_PLANE, Math::Vector4d::UNIT_W().addr());

	glMatrixMode(GL_TEXTURE);
	GL::loadMatrix(Math::Matrix4d::T05_S05());
	GL::multMatrix(getLightSpace());
	glMatrixMode(GL_MODELVIEW);

	fboDepth->getDepthTex().bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_COMPARE_R_TO_TEXTURE_ARB);

	glActiveTextureARB(GL_TEXTURE0);
}

void FrameState::unprepareShadowMapping() {
	glActiveTextureARB(texShadowUnit);
	//Disable textures and texgen
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_NONE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glActiveTextureARB(GL_TEXTURE0);
}

void FrameState::setupLightView() {
	updateLightMtx();

	glMatrixMode(GL_PROJECTION);
	GL::loadMatrix(getLightProj());

	glMatrixMode(GL_MODELVIEW);
	GL::loadMatrix(getLightView());
}

void FrameState::setupEyeView() {
	glMatrixMode(GL_PROJECTION);
	GL::loadMatrix(getEyeProj());

	glMatrixMode(GL_MODELVIEW);
	GL::loadMatrix(getEyeView());
}

void FrameState::setLightPos() {
	const V3& lightDir = getLightDir();
	if(isLightDirectional()) {
		const Math::Vector4f lightPos = GL::convertDirToPos<float>(getLightDir().convert2<float>());
		glLightfv(GL_LIGHT0,GL_POSITION,lightPos.addr());
	    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0f);
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,(-Math::Vector3f::UNIT_Z()).addr());
	}
	else {
		const Math::Vector4f spotPos = Math::Vector4f(getSpotLightPos().convert2<float>(),1.0);
		glLightfv(GL_LIGHT0,GL_POSITION,spotPos.addr());
		glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,halfAngleDeg);
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,getLightDir().convert2<float>().addr());
//todo:		glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1f);

	}
}

void FrameState::drawPolyhedron(const C4& col, const Polyhedron& ph) {
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GL::color(col);
	glEnable(GL_POLYGON_OFFSET_FILL);
	GL::drawPolyhedronVolume(ph);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_BLEND);

	GL::color(C3::BLACK());
	GL::drawPolyhedronLine(ph);

	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
}

void FrameState::drawClippedViewFrustum() {
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	//draw view vector in red
	GL::color(C3::RED());
	const V3& startPos = getEyePos();
	const V3& dir = getEyeDir();
	GL::drawLine(startPos+dir*getNearDist(),startPos+dir*getFarDist());

	//draw view frustum
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Polyhedron clippedViewFrustum;
	Object intersect;
	intersect.createFromFrustMatrixGL(getInvEyeSpace());
	//clippes the ViewFrustum Object By the sceneAABox
	intersect.clipByAABox(scene->getAABox());
	Math::Geometry::convert(clippedViewFrustum,intersect);

	GL::color(viewFrustColor);
	glEnable(GL_POLYGON_OFFSET_FILL);
	GL::drawPolyhedronVolume(clippedViewFrustum);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_BLEND);

	GL::color(C3::BLACK());
	GL::drawPolyhedronLine(clippedViewFrustum);

	glPushMatrix();
	GL::translate(getEyePos());
	GL::color(C3::BLUE());
	glutSolidSphere(0.05,10,10);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
}


void FrameState::drawSphere(const V3& pos, const C3& col, const Real size) {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	GL::translate(pos);
	GL::color(col);
	glutSolidSphere(size,10,10);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void FrameState::drawLight() {
	if(!isLightDirectional()) {
		//show spot light position
		drawSphere(getSpotLightPos(),C3::ORANGE());
	}
}

void FrameState::drawEyeView() {
	setLightPos();
	//contains frustum and gl state manager
	setVisibleFrustum(getEyeSpace().convert2<float>());
	scene->draw(*this);
	drawLight();
}

void FrameState::drawLightView() {
	setLightPos();
	glDisable(GL_FOG);
	const V3 eyePos = getEyePos();
	setEyePos(V3::ONE()*Math::Const<Real>::infinity());
	setVisibleFrustum((getLightSpace()).convert2<float>());
	scene->draw(*this);
	setEyePos(eyePos);
	glDisable(GL_DEPTH_TEST);
	drawClippedViewFrustum();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
}

void FrameState::generateDepthTexture() {
	// bind fbo
	fboDepth->bind();
	const unsigned s = getDepthMapSize();
	glViewport(0,0,s,s);
	glClear(GL_DEPTH_BUFFER_BIT);

	if(mActionStateHandler->useBiasingMethod()) {
		glEnable(GL_POLYGON_OFFSET_FILL);
	}
	else {
		glCullFace(GL_FRONT);
	}
	setupLightView();
	//contains frustum and gl state manager
	const V3 eyePos = getEyePos();
	setEyePos(V3::ONE()*Math::Const<Real>::infinity());
	setVisibleFrustum((getLightSpace()).convert2<float>());
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	shdDepthOnly->begin();
		scene->drawShadowCasters(*this);
	shdDepthOnly->end();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	setEyePos(eyePos);
	if(mActionStateHandler->useBiasingMethod()) {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	else {
		glCullFace(GL_BACK);
	}
	// unbind fbo
	fboDepth->unbind();
	glViewport(0, 0, getWidth(), getHeight());
}

void FrameState::drawTexture(const TextureObjectGL& tex) {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	tex.bind();
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glBegin(GL_QUADS);    
		GL::color(depthTexColor);
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void FrameState::drawTexture(const TextureObjectGL& tex, const float& lod, const float& alpha) {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	tex.bind();
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS_EXT,lod);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glBegin(GL_QUADS);    
		GL::color(C4(depthTexColor,alpha));
		glTexCoord2f(0,1);
		glVertex2f(-1,1);
		glTexCoord2f(0,0);
		glVertex2f(-1,-1);
		glTexCoord2f(1,0);
		glVertex2f(1,-1);
		glTexCoord2f(1,1);
		glVertex2f(1,1);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void FrameState::begin2D() {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,getWidth(),getHeight(),0);
}


void FrameState::end2D() {
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void FrameState::render() {
	unsigned start = getTime();
	if(!mActionStateHandler->flying()) {
		V3 eyePos = getEyePos();
		eyePos[1] = scene->getHeight(eyePos.convert2<float>())+getNearDist()*2.0;
		setEyePos(eyePos);
	}

	updateEyeViewMtx(); //bring eye modelview matrix up-to-date
	const bool shadowed = mActionStateHandler->isShadowed();
	switch(mActionStateHandler->getView()) {
	case ActionStateHandler::LIGHT: {
		setupLightView();
		mActionStateHandler->setShadowed(false);
		drawLightView();
		mActionStateHandler->setShadowed(shadowed);
		break; }
	case ActionStateHandler::EYE:
		if(shadowed) {
			generateDepthTexture();
		}
		setupEyeView();
		if(shadowed) {
			prepareShadowMapping();
		}
		drawEyeView();
		if(shadowed) {
			unprepareShadowMapping();
		}
		break;
	case ActionStateHandler::DEPTH_TEX:
		generateDepthTexture();
		drawTexture(fboDepth->getDepthTex());
		break;
	};

	begin2D();
	if(mActionStateHandler->showHelp()) {
		drawHelpMessage(*this);
	}
	else {
		if(mActionStateHandler->showStatistics()) {
			GL::color(textColor);
			multiLineOutput(10,getHeight()-100,getStatistics(*this));
		}
	}
	end2D();
	glutSwapBuffers();
	//add new frame sample for fps calc
	frames.add(Math::clampConst(getTime()-start,1u,50000u));

	nextFrame();
	//adjust sample count to the avg count of frames per second
	frames.setSampleCount(1000/frames.avg());
}

void FrameState::onResize(const int w, const int h) {
//	DMS::saveDelete(fboImpostor);
//	fboImpostor = new FBOImpostor(512,512);
	setWindowsExtents(w,h);
}