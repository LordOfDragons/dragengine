// include only once
#ifndef _DET_THREADSAFEOBJECTREFERENCE_H_
#define _DET_THREADSAFEOBJECTREFERENCE_H_

// includes
#include "detCase.h"

// class detThreadSafeObjectReference
class detThreadSafeObjectReference : public detCase {
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
	void TestUpcastDowncast();

public:
	detThreadSafeObjectReference();
	~detThreadSafeObjectReference() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
