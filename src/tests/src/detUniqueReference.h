// include only once
#ifndef _DET_UNIQUEREFERENCE_H_
#define _DET_UNIQUEREFERENCE_H_

// includes
#include "detCase.h"

// class detUniqueReference
class detUniqueReference : public detCase {
private:
	// Test methods
	void TestConstruction();
	void TestAssignment();
	void TestComparison();
	void TestConversion();
	void TestPointerAccess();
	void TestLifecycle();
	void TestMoveSemantics();
	void TestCasting();
	void TestHash();
	void TestClear();
	void TestSwap();

public:
	detUniqueReference();
	~detUniqueReference() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
