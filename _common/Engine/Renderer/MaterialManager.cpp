//---------------------------------------------------------------------------
#pragma hdrstop
#include "MaterialManager.h"
#include "Renderer.h"
//#pragma package(smart_init)
//---------------------------------------------------------------------------
void MaterialManager::addMaterial(SmpMaterialState& m, const bool nameIdentity) {
    if(m == 0) {
		throw EngineException("SmpMaterialState == 0");
	}
	else {
	    MapMaterialState::const_iterator p = mapMaterialState.find(m->name);
	    if(p == mapMaterialState.end()) {
			mapMaterialState[m->name] = m;
		}
		else {
			if(!nameIdentity) {
				throw EngineException("MaterialState "+m->name+" already exists");
			}
		}
	}
}

const SmpMaterialState& MaterialManager::getMaterial(const std::string& name) const {
    MapMaterialState::const_iterator p = mapMaterialState.find(name);
    if(mapMaterialState.end() != p) {
        return p->second;
    }
    else {
        return MaterialState::DEFAULT;
    }
}
