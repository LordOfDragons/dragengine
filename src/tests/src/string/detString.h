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
	~detString() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
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

