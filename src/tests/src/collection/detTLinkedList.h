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

public:
	detTLinkedList();
	~detTLinkedList() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

#endif
