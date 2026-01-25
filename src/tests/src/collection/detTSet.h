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
	void TestIntIndexOf();
	void TestIntHasMatching();
	void TestIntAllMatching();
	void TestIntNoneMatching();
	void TestIntFindOrDefault();
	void TestIntFold();
	void TestIntInject();
	void TestIntMoveSemantics();
	void TestIntIteratorConstructor();
	void TestIntCollect();
	void TestIntVisit();
	void TestIntRemoveIf();
	void TestIntFind();

	// Test methods for T=decString
	void TestStringBasic();
	void TestStringOperators();
	void TestStringHasMatching();
	void TestStringAllMatching();
	void TestStringNoneMatching();
	void TestStringFindOrDefault();
	void TestStringFold();
	void TestStringInject();
	void TestStringVisit();
	void TestStringRemoveIf();

public:
	detTSet();
	~detTSet() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
