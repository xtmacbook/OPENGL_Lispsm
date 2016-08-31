//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2005.

#include <algorithm>
#include <stdlib.h>
#include <Base/Base.h>
#include <Base/SmartPointer.h>
#include <Types/DynamicArray.h>
#include <Types/StaticArray.h>
#include <Mathematic/Vector2.h>
#include <Mathematic/Vector3.h>
#include <Mathematic/Vector4.h>
#include <Mathematic/perlin.h>
#include <Mathematic/MathTools.h>
#include <Mathematic/Geometry/GeometryIO.h>
#include <Engine/EngineBase/Geometry.h>
#include <Engine/Renderer/State.h>
#include "SceneData.h"
#include "GeometryObjects.h"
#include "Materials.h"

const float Scene::SIZE = 200.0;
const float Scene::HEIGHT = 10.0;
const float Scene::TREE_SIZE = 5.0;
//height map extent
const unsigned Scene::MAP_SIZE = 128;
//const unsigned Scene::MAP_SIZE = 512;
const unsigned Scene::TREE_COUNT = 3000;
//const unsigned Scene::TREE_COUNT = 10000;

using Math::Vector3f;

void Scene::makeHeightTexture(FrameState& fs) {
	const unsigned SIZE = 8;
	GLubyte heightTexture[SIZE*3];

	// Setup RGB image for the texture. 
	GLubyte *loc = (GLubyte*) heightTexture;
	// deep water. 
	loc[0] = 0x00;
	loc[1] = 0x00;
	loc[2] = 0xff;
	loc += 3;

	loc[0] = 0x00;
	loc[1] = 0xaf;
	loc[2] = 0xff;
	loc += 3;

	// sand.
	loc[0] = 0xa0;
	loc[1] = 0xff;
	loc[2] = 0xa0;
	loc += 3;

	// green. 
	loc[0] = 0x00;
	loc[1] = 0x8f;
	loc[2] = 0x00;
	loc += 3;

	// green. 
	loc[0] = 0x00;
	loc[1] = 0xaf;
	loc[2] = 0x10;
	loc += 3;

	// gray. 
	loc[0] = 0x6f;
	loc[1] = 0x6f;
	loc[2] = 0x6f;
	loc += 3;

	loc[0] = 0xaf;
	loc[1] = 0xaf;
	loc[2] = 0xaf;
	loc += 3;

	loc[0] = 0xff;
	loc[1] = 0xff;
	loc[2] = 0xff;

	TextureObjectGL& texHeight = fs.getTexture("height");
	texHeight.load(heightTexture,SIZE);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
}

void Scene::createHeightFieldObjects(const unsigned mapSize, const unsigned objVertexDelta) {
	typedef Math::Geometry::HeightField<unsigned char> HeightField;
	//create triangle indices
	const unsigned delta = Math::clampConst(objVertexDelta,2u,mapSize);
	const unsigned step = delta-1;
	const unsigned big = mapSize-1;
	for(unsigned u = 0; u < big; u += step) {
		for(unsigned v = 0; v < big; v += step) {
			HeightField::ArrID vertexID;
			HeightField::createFaceTriangleStrip(vertexID,u,v,delta,delta,mapSize,mapSize);
			SmpGeometry hfr(new HeightFieldRgn(matHF,vertexID,*heightField));
			scene.push_back(hfr);
			caster.push_back(hfr);
		}
	}
}

void Scene::createTrees(const HeightField::ArrVector3& v, const unsigned count, const unsigned treeSize, const unsigned MAP_SIZE, const float terrainSize) {
	typedef Math::Geometry::HeightField<unsigned char> HeightField;
	if(v.empty() || count == 0) return;
	const double identMap = (1.0/RAND_MAX);
	const unsigned step1 = Math::clampConst<unsigned>(1,MAP_SIZE/(terrainSize/treeSize),MAP_SIZE/2);
	const unsigned step2 = MAP_SIZE/Math::sqrt(double(count));
	const unsigned step = Math::minimum(step1,step2);

	HeightField::ArrID treeVertexID;
	srand(v[0][0]);
	const int maxVar = step/4;
	for(unsigned u = step; u < MAP_SIZE-step; u += step) {
		for(unsigned v = step; v < MAP_SIZE-step; v += step) {
			const int var = Math::randRange(-maxVar,maxVar);
			treeVertexID.append(HeightField::getID(u+var,v+var,MAP_SIZE));
		}
	}
	treeVertexID.resize(count);
	Math::randomizeArray(treeVertexID);
	if(step1 < step2) 
		treeVertexID.resize(count);
	for(unsigned i = 0; i < treeVertexID.size(); i++) {
		SmpGeometry tree(new Tree(v[treeVertexID[i]],(rand()+1)*identMap*treeSize,baseTree));
		scene.push_back(tree);
		caster.push_back(tree);
	}
}

void Scene::loadResources(FrameState& fs) {
	fs.addTexture("height",new TextureObjectGL(GL_TEXTURE_1D,GL_RGB,GL_RGB8));
	fs.addTexture("sky",new TextureObjectGL(GL_TEXTURE_2D,GL_ALPHA,GL_ALPHA));
	fs.addTexture("noise",new TextureObjectGL());
}

struct Noise {
	typedef unsigned char Base;
	typedef Math::Vector4<Base> Tuppel4;
	typedef DynamicField<Tuppel4> D;
	Base operator()(const Base& b, const Tuppel4& d) const {
		float v = d[0];
		v += d[1]*0.5;
		v += d[2]*0.25;
		v += d[3]*0.125;
		v /= 256.0;
		v -= 0.5;
		return Math::clampConst<Base>(v*256,0,255);
	}
};

Scene::Scene(FrameState& fs): sceneAABox(V3::ZERO(),V3::ZERO()),  
							heightField(0), cull(0), 
							baseTree(fs.addMaterial(new TreeState(fs))),
							sky(fs.addMaterial(new SkyState(fs)),
								Math::Geometry::AABox<float>(
								Vector3f(-SIZE,10.0,-SIZE),
								Vector3f(SIZE,10.0,SIZE))),
							matHF(fs.addMaterial(new HeightFieldState(fs))) {
	//matHF = baseTree.getMaterialID();
	makeHeightTexture(fs);
	
	HeightMap heightMap(MAP_SIZE,MAP_SIZE);
	Noise::D t;
	Perlin::createFieldWithOctave123(t,MAP_SIZE,MAP_SIZE,7);
	Noise noise;
	heightMap.manipulate(noise,t);
	TextureObjectGL& texNoise = fs.getTexture("noise");
	texNoise.bind();
	gluBuild2DMipmaps(texNoise.type(),GL_RGBA,MAP_SIZE,MAP_SIZE,texNoise.formatExtern(),GL_UNSIGNED_BYTE,t.getData().addr());
	glTexParameteri(texNoise.type(),GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texNoise.type(),GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	std::cout << "creating height field\n";
	heightField = new HeightField(heightMap.getData(),MAP_SIZE,MAP_SIZE,SIZE,HEIGHT);

	std::cout << "creating height field objects\n";
	createHeightFieldObjects(MAP_SIZE,30);

	std::cout << "creating " << TREE_COUNT << " trees with "<< 
		baseTree.triangleCount() << "tri/tree\n";
	createTrees(heightField->getVertex(),TREE_COUNT,TREE_SIZE,MAP_SIZE,SIZE);
	
	sceneAABox = scene.front()->getBoundingBox().convert2<double>();
	for(Objects::iterator i = scene.begin(); i != scene.end(); i++) {
		sceneAABox += (*i)->getBoundingBox().convert2<double>();
	}
	std::cout << "scenebb:" << sceneAABox << '\n';

	int delta = glutGet(GLUT_ELAPSED_TIME);
	cull = new Occlusion(scene);
	delta = glutGet(GLUT_ELAPSED_TIME)-delta;
	std::cout.precision(2);
	std::cout << "kdtree build time:" << delta << "ms for: " << scene.size() << " objects" <<
		" maxDepth=" << cull->getKDTree().depthMax() << "\n" 
		//debug << *kdTree << '\n'
		;
}

Scene::~Scene() {
	delete heightField;
	delete cull;
	scene.clear();
	caster.clear();
}

void initShader(ShaderObject& shd, const FrameState& fs) {
	shd.begin();
	Math::Matrix4d invView;
	invView.invert(fs.getEyeView());
	shd.sendUniformMatrix4fv("invView",1,false,invView.convert2<float>().addr());

	Math::Matrix4d lpv(Math::Matrix4d::IDENTITY());
	lpv *= fs.getLightSpace();
	shd.sendUniformMatrix4fv("texMatrix",1,false,lpv.convert2<float>().addr());
	shd.end();
}

void Scene::drawSky(FrameState& fs) {
	if(ActionStateHandler::LIGHT != fs.mActionStateHandler->getView()) {
		if(fs.frustum.visible(sky.getBoundingBox())) {
			//draw seperate because it increases scene bounding box greatly!
			sky.draw(fs);
		}
	}
}

void Scene::draw(FrameState& fs) {
	if(false) {
		//FTL mode
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//render with culling (vfc) and without gl states, so it gets realy fast
		cull->renderWithCheck(fs,false);
		drawSky(fs);
		return;
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	const float size = fs.getDepthMapSize()-1.0;
	const float size = fs.getDepthMapSize(); //debug
	ShaderObject& shd = *fs.shdShadowNdiffuse;
	shd.begin();
		shd.sendUniform1i("tex1D",0); // Texture Unit 0
		shd.sendUniform1i("texNoise",fs.texNoiseUnit-GL_TEXTURE0);
		shd.sendUniform1i("texShadow",fs.texShadowUnit-GL_TEXTURE0);
		shd.sendUniform2f("texMapSize",size,size);
		shd.sendUniform2f("texMapScale",1.0/size,1.0/size);
		shd.sendUniform1i("shadowing",fs.mActionStateHandler->isShadowed());
		shd.sendUniform1i("directional",fs.isLightDirectional());
		//some oglsl implementations do not give you the proper texture matrix so we do it ourselfs
		const Math::Matrix4f invEyeView = fs.getEyeView().getInverse().convert2<float>();
		const Math::Matrix4f lsp = fs.getLightSpace().convert2<float>();
		Math::Matrix4f texMatrix = Math::Matrix4f::T05_S05()*lsp*invEyeView;
		shd.sendUniformMatrix4fv("texMatrix",1,false,texMatrix.addr());
		//render with culling (vfc) and with gl states
		cull->renderWithCheck(fs,true);
		drawSky(fs);
	shd.end();
	//revert to the initial gl state
	fs.stateManager.setState(State::EMPTY);
}

void Scene::drawShadowCasters(FrameState& fs) {
	for(Objects::iterator i = caster.begin(); i != caster.end(); i++) {
		Geometry& g = **i;
		const Geometry::AABox& box = g.getBoundingBox();
		if(fs.frustum.visible(box)) {
			g.drawGeometry(fs);
		}
	}
}

const Math::Vector3f::ElementType Scene::getHeight(const Math::Vector3f& pos) const {
	const ArrVector3& vertex = heightField->getVertex();
	const unsigned MAP_SIZE = Math::sqrt(double(vertex.size()));
	const unsigned MAP_SIZE_1 = MAP_SIZE-1;
	//with scenebb get u,v of equidistant grid
	const Vector3f min = sceneAABox.getMin().convert2<float>();
	const Vector3f max = sceneAABox.getMax().convert2<float>();
	const float x_norm = (pos[0]-min[0])/(max[0]-min[0]);
	const float z_norm = (pos[2]-min[2])/(max[2]-min[2]);
	const float uf = x_norm*MAP_SIZE_1;
	const float vf = (1.0-z_norm)*MAP_SIZE_1;
	const unsigned u0 = Math::clampConst<unsigned>(Math::floor(uf),0,MAP_SIZE_1);
	const unsigned v0 = Math::clampConst<unsigned>(Math::floor(vf),0,MAP_SIZE_1);
	const unsigned u1 = Math::clampConst<unsigned>(Math::ceil(uf),0,MAP_SIZE_1);
	const unsigned v1 = Math::clampConst<unsigned>(Math::ceil(vf),0,MAP_SIZE_1);
	//lookup with getID in vertex arrray
	const unsigned id0 = HeightField::getID(u0,v0,MAP_SIZE);
	const unsigned id1 = HeightField::getID(u0,v1,MAP_SIZE);
	const unsigned id2 = HeightField::getID(u1,v0,MAP_SIZE);
	const unsigned id3 = HeightField::getID(u1,v1,MAP_SIZE);
	const Vector3f& vert0 = vertex[id0];
	const Vector3f& vert1 = vertex[id1];
	const Vector3f& vert2 = vertex[id2];
	const Vector3f& vert3 = vertex[id3];
	//bilinearly interpolate
	const float fractV = Math::fract<float>(vf);
	const float fractU = Math::fract<float>(uf);
	const float a = Math::lerp<float>(vert0[1],vert1[1],fractV);
	const float b = Math::lerp<float>(vert2[1],vert3[1],fractV);
	return Math::lerp(a,b,fractU);
}
