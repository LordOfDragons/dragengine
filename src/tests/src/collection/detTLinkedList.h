// include only once
#ifndef _DETTLINKEDLIST_H_
#define _DETTLINKEDLIST_H_

// includes
#include "../detCase.h"

// Forward declarations
class TestLLObject;

// class detTLinkedList
class detTLinkedList : public detCase {
private:
	void TestBasicAddRemove();
	void TestInsertions();
	void TestIndexingAccess();
	void TestIterators();
	void TestElementIteratorAdvanced();
	void TestStability();

	// Test methods for new functions in commit 1d5552384445f75f7cf67ed21a59f55c7127cae5
	void TestVisit();
	void TestFind();
	void TestFindOrNull();
	void TestRemoveIf();
	void TestBidirectionalIterator();

public:
	detTLinkedList();
	~detTLinkedList() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
