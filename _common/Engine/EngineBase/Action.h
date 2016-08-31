//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef Action_DMS_H
#define Action_DMS_H

#include <list>
#include <hash_map>
#include <Base/System.h>
#include <Base/BaseException.h>
#include <Base/Persistent.h>
#include <Base/Callback.h>
#include <Base/StringTools.h>
#include "../Smp.h"
#include "KeyState.h"

namespace Engine {

class Action : public Base {
public:
	typedef Action& PARAM;
	typedef ICallback<PARAM>* CB;
	CB callback;

	const std::string name;
	const std::string helpText;

	Action(const std::string& name_, const std::string& helpText_ = "", CB pCallback_ = 0) : 
			name(name_), helpText(helpText_), callback(pCallback_) { }	

	virtual ~Action() {
		DMS::saveDelete(callback);
	}

	void executeCallback() { 
		if(0 != callback) {
			callback->execute(*this);
		}
	}

	virtual	void execute() {
		executeCallback();
	}

	virtual std::string getStateString() const = 0;

};
typedef SmartPointer<Action> SmpAction;

struct ActionBindings : public Persistent {
	typedef std::pair<KeyState::KEY,SmpAction> PairKeyAct;

protected:
	typedef stdext::hash_map<std::string,SmpAction> MapName;
	typedef stdext::hash_map<KeyState::KEY,SmpAction> MapKey;
	typedef std::list<PairKeyAct> LstKeyAct;
	MapName mapName;
	MapKey mapKey;
	LstKeyAct mLstKeyAct;

	Action& getAction(const MapKey::key_type actionKey) const {
		MapKey::const_iterator i = mapKey.find(actionKey);
		if(mapKey.end() == i) {
			throw BaseException("Invalid Action "+KeyConst::getKeyString(actionKey)+" requested");
		}
		return *i->second;
	}

	Action& getAction(const MapName::key_type actionName) const {
		MapName::const_iterator i = mapName.find(actionName);
		if(mapName.end() == i) {
			throw BaseException("Invalid Action "+actionName+" requested");
		}
		return *i->second;
	}

public:
	typedef LstKeyAct::const_iterator const_iterator;

	void add(SmpAction action,const KeyState::KEY& key) {
		//check if not already registered
		if(mapName.end() != mapName.find(action->name)) {
			throw BaseException("ActionBindings: Action "+action->name+" already registered");
		}
		if(mapKey.end() != mapKey.find(key)) {
			throw BaseException("ActionBindings: Key "+Tools::toString(key)+" already registered");
		}
		//good -> add action
		mLstKeyAct.push_back(PairKeyAct(key,action));
		mapName[action->name] = action;
		mapKey[key] = action;
	}

	ActionBindings() { }

	template <class T>
	void executeAction(const T& actionID) {
		getAction(actionID).execute();
	}

	template <class T>
	void executeCallbackAction(const T& actionID) {
		getAction(actionID).executeCallback();
	}

	const_iterator begin() const { return mLstKeyAct.begin(); }
	const_iterator end() const { return mLstKeyAct.end(); }
};

//namespace
}
#endif
