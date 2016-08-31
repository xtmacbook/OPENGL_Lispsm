//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#ifndef StateGeometryH
#define StateGeometryH

#include <Engine/EngineBase/Geometry.h>
#include <Engine/Renderer/State.h>
#include "FrameState.h"

class StateGeometry : public Geometry {
protected:
	State& state;
public:
	StateGeometry(State& neededState);
	virtual void draw(FrameState&);
};

class MaterialGeometry : public Geometry {
protected:
	unsigned materialID;
public:
	MaterialGeometry(const unsigned matID);
	virtual void draw(FrameState&);
	unsigned getMaterialID() const;
};

#endif
