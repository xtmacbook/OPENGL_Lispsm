// Test.h

#if !defined(TEST_H)
#define TEST_H


// globalCallback is in global namespace
extern void globalCallback(void* Argument);


class cTest  
{
public:
	void TestStart();


private:
	void Callback1(char*);
	void Callback2();
	void Callback3(char*);

};

#endif // !defined(TEST_H)
