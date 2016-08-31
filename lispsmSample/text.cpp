//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#include "FrameState.h"
#include <Mathematic/MathIO.h>
#include <Engine/GLInterface/BaseGL.h>
#include <Engine/GLInterface/ToolsGL.h>
#include "Text.h"


void drawHelpMessage(const FrameState& fs) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	GL::color(C4(C3::GREEN(),0.2));  // 20% green. 

	// Drawn clockwise because the flipped Y axis flips CCW and CW. 
	glRecti(fs.getWidth()-5,5,5,fs.getHeight()-5);

	glDisable(GL_BLEND);

	GL::color(C3::WHITE());
	std::string msg = "Help information\n";
	msg += '\n';
	using Engine::ActionBindings;
	for(ActionBindings::const_iterator i = fs.mActionStateHandler->getActionBindings().begin(); i != fs.mActionStateHandler->getActionBindings().end(); i++) {
		const ActionBindings::PairKeyAct& aPair = *i;
		const Engine::Action& act = *aPair.second;
		std::string name(act.name);
		name += '(';
		name += act.getStateString();
		name += ')';

		name += "[";
		name += Engine::KeyConst::getKeyString(aPair.first);
		name += "]";
		msg += Tools::rFixLen(name,25);
		msg += " - ";
		msg += act.helpText;
		msg += '\n';
	}
	msg += '\n';
	msg += "'MOUSE-LEFT'  - eye look up/down left/right\n";
	msg += "'8'           - eye look up\n";
	msg += "'5'           - eye look down\n";
	msg += "'4'           - eye look left\n";
	msg += "'6'           - eye look right\n";
	msg += "'MOUSE-RIGHT' - light direction angle incr/decr rotate cw/ccw\n";
	msg += "'UP'          - move forward\n";
	msg += "'DOWN'        - move backward\n";
	msg += "'LEFT'        - strafe left\n";
	msg += "'RIGHT'       - strafe right\n";
	msg += "'PAGE-UP'     - move up\n";
	msg += "'PAGE-DOWN'   - move down\n";
	msg += '\n';
	msg += "'0'           - set depth(test) texture size to 256x256\n";
	msg += "'1'           - set depth(test) texture size to 512x512\n";
	msg += "'2'           - set depth(test) texture size to 1024x1024\n";
	msg += "'3'           - set depth(test) texture size to 2048x2048\n";
	multiLineOutput(10,10,msg,12);
}

std::string getStatistics(const FrameState& frameState) {
	std::string msg;
	ActionStateHandler& state = *frameState.mActionStateHandler;
	if(state.useBiasingMethod()) {
		msg += "polygon offset;";
	}
	else {
		msg += "back-side;";
	}
	if(state.flying()) {
		msg += "fly;";
	}

	if(state.useLiSPSM()) {
		msg += " z0:";
		msg += Tools::toString(state.getZ0(),3);
		msg += " z1:";
		msg += Tools::toString(state.getZ1(),3);
	}

	msg += '\n';

	if(state.isLightDirectional()) {
		msg += "directional ";
	}
	else {
		msg += "spot ";
	}
	if(state.useLiSPSM()) {
		msg += "LispSM[";
		if(state.useFormula()) {
			msg += "nOpt=";
		}
		else {
			msg += "n=";
		}
		msg += Tools::toString(state.getN());
		msg += ']';
	}
	else {
		msg += "UNIFORM-SM";
	}
	msg += '(';
	msg += Tools::toString(frameState.getDepthMapSize());
	msg += ')';
	msg += '\n';

	switch(state.getView()) {
	case ActionStateHandler::LIGHT:
		msg += "light view";
		break;
	case ActionStateHandler::EYE:
		if(state.isShadowed()) {
			msg += "shadowed ";
		}
		msg += "eye view";
		break;
	case ActionStateHandler::DEPTH_TEX:
		msg += "depth texture view";
		break;
	};
	msg += " near/far:";
	msg += Tools::toString(frameState.getNearDist());
	msg += '/';
	msg += Tools::toString(frameState.getFarDist());
	msg += " FOV:";
	msg += Tools::toString(state.getFovY());
	msg += '\n';

	msg += "pos:";
	msg += Tools::toString(frameState.getEyePos(),2);
	msg += '\n';

	msg += '(';
	msg += Tools::toString(1000/frameState.getFrames().maximum());
	msg += '/';
	msg += Tools::toString(1000/frameState.getFrames().avg());
	msg += '/';
	msg += Tools::toString(1000/frameState.getFrames().minimum());
	msg += ")F/sec ";
	if(!frameState.getFrames().empty())
	msg += Tools::toString(frameState.getFrames().last());
	msg += "msec";

	static unsigned oldStateChangeCount = 0;
	msg += " State Changes/frame:";
	msg += Tools::toString(frameState.stateManager.getStateChangeCount()-oldStateChangeCount);
	oldStateChangeCount = frameState.stateManager.getStateChangeCount();

	return msg;
}


