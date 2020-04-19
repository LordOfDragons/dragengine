// include only once
#ifndef _DETSTRINGLIST_H_
#define _DETSTRINGLIST_H_

// includes
#include "../detCase.h"

// predefinitions


// class detStringList
class detStringList : public detCase{
public:
	detStringList();
	~detStringList();
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

