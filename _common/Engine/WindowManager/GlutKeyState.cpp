//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#include "GlutKeyState.h"
#include <GL/glHeader.h>
//---------------------------------------------------------------------------
//#pragma package(smart_init)
namespace Engine {

const KeyConst::Key GlutKeyConst::keyDelta = 256;

const KeyConst::Key KeyConst::KEY_TAB = '\t';
const KeyConst::Key KeyConst::KEY_BACKSPACE = '\b';
const KeyConst::Key KeyConst::KEY_ESCAPE = 0x1B;
const KeyConst::Key KeyConst::KEY_ENTER = 0x0D;
const KeyConst::Key KeyConst::KEY_LEFT_ARROW = GLUT_KEY_LEFT+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_RIGHT_ARROW = GLUT_KEY_RIGHT+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_UP_ARROW = GLUT_KEY_UP+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_DOWN_ARROW = GLUT_KEY_DOWN+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_HOME = GLUT_KEY_HOME+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_END = GLUT_KEY_END+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_PAGE_UP = GLUT_KEY_PAGE_UP+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_PAGE_DOWN = GLUT_KEY_PAGE_DOWN+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_INSERT = GLUT_KEY_INSERT+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_DELETE = 0x7F;
const KeyConst::Key KeyConst::KEY_SPACE = ' ';
const KeyConst::Key KeyConst::KEY_F1 = GLUT_KEY_F1+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F2 = GLUT_KEY_F2+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F3 = GLUT_KEY_F3+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F4 = GLUT_KEY_F4+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F5 = GLUT_KEY_F5+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F6 = GLUT_KEY_F6+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F7 = GLUT_KEY_F7+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F8 = GLUT_KEY_F8+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F9 = GLUT_KEY_F9+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F10 = GLUT_KEY_F10+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F11 = GLUT_KEY_F11+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_F12 = GLUT_KEY_F12+GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_MOUSE_LEFT = GLUT_LEFT_BUTTON+2*GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_MOUSE_MIDDLE = GLUT_MIDDLE_BUTTON+2*GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_MOUSE_RIGHT = GLUT_RIGHT_BUTTON+2*GlutKeyConst::keyDelta;
//const KeyConst::Key KeyConst::KEY_MOUSE_X = 3+2*GlutKeyConst::keyDelta;
//const KeyConst::Key KeyConst::KEY_MOUSE_Y = 4+2*GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_SHIFT = GLUT_ACTIVE_SHIFT+3*GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_CTRL = GLUT_ACTIVE_CTRL+3*GlutKeyConst::keyDelta;
const KeyConst::Key KeyConst::KEY_ALT = GLUT_ACTIVE_ALT+3*GlutKeyConst::keyDelta;


GlutKeyState::GlutKeyState(IInputEventHandler* ihndl): KeyState(ihndl) { 
}

void GlutKeyState::checkGlutModifiers() {
	int state = glutGetModifiers();
	keys[KeyConst::KEY_SHIFT] = (state & GLUT_ACTIVE_SHIFT) != 0;
	keys[KeyConst::KEY_CTRL] = (state & GLUT_ACTIVE_CTRL) != 0;
	keys[KeyConst::KEY_ALT] = (state & GLUT_ACTIVE_ALT) != 0;
}

const KeyState::KEY GlutKeyState::convertGlutSpecialKey(const int key) {
	return key+GlutKeyConst::keyDelta;
}

const KeyState::KEY GlutKeyState::convertGlutMouseButton(const int key) {
	return key+2*GlutKeyConst::keyDelta;
}

const KeyState::KEY GlutKeyState::convertGlutKey(const char key) {
	return key;
}

void GlutKeyState::keyDown(const KeyState::KEY key) {
	checkGlutModifiers();
	KeyState::keyDown(key);
}

void GlutKeyState::keyUp(const KeyState::KEY key) {
	//don't check modifiers here that doesn't work
	KeyState::keyUp(key);
}

void GlutKeyState::glutMouseButton(const int button, const int state) {
	checkGlutModifiers();
	if(state == GLUT_UP) {
		KeyState::keyUp(convertGlutMouseButton(button));
	}
	else {
		KeyState::keyDown(convertGlutMouseButton(button));
	}
}

/*void GlutKeyState::createEventGLUTModifiers(IInputEventHandler&) {
	int state = glutGetModifiers();
	if(
	keys[KEY_SHIFT] = (state & GLUT_ACTIVE_SHIFT) != 0;
	keys[KEY_CTRL] = (state & GLUT_ACTIVE_CTRL) != 0;
	keys[KEY_ALT] = (state & GLUT_ACTIVE_ALT) != 0;
}*/

void GlutKeyState::createEventGLUTMouseButton(IInputEventHandler& hndl, const int button, const int state) {
	if(state == GLUT_UP) {
		hndl.onKeyUp(convertGlutMouseButton(button));
	}
	else {
		hndl.onKeyDown(convertGlutMouseButton(button));
	}
}

//namespace
}