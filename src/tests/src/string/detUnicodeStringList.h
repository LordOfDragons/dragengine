// include only once
#ifndef _DETUNICODESTRINGLIST_H_
#define _DETUNICODESTRINGLIST_H_

// includes
#include "../detCase.h"

// predefinitions


// class detUnicodeStringList
class detUnicodeStringList : public detCase{
public:
	detUnicodeStringList();
	~detUnicodeStringList() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void TestModify();
	void TestTesting();
	void TestSort();
	void TestOperator();
};

// end of include only once
#endif

