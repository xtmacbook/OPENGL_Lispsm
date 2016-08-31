//---------------------------------------------------------------------------
#ifndef MaterialManagerH
#define MaterialManagerH
#pragma warning(disable:4786)  // Disable warning message
#include <map>
#include "MaterialState.h"
//---------------------------------------------------------------------------
class MaterialManager : public Base {
protected:
    typedef std::map<std::string,SmpMaterialState> MapMaterialState;
	MapMaterialState mapMaterialState;
public:
    void addMaterial(SmpMaterialState&, const bool nameIdentity);
	const SmpMaterialState& getMaterial(const std::string& name) const;
};

#endif
