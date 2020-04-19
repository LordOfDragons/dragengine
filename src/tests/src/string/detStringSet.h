// include only once
#ifndef _DETSTRINGSET_H_
#define _DETSTRINGSET_H_

// includes
#include "../detCase.h"

// predefinitions


// class detStringSet
class detStringSet : public detCase{
public:
	detStringSet();
	~detStringSet();
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

