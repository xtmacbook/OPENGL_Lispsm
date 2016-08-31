//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#include <typeinfo>
#define NOMINMAX
#include <Windows.h>
#include <crtdbg.h>
#include <string>
#include "OSFunc.h"
#include <Base/BaseException.h>
#include <Base/StringTools.h>
//-------------------------------------------------------------------------------------------
namespace OS {

void sleep(const unsigned msec) {
	::Sleep(msec);
}

void showMessage(const char* msg) {
	::MessageBox(0,msg,"Error",MB_OK);
}

void showMessage(const char* title, const char* msg) {
	::MessageBox(0,msg,title,MB_OK);
}

void showException(const BaseException& e) {
	OS::showMessage(typeid(e).name(),e.what());
}

std::string getErrorString(const unsigned error) {
    char* lpMsgBuf = new char[400];
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
    );
    std::string msg = Tools::toString(lpMsgBuf);
    // Free the buffer.
    delete lpMsgBuf;
    return msg;
}

std::string getLastErrorString() {
    DWORD err = GetLastError();
    if(0 != err) {
        SetLastError(0);
        return OS::getErrorString(err);
    }
    else {
        return "";
    }
}

//Now just call this function at the start of your program and if you're
//compiling in debug mode (F5), any leaks will be displayed in the Output
//window when the program shuts down. If you're not in debug mode this will
//be ignored. Use it as you will!
//note: from GDNet Direct [3.8.04 - 3.14.04] 
void detectMemoryLeaks() {
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR);
}


//namespace
}

