//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef TypedAction_DMS_H
#define TypedAction_DMS_H

#include "Action.h"

namespace Engine {

template<class T>
class TypedAction : public Action {
protected:
	T& mValue;

public:
	TypedAction(T& dataLocation_, const std::string& name_, 
			const std::string& helpText_ = "", CB pCallback_ = 0) : 
		Action(name_,helpText_,pCallback_), mValue(dataLocation_) { }	

	const T& get() const { return mValue; }

	void set(const T& v_) {
		if(v_ != mValue) {
			mValue = v_;
			executeCallback();
		}
	}

	virtual std::string getStateString() const { 
		return Tools::toString(mValue);
	}
};

//namespace
}
#endif
