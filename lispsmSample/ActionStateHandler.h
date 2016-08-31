//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#ifndef ActionStateHandler_DMS_H
#define ActionStateHandler_DMS_H

#include <Engine/EngineBase/Action.h>
#include "DataTypes.h"


struct IActionStateNotificationReceiver {
	virtual void updateEyeMtx() = 0;
};

class ActionStateHandler {
public:
	enum TView {
		EYE,
		LIGHT,
		DEPTH_TEX,
	};

protected:
	IActionStateNotificationReceiver& mReceiver;
	Real mEye_fovY;
	Real mN;
	Real mZ0, mZ1;
	bool mShowHelp;
	bool mShowStatistics;
	bool mFastMoving;
	bool mFly;
	bool mShadowed;
	bool mDirectionalLight; //toggle between directional and spot light
	bool mUseLispSM;  // toggle between LispSM and Uniform shadow mapping
	bool mUseFormula;
	bool mBiasingMethod;
	TView mView;

	Engine::ActionBindings mActionBindings;

	void incN(Engine::Action&) { mN *= 1.2; }
	void decN(Engine::Action&) { mN /= 1.2; }
	void incShear(Engine::Action&);
	void decShear(Engine::Action&);
	void incFov(Engine::Action&);
	void decFov(Engine::Action&);

public:
	ActionStateHandler(IActionStateNotificationReceiver&);
	virtual ~ActionStateHandler();

	const Engine::ActionBindings& getActionBindings() const { return mActionBindings; }
	void doAction(const Engine::KeyState::KEY& k) { mActionBindings.executeAction(k); }

	const TView getView() const { return mView; }

	bool useBiasingMethod() const { return mBiasingMethod; }
	bool showHelp() const { return mShowHelp; }
	bool showStatistics() const { return mShowStatistics; }
	bool fastMoving() const { return mFastMoving; }
	bool useLiSPSM() const { return mUseLispSM; }
	bool useFormula() const { return mUseFormula; }
	void setShadowed(const bool shadowed_) { mShadowed = shadowed_; }
	bool isShadowed() const { return mShadowed; }
	bool isLightDirectional() const { return mDirectionalLight; }
	const Real getFovY() const { return mEye_fovY; }
	bool flying() const { return mFly; }
	void setFly(const bool value_) { mFly = value_; }

	void setN(const Real& n_) {	mN = n_; }
	const Real getN() const { return mN; }

	const Real getZ0() const { return mZ0; }
	const Real getZ1() const { return mZ1; }
};

#endif
