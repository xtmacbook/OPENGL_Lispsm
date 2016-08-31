//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef SetAction_DMS_H
#define SetAction_DMS_H

#include "TypedAction.h"

namespace Engine {

template<class T>
class SetAction : public TypedAction<T> {
	typedef TypedAction<T> FATHER;
	const T mSetValue;

public:
	SetAction(T& dataLocation_, const std::string& name_, const T& setValue_,
			const std::string& helpText_ = "", CB pCallback_ = 0) : 
		FATHER(dataLocation_,name_,helpText_,pCallback_), mSetValue(setValue_) { }	

	void execute() { 
		set(mSetValue);
	}
};


//namespace
}
#endif
