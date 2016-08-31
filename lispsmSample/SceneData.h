//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2005.

#ifndef SceneDataH
#define SceneDataH

#include <Base/Singleton.h>
#include "MathStuff.h"
#include <Mathematic/Vector2.h>
#include <Mathematic/Geometry/Grid.h>
#include "MyHeightField.h"
#include <GL/glHeader.h>
#include <Engine/GLInterface/TextureObjectGL.h>
#include <Engine/EngineBase/Geometry.h>
#include "GeometryObjects.h"
#include "Occlusion.h"

class FrameState;

class Scene : public Singleton {
protected:
	Occlusion* cull;

	typedef std::list<SmpGeometry> Objects;
	typedef Math::Vector2f V2;
	typedef MyHeightField HeightField;
	typedef Math::Geometry::Grid<unsigned char> HeightMap;
	typedef HeightField::ArrVector3 ArrVector3;
	HeightField* heightField;

	BaseTree baseTree;
	Sky sky;
	Objects scene;
	Objects caster;
	unsigned matHF;

	void makeHeightTexture(FrameState&);
	void createHeightFieldObjects(const unsigned mapSize, const unsigned objVertexDelta);
	void createTrees(const ArrVector3& v, const unsigned count, const unsigned treeSize, const unsigned MAP_SIZE, const float terrainSize);
	
	//Axis-Aligned Bounding Box of the scene
	AABox sceneAABox;

	void drawSky(FrameState&);

	static const float SIZE;
	static const float HEIGHT;
	static const float TREE_SIZE;
	//height map extent
	static const unsigned MAP_SIZE;
	static const unsigned TREE_COUNT;

public:

	static void loadResources(FrameState&);

	Scene(FrameState&);
	~Scene();
	
	//returns Axis-Aligned Bounding Box of the scene
	const AABox& getAABox() const { return sceneAABox; }

	//draws the scene :)
	void draw(FrameState&);
	//draws the shadow casters that are all objects in a list
	void drawShadowCasters(FrameState&);
	//used for walking around and keeping your head on the ground
	const Math::Vector3f::ElementType getHeight(const Math::Vector3f& pos) const;
};

#endif
