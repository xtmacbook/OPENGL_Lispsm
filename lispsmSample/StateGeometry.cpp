//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#include "StateGeometry.h"

StateGeometry::StateGeometry(State& neededState): state(neededState) { 
}

void StateGeometry::draw(FrameState& fs) {
	fs.stateManager.setState(state);
	drawGeometry(fs);
}

MaterialGeometry::MaterialGeometry(const unsigned matID): materialID(matID) { 
}

void MaterialGeometry::draw(FrameState& fs) {
	fs.setMaterial(materialID);
	drawGeometry(fs);
}

unsigned MaterialGeometry::getMaterialID() const {
	return materialID;
}
