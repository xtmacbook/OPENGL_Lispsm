//---------------------------------------------------------------------------
#ifndef ToolsGLH
#define ToolsGLH

#include <string>
#include <vector>
#include <GL/glHeader.h>
//---------------------------------------------------------------------------
namespace ToolsGL {
	typedef std::vector<std::string> VecString;
}

inline unsigned getTime()  { return glutGet(GLUT_ELAPSED_TIME); }

void output(const int x, const int y, const std::string& sz);

void multiLineOutput(const int x, const int y, const std::string& sz, const unsigned lineheight = 20);

void multiLineOutput(const int x, const int y, const ToolsGL::VecString& vecSz, const unsigned lineheight = 20);
#endif
