#ifndef TextFileToolsH
#define TextFileToolsH
//---------------------------------------------------------------------------
#pragma warning(disable:4786)  // Disable warning message
#include <fstream>
#include <string>
#include "BaseException.h"
#include "FileTools.h"
//---------------------------------------------------------------------------
namespace Tools {

inline void appendTextFileToString(const std::string& fileName, std::string& sz) {
	if(!fileExists(fileName)) {
		throw FileExistException(fileName);
	}
	std::ifstream f;
	f.open(fileName.c_str());
	std::string::size_type s = std::string::size_type(getFileStreamSize(f));
	sz.reserve(sz.size()+s);
	char c;
	while(f.get(c)) {
		sz += c;
	}
}

inline void appendStringToTextFile(const std::string& fileName, const std::string& sz) {
	using namespace std;
	ofstream f;
	f.open(fileName.c_str(), ios_base::out | ios_base::app);
	f << sz;
}

inline void appendStringLineToTextFile(const std::string& fileName, const std::string& sz) {
	appendStringToTextFile(fileName,sz+'\n');
}

inline void emptyTextFile(const std::string& fileName) {
	using namespace std;
	ofstream f;
	f.open(fileName.c_str(), ios_base::out | ios_base::trunc);
}

template<class T> 
inline const bool parseTextFile(const std::string& fileName, T& destination) {
	if(!fileExists(fileName)) {
		return false;
	}
	std::ifstream f;
	f.open(fileName.c_str());
	f >> destination;
	return f.good();
}

//namespace
}
#endif
