#define NOMINMAX
#include <Windows.h>
#include "OSFuncExt.h"
#include <Engine/WindowManager/Window.h>
//-------------------------------------------------------------------------------------------
namespace OS {

void mousePos(const Window& wnd, const unsigned x, const unsigned y) {
    SetCursorPos(wnd.getClientX()+x,wnd.getClientY()+y);
}

//namespace
}

