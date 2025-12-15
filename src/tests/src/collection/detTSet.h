// include only once
#ifndef _DETTSET_H_
#define _DETTSET_H_

// includes
#include "../detCase.h"

// class detTSet
class detTSet : public detCase {
private:
	// Test methods for T=int
	void TestIntBasic();
	void TestIntUniqueness();
	void TestIntOperators();
	void TestIntIterators();
	void TestIntAlgorithms();

	// Test methods for T=decString
	void TestStringBasic();
	void TestStringOperators();

public:
	detTSet();
	~detTSet() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
