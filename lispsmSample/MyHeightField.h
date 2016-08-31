#ifndef MyHeightField_DMS_H
#define MyHeightField_DMS_H
//---------------------------------------------------------------------------
#include <Mathematic/Geometry/HeightField.h>
//---------------------------------------------------------------------------
struct MyHeightField : public Math::Geometry::HeightField<unsigned char> {
	typedef Math::Geometry::HeightField<unsigned char> FATHER;
	typedef Array<float> ArrFloat;
	typedef DynamicArray<float> DynF;

protected:
	DynF texCoord;

	void createTexCoord(const HeightArray& heightMap) {
		texCoord.resize(heightMap.size());
		const float fY = 1.0f/Math::Const<HT>::max_value();
		for(unsigned i = 0; i < texCoord.size(); i++) {
			texCoord[i] = Math::clampConst<float>(fY*heightMap[i],0.0,1.0);
		}
	}

public:
	MyHeightField(const HeightArray& heightMap_, const unsigned xCount, 
		const unsigned yCount, const float& scale, const float& sY): 
			FATHER(heightMap_,xCount,yCount,scale,sY,true) {
		createTexCoord(heightMap_);
	}

	const ArrFloat& getTexCoord() const { return texCoord; }
};

#endif