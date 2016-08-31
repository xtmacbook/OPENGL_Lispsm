#include "test.h"


int main(int argc, char* argv[])
{
	// creating an instance from cTest
	cTest iTest;
	
	iTest.TestStart();
	return 0;
}

/****************************************************************

 DEMO number 3 :
 Callbacks from global namespace (e.g. API Callbacks)

*****************************************************************

When writing C++ code you someday get the problem that you want
to be called back by the operating system into any C++ class.

This project is platform independent, so I will describe this only theoretically


Windows API callback functions (as pure C functions) are from type __cdecl 
but C++ functions are from the type __thiscall.
(They differ in the way how they pass arguments on the stack)

So the Windows API CAN NOT callback directly to C++ functions  !!!

But you can use a trick :
If you give a pointer to an instance of cCallback (i_MyCallback) as a parameter 
to the API function (e.g. CreateThread()) 
Windows passes this parameter unchanged to your global Callback function (globalCallback()).
There you can use this parameter to get back to your C++ project as shown below.

Example :
::CreateThread(0, 0, (LPTHREAD_START_ROUTINE) &globalCallback, &i_MyThreadCallback, 0, &ID);
*/




// This function is NOT part of any class, it belongs 
// to the global namespace and is a pure C function (whithout plus plus)!
/*void globalCallback(void* pArgument)
{
	// converting void* pointer into pointer to cCallback instance
	cCallback *pCallback = (cCallback*) pArgument;
	pCallback->Execute((void*) " Greetings from the global namespace!");
}*/



