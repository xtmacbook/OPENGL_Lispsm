//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
#ifndef HistoryListH
#define HistoryListH
//---------------------------------------------------------------------------
#include <list>
#include <algorithm>
//---------------------------------------------------------------------------
namespace Math {

template<class T>
class HistoryList {
	typedef std::list<T> LST;
	LST data;
	unsigned size;
	T sum;

	void delFront() {
		sum -= data.front();
		data.erase(data.begin());
	}

public:
	HistoryList(const unsigned count): size(count), sum(0) { }
	
	void add(const T& t) {
		if(data.size() > size) {
			delFront();
		}
		data.push_back(t);
		sum += t;
	}

	void setSampleCount(const unsigned count) { 
		size = count; 
		while(data.size() > size) {
			delFront();
		}
	}

	T last() const { return data.back(); }

	T minimum() const {
		if(data.empty()) {
			return 1;
		}
		return *std::min_element(data.begin(),data.end());
	}

	T maximum() const {
		if(data.empty()) {
			return 1;
		}
		return *std::max_element(data.begin(),data.end());
	}

	T avg() const {
		if(data.empty()) {
			return 1;
		}
		return sum/data.size();
	}

	bool empty()const {return data.empty();}
};

//namespace
}
#endif
