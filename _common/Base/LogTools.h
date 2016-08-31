//---------------------------------------------------------------------------
#ifndef LogToolsH
#define LogToolsH
//---------------------------------------------------------------------------
#include <iostream>
#include "BaseException.h"
#include "TextFileTools.h"
#include "DateTimeTools.h"
//---------------------------------------------------------------------------
namespace Tools {

class Logger {
protected:
	typedef std::string string;
	const std::string mLogFileName;
	bool mCoutEcho;
public:
	Logger(const string& logFileName_, const bool coutEcho_ = false): 
		mLogFileName(logFileName_), mCoutEcho(coutEcho_) { }

	inline void log(const string msg) {
		string output = getDateTimeStr();
		output += "  ";
		output += msg;
		appendStringLineToTextFile(mLogFileName,output);
		if(mCoutEcho) {
			std::cout << msg << std::endl;
		}
	}

	inline void emptyLog() {
		emptyTextFile(mLogFileName);
	}

	inline void log(const BaseException e) {
		string msg;
		msg += '[';
		msg += typeid(e).name();
		msg += ']';
		msg += e.getMsg();
		log(msg);
	}
};

static std::string logFileName = "d:/thelog.log";
static bool logCoutEcho = false;

inline void log(const std::string msg) {
	std::string output = getDateTimeStr();
	output += "  ";
	output += msg;
	appendStringLineToTextFile(logFileName,output);
	if(logCoutEcho) {
		std::cout << msg << std::endl;
	}
}

inline void emptyLog() {
	emptyTextFile(logFileName);
}

inline void log(const BaseException e) {
	std::string msg;
	msg += '[';
	msg += typeid(e).name();
	msg += ']';
	msg += e.getMsg();
	log(msg);
}

//namespace
};

#endif
