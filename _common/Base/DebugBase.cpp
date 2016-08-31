#ifdef __DMS_DEBUG_BASE__

	//#define __DMS_DEBUG_BASE_MSG__
	#ifdef __DMS_DEBUG_BASE_MSG__
		#include <string>
		#include "../helper/Helper.h"
		#include "Debug.h"
	#endif
	#ifndef __DMS_DEBUG_BASE_MSG__
		#include "NoDebug.h"
	#endif
	//---------------------------------------------------------------------------
	#include "Base.h"
	#include "BaseException.h"
	//---------------------------------------------------------------------------
	uint Base::objectCount = 0;

	Base::Base() : cRefs(0) {
		DEBUG("base constructor");
		objectCount++;
	}

	Base::~Base() {
		DEBUG("base destructor");
		objectCount--;
	}

	void Base::newRef() {
		++cRefs;
		DEBUG("up to:"+helper::toString(cRefs));
	}

	void Base::delRef() {
		if (--cRefs == 0) {
			delete this;
		}
		else
			DEBUG("downto:"+helper::toString(cRefs));
	}

#endif