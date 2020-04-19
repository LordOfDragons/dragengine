// include only once
#ifndef _DETUNICODESTRINGSET_H_
#define _DETUNICODESTRINGSET_H_

// includes
#include "../detCase.h"

// predefinitions


// class detUnicodeStringSet
class detUnicodeStringSet : public detCase{
public:
	detUnicodeStringSet();
	~detUnicodeStringSet();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void TestModify();
	void TestTesting();
	void TestSort();
	void TestOperator();
};

// end of include only once
#endif

