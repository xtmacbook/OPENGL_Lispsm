//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef KeyState_DMS_H
#define KeyState_DMS_H

#include <hash_map>
#include <string>
//---------------------------------------------------------------------------
namespace Engine {

struct KeyConst {
	typedef int Key;

	static std::string getKeyString(const Key&);

	static const Key KEY_TAB;
	static const Key KEY_BACKSPACE;
	static const Key KEY_ESCAPE;
    static const Key KEY_ENTER;
    static const Key KEY_LEFT_ARROW;
    static const Key KEY_RIGHT_ARROW;
    static const Key KEY_UP_ARROW;
    static const Key KEY_DOWN_ARROW;
    static const Key KEY_HOME;
    static const Key KEY_END;
    static const Key KEY_PAGE_UP;
    static const Key KEY_PAGE_DOWN;
    static const Key KEY_INSERT;
    static const Key KEY_DELETE;
    static const Key KEY_SPACE;
    static const Key KEY_F1;
    static const Key KEY_F2;
    static const Key KEY_F3;
    static const Key KEY_F4;
    static const Key KEY_F5;
    static const Key KEY_F6;
    static const Key KEY_F7;
    static const Key KEY_F8;
    static const Key KEY_F9;
    static const Key KEY_F10;
    static const Key KEY_F11;
    static const Key KEY_F12;
	static const Key KEY_MOUSE_LEFT;
	static const Key KEY_MOUSE_MIDDLE;
	static const Key KEY_MOUSE_RIGHT;
//	static const Key KEY_MOUSE_X;
//	static const Key KEY_MOUSE_Y;
	static const Key KEY_SHIFT;
	static const Key KEY_CTRL;
	static const Key KEY_ALT;
};

class KeyBuffer {
protected:
	typedef KeyConst::Key Key;
	typedef stdext::hash_map<Key,bool> MapKey;
	MapKey keys;
public:
	virtual bool isKeyDown(const Key&) const;
	virtual void setKeyDown(const Key&);
	virtual void setKeyUp(const Key&);
};

class MousePosBuffer {
protected:
	int _x, _y;
public:
	const int getMouseX() const { return _x; }
	const int getMouseY() const { return _y; }
};


struct IInputEventHandler;


class KeyState {
public:
	typedef int KEY;
protected:
	int _x, _y;
	typedef stdext::hash_map<KEY,bool> MapKey;
	MapKey keys;
	IInputEventHandler* hndl; 

public:
	KeyState(IInputEventHandler* ihndl = 0);
	virtual ~KeyState() { }
	virtual void keyDown(const KEY);
	virtual void keyUp(const KEY);
	virtual bool isKeyDown(const KEY) const;

	virtual void mouseMove(const int x, const int y);
	int getMouseX() const { return _x; }
	int getMouseY() const { return _y; }
};

struct IInputEventHandler {
	typedef KeyConst::Key Key;
	virtual void onKeyDown(const Key& key) = 0;
	virtual void onKeyUp(const Key& key) = 0;
	virtual void onMouseMove(const int x, const int y) = 0;
//	virtual KeyBuffer* getKeyBuffer() = 0;
};


//namespace
}
#endif
