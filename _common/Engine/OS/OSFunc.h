//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef __DMS_OSFUNC_H__
#define __DMS_OSFUNC_H__

#include <string>

class BaseException;

namespace OS {

	void sleep(const unsigned msec);
	void showMessage(const char* msg);
	void showMessage(const char* title, const char* msg);
	void showException(const BaseException&);
    std::string getErrorString(const unsigned error);
    std::string getLastErrorString();
	void detectMemoryLeaks();

//namespace
}

#endif  // __DMS_OSFUNC_H__
