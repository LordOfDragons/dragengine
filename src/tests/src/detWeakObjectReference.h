// include only once
#ifndef _DET_WEAKOBJECTREFERENCE_H_
#define _DET_WEAKOBJECTREFERENCE_H_

// includes
#include "detCase.h"

// class detWeakObjectReference
class detWeakObjectReference : public detCase {
private:
	// Test methods
	void TestConstruction();
	void TestAssignment();
	void TestComparison();
	void TestConversion();
	void TestPointerAccess();
	void TestLifecycleWeakRef();
	void TestObjectDeletion();

public:
	detWeakObjectReference();
	~detWeakObjectReference() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
