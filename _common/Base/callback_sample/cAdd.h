//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#ifndef AddH
#define AddH
//---------------------------------------------------------------------------
#include <Base/System.h>
#include "../Callback.h"
//---------------------------------------------------------------------------
class Add {
public:
	typedef char* PARAM;
protected:
	typedef ICallback<PARAM>* CB;
	CB callback;

public:

	Add(CB pCallback): callback(pCallback) { }
//	Add(const Add& add): callback(pCallback) { }
//	Add(const ICallback& refCallback): callback(new pCallback) { }
	virtual ~Add() { saveDelete(callback); }

	void AddNumbers(int Num_A, int Num_B)
	{
		char s8_Out[50];
		sprintf(s8_Out, "Adding %3d + %3d = %3d", Num_A, Num_B, Num_A + Num_B);
		callback->execute(s8_Out);
	}

};

#endif // !defined(CADD_H)
