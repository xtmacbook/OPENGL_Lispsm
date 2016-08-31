#include "ToolsGL.h"
//---------------------------------------------------------------------------
namespace ToolsGL {
	const unsigned height = 10;
	void* font = GLUT_BITMAP_8_BY_13;
}

void output(const int x, const int y, const std::string& sz) {
	if(sz.empty()) {
		return;
	}
	glRasterPos2f(x,y+ToolsGL::height);
	for(std::string::const_iterator i = sz.begin(); i != sz.end(); i++) {
		glutBitmapCharacter(ToolsGL::font,*i);
	}
}


void multiLineOutput(const int x, const int y, const std::string& sz, const unsigned lineheight) {
	if(sz.empty()) {
		return;
	}
	int newY = y+ToolsGL::height;
	glRasterPos2f(x,newY);
	for(std::string::const_iterator i = sz.begin(); i != sz.end(); i++) {
		if('\n' == *i) {
			newY += lineheight;
			glRasterPos2f(x,newY);
		}
		else {
			glutBitmapCharacter(ToolsGL::font,*i);
		}
	}
}

void multiLineOutput(const int x, const int y, const ToolsGL::VecString& vecSz, const unsigned lineheight) {
	if(vecSz.empty()) {
		return;
	}
	int newY = y+ToolsGL::height;
	for(ToolsGL::VecString::const_iterator i = vecSz.begin(); i != vecSz.end(); i++) {
		output(x,newY,*i);
		newY += lineheight;
	}
}

