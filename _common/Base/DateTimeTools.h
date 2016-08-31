//---------------------------------------------------------------------------
#ifndef DateTimeToolsH
#define DateTimeToolsH
//---------------------------------------------------------------------------
#include <sys/timeb.h>
#include <time.h>
#include <string>
#include "StringTools.h"

namespace Tools {

inline const std::string getDateTimeStr() {
	const unsigned len = 4+1+2+1+2+1+ 2+1+2+1+2 +1;
	char str[len];
	struct __timeb64 timebuffer;
    struct tm *newtime;
	_ftime64(&timebuffer);  // get time
	newtime = _localtime64(&timebuffer.time); // Convert to local time.
	
	strftime(str,len,"%Y.%m.%d %H:%M:%S",newtime);
	std::string sz = str;
	sz += ':';
	sz += Tools::toString(timebuffer.millitm);
	return sz;
}

//namespace
};

#endif
