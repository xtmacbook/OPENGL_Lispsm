#ifndef StringVectorH
#define StringVectorH
//---------------------------------------------------------------------------
#pragma warning(disable:4786)  // Disable warning message
#include <iostream>
#include <string>
#include <vector>
#include "../Base/FileTools.h"
#include "../Base/StringTools.h"
//---------------------------------------------------------------------------
struct StringVector : public std::vector<std::string> {
	StringVector() { }
	StringVector(const std::string& fileName) { 
		loadFromFile(fileName);
	}
	void loadFromFile(const std::string& fileName) /*throw(ExistException)*/ {
		if(!Tools::fileExists(fileName)) {
			throw Tools::FileExistException(fileName);
		}
		std::ifstream f;
		f.open(fileName.c_str());
		char c;
		std::string output;
		clear();
		while(f.get(c)) {
			if('\n' != c) {
				output += c;
			}
			else {
				push_back(output);
				output = "";
			}
		}
		if(!output.empty())
			push_back(output);
	}

	void saveToFile(const std::string& fileName) {
		using namespace std;
		ofstream f;
		f.open(fileName.c_str(),ios_base::out | ios_base::trunc);
		for(const_iterator i = begin(); i != end(); i++) {
			f << *i << '\n';
		}
	}

	void removeLinesStartingWith(const char& filterSign) {
		iterator p = begin();
		while(p != end()) {
			std::string line(Tools::lTrim(*p));
			if(line[0] == filterSign) {
				p = erase(p);
			}
			else {
				p++;
			}

		}
	}
};

#endif
