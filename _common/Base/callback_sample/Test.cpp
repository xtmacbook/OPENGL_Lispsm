#include <iostream>
#include "Test.h"
#include "cAdd.h"

void cTest::TestStart() {
	Add i_Add(new TCallback<cTest,Add::PARAM>(this,&cTest::Callback1));

	i_Add.AddNumbers(120, 30);
	i_Add.AddNumbers(10, 30);
//	cAdd add(i_Add);
//	i_Add = cAdd(new TCallback<cTest,cAdd::PARAM>(this, &cTest::Callback2));
//	i_Add.AddNumbers(120, 30);
//	i_Add.AddNumbers(120, 30);
}

void cTest::Callback1(char* p) {
	char s8_Out[200];
	sprintf(s8_Out, "********** %s\n", p);
	printf(s8_Out);
}

void cTest::Callback2() {
	char s8_Out[200];
	sprintf(s8_Out, "----------- %s\n");
	printf(s8_Out);
}

