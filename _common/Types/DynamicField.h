//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#ifndef DynamicFieldH
#define DynamicFieldH
#include "DynamicArray.h"
//---------------------------------------------------------------------------
template<class T> 
struct DynamicField {
	typedef Array<T> DataOut;
	typedef DynamicArray<T> Data;
	typedef T ElementType;
protected:
	unsigned X,Y;
	Data data;

public:
	DynamicField(): X(0), Y(0) { }

	DynamicField(const unsigned xSize, const unsigned ySize): X(xSize), Y(ySize) {
		resize(X,Y);
	}

	//invalidates data
	void resize(const unsigned xSize, const unsigned ySize) {
		if(xSize != X || ySize != Y) {
			data.resize(xSize*ySize);
			X = xSize;
			Y = ySize;
		}
    }

 	void fillWith(const T& d) {
		data.fillWith(d);
	}

	const T& operator()(const unsigned x, const unsigned y) const { return data[x+X*y]; }
	T& operator()(const unsigned x, const unsigned y) { return data[x+X*y]; }

	const T& operator[](const unsigned i) const { return data[i]; }
	T& operator[](const unsigned i) { return data[i]; }

	const DataOut& getData() const { return data; }
	DataOut& getData() { return data; } //DataOut is only an Array -> no size change possible

	const unsigned sizeX() const { return X; }
	const unsigned sizeY() const { return Y; }
	const unsigned size() const { return data.size(); }

};

#endif


