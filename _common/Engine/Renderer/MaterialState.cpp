//---------------------------------------------------------------------------
#pragma hdrstop
#include "MaterialState.h"
//#pragma package(smart_init)
//---------------------------------------------------------------------------
RenderStateID MaterialState::id = RenderState::getRenderStateID();
const SmpMaterialState MaterialState::DEFAULT = new MaterialState("default");
