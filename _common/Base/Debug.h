#ifndef __DMS_DEBUG_H__
#define __DMS_DEBUG_H__

	#ifdef _DEBUG
		#include <iostream>
		#include "logTools.h"
		template<class T> inline void DEBUG(const T& input) { 
			std::cerr << input << std::endl;
			using namespace Tools;
			log(toString(input));
		}
	#endif

	#ifndef _DEBUG
		#define DEBUG(ostrm)
	#endif

#endif // __DMS_DEBUG_H__