// include only once
#ifndef _DETUNICODE_H_
#define _DETUNICODE_H_

// includes
#include "../detCase.h"

// predefinitions
class decUnicode;


// class detUnicode
class detUnicodeString : public detCase{
public:
	detUnicodeString();
	~detUnicodeString();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
private:
	void TestDecodeUTF8();
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

