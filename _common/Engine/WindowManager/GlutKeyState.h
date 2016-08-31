//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef GlutKeyStateH
#define GlutKeyStateH

#include "../EngineBase/KeyState.h"
//---------------------------------------------------------------------------
namespace Engine {

struct GlutKeyConst: public KeyConst {
	static const Key keyDelta;
};

class GlutKeyState: public KeyState {
protected:
	void checkGlutModifiers();
	friend class KeyState;

public:
	GlutKeyState(IInputEventHandler* ihndl = 0);
	static const KEY convertGlutMouseButton(const int key);
	static const KEY convertGlutSpecialKey(const int key);
	static const KEY convertGlutKey(const char key);

	virtual void keyDown(const KEY key);
	virtual void keyUp(const KEY key);
	void glutMouseButton(const int button, const int state);
//	static void createEventGLUTModifiers(IInputEventHandler&);
	static void createEventGLUTMouseButton(IInputEventHandler&, const int button, const int state);
};


//namespace
}
#endif
