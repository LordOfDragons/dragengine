// include only once
#ifndef _DETTLIST_H_
#define _DETTLIST_H_

// includes
#include "../detCase.h"

// class detTList
class detTList : public detCase {
private:
	// Test methods for T=int
	void TestIntBasic();
	void TestIntAddRemove();
	void TestIntIndexingAccess();
	void TestIntOperators();
	void TestIntIterators();
	void TestIntAlgorithms();

	// Test methods for T=decString
	void TestStringBasic();
	void TestStringOperators();

	// Test methods for T=decXmlElementTag::Ref, TP=decXmlElementTag*
	void TestObjectRefBasic();
	void TestObjectRefOperations();

public:
	detTList();
	~detTList() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
