C++ Callback demo 2.0
-----------------------
by Elmü  (http://elmue.de.vu)


This fully functional example shows how in C++ callbacks can be done in an absolute flexible way ! 

This demo is written on Visual Studio 6 but the Callback class is platform independent 
and also runs under Linux, Mac etc.... 

Callbacks in C++ are not as simple as in C. 
Pure C functions are from the type __cdecl. 
C++ functions are from the type __thiscall. 
(They differ in the way how they pass arguments on the stack)

In C++ you have classes and additionally instances of classes. 
Every instance uses its own memory area for storing class variables. 
The pointer to this area of variables is the "this" pointer. It represents the instance. 
Every time you call any C++ function the "this" pointer is passed to the function as an invisible parameter! 
(M$ Visual Studio 6 uses the processor register ECX to pass the "this" pointer) 

So in C++ it is not enough to store the address of the function, which you want to call back. 
You also have to store the "this" pointer! 



