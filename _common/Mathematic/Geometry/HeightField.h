#ifndef HeightField_H
#define HeightField_H
//---------------------------------------------------------------------------
#include <Types/DynamicArray.h>
#include "../Vector3.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

template<class HEIGHT_TYPE>
struct HeightField {
	typedef HEIGHT_TYPE HT;
	typedef Vector3f V3;
	typedef Array<V3> ArrVector3;
	typedef DynamicArray<V3> DynVector3;

	typedef DynamicArray<unsigned>  ArrID;
	typedef Array<HEIGHT_TYPE> HeightArray;

protected:
	DynVector3 vertex;
	DynVector3 normal;

	void createHeightField(const HeightArray& heightMap, 
							const unsigned uCount, const unsigned vCount, 
							const V3& origin_, const V3& extents_) {
		if(uCount*vCount != heightMap.size()) {
			throw BaseException("HeightField::createHeightField invalid heightMap size");
		}
		using Math::Vector3f;
		//create vertices
        vertex.resize(heightMap.size());
		const float fU = (1.0f/(uCount-1))*extents_.x();
		const float fV = (1.0f/(vCount-1))*extents_.z();
		const float fY = (1.0f/Math::Const<HEIGHT_TYPE>::max_value())*extents_.y();
		for(unsigned u = 0; u < uCount; u++) {
			for(unsigned v = 0; v < vCount; v++) {
				const unsigned id = getID(u,v,uCount);
				V3& p = vertex[id];
				p.x() = origin_.x()+u*fU;
				p.y() = origin_.y()+heightMap[id]*fY;
				p.z() = origin_.z()+v*fV;
			}
		}
	}

	void createNormals(const unsigned uCount, const unsigned vCount) {
		//todo: is very slow
		using Math::Vector3f;
		HeightField::ArrID vertexID;
		createFaceTriangleStrip(vertexID,0,0,uCount,vCount,uCount,vCount);

		normal.resize(vertex.size());
		normal.fillWith(Vector3f::ZERO());
		for(unsigned i = 0; i < vertexID.size()-2; i ++) {
			const Math::Vector3f a = vertex[vertexID[i]]-vertex[vertexID[i+1]];
			const Math::Vector3f b = vertex[vertexID[i+1]]-vertex[vertexID[i+2]];
			//face normal
			Math::Vector3f fn;
			fn.unitCross(a,b);
			//todo: check if invert necessary
			if(fn.dot(Vector3f::UNIT_Y()) < 0.0)
				fn.invert();
			normal[vertexID[i]] += fn;
			normal[vertexID[i+1]] += fn;
			normal[vertexID[i+2]] += fn;
		}
		for(unsigned i = 0; i < normal.size(); i++) {
			normal[i].normalize();
		}
	}

public:
	static inline unsigned getID(const unsigned u, const unsigned v, const unsigned uCount) {
	//	assert(u < uCount);
	//	assert(v < vCount);
		return v*uCount+u;
	}

	//for ccw ordering we run through the vertex numbers 
	//in the order of the numbers in the parantheses
	// ^ v       5(0)    6(2)    7(4)    8(6)    9(8)
	// |         
	// |--> u    0(1)    1(3)    2(5)    3(7)    4(9)
	static void createFaceTriangleStrip(ArrID& vertexID,
							 const unsigned U, const unsigned V,
							 const unsigned deltaU, const unsigned deltaV,
							 const unsigned uCount, const unsigned vCount) {
		unsigned U2 = U+deltaU;
		if(uCount < U2)
			U2 = uCount;
		unsigned V2 = V+deltaV;
		if(vCount < V2)
			V2 = vCount;
		vertexID.resize(deltaV*(deltaU*2+2)+1); //worst case size
		unsigned id = 0;
		for(unsigned v = V; v < V2-1; v++) {
			//to join the row triangle-strips send first and last vertex of each row twice 
			//-> 4 degenerate triangels are created
			if(0 < id) { //check if not first strip
				//send last vertex again
				unsigned a = vertexID[id -1];
				vertexID[id++] = a - 1;
				a = vertexID[id - 1];
				//vertexID[id++] = vertexID[id-1];
				//send first vertex of new strip
				vertexID[id++] = getID(U,v+1,uCount);
			}
			for(unsigned u = U; u < U2; u++) {
				vertexID[id++] = getID(u,v+1,uCount);
				vertexID[id++] = getID(u,v,uCount);
			}
		}
		vertexID.resize(id);
	}

	HeightField(const HeightArray& heightMap, const unsigned xCount, 
		const unsigned yCount, const float& scale, const float& sY, const bool normals_ = true) {
		//to get a heightmap centered arround the origin with height sY and 
		//scale-factor scale for the x-z extents
		const V3 origin(-0.5f*scale,-sY*0.5f,0.5f*scale);
		const V3 extents(scale,sY,-scale);
		createHeightField(heightMap,xCount,yCount,origin,extents);
		if(normals_) {
			createNormals(xCount,yCount);
		}
	}

	//heightMaps are memory dumps
	//with first coordinate u left to right and second v bottom to top
	//    ^ height (y)
	//    |  
	//    |  / v
	//    | /-----/
	//    |/ map /
	//    |-----/--> u (x)
	//   /
	//  /
	// / -v (z)
	//
    //the origin is (u,v) = (0,0), the position where the lower left corner should be mapped to
	//x -> 0(u), y -> smallest height value, z -> 0(v)
	//the extents is defined through 
	//origin+extents gives the position where the upper right corner should be mapped to
	//and in the y coordinate the height of the terrain
	HeightField(const HeightArray& heightMap, const unsigned xCount, 
		const unsigned yCount, const V3& origin_, const V3& extents_, const bool normals_ = true) {
		createHeightField(heightMap,xCount,yCount,origin_,extents_);
		if(normals_) {
			createNormals(xCount,yCount);
		}
	}

	const ArrVector3& getVertex() const { return vertex; }
	const ArrVector3& getNormal() const { return normal; }
};


//namespace
}
//namespace
}
#endif