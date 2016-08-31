//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#include <Base/StringTools.h>
#include "KeyState.h"
//---------------------------------------------------------------------------
namespace Engine {

std::string KeyConst::getKeyString(const Key& key) {
	//todo: other string representations of Keys
	//todo: hash table for string lookup
	std::string output;
	if(key == KEY_TAB) {
		output = "TAB";
	}
	else if(key == KEY_BACKSPACE) {
		output = "BACKSPACE";
	}
	else if(key == KEY_SPACE) {
		output = "SPACE";
	}
	else if(KEY_F1 <= key && key <= KEY_F12) {
		output = "F";
		output += Tools::toString(key-KEY_F1+1);
	}
	else if(key > 255) {
		output = Tools::toString(key);
	}
	else {
		output = key;
	}
	return output;
}

void KeyBuffer::setKeyDown(const Key& key) { 
	keys[key] = true; 
}

void KeyBuffer::setKeyUp(const Key& key) { 
	keys[key] = false; 
}

bool KeyBuffer::isKeyDown(const Key& key) const { 
	MapKey::const_iterator i = keys.find(key);
	if(keys.end() != i) {
		return i->second;
	}
	else {
		return false;
	}
}


KeyState::KeyState(IInputEventHandler* ihndl): hndl(ihndl), _x(0), _y(0) { 
}

void KeyState::keyDown(const KEY key) { 
	keys[key] = true; 
	if(0 != hndl) {
		hndl->onKeyDown(key);
	}
}

void KeyState::keyUp(const KEY key) { 
	keys[key] = false; 
	if(0 != hndl) {
		hndl->onKeyUp(key);
	}
}

bool KeyState::isKeyDown(const KEY key) const { 
	MapKey::const_iterator i = keys.find(key);
	if(keys.end() != i) {
		return i->second;
	}
	else {
		return false;
	}
}

void KeyState::mouseMove(const int x, const int y) { 
	_x = x;
	_y = y;
	if(0 != hndl) {
		hndl->onMouseMove(x,y);
	}
}

//namespace
}
