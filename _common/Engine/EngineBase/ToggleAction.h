//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef ToggleAction_DMS_H
#define ToggleAction_DMS_H

#include "TypedAction.h"

namespace Engine {

struct ToggleAction : public TypedAction<bool> {
	typedef TypedAction<bool> FATHER;
public:
	ToggleAction(bool& dataLocation_, const std::string& name_, const bool& defaultValue_, 
			const std::string& helpText_ = "", CB pCallback_ = 0) : 
			FATHER(dataLocation_,name_,helpText_,pCallback_) { 
		mValue = defaultValue_;
	}	

	void execute() { 
		mValue = !mValue;
		Action::execute();
	}

	virtual std::string getStateString() const { 
		return mValue? "on":"off";
	}
};
typedef SmartPointer<ToggleAction> SmpToggleAction;


//namespace
}
#endif
