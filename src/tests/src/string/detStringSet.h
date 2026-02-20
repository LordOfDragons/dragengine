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
	~detStringSet() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
	
private:
	void TestModify();
	void TestTesting();
	void TestOperator();
};

// end of include only once
#endif

