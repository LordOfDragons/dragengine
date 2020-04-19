// include only once
#ifndef _DETSTRING_H_
#define _DETSTRING_H_

// includes
#include "../detCase.h"

// predefinitions


// class detString
class detString : public detCase{
public:
	detString();
	~detString();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestConstructor();
	void TestAccessors();
	void TestMutators();
	void TestOperators();
	void TestAppend();
	void TestConversion();
	void TestFormat();
	void TestFind();
	void TestSubString();
	void TestReverse();
	void TestSplit();
	void TestReplace();
	void TestTrim();
	void TestLowerUpper();
};

// end of include only once
#endif

