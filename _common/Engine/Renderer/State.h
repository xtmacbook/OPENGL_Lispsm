//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
#ifndef StateH
#define StateH
//---------------------------------------------------------------------------
#include <Engine/Smp.h>
#include <Base/Singleton.h>
//---------------------------------------------------------------------------
class StateManager;

struct State : public Base , public Singleton {
	static State EMPTY;
	virtual void begin() { }
	virtual void end() { }
//	inline bool noState() const { return &State::EMPTY == this; }
};
typedef SmartPointer<State> SmpState;

class StateManager {
protected:
	State* currentState;
	unsigned stateChangeCount;

public:
	StateManager(): stateChangeCount(0), currentState(&State::EMPTY) { }
	State* getState() const { return currentState; }

	void setState(State& newState) {
		if(&newState != currentState) {
			stateChangeCount++;
			currentState->end();
			currentState = &newState;
			currentState->begin();
		}
	}

	const unsigned getStateChangeCount() const { return stateChangeCount; }
};

#endif
