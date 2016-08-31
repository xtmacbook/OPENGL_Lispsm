//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.
#include <Base/StreamTools.h>
#include <Base/StringTools.h>
#include "ActionStateHandler.h"
#include <Engine/EngineBase/ToggleAction.h>
#include <Engine/EngineBase/SetAction.h>

ActionStateHandler::ActionStateHandler(IActionStateNotificationReceiver& receiver_): 
		mN(1.0), mZ0(0.0), mZ1(0.0), mView(EYE), mEye_fovY(60.0), mReceiver(receiver_) { 
	using Engine::KeyState;
	using Engine::KeyConst;
	using Engine::TypedAction;
	using Engine::ToggleAction;
	using Engine::SetAction;
	typedef TCallback<ActionStateHandler,Engine::Action::PARAM> CB;

	mActionBindings.add(new ToggleAction(mShowHelp,"showHelp",false,
		"shows and dismisses this message"),KeyConst::KEY_F1);

	mActionBindings.add(new SetAction<TView>(mView,"eyeView",EYE,"show eye view"),KeyConst::KEY_F2);
	mActionBindings.add(new SetAction<TView>(mView,"lightView",LIGHT,"show light view"),KeyConst::KEY_F3);
	mActionBindings.add(new SetAction<TView>(mView,"depthTexView",DEPTH_TEX,"show depth texture"),KeyConst::KEY_F4);

	mActionBindings.add(new ToggleAction(mShadowed,"shadowed",true,
		"toggle shadows"),KeyConst::KEY_TAB);
	mActionBindings.add(new ToggleAction(mUseLispSM,"LispSM/SSM",true,
		"toggle lispsm/ssm"),KeyConst::KEY_SPACE);

	mActionBindings.add(new ToggleAction(mDirectionalLight,"directionalLight",true,
		"toggle directional/spot light"),'d');

	mActionBindings.add(new ToggleAction(mFly,"fly",false,"toggle flying/walking"),'f');

	mActionBindings.add(new ToggleAction(mShowStatistics,"statistics",true,
		"enable/disable statistics"),'i');
	mActionBindings.add(new TypedAction<Real>(mN,"nInc",
		"increase the n parameter for LispSM",new CB(this,&ActionStateHandler::incN)),'N');
	mActionBindings.add(new TypedAction<Real>(mN,"nDec",
		"decrease the n parameter for LispSM",new CB(this,&ActionStateHandler::decN)),'n');
	mActionBindings.add(new ToggleAction(mUseFormula,"Nopt",true,
		"enable/disable the nOpt calculation for LispSM"),'o');
	mActionBindings.add(new ToggleAction(mBiasingMethod,"biasingMethod",false,
		"toggle (use polygon offset)/(render back faces) for depth texture biasing"),'b');

	mActionBindings.add(new ToggleAction(mFastMoving,"fastMoving",false,
		"enable/disable fast moving"),'#');
}
	
ActionStateHandler::~ActionStateHandler() { }

void ActionStateHandler::incFov(Engine::Action&) { 
	mEye_fovY += 1.1;
	Math::clamp<Real>(mEye_fovY,0.0,180.0);
	mReceiver.updateEyeMtx();
}

void ActionStateHandler::decFov(Engine::Action&) { 
	mEye_fovY /= 1.1; 
	Math::clamp<Real>(mEye_fovY,0.0,180.0);
	mReceiver.updateEyeMtx();
}
