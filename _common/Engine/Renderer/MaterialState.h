#ifndef MaterialStateH
#define MaterialStateH
//---------------------------------------------------------------------------
#include "RenderState.h"
#include "../EngineBase/Color4.h"
#include "../EngineBase/Color3.h"
#include "../Mathematic/Mathematic.h"
#include "TextureManager.h"
//---------------------------------------------------------------------------
class MaterialState;
typedef SmartPointer<MaterialState> SmpMaterialState;

class MaterialState : public RenderState {
private:
	static RenderStateID id;
public:
	typedef Color4<Math::Real> C4;
	typedef Color3<Math::Real> C3;
	C4 ambient, diffuse, specular;
	Math::Real shininess;
	bool twoSided;
    std::string name;
	ITexture texture1;

    MaterialState(const std::string& vName): name(vName), ambient(C4::WHITE()), 
		diffuse(C4::WHITE()), specular(C4::WHITE()), shininess(1), 
		twoSided(false) { }

	static const SmpMaterialState DEFAULT;
};

#endif
