// include only once
#ifndef _DETTORDEREDSET_H_
#define _DETTORDEREDSET_H_

// includes
#include "../detCase.h"
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlElement.h>

// class detTOrderedSet
class detTOrderedSet : public detCase {
private:
	// Test methods for T=int
	void TestIntConstructors();
	void TestIntGetCount();
	void TestIntIsEmpty();
	void TestIntIsNotEmpty();
	void TestIntGetAt();
	void TestIntFirst();
	void TestIntLast();
	void TestIntIndexOf();
	void TestIntHas();
	void TestIntSetAt();
	void TestIntAdd();
	void TestIntAddIfAbsent();
	void TestIntInsert();
	void TestIntMove();
	void TestIntRemove();
	void TestIntRemoveIfPresent();
	void TestIntRemoveFrom();
	void TestIntRemoveAll();
	void TestIntEquals();
	void TestIntGetHead();
	void TestIntGetTail();
	void TestIntGetMiddle();
	void TestIntVisit();
	void TestIntFind();
	void TestIntCollect();
	void TestIntRemoveIf();
	void TestIntSort();
	void TestIntOperators();
	void TestIntGetHeadRef();
	void TestIntGetTailRef();
	void TestIntGetMiddleRef();
	void TestIntGetSliced();
	void TestIntVisitOverloads();
	void TestIntFindOverloads();
	void TestIntFindDefault();
	void TestIntCollectOverloads();
	void TestIntRemoveIfOverloads();
	void TestIntGetSorted();
	void TestIntOperatorNotEqual();
	void TestIntIterators();

	// Test methods for T=decString
	void TestStringConstructors();
	void TestStringGetCount();
	void TestStringIsEmpty();
	void TestStringIsNotEmpty();
	void TestStringGetAt();
	void TestStringFirst();
	void TestStringLast();
	void TestStringIndexOf();
	void TestStringHas();
	void TestStringSetAt();
	void TestStringAdd();
	void TestStringAddIfAbsent();
	void TestStringInsert();
	void TestStringMove();
	void TestStringRemove();
	void TestStringRemoveIfPresent();
	void TestStringRemoveFrom();
	void TestStringRemoveAll();
	void TestStringEquals();
	void TestStringGetHead();
	void TestStringGetTail();
	void TestStringGetMiddle();
	void TestStringVisit();
	void TestStringFind();
	void TestStringCollect();
	void TestStringRemoveIf();
	void TestStringSort();
	void TestStringOperators();
	void TestStringGetHeadRef();
	void TestStringGetTailRef();
	void TestStringGetMiddleRef();
	void TestStringGetSliced();
	void TestStringVisitOverloads();
	void TestStringFindOverloads();
	void TestStringFindDefault();
	void TestStringCollectOverloads();
	void TestStringRemoveIfOverloads();
	void TestStringGetSorted();
	void TestStringOperatorNotEqual();
	void TestStringIterators();

	// Test methods for T=deObject::Ref, TP=deObject*
	void TestObjectRefConstructors();
	void TestObjectRefGetCount();
	void TestObjectRefIsEmpty();
	void TestObjectRefIsNotEmpty();
	void TestObjectRefGetAt();
	void TestObjectRefFirst();
	void TestObjectRefLast();
	void TestObjectRefIndexOf();
	void TestObjectRefHas();
	void TestObjectRefSetAt();
	void TestObjectRefAdd();
	void TestObjectRefAddIfAbsent();
	void TestObjectRefInsert();
	void TestObjectRefMove();
	void TestObjectRefRemove();
	void TestObjectRefRemoveIfPresent();
	void TestObjectRefRemoveFrom();
	void TestObjectRefRemoveAll();
	void TestObjectRefEquals();
	void TestObjectRefGetHead();
	void TestObjectRefGetTail();
	void TestObjectRefGetMiddle();
	void TestObjectRefVisit();
	void TestObjectRefFind();
	void TestObjectRefCollect();
	void TestObjectRefRemoveIf();
	void TestObjectRefSort();
	void TestObjectRefOperators();
	void TestObjectRefGetHeadRef();
	void TestObjectRefGetTailRef();
	void TestObjectRefGetMiddleRef();
	void TestObjectRefGetSliced();
	void TestObjectRefVisitOverloads();
	void TestObjectRefFindOverloads();
	void TestObjectRefFindDefault();
	void TestObjectRefCollectOverloads();
	void TestObjectRefRemoveIfOverloads();
	void TestObjectRefGetSorted();
	void TestObjectRefOperatorNotEqual();
	void TestObjectRefIterators();

public:
	detTOrderedSet();
	~detTOrderedSet() override;
	void Prepare() override;
	void Run() override;
	void CleanUp() override;
	const char *GetTestName() override;
};

// end of include only once
#endif
