#ifndef FileToolsH
#define FileToolsH
//---------------------------------------------------------------------------
#pragma warning(disable:4786)  // Disable warning message
#include <fstream>
#include <string>
#include "BaseException.h"
//---------------------------------------------------------------------------
namespace Tools {

struct FileException : public BaseException {
	FileException(const std::string& vMsg): BaseException(vMsg) { };
};

struct FileExistException : public BaseException {
	FileExistException(const std::string& vMsg): BaseException(vMsg) { };
};

inline const bool fileExists(const std::string& fileName) {
	if(fileName.empty()) {
		return false;
	}
	std::ifstream f;
	f.open(fileName.c_str(),std::ios::binary);
    return f != 0;
}

inline const __int64 getFileStreamSize(std::ifstream& file) {
	if(file == 0) {
		return 0;
	}
	else {
		int oldPos = file.tellg();
		file.seekg(0,std::ios::end);
		__int64 size = file.tellg();
		file.seekg(oldPos,std::ios::beg);
		return size;
	}
}

//extension with .
inline std::string getFileExt(const std::string& fileName) {
	std::string::size_type pos = fileName.rfind('.');
    if(pos == std::string::npos) {
        return "";
    }
    return fileName.substr(pos,fileName.length());
}

//extension with .
inline std::string changeFileExt(const std::string& fileName, const std::string& ext) {
    std::string::size_type pos = fileName.rfind('.');
    if(pos == std::string::npos) {
        return fileName+ext;
    }
    return fileName.substr(0,pos)+ext;
}

inline std::string getFileName(const std::string& fileName) {
    std::string::size_type pos = fileName.rfind('/');
    if(pos == std::string::npos) {
        return "";
    }
    return fileName.substr(pos+1,fileName.length());
}

inline std::string getFilePath(const std::string& fileName) {
    std::string::size_type pos = fileName.rfind('/');
    if(pos == std::string::npos) {
        return "";
    }
    return fileName.substr(0,pos);
}

//namespace
}
#endif
