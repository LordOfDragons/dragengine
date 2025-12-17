// includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cstdint>

#include "detTOrderedSet.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>

// Type definition for template with object references
typedef decTOrderedSet<int> decTOrderedSetInt;
typedef decTOrderedSet<decString> decTOrderedSetString;
typedef decTObjectOrderedSet<decXmlElementTag> decTOrderedSetXmlElementTag;


// Class detTOrderedSet
///////////////////////

// Constructors, destructor
/////////////////////////////

detTOrderedSet::detTOrderedSet(){
	Prepare();
}

detTOrderedSet::~detTOrderedSet(){
	CleanUp();
}


// Testing
////////////

void detTOrderedSet::Prepare(){
}

void detTOrderedSet::Run(){
	// Test integer type
	TestIntConstructors();
	TestIntGetCount();
	TestIntIsEmpty();
	TestIntIsNotEmpty();
	TestIntGetAt();
	TestIntFirst();
	TestIntLast();
	TestIntIndexOf();
	TestIntHas();
	TestIntSetAt();
	TestIntAdd();
	TestIntAddIfAbsent();
	TestIntInsert();
	TestIntMove();
	TestIntRemove();
	TestIntRemoveIfPresent();
	TestIntRemoveFrom();
	TestIntRemoveAll();
	TestIntEquals();
	TestIntGetHead();
	TestIntGetTail();
	TestIntGetMiddle();
	TestIntVisit();
	TestIntFind();
	TestIntCollect();
	TestIntRemoveIf();
	TestIntSort();
	TestIntOperators();
	TestIntGetHeadRef();
	TestIntGetTailRef();
	TestIntGetMiddleRef();
	TestIntGetSliced();
	TestIntVisitOverloads();
	TestIntFindOverloads();
	TestIntFindOrDefault();
	TestIntCollectOverloads();
	TestIntRemoveIfOverloads();
	TestIntGetSorted();
	TestIntOperatorNotEqual();
	TestIntIterators();
	TestIntIndexOfMatching();
	TestIntHasMatching();
	TestIntFold();
	TestIntInject();
	TestIntReverse();
	TestIntGetReversed();
	TestIntFindReverseOrDefault();
	
	// Test string type
	TestStringConstructors();
	TestStringGetCount();
	TestStringIsEmpty();
	TestStringIsNotEmpty();
	TestStringGetAt();
	TestStringFirst();
	TestStringLast();
	TestStringIndexOf();
	TestStringHas();
	TestStringSetAt();
	TestStringAdd();
	TestStringAddIfAbsent();
	TestStringInsert();
	TestStringMove();
	TestStringRemove();
	TestStringRemoveIfPresent();
	TestStringRemoveFrom();
	TestStringRemoveAll();
	TestStringEquals();
	TestStringGetHead();
	TestStringGetTail();
	TestStringGetMiddle();
	TestStringVisit();
	TestStringFind();
	TestStringCollect();
	TestStringRemoveIf();
	TestStringSort();
	TestStringOperators();
	TestStringGetHeadRef();
	TestStringGetTailRef();
	TestStringGetMiddleRef();
	TestStringGetSliced();
	TestStringVisitOverloads();
	TestStringFindOverloads();
	TestStringFindOrDefault();
	TestStringCollectOverloads();
	TestStringRemoveIfOverloads();
	TestStringGetSorted();
	TestStringOperatorNotEqual();
	TestStringIterators();
	TestStringIndexOfMatching();
	TestStringHasMatching();
	TestStringFold();
	TestStringInject();
	TestStringReverse();
	TestStringGetReversed();
	TestStringFindReverseOrDefault();
	
	// Test object reference type
	TestObjectRefConstructors();
	TestObjectRefGetCount();
	TestObjectRefIsEmpty();
	TestObjectRefIsNotEmpty();
	TestObjectRefGetAt();
	TestObjectRefFirst();
	TestObjectRefLast();
	TestObjectRefIndexOf();
	TestObjectRefHas();
	TestObjectRefSetAt();
	TestObjectRefAdd();
	TestObjectRefAddIfAbsent();
	TestObjectRefInsert();
	TestObjectRefMove();
	TestObjectRefRemove();
	TestObjectRefRemoveIfPresent();
	TestObjectRefRemoveFrom();
	TestObjectRefRemoveAll();
	TestObjectRefEquals();
	TestObjectRefGetHead();
	TestObjectRefGetTail();
	TestObjectRefGetMiddle();
	TestObjectRefVisit();
	TestObjectRefFind();
	TestObjectRefCollect();
	TestObjectRefRemoveIf();
	TestObjectRefSort();
	TestObjectRefOperators();
	TestObjectRefGetHeadRef();
	TestObjectRefGetTailRef();
	TestObjectRefGetMiddleRef();
	TestObjectRefGetSliced();
	TestObjectRefVisitOverloads();
	TestObjectRefFindOverloads();
	TestObjectRefFindOrDefault();
	TestObjectRefCollectOverloads();
	TestObjectRefRemoveIfOverloads();
	TestObjectRefGetSorted();
	TestObjectRefOperatorNotEqual();
	TestObjectRefIterators();
	TestObjectRefIndexOfMatching();
	TestObjectRefHasMatching();
	TestObjectRefFold();
	TestObjectRefInject();
	TestObjectRefReverse();
	TestObjectRefGetReversed();
	TestObjectRefFindReverseOrDefault();
}

void detTOrderedSet::CleanUp(){
}

const char *detTOrderedSet::GetTestName(){
	return "TOrderedSet";
}


// Tests - Integer Type
////////////////////////

void detTOrderedSet::TestIntConstructors(){
	SetSubTestNum(0);

	// Default constructor
	decTOrderedSetInt set1;
	ASSERT_EQUAL(set1.GetCount(), 0);
	ASSERT_TRUE(set1.IsEmpty());

	// Constructor with capacity
	decTOrderedSetInt set2(10);
	ASSERT_EQUAL(set2.GetCount(), 0);
	ASSERT_TRUE(set2.IsEmpty());

	// Copy constructor
	decTOrderedSetInt set3;
	set3.Add(1);
	set3.Add(2);
	set3.Add(3);
	decTOrderedSetInt set4(set3);
	ASSERT_EQUAL(set4.GetCount(), 3);
	ASSERT_EQUAL(set4.GetAt(0), 1);
	ASSERT_EQUAL(set4.GetAt(1), 2);
	ASSERT_EQUAL(set4.GetAt(2), 3);
}

void detTOrderedSet::TestIntGetCount(){
	SetSubTestNum(1);

	decTOrderedSetInt set;
	ASSERT_EQUAL(set.GetCount(), 0);
	
	set.Add(5);
	ASSERT_EQUAL(set.GetCount(), 1);
	
	set.Add(10);
	ASSERT_EQUAL(set.GetCount(), 2);
	
	set.RemoveFrom(0);
	ASSERT_EQUAL(set.GetCount(), 1);
	
	set.RemoveAll();
	ASSERT_EQUAL(set.GetCount(), 0);
}

void detTOrderedSet::TestIntIsEmpty(){
	SetSubTestNum(2);

	decTOrderedSetInt set;
	ASSERT_TRUE(set.IsEmpty());
	
	set.Add(1);
	ASSERT_FALSE(set.IsEmpty());
	
	set.RemoveAll();
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestIntIsNotEmpty(){
	SetSubTestNum(3);

	decTOrderedSetInt set;
	ASSERT_FALSE(set.IsNotEmpty());
	
	set.Add(1);
	ASSERT_TRUE(set.IsNotEmpty());
	
	set.RemoveAll();
	ASSERT_FALSE(set.IsNotEmpty());
}

void detTOrderedSet::TestIntGetAt(){
	SetSubTestNum(4);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	ASSERT_EQUAL(set.GetAt(0), 10);
	ASSERT_EQUAL(set.GetAt(1), 20);
	ASSERT_EQUAL(set.GetAt(2), 30);
	
	ASSERT_DOES_FAIL(set.GetAt(-1));
	ASSERT_DOES_FAIL(set.GetAt(3));
}

void detTOrderedSet::TestIntFirst(){
	SetSubTestNum(5);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	ASSERT_EQUAL(set.First(), 10);
	
	decTOrderedSetInt empty;
	ASSERT_DOES_FAIL(empty.First());
}

void detTOrderedSet::TestIntLast(){
	SetSubTestNum(6);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	ASSERT_EQUAL(set.Last(), 30);
	
	decTOrderedSetInt empty;
	ASSERT_DOES_FAIL(empty.Last());
}

void detTOrderedSet::TestIntIndexOf(){
	SetSubTestNum(7);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	ASSERT_EQUAL(set.IndexOf(10), 0);
	ASSERT_EQUAL(set.IndexOf(20), 1);
	ASSERT_EQUAL(set.IndexOf(30), 2);
	ASSERT_EQUAL(set.IndexOf(40), -1);
}

void detTOrderedSet::TestIntHas(){
	SetSubTestNum(8);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	ASSERT_TRUE(set.Has(10));
	ASSERT_TRUE(set.Has(20));
	ASSERT_TRUE(set.Has(30));
	ASSERT_FALSE(set.Has(40));
}

void detTOrderedSet::TestIntSetAt(){
	SetSubTestNum(9);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	set.SetAt(1, 25);
	ASSERT_EQUAL(set.GetAt(1), 25);
	ASSERT_TRUE(set.Has(25));
	ASSERT_FALSE(set.Has(20));
	
	ASSERT_DOES_FAIL(set.SetAt(-1, 99));
	ASSERT_DOES_FAIL(set.SetAt(3, 99));
}

void detTOrderedSet::TestIntAdd(){
	SetSubTestNum(10);

	decTOrderedSetInt set;
	
	set.Add(10);
	ASSERT_EQUAL(set.GetCount(), 1);
	ASSERT_TRUE(set.Has(10));
	
	set.Add(20);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_TRUE(set.Has(20));
	
	// Try to add duplicate - should fail
	ASSERT_DOES_FAIL(set.Add(10));
}

void detTOrderedSet::TestIntAddIfAbsent(){
	SetSubTestNum(11);

	decTOrderedSetInt set;
	
	ASSERT_TRUE(set.AddIfAbsent(10));
	ASSERT_EQUAL(set.GetCount(), 1);
	ASSERT_TRUE(set.Has(10));
	
	ASSERT_TRUE(set.AddIfAbsent(20));
	ASSERT_EQUAL(set.GetCount(), 2);
	
	// Adding duplicate should return false
	ASSERT_FALSE(set.AddIfAbsent(10));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestIntInsert(){
	SetSubTestNum(12);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(30);
	
	// Insert at beginning
	set.Insert(5, 0);
	ASSERT_EQUAL(set.GetAt(0), 5);
	ASSERT_EQUAL(set.GetAt(1), 10);
	ASSERT_EQUAL(set.GetAt(2), 30);
	
	// Insert in middle
	set.Insert(20, 2);
	ASSERT_EQUAL(set.GetAt(0), 5);
	ASSERT_EQUAL(set.GetAt(1), 10);
	ASSERT_EQUAL(set.GetAt(2), 20);
	ASSERT_EQUAL(set.GetAt(3), 30);
	
	// Try to insert duplicate - should fail
	ASSERT_DOES_FAIL(set.Insert(10, 0));
}

void detTOrderedSet::TestIntMove(){
	SetSubTestNum(13);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	// Move element from position 0 to position 2
	set.Move(10, 2);
	ASSERT_EQUAL(set.GetAt(0), 20);
	ASSERT_EQUAL(set.GetAt(1), 30);
	ASSERT_EQUAL(set.GetAt(2), 10);
	ASSERT_EQUAL(set.GetAt(3), 40);
	
	// Move element from position 3 to position 1
	set.Move(40, 1);
	ASSERT_EQUAL(set.GetAt(0), 20);
	ASSERT_EQUAL(set.GetAt(1), 40);
	ASSERT_EQUAL(set.GetAt(2), 30);
	ASSERT_EQUAL(set.GetAt(3), 10);
}

void detTOrderedSet::TestIntRemove(){
	SetSubTestNum(14);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	set.Remove(20);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_FALSE(set.Has(20));
	ASSERT_EQUAL(set.GetAt(0), 10);
	ASSERT_EQUAL(set.GetAt(1), 30);
	
	// Try to remove non-existent element - should fail
	ASSERT_DOES_FAIL(set.Remove(99));
}

void detTOrderedSet::TestIntRemoveIfPresent(){
	SetSubTestNum(15);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	ASSERT_TRUE(set.RemoveIfPresent(20));
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_FALSE(set.Has(20));
	
	// Try to remove non-existent element - should return false
	ASSERT_FALSE(set.RemoveIfPresent(99));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestIntRemoveFrom(){
	SetSubTestNum(16);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	set.RemoveFrom(1);
	ASSERT_EQUAL(set.GetCount(), 3);
	ASSERT_EQUAL(set.GetAt(0), 10);
	ASSERT_EQUAL(set.GetAt(1), 30);
	ASSERT_EQUAL(set.GetAt(2), 40);
	
	ASSERT_DOES_FAIL(set.RemoveFrom(-1));
	ASSERT_DOES_FAIL(set.RemoveFrom(5));
}

void detTOrderedSet::TestIntRemoveAll(){
	SetSubTestNum(17);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	ASSERT_EQUAL(set.GetCount(), 3);
	
	set.RemoveAll();
	ASSERT_EQUAL(set.GetCount(), 0);
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestIntEquals(){
	SetSubTestNum(18);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	
	decTOrderedSetInt set2;
	set2.Add(10);
	set2.Add(20);
	set2.Add(30);
	
	decTOrderedSetInt set3;
	set3.Add(10);
	set3.Add(20);
	
	ASSERT_TRUE(set1.Equals(set2));
	ASSERT_FALSE(set1.Equals(set3));
	ASSERT_FALSE(set2.Equals(set3));
}

void detTOrderedSet::TestIntGetHead(){
	SetSubTestNum(19);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	decTOrderedSetInt head = set.GetHead(2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), 10);
	ASSERT_EQUAL(head.GetAt(1), 20);
	
	decTOrderedSetInt head_large = set.GetHead(10);
	ASSERT_EQUAL(head_large.GetCount(), 4);
	
	decTOrderedSetInt empty_head = set.GetHead(0);
	ASSERT_EQUAL(empty_head.GetCount(), 0);
}

void detTOrderedSet::TestIntGetTail(){
	SetSubTestNum(20);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	decTOrderedSetInt tail = set.GetTail(2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), 30);
	ASSERT_EQUAL(tail.GetAt(1), 40);
	
	decTOrderedSetInt tail_large = set.GetTail(10);
	ASSERT_EQUAL(tail_large.GetCount(), 4);
	
	decTOrderedSetInt empty_tail = set.GetTail(0);
	ASSERT_EQUAL(empty_tail.GetCount(), 0);
}

void detTOrderedSet::TestIntGetMiddle(){
	SetSubTestNum(21);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	set.Add(50);
	
	decTOrderedSetInt middle = set.GetMiddle(1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), 20);
	ASSERT_EQUAL(middle.GetAt(1), 30);
	ASSERT_EQUAL(middle.GetAt(2), 40);
	
	decTOrderedSetInt single = set.GetMiddle(2, 2);
	ASSERT_EQUAL(single.GetCount(), 1);
	ASSERT_EQUAL(single.GetAt(0), 30);
}

void detTOrderedSet::TestIntVisit(){
	SetSubTestNum(22);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	int sum = 0;
	auto visitor = [&sum](int value){ sum += value; };
	set.Visit(visitor);
	ASSERT_EQUAL(sum, 60);
}

void detTOrderedSet::TestIntFind(){
	SetSubTestNum(23);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	const int *found = nullptr;
	auto evaluator = [](int value){ return value > 15; };
	ASSERT_TRUE(set.Find(evaluator, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(*found, 20);
	
	ASSERT_EQUAL(set.FindOrDefault(evaluator, -1), 20);
	
	auto evaluator2 = [](int value){ return value > 100; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_EQUAL(set.FindOrDefault(evaluator2, -1), -1);
}

void detTOrderedSet::TestIntCollect(){
	SetSubTestNum(24);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	set.Add(50);
	
	auto evaluator = [](int value){ return value > 25; };
	decTOrderedSetInt collected = set.Collect(evaluator);
	ASSERT_EQUAL(collected.GetCount(), 3);
	ASSERT_TRUE(collected.Has(30));
	ASSERT_TRUE(collected.Has(40));
	ASSERT_TRUE(collected.Has(50));
}

void detTOrderedSet::TestIntRemoveIf(){
	SetSubTestNum(25);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	set.Add(50);
	
	auto evaluator = [](int value){ return value > 25; };
	set.RemoveIf(evaluator);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_TRUE(set.Has(10));
	ASSERT_TRUE(set.Has(20));
	ASSERT_FALSE(set.Has(30));
}

void detTOrderedSet::TestIntSort(){
	SetSubTestNum(26);

	decTOrderedSetInt set;
	set.Add(30);
	set.Add(10);
	set.Add(40);
	set.Add(20);
	
	auto comparator = [](int a, int b){ return a - b; };
	set.Sort(comparator);
	ASSERT_EQUAL(set.GetAt(0), 10);
	ASSERT_EQUAL(set.GetAt(1), 20);
	ASSERT_EQUAL(set.GetAt(2), 30);
	ASSERT_EQUAL(set.GetAt(3), 40);
	
	// Test GetSorted (non-mutating)
	decTOrderedSetInt set2;
	set2.Add(30);
	set2.Add(10);
	set2.Add(40);
	set2.Add(20);
	
	decTOrderedSetInt sorted = set2.GetSorted(comparator);
	ASSERT_EQUAL(set2.GetAt(0), 30); // Original unchanged
	ASSERT_EQUAL(sorted.GetAt(0), 10);
	ASSERT_EQUAL(sorted.GetAt(1), 20);
	ASSERT_EQUAL(sorted.GetAt(2), 30);
	ASSERT_EQUAL(sorted.GetAt(3), 40);
}

void detTOrderedSet::TestIntOperators(){
	SetSubTestNum(27);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	
	// Test operator[]
	ASSERT_EQUAL(set1[0], 10);
	ASSERT_EQUAL(set1[1], 20);
	
	// Test operator==
	decTOrderedSetInt set2;
	set2.Add(10);
	set2.Add(20);
	ASSERT_EQUAL(set1, set2);
	
	// Test operator+
	decTOrderedSetInt set3;
	set3.Add(30);
	decTOrderedSetInt combined = set1 + set3;
	ASSERT_EQUAL(combined.GetCount(), 3);
	ASSERT_TRUE(combined.Has(10));
	ASSERT_TRUE(combined.Has(20));
	ASSERT_TRUE(combined.Has(30));
	
	// Test operator=
	decTOrderedSetInt set4 = set1;
	ASSERT_EQUAL(set4, set1);
	
	// Test operator+=
	set4 += set3;
	ASSERT_EQUAL(set4.GetCount(), 3);
	ASSERT_TRUE(set4.Has(30));
}


// Tests - String Type
///////////////////////

void detTOrderedSet::TestStringConstructors(){
	SetSubTestNum(28);

	decTOrderedSetString set1;
	ASSERT_EQUAL(set1.GetCount(), 0);
	ASSERT_TRUE(set1.IsEmpty());

	decTOrderedSetString set2(10);
	ASSERT_EQUAL(set2.GetCount(), 0);

	decTOrderedSetString set3;
	set3.Add("apple");
	set3.Add("banana");
	decTOrderedSetString set4(set3);
	ASSERT_EQUAL(set4.GetCount(), 2);
	ASSERT_EQUAL(set4.GetAt(0), "apple");
	ASSERT_EQUAL(set4.GetAt(1), "banana");
}

void detTOrderedSet::TestStringGetCount(){
	SetSubTestNum(29);

	decTOrderedSetString set;
	ASSERT_EQUAL(set.GetCount(), 0);
	
	set.Add("hello");
	ASSERT_EQUAL(set.GetCount(), 1);
	
	set.Add("world");
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestStringIsEmpty(){
	SetSubTestNum(30);

	decTOrderedSetString set;
	ASSERT_TRUE(set.IsEmpty());
	
	set.Add("test");
	ASSERT_FALSE(set.IsEmpty());
	
	set.RemoveAll();
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestStringIsNotEmpty(){
	SetSubTestNum(31);

	decTOrderedSetString set;
	ASSERT_FALSE(set.IsNotEmpty());
	
	set.Add("test");
	ASSERT_TRUE(set.IsNotEmpty());
}

void detTOrderedSet::TestStringGetAt(){
	SetSubTestNum(32);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");

	ASSERT_EQUAL(set.GetAt(0), "apple");
	ASSERT_EQUAL(set.GetAt(1), "banana");
	ASSERT_EQUAL(set.GetAt(2), "cherry");
	
	ASSERT_DOES_FAIL(set.GetAt(-1));
	ASSERT_DOES_FAIL(set.GetAt(3));
}

void detTOrderedSet::TestStringFirst(){
	SetSubTestNum(33);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");

	ASSERT_EQUAL(set.First(), "apple");
}

void detTOrderedSet::TestStringLast(){
	SetSubTestNum(34);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");

	ASSERT_EQUAL(set.Last(), "banana");
}

void detTOrderedSet::TestStringIndexOf(){
	SetSubTestNum(35);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");

	ASSERT_EQUAL(set.IndexOf("apple"), 0);
	ASSERT_EQUAL(set.IndexOf("banana"), 1);
	ASSERT_EQUAL(set.IndexOf("cherry"), 2);
	ASSERT_EQUAL(set.IndexOf("date"), -1);
}

void detTOrderedSet::TestStringHas(){
	SetSubTestNum(36);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");

	ASSERT_TRUE(set.Has("apple"));
	ASSERT_TRUE(set.Has("banana"));
	ASSERT_FALSE(set.Has("cherry"));
}

void detTOrderedSet::TestStringSetAt(){
	SetSubTestNum(37);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");

	set.SetAt(1, "blueberry");
	ASSERT_EQUAL(set.GetAt(1), "blueberry");
	ASSERT_TRUE(set.Has("blueberry"));
	ASSERT_FALSE(set.Has("banana"));
}

void detTOrderedSet::TestStringAdd(){
	SetSubTestNum(38);

	decTOrderedSetString set;
	
	set.Add("apple");
	ASSERT_EQUAL(set.GetCount(), 1);
	ASSERT_TRUE(set.Has("apple"));
	
	set.Add("banana");
	ASSERT_EQUAL(set.GetCount(), 2);
	
	ASSERT_DOES_FAIL(set.Add("apple"));
}

void detTOrderedSet::TestStringAddIfAbsent(){
	SetSubTestNum(39);

	decTOrderedSetString set;
	
	ASSERT_TRUE(set.AddIfAbsent("apple"));
	ASSERT_EQUAL(set.GetCount(), 1);
	
	ASSERT_TRUE(set.AddIfAbsent("banana"));
	ASSERT_EQUAL(set.GetCount(), 2);
	
	ASSERT_FALSE(set.AddIfAbsent("apple"));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestStringInsert(){
	SetSubTestNum(40);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("cherry");
	
	set.Insert("banana", 1);
	ASSERT_EQUAL(set.GetAt(0), "apple");
	ASSERT_EQUAL(set.GetAt(1), "banana");
	ASSERT_EQUAL(set.GetAt(2), "cherry");
}

void detTOrderedSet::TestStringMove(){
	SetSubTestNum(41);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	set.Move("apple", 2);
	ASSERT_EQUAL(set.GetAt(0), "banana");
	ASSERT_EQUAL(set.GetAt(1), "cherry");
	ASSERT_EQUAL(set.GetAt(2), "apple");
}

void detTOrderedSet::TestStringRemove(){
	SetSubTestNum(42);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	set.Remove("banana");
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_FALSE(set.Has("banana"));
}

void detTOrderedSet::TestStringRemoveIfPresent(){
	SetSubTestNum(43);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	
	ASSERT_TRUE(set.RemoveIfPresent("banana"));
	ASSERT_EQUAL(set.GetCount(), 1);
	
	ASSERT_FALSE(set.RemoveIfPresent("date"));
	ASSERT_EQUAL(set.GetCount(), 1);
}

void detTOrderedSet::TestStringRemoveFrom(){
	SetSubTestNum(44);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	set.RemoveFrom(1);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_EQUAL(set.GetAt(0), "apple");
	ASSERT_EQUAL(set.GetAt(1), "cherry");
}

void detTOrderedSet::TestStringRemoveAll(){
	SetSubTestNum(45);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	
	set.RemoveAll();
	ASSERT_EQUAL(set.GetCount(), 0);
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestStringEquals(){
	SetSubTestNum(46);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	
	decTOrderedSetString set2;
	set2.Add("apple");
	set2.Add("banana");
	
	decTOrderedSetString set3;
	set3.Add("apple");
	
	ASSERT_TRUE(set1.Equals(set2));
	ASSERT_FALSE(set1.Equals(set3));
}

void detTOrderedSet::TestStringGetHead(){
	SetSubTestNum(47);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	decTOrderedSetString head = set.GetHead(2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), "apple");
	ASSERT_EQUAL(head.GetAt(1), "banana");
}

void detTOrderedSet::TestStringGetTail(){
	SetSubTestNum(48);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	decTOrderedSetString tail = set.GetTail(2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), "cherry");
	ASSERT_EQUAL(tail.GetAt(1), "date");
}

void detTOrderedSet::TestStringGetMiddle(){
	SetSubTestNum(49);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	set.Add("elderberry");
	
	decTOrderedSetString middle = set.GetMiddle(1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), "banana");
	ASSERT_EQUAL(middle.GetAt(1), "cherry");
	ASSERT_EQUAL(middle.GetAt(2), "date");
}

void detTOrderedSet::TestStringVisit(){
	SetSubTestNum(50);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	int count = 0;
	auto visitor = [&count](const decString &){ count++; };
	set.Visit(visitor);
	ASSERT_EQUAL(count, 3);
}

void detTOrderedSet::TestStringFind(){
	SetSubTestNum(51);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	const decString *found = nullptr;
	auto evaluator = [](const decString &value){ return value.GetLength() > 5; };
	ASSERT_TRUE(set.Find(evaluator, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(*found, "banana");
	
	ASSERT_EQUAL(set.FindOrDefault(evaluator, ""), "banana");
	
	auto evaluator2 = [](const decString &value){ return value.GetLength() > 10; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_EQUAL(set.FindOrDefault(evaluator2, ""), "");
}

void detTOrderedSet::TestStringCollect(){
	SetSubTestNum(52);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	auto evaluator = [](const decString &value){ return value.GetLength() > 5; };
	decTOrderedSetString collected = set.Collect(evaluator);
	ASSERT_EQUAL(collected.GetCount(), 2);
	ASSERT_TRUE(collected.Has("banana"));
	ASSERT_TRUE(collected.Has("cherry"));
}

void detTOrderedSet::TestStringRemoveIf(){
	SetSubTestNum(53);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	auto evaluator = [](const decString &value){ return value.GetLength() > 5; };
	set.RemoveIf(evaluator);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_TRUE(set.Has("apple"));
	ASSERT_TRUE(set.Has("date"));
}

void detTOrderedSet::TestStringSort(){
	SetSubTestNum(54);

	decTOrderedSetString set;
	set.Add("cherry");
	set.Add("apple");
	set.Add("banana");
	
	auto comparator = [](const decString &a, const decString &b){ 
		return a.Compare(b);
	};
	set.Sort(comparator);
	ASSERT_EQUAL(set.GetAt(0), "apple");
	ASSERT_EQUAL(set.GetAt(1), "banana");
	ASSERT_EQUAL(set.GetAt(2), "cherry");
}

void detTOrderedSet::TestStringOperators(){
	SetSubTestNum(55);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	
	ASSERT_EQUAL(set1[0], "apple");
	ASSERT_EQUAL(set1[1], "banana");
	
	decTOrderedSetString set2;
	set2.Add("apple");
	set2.Add("banana");
	ASSERT_EQUAL(set1, set2);
	
	decTOrderedSetString set3;
	set3.Add("cherry");
	decTOrderedSetString combined = set1 + set3;
	ASSERT_EQUAL(combined.GetCount(), 3);
	ASSERT_TRUE(combined.Has("cherry"));
	
	decTOrderedSetString set4 = set1;
	ASSERT_EQUAL(set4, set1);
	
	set4 += set3;
	ASSERT_EQUAL(set4.GetCount(), 3);
}


// Tests - Pointer Type
///////////////////////

void detTOrderedSet::TestObjectRefConstructors(){
	SetSubTestNum(56);

	decTOrderedSetXmlElementTag set1;
	ASSERT_EQUAL(set1.GetCount(), 0);
	ASSERT_TRUE(set1.IsEmpty());

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set2;
	set2.Add(obj1);
	set2.Add(obj2);
	decTOrderedSetXmlElementTag set3(set2);
	ASSERT_EQUAL(set3.GetCount(), 2);
}

void detTOrderedSet::TestObjectRefGetCount(){
	SetSubTestNum(57);

	decTOrderedSetXmlElementTag set;
	ASSERT_EQUAL(set.GetCount(), 0);
	
	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	set.Add(obj1);
	ASSERT_EQUAL(set.GetCount(), 1);
	
	set.Add(obj2);
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestObjectRefIsEmpty(){
	SetSubTestNum(58);

	decTOrderedSetXmlElementTag set;
	ASSERT_TRUE(set.IsEmpty());
	
	decXmlElementTag::Ref obj(decXmlElementTag::Ref::New("tag"));
	set.Add(obj);
	ASSERT_FALSE(set.IsEmpty());
	
	set.RemoveAll();
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestObjectRefIsNotEmpty(){
	SetSubTestNum(59);

	decTOrderedSetXmlElementTag set;
	ASSERT_FALSE(set.IsNotEmpty());
	
	decXmlElementTag::Ref obj(decXmlElementTag::Ref::New("tag"));
	set.Add(obj);
	ASSERT_TRUE(set.IsNotEmpty());
}

void detTOrderedSet::TestObjectRefGetAt(){
	SetSubTestNum(60);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);

	ASSERT_EQUAL(set.GetAt(0), obj1);
	ASSERT_EQUAL(set.GetAt(1), obj2);
	ASSERT_EQUAL(set.GetAt(2), obj3);
	
	ASSERT_DOES_FAIL(set.GetAt(-1));
	ASSERT_DOES_FAIL(set.GetAt(3));
}

void detTOrderedSet::TestObjectRefFirst(){
	SetSubTestNum(61);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);

	ASSERT_EQUAL(set.First(), obj1);
}

void detTOrderedSet::TestObjectRefLast(){
	SetSubTestNum(62);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);

	ASSERT_EQUAL(set.Last(), obj2);
}

void detTOrderedSet::TestObjectRefIndexOf(){
	SetSubTestNum(63);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);

	ASSERT_EQUAL(set.IndexOf(obj1), 0);
	ASSERT_EQUAL(set.IndexOf(obj2), 1);
	ASSERT_EQUAL(set.IndexOf(obj3), 2);
	ASSERT_EQUAL(set.IndexOf(obj4), -1);
}

void detTOrderedSet::TestObjectRefHas(){
	SetSubTestNum(64);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);

	ASSERT_TRUE(set.Has(obj1));
	ASSERT_TRUE(set.Has(obj2));
	ASSERT_FALSE(set.Has(obj3));
}

void detTOrderedSet::TestObjectRefSetAt(){
	SetSubTestNum(65);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);

	set.SetAt(1, obj3);
	ASSERT_EQUAL(set.GetAt(1), obj3);
	ASSERT_TRUE(set.Has(obj3));
	ASSERT_FALSE(set.Has(obj2));
}

void detTOrderedSet::TestObjectRefAdd(){
	SetSubTestNum(66);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	ASSERT_EQUAL(set.GetCount(), 1);
	
	set.Add(obj2);
	ASSERT_EQUAL(set.GetCount(), 2);
	
	ASSERT_DOES_FAIL(set.Add(obj1));
}

void detTOrderedSet::TestObjectRefAddIfAbsent(){
	SetSubTestNum(67);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set;
	ASSERT_TRUE(set.AddIfAbsent(obj1));
	ASSERT_EQUAL(set.GetCount(), 1);
	
	ASSERT_TRUE(set.AddIfAbsent(obj2));
	ASSERT_EQUAL(set.GetCount(), 2);
	
	ASSERT_FALSE(set.AddIfAbsent(obj1));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTOrderedSet::TestObjectRefInsert(){
	SetSubTestNum(68);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj3);
	
	set.Insert(obj2, 1);
	ASSERT_EQUAL(set.GetAt(0), obj1);
	ASSERT_EQUAL(set.GetAt(1), obj2);
	ASSERT_EQUAL(set.GetAt(2), obj3);
}

void detTOrderedSet::TestObjectRefMove(){
	SetSubTestNum(69);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	set.Move(obj1, 2);
	ASSERT_EQUAL(set.GetAt(0), obj2);
	ASSERT_EQUAL(set.GetAt(1), obj3);
	ASSERT_EQUAL(set.GetAt(2), obj1);
}

void detTOrderedSet::TestObjectRefRemove(){
	SetSubTestNum(70);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	set.Remove(obj2);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_FALSE(set.Has(obj2));
}

void detTOrderedSet::TestObjectRefRemoveIfPresent(){
	SetSubTestNum(71);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	
	ASSERT_TRUE(set.RemoveIfPresent(obj2));
	ASSERT_EQUAL(set.GetCount(), 1);
	
	ASSERT_FALSE(set.RemoveIfPresent(obj3));
	ASSERT_EQUAL(set.GetCount(), 1);
}

void detTOrderedSet::TestObjectRefRemoveFrom(){
	SetSubTestNum(72);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	set.RemoveFrom(1);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_EQUAL(set.GetAt(0), obj1);
	ASSERT_EQUAL(set.GetAt(1), obj3);
}

void detTOrderedSet::TestObjectRefRemoveAll(){
	SetSubTestNum(73);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	
	set.RemoveAll();
	ASSERT_EQUAL(set.GetCount(), 0);
	ASSERT_TRUE(set.IsEmpty());
}

void detTOrderedSet::TestObjectRefEquals(){
	SetSubTestNum(74);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	
	decTOrderedSetXmlElementTag set2;
	set2.Add(obj1);
	set2.Add(obj2);
	
	decTOrderedSetXmlElementTag set3;
	set3.Add(obj1);
	
	ASSERT_TRUE(set1.Equals(set2));
	ASSERT_FALSE(set1.Equals(set3));
}

void detTOrderedSet::TestObjectRefGetHead(){
	SetSubTestNum(75);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	decTOrderedSetXmlElementTag head = set.GetHead(2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), obj1);
	ASSERT_EQUAL(head.GetAt(1), obj2);
}

void detTOrderedSet::TestObjectRefGetTail(){
	SetSubTestNum(76);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	decTOrderedSetXmlElementTag tail = set.GetTail(2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), obj3);
	ASSERT_EQUAL(tail.GetAt(1), obj4);
}

void detTOrderedSet::TestObjectRefGetMiddle(){
	SetSubTestNum(77);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	decXmlElementTag::Ref obj5(decXmlElementTag::Ref::New("tag5"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	set.Add(obj5);
	
	decTOrderedSetXmlElementTag middle = set.GetMiddle(1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), obj2);
	ASSERT_EQUAL(middle.GetAt(1), obj3);
	ASSERT_EQUAL(middle.GetAt(2), obj4);
}

void detTOrderedSet::TestObjectRefVisit(){
	SetSubTestNum(78);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	int count = 0;
	auto visitor = [&count](const decXmlElementTag*){ count++; };
	set.Visit(visitor);
	ASSERT_EQUAL(count, 3);
}

void detTOrderedSet::TestObjectRefFind(){
	SetSubTestNum(79);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	const decXmlElementTag::Ref* found;
	auto evaluator = [obj2](const decXmlElementTag *val){ return val == obj2; };
	ASSERT_TRUE(set.Find(evaluator, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(*found, obj2);
	
	ASSERT_EQUAL(set.FindOrDefault(evaluator, {}), obj2);
	
	auto evaluator2 = [](const decXmlElementTag *val){ return val->GetName() == "nonexistent"; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_NULL(set.FindOrDefault(evaluator2, {}));
}

void detTOrderedSet::TestObjectRefCollect(){
	SetSubTestNum(80);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	auto evaluator = [obj1, obj4](const decXmlElementTag *val){
		return val == obj1 || val == obj4;
	};
	decTOrderedSetXmlElementTag collected = set.Collect(evaluator);
	ASSERT_EQUAL(collected.GetCount(), 2);
	ASSERT_TRUE(collected.Has(obj1));
	ASSERT_TRUE(collected.Has(obj4));
}

void detTOrderedSet::TestObjectRefRemoveIf(){
	SetSubTestNum(81);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	auto evaluator = [obj1, obj3](const decXmlElementTag *val){
		return val == obj1 || val == obj3;
	};
	set.RemoveIf(evaluator);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_TRUE(set.Has(obj2));
	ASSERT_TRUE(set.Has(obj4));
}

void detTOrderedSet::TestObjectRefSort(){
	SetSubTestNum(82);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj1);
	
	auto comparator = [](const decXmlElementTag *a, const decXmlElementTag *b){ 
		return a->GetName().Compare(b->GetName()); 
	};
	set.Sort(comparator);
	ASSERT_EQUAL(set.GetAt(0), obj1);
	ASSERT_EQUAL(set.GetAt(1), obj2);
	ASSERT_EQUAL(set.GetAt(2), obj3);
}

void detTOrderedSet::TestObjectRefOperators(){
	SetSubTestNum(83);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	
	ASSERT_EQUAL(set1[0], obj1);
	ASSERT_EQUAL(set1[1], obj2);
	
	decTOrderedSetXmlElementTag set2;
	set2.Add(obj1);
	set2.Add(obj2);
	ASSERT_EQUAL(set1, set2);
	
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decTOrderedSetXmlElementTag set3;
	set3.Add(obj3);
	decTOrderedSetXmlElementTag combined = set1 + set3;
	ASSERT_EQUAL(combined.GetCount(), 3);
	ASSERT_TRUE(combined.Has(obj3));
	
	decTOrderedSetXmlElementTag set4 = set1;
	ASSERT_EQUAL(set4, set1);
	
	set4 += set3;
	ASSERT_EQUAL(set4.GetCount(), 3);
}

// ============================================================================
// Additional Tests for Missing Overloads - INT
// ============================================================================

void detTOrderedSet::TestIntGetHeadRef(){
	SetSubTestNum(84);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	// Test GetHead with reference parameter
	decTOrderedSetInt head;
	set1.GetHead(head, 2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), 10);
	ASSERT_EQUAL(head.GetAt(1), 20);
}

void detTOrderedSet::TestIntGetTailRef(){
	SetSubTestNum(85);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	// Test GetTail with reference parameter
	decTOrderedSetInt tail;
	set1.GetTail(tail, 2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), 30);
	ASSERT_EQUAL(tail.GetAt(1), 40);
}

void detTOrderedSet::TestIntGetMiddleRef(){
	SetSubTestNum(86);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	set1.Add(50);
	
	// Test GetMiddle with reference parameter
	decTOrderedSetInt middle;
	set1.GetMiddle(middle, 1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), 20);
	ASSERT_EQUAL(middle.GetAt(1), 30);
	ASSERT_EQUAL(middle.GetAt(2), 40);
}

void detTOrderedSet::TestIntGetSliced(){
	SetSubTestNum(87);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	set1.Add(50);
	
	// GetSliced with step=1 uses GetMiddle internally
	decTOrderedSetInt sliced = set1.GetSliced(1, 3, 1);
	ASSERT_EQUAL(sliced.GetCount(), 3);
	ASSERT_EQUAL(sliced.GetAt(0), 20);
	ASSERT_EQUAL(sliced.GetAt(1), 30);
	ASSERT_EQUAL(sliced.GetAt(2), 40);
}

void detTOrderedSet::TestIntVisitOverloads(){
	SetSubTestNum(88);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	// Test Visit with from parameter only
	int count = 0;
	auto visitor1 = [&count](int val){ count++; };
	set1.Visit(visitor1, 2);
	ASSERT_EQUAL(count, 2);  // Visits index 2 and 3
	
	// Test Visit with from, to, step parameters
	count = 0;
	set1.Visit(visitor1, 0, 4, 2);
	ASSERT_EQUAL(count, 2);  // Visits indices 0 and 2
}

void detTOrderedSet::TestIntFindOverloads(){
	SetSubTestNum(89);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	// Test Find with from parameter only
	const int *found = nullptr;
	auto evaluator = [](int val){ return val == 30; };
	ASSERT_TRUE(set1.Find(evaluator, found, 2));
	ASSERT_EQUAL(*found, 30);
	
	// Test Find with from, to, step parameters
	found = nullptr;
	auto evaluator2 = [](int val){ return val == 30; };
	ASSERT_TRUE(set1.Find(evaluator2, found, 0, 4, 2));
	ASSERT_EQUAL(*found, 30);
}

void detTOrderedSet::TestIntFindOrDefault(){
	SetSubTestNum(90);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	
	// Test FindOrDefault - found case
	auto evaluator1 = [](int val){ return val == 20; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator1, 99), 20);
	
	// Test FindOrDefault - not found case
	auto evaluator2 = [](int val){ return val == 50; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator2, 99), 99);
	
	// Test FindOrDefault with from parameter
	auto evaluator3 = [](int val){ return val == 10; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator3, 99, 1), 99);  // Start from index 1. 10 is at index 0, not found from index 1
}

void detTOrderedSet::TestIntCollectOverloads(){
	SetSubTestNum(91);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	// Test Collect with from parameter only
	auto evaluator1 = [](int val){ return val >= 30; };
	decTOrderedSetInt collected1 = set1.Collect(evaluator1, 2);
	ASSERT_EQUAL(collected1.GetCount(), 2);  // 30 and 40
	ASSERT_TRUE(collected1.Has(30));
	ASSERT_TRUE(collected1.Has(40));
	
	// Test Collect with from, to, step parameters
	auto evaluator2 = [](int val){ return val == 10 || val == 30; };
	decTOrderedSetInt collected2 = set1.Collect(evaluator2, 0, 4, 2);
	ASSERT_EQUAL(collected2.GetCount(), 2);  // 10 and 30 (even indices)
	ASSERT_TRUE(collected2.Has(10));
	ASSERT_TRUE(collected2.Has(30));
}

void detTOrderedSet::TestIntRemoveIfOverloads(){
	SetSubTestNum(92);

	// Test RemoveIf with from parameter only
	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	set1.Add(30);
	set1.Add(40);
	
	auto evaluator1 = [](int val){ return val >= 30; };
	set1.RemoveIf(evaluator1, 2);
	ASSERT_EQUAL(set1.GetCount(), 2);
	ASSERT_FALSE(set1.Has(30));
	ASSERT_FALSE(set1.Has(40));
	
	// Test RemoveIf with from, to, step parameters
	decTOrderedSetInt set2;
	set2.Add(10);
	set2.Add(20);
	set2.Add(30);
	set2.Add(40);
	
	auto evaluator2 = [](int val){ return val == 10 || val == 30; };
	set2.RemoveIf(evaluator2, 0, 4, 2);
	ASSERT_EQUAL(set2.GetCount(), 2);
	ASSERT_FALSE(set2.Has(10));
	ASSERT_FALSE(set2.Has(30));
}

void detTOrderedSet::TestIntGetSorted(){
	SetSubTestNum(93);

	decTOrderedSetInt set1;
	set1.Add(30);
	set1.Add(10);
	set1.Add(40);
	set1.Add(20);
	
	auto comparator = [](int a, int b){ return a - b; };
	
	// Test GetSorted - should not modify original
	decTOrderedSetInt sorted = set1.GetSorted(comparator);
	ASSERT_EQUAL(set1.GetAt(0), 30);  // Original unchanged
	ASSERT_EQUAL(sorted.GetAt(0), 10);  // Sorted
	ASSERT_EQUAL(sorted.GetAt(1), 20);
	ASSERT_EQUAL(sorted.GetAt(2), 30);
	ASSERT_EQUAL(sorted.GetAt(3), 40);
}

void detTOrderedSet::TestIntOperatorNotEqual(){
	SetSubTestNum(94);

	decTOrderedSetInt set1;
	set1.Add(10);
	set1.Add(20);
	
	decTOrderedSetInt set2;
	set2.Add(10);
	set2.Add(20);
	
	decTOrderedSetInt set3;
	set3.Add(10);
	set3.Add(30);
	
	ASSERT_FALSE(set1 != set2);  // Equal
	ASSERT_TRUE(set1 != set3);   // Not equal
}

// ============================================================================
// Additional Tests for Missing Overloads - STRING
// ============================================================================

void detTOrderedSet::TestStringGetHeadRef(){
	SetSubTestNum(95);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	set1.Add("c");
	set1.Add("d");
	
	decTOrderedSetString head;
	set1.GetHead(head, 2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), "a");
	ASSERT_EQUAL(head.GetAt(1), "b");
}

void detTOrderedSet::TestStringGetTailRef(){
	SetSubTestNum(96);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	set1.Add("c");
	set1.Add("d");
	
	decTOrderedSetString tail;
	set1.GetTail(tail, 2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), "c");
	ASSERT_EQUAL(tail.GetAt(1), "d");
}

void detTOrderedSet::TestStringGetMiddleRef(){
	SetSubTestNum(97);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	set1.Add("c");
	set1.Add("d");
	set1.Add("e");
	
	decTOrderedSetString middle;
	set1.GetMiddle(middle, 1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), "b");
	ASSERT_EQUAL(middle.GetAt(1), "c");
	ASSERT_EQUAL(middle.GetAt(2), "d");
}

void detTOrderedSet::TestStringGetSliced(){
	SetSubTestNum(98);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	set1.Add("c");
	set1.Add("d");
	set1.Add("e");
	
	decTOrderedSetString sliced = set1.GetSliced(1, 3, 1);
	ASSERT_EQUAL(sliced.GetCount(), 3);
	ASSERT_EQUAL(sliced.GetAt(0), "b");
	ASSERT_EQUAL(sliced.GetAt(1), "c");
	ASSERT_EQUAL(sliced.GetAt(2), "d");
}

void detTOrderedSet::TestStringVisitOverloads(){
	SetSubTestNum(99);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	set1.Add("c");
	set1.Add("d");
	
	int count = 0;
	auto visitor = [&count](decString val){ count++; };
	set1.Visit(visitor, 2);
	ASSERT_EQUAL(count, 2);
	
	count = 0;
	set1.Visit(visitor, 0, 4, 2);
	ASSERT_EQUAL(count, 2);
}

void detTOrderedSet::TestStringFindOverloads(){
	SetSubTestNum(100);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	set1.Add("cherry");
	set1.Add("date");
	
	const decString *found = nullptr;
	auto evaluator = [](decString val){ return val == "cherry"; };
	ASSERT_TRUE(set1.Find(evaluator, found, 2));
	ASSERT_EQUAL(*found, "cherry");
	
	found = nullptr;
	auto evaluator2 = [](decString val){ return val == "cherry"; };
	ASSERT_TRUE(set1.Find(evaluator2, found, 0, 4, 2));
	ASSERT_EQUAL(*found, "cherry");
}

void detTOrderedSet::TestStringFindOrDefault(){
	SetSubTestNum(101);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	set1.Add("cherry");
	
	auto evaluator1 = [](decString val){ return val == "banana"; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator1, "unknown"), "banana");
	
	auto evaluator2 = [](decString val){ return val == "grape"; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator2, "unknown"), "unknown");
	
	auto evaluator3 = [](decString val){ return val == "apple"; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator3, "unknown", 1), "unknown");  // apple is at index 0
}

void detTOrderedSet::TestStringCollectOverloads(){
	SetSubTestNum(102);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	set1.Add("cherry");
	set1.Add("date");
	
	auto evaluator1 = [](decString val){ return val.Find("a") >= 0; };
	decTOrderedSetString collected1 = set1.Collect(evaluator1, 2);
	ASSERT_EQUAL(collected1.GetCount(), 1);  // "cherry" starts at index 2
	
	auto evaluator2 = [](decString val){ return val.GetLength() >= 5; };
	decTOrderedSetString collected2 = set1.Collect(evaluator2, 0, 4, 2);
	ASSERT_EQUAL(collected2.GetCount(), 2);  // "apple" and "cherry" have length 5 and 6
}

void detTOrderedSet::TestStringRemoveIfOverloads(){
	SetSubTestNum(103);

	decTOrderedSetString set1;
	set1.Add("apple");
	set1.Add("banana");
	set1.Add("cherry");
	set1.Add("date");
	
	auto evaluator1 = [](decString val){ return val.GetLength() > 4; };
	set1.RemoveIf(evaluator1, 2);
	ASSERT_FALSE(set1.Has("cherry"));
	
	decTOrderedSetString set2;
	set2.Add("apple");
	set2.Add("banana");
	set2.Add("cherry");
	set2.Add("date");
	
	auto evaluator2 = [](decString val){ return val == "apple" || val == "cherry"; };
	set2.RemoveIf(evaluator2, 0, 4, 2);
	ASSERT_FALSE(set2.Has("apple"));
	ASSERT_FALSE(set2.Has("cherry"));
}

void detTOrderedSet::TestStringGetSorted(){
	SetSubTestNum(104);

	decTOrderedSetString set1;
	set1.Add("cherry");
	set1.Add("apple");
	set1.Add("date");
	set1.Add("banana");
	
	auto comparator = [](decString a, decString b){ return a.Compare(b); };
	decTOrderedSetString sorted = set1.GetSorted(comparator);
	ASSERT_EQUAL(set1.GetAt(0), "cherry");  // Original unchanged
	ASSERT_EQUAL(set1.GetAt(1), "apple");
	ASSERT_EQUAL(set1.GetAt(2), "date");
	ASSERT_EQUAL(set1.GetAt(3), "banana");
	ASSERT_EQUAL(sorted.GetAt(0), "apple");  // Sorted
	ASSERT_EQUAL(sorted.GetAt(1), "banana");
	ASSERT_EQUAL(sorted.GetAt(2), "cherry");
	ASSERT_EQUAL(sorted.GetAt(3), "date");
}

void detTOrderedSet::TestStringOperatorNotEqual(){
	SetSubTestNum(105);

	decTOrderedSetString set1;
	set1.Add("a");
	set1.Add("b");
	
	decTOrderedSetString set2;
	set2.Add("a");
	set2.Add("b");
	
	decTOrderedSetString set3;
	set3.Add("a");
	set3.Add("c");
	
	ASSERT_FALSE(set1 != set2);
	ASSERT_TRUE(set1 != set3);
}

// ============================================================================
// Additional Tests for Missing Overloads - OBJECTREF
// ============================================================================

void detTOrderedSet::TestObjectRefGetHeadRef(){
	SetSubTestNum(106);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	decTOrderedSetXmlElementTag head;
	set1.GetHead(head, 2);
	ASSERT_EQUAL(head.GetCount(), 2);
	ASSERT_EQUAL(head.GetAt(0), obj1);
	ASSERT_EQUAL(head.GetAt(1), obj2);
}

void detTOrderedSet::TestObjectRefGetTailRef(){
	SetSubTestNum(107);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	decTOrderedSetXmlElementTag tail;
	set1.GetTail(tail, 2);
	ASSERT_EQUAL(tail.GetCount(), 2);
	ASSERT_EQUAL(tail.GetAt(0), obj3);
	ASSERT_EQUAL(tail.GetAt(1), obj4);
}

void detTOrderedSet::TestObjectRefGetMiddleRef(){
	SetSubTestNum(108);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	decXmlElementTag::Ref obj5(decXmlElementTag::Ref::New("tag5"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	set1.Add(obj5);
	
	decTOrderedSetXmlElementTag middle;
	set1.GetMiddle(middle, 1, 3);
	ASSERT_EQUAL(middle.GetCount(), 3);
	ASSERT_EQUAL(middle.GetAt(0), obj2);
	ASSERT_EQUAL(middle.GetAt(1), obj3);
	ASSERT_EQUAL(middle.GetAt(2), obj4);
}

void detTOrderedSet::TestObjectRefGetSliced(){
	SetSubTestNum(109);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	decXmlElementTag::Ref obj5(decXmlElementTag::Ref::New("tag5"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	set1.Add(obj5);
	
	decTOrderedSetXmlElementTag sliced = set1.GetSliced(1, 3, 1);
	ASSERT_EQUAL(sliced.GetCount(), 3);
	ASSERT_EQUAL(sliced.GetAt(0), obj2);
	ASSERT_EQUAL(sliced.GetAt(1), obj3);
	ASSERT_EQUAL(sliced.GetAt(2), obj4);
}

void detTOrderedSet::TestObjectRefVisitOverloads(){
	SetSubTestNum(110);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	int count = 0;
	auto visitor = [&count](decXmlElementTag::Ref val){ count++; };
	set1.Visit(visitor, 2);
	ASSERT_EQUAL(count, 2);
	
	count = 0;
	set1.Visit(visitor, 0, 4, 2);
	ASSERT_EQUAL(count, 2);
}

void detTOrderedSet::TestObjectRefFindOverloads(){
	SetSubTestNum(111);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	const decXmlElementTag::Ref *found = nullptr;
	auto evaluator = [obj3](decXmlElementTag::Ref val){ return val == obj3; };
	ASSERT_TRUE(set1.Find(evaluator, found, 2));
	ASSERT_EQUAL(*found, obj3);
	
	found = nullptr;
	auto evaluator2 = [obj3](decXmlElementTag::Ref val){ return val == obj3; };
	ASSERT_TRUE(set1.Find(evaluator2, found, 0, 4, 2));
	ASSERT_EQUAL(*found, obj3);
}

void detTOrderedSet::TestObjectRefFindOrDefault(){
	SetSubTestNum(112);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref defaultObj(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	
	auto evaluator1 = [obj2](decXmlElementTag::Ref val){ return val == obj2; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator1, defaultObj), obj2);
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return false; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator2, defaultObj), defaultObj);
	
	auto evaluator3 = [obj1](decXmlElementTag::Ref val){ return val == obj1; };
	ASSERT_EQUAL(set1.FindOrDefault(evaluator3, defaultObj, 1), defaultObj);  // obj1 is at index 0
}

void detTOrderedSet::TestObjectRefCollectOverloads(){
	SetSubTestNum(113);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	auto evaluator1 = [obj3, obj4](decXmlElementTag::Ref val){ return val == obj3 || val == obj4; };
	decTOrderedSetXmlElementTag collected1 = set1.Collect(evaluator1, 2);
	ASSERT_EQUAL(collected1.GetCount(), 2);
	ASSERT_TRUE(collected1.Has(obj3));
	ASSERT_TRUE(collected1.Has(obj4));
	
	auto evaluator2 = [obj1, obj3](decXmlElementTag::Ref val){ return val == obj1 || val == obj3; };
	decTOrderedSetXmlElementTag collected2 = set1.Collect(evaluator2, 0, 4, 2);
	ASSERT_EQUAL(collected2.GetCount(), 2);
	ASSERT_TRUE(collected2.Has(obj1));
	ASSERT_TRUE(collected2.Has(obj3));
}

void detTOrderedSet::TestObjectRefRemoveIfOverloads(){
	SetSubTestNum(114);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	set1.Add(obj3);
	set1.Add(obj4);
	
	auto evaluator1 = [obj3, obj4](decXmlElementTag::Ref val){ return val == obj3 || val == obj4; };
	set1.RemoveIf(evaluator1, 2);
	ASSERT_EQUAL(set1.GetCount(), 2);
	ASSERT_FALSE(set1.Has(obj3));
	ASSERT_FALSE(set1.Has(obj4));
	
	decXmlElementTag::Ref obj5(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj6(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj7(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj8(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set2;
	set2.Add(obj5);
	set2.Add(obj6);
	set2.Add(obj7);
	set2.Add(obj8);
	
	auto evaluator2 = [obj5, obj7](decXmlElementTag::Ref val){ return val == obj5 || val == obj7; };
	set2.RemoveIf(evaluator2, 0, 4, 2);
	ASSERT_EQUAL(set2.GetCount(), 2);
	ASSERT_FALSE(set2.Has(obj5));
	ASSERT_FALSE(set2.Has(obj7));
}

void detTOrderedSet::TestObjectRefGetSorted(){
	SetSubTestNum(115);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj3);
	set1.Add(obj1);
	set1.Add(obj4);
	set1.Add(obj2);
	
	auto comparator = [](decXmlElementTag::Ref a, decXmlElementTag::Ref b){ 
		return a->GetName().Compare(b->GetName()); 
	};
	decTOrderedSetXmlElementTag sorted = set1.GetSorted(comparator);
	ASSERT_EQUAL(set1.GetAt(0), obj3);  // Original unchanged
	ASSERT_EQUAL(set1.GetAt(1), obj1);
	ASSERT_EQUAL(set1.GetAt(2), obj4);
	ASSERT_EQUAL(set1.GetAt(3), obj2);
	ASSERT_EQUAL(sorted.GetAt(0), obj1);  // Sorted
	ASSERT_EQUAL(sorted.GetAt(1), obj2);
	ASSERT_EQUAL(sorted.GetAt(2), obj3);
	ASSERT_EQUAL(sorted.GetAt(3), obj4);
	ASSERT_EQUAL(sorted.GetCount(), 4);
}

void detTOrderedSet::TestObjectRefOperatorNotEqual(){
	SetSubTestNum(116);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set1;
	set1.Add(obj1);
	set1.Add(obj2);
	
	decTOrderedSetXmlElementTag set2;
	set2.Add(obj1);
	set2.Add(obj2);
	
	decTOrderedSetXmlElementTag set3;
	set3.Add(obj1);
	set3.Add(obj3);
	
	ASSERT_FALSE(set1 != set2);
	ASSERT_TRUE(set1 != set3);
}

void detTOrderedSet::TestIntIterators(){
	SetSubTestNum(117);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	int sum = 0;
	int count = 0;
	for(auto it = set.begin(); it != set.end(); ++it){
		sum += *it;
		count++;
	}
	ASSERT_EQUAL(count, 3);
	ASSERT_EQUAL(sum, 60);
	ASSERT_EQUAL(set.begin()[1], 20);
	ASSERT_TRUE(set.begin() < set.end());

	// random access arithmetic
	auto it0 = set.begin();
	it0 += 2;
	ASSERT_EQUAL(*it0, 30);
	it0 -= 1;
	ASSERT_EQUAL(*it0, 20);
	ASSERT_EQUAL(set.end() - set.begin(), 3);
	ASSERT_TRUE((set.begin() + 1) > set.begin());

	// reverse iterator
	int firstReverse = *set.rbegin();
	ASSERT_EQUAL(firstReverse, 30);
	auto r = set.rbegin();
	++r;
	ASSERT_EQUAL(*r, 20);
}

void detTOrderedSet::TestStringIterators(){
	SetSubTestNum(118);

	decTOrderedSetString set;
	set.Add("a");
	set.Add("b");
	set.Add("c");

	decString concat;
	for(auto it = set.cbegin(); it != set.cend(); ++it){
		concat += *it;
	}
	ASSERT_EQUAL(concat, "abc");
	ASSERT_EQUAL(set.cbegin()[2], "c");
	ASSERT_TRUE(set.cbegin() < set.cend());
	ASSERT_EQUAL(set.cend() - set.cbegin(), 3);
	auto itc = set.cbegin();
	itc += 1;
	ASSERT_EQUAL(*itc, "b");
	itc -= 1;
	ASSERT_EQUAL(*itc, "a");

	decString revConcat;
	for(auto it = set.crbegin(); it != set.crend(); ++it){
		revConcat += *it;
	}
	ASSERT_EQUAL(revConcat, "cba");
	auto r = set.crbegin();
	++r;
	ASSERT_EQUAL(*r, "b");
}

void detTOrderedSet::TestObjectRefIterators(){
	SetSubTestNum(119);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));

	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);

	decString concat;
	for(auto it = set.begin(); it != set.end(); ++it){
		concat += (*it)->GetName();
	}
	ASSERT_EQUAL(concat, "tag1tag2tag3");
	ASSERT_EQUAL(set.begin()[1]->GetName(), decString("tag2"));
	ASSERT_TRUE(set.begin() < set.end());
	ASSERT_EQUAL(set.end() - set.begin(), 3);
	auto ito = set.begin();
	ito += 2;
	ASSERT_EQUAL((*ito)->GetName(), decString("tag3"));
	ito -= 1;
	ASSERT_EQUAL((*ito)->GetName(), decString("tag2"));

	decString rev;
	for(auto it = set.rbegin(); it != set.rend(); ++it){
		rev += (*it)->GetName();
	}
	ASSERT_EQUAL(rev, "tag3tag2tag1");
	auto r = set.rbegin();
	++r;
	ASSERT_EQUAL((*r)->GetName(), decString("tag2"));
}

// ============================================================================
// New Function Tests - INT
// ============================================================================

void detTOrderedSet::TestIntIndexOfMatching(){
	SetSubTestNum(120);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	auto evaluator1 = [](int val){ return val == 30; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](int val){ return val >= 30; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator3, 3), 3);
	ASSERT_EQUAL(set.IndexOfMatching(evaluator3, 2), 2);
}

void detTOrderedSet::TestIntHasMatching(){
	SetSubTestNum(121);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto evaluator1 = [](int val){ return val == 20; };
	ASSERT_TRUE(set.HasMatching(evaluator1));
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_FALSE(set.HasMatching(evaluator2));
	
	auto evaluator3 = [](int val){ return val < 15; };
	ASSERT_TRUE(set.HasMatching(evaluator3));
}

void detTOrderedSet::TestIntFold(){
	SetSubTestNum(122);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(set.Fold(combiner), 60);
	
	decTOrderedSetInt emptySet;
	ASSERT_DOES_FAIL(emptySet.Fold(combiner));
	
	decTOrderedSetInt set2;
	set2.Add(2);
	set2.Add(3);
	set2.Add(4);
	auto multiplier = [](int acc, int val){ return acc * val; };
	ASSERT_EQUAL(set2.Fold(multiplier), 24);
}

void detTOrderedSet::TestIntInject(){
	SetSubTestNum(123);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(set.Inject(100, combiner), 160);
	
	decTOrderedSetInt emptySet;
	ASSERT_EQUAL(emptySet.Inject(100, combiner), 100);
	
	// Test with different return type
	auto stringCombiner = [](decString acc, int val){ 
		decString str;
		str.Format("%d", val);
		return acc + str; 
	};
	ASSERT_EQUAL(set.Inject(decString("values:"), stringCombiner), decString("values:102030"));
}

void detTOrderedSet::TestIntReverse(){
	SetSubTestNum(124);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	set.Reverse();
	
	ASSERT_EQUAL(set.GetCount(), 4);
	ASSERT_EQUAL(set.GetAt(0), 40);
	ASSERT_EQUAL(set.GetAt(1), 30);
	ASSERT_EQUAL(set.GetAt(2), 20);
	ASSERT_EQUAL(set.GetAt(3), 10);
}

void detTOrderedSet::TestIntGetReversed(){
	SetSubTestNum(125);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	decTOrderedSetInt reversed = set.GetReversed();
	
	// Original unchanged
	ASSERT_EQUAL(set.GetAt(0), 10);
	ASSERT_EQUAL(set.GetAt(1), 20);
	ASSERT_EQUAL(set.GetAt(2), 30);
	ASSERT_EQUAL(set.GetAt(3), 40);
	
	// Reversed copy
	ASSERT_EQUAL(reversed.GetCount(), 4);
	ASSERT_EQUAL(reversed.GetAt(0), 40);
	ASSERT_EQUAL(reversed.GetAt(1), 30);
	ASSERT_EQUAL(reversed.GetAt(2), 20);
	ASSERT_EQUAL(reversed.GetAt(3), 10);
}

void detTOrderedSet::TestIntFindReverseOrDefault(){
	SetSubTestNum(126);

	decTOrderedSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	auto evaluator1 = [](int val){ return val == 30; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator1, 99), 30);
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator2, 99), 99);
	
	// FindReverse should find last matching element
	auto evaluator3 = [](int val){ return val >= 20; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator3, 99), 40);
}


// ============================================================================
// New Function Tests - STRING
// ============================================================================

void detTOrderedSet::TestStringIndexOfMatching(){
	SetSubTestNum(127);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	auto evaluator1 = [](const decString &val){ return val == "cherry"; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](const decString &val){ return val.GetLength() == 4; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator3, 3), 3);
}

void detTOrderedSet::TestStringHasMatching(){
	SetSubTestNum(128);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	auto evaluator1 = [](const decString &val){ return val == "banana"; };
	ASSERT_TRUE(set.HasMatching(evaluator1));
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_FALSE(set.HasMatching(evaluator2));
	
	auto evaluator3 = [](const decString &val){ return val.GetLength() == 5; };
	ASSERT_TRUE(set.HasMatching(evaluator3));
}

void detTOrderedSet::TestStringFold(){
	SetSubTestNum(129);

	decTOrderedSetString set;
	set.Add("a");
	set.Add("b");
	set.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(set.Fold(combiner), decString("abc"));
	
	decTOrderedSetString emptySet;
	ASSERT_DOES_FAIL(emptySet.Fold(combiner));
}

void detTOrderedSet::TestStringInject(){
	SetSubTestNum(130);

	decTOrderedSetString set;
	set.Add("a");
	set.Add("b");
	set.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(set.Inject(decString("start:"), combiner), decString("start:abc"));
	
	decTOrderedSetString emptySet;
	ASSERT_EQUAL(emptySet.Inject(decString("start:"), combiner), decString("start:"));
	
	// Test with different return type
	auto lengthCombiner = [](int acc, const decString &val){ 
		return acc + val.GetLength(); 
	};
	ASSERT_EQUAL(set.Inject(0, lengthCombiner), 3);
}

void detTOrderedSet::TestStringReverse(){
	SetSubTestNum(131);

	decTOrderedSetString set;
	set.Add("first");
	set.Add("second");
	set.Add("third");
	set.Add("fourth");
	
	set.Reverse();
	
	ASSERT_EQUAL(set.GetCount(), 4);
	ASSERT_EQUAL(set.GetAt(0), decString("fourth"));
	ASSERT_EQUAL(set.GetAt(1), decString("third"));
	ASSERT_EQUAL(set.GetAt(2), decString("second"));
	ASSERT_EQUAL(set.GetAt(3), decString("first"));
}

void detTOrderedSet::TestStringGetReversed(){
	SetSubTestNum(132);

	decTOrderedSetString set;
	set.Add("first");
	set.Add("second");
	set.Add("third");
	set.Add("fourth");
	
	decTOrderedSetString reversed = set.GetReversed();
	
	// Original unchanged
	ASSERT_EQUAL(set.GetAt(0), decString("first"));
	ASSERT_EQUAL(set.GetAt(1), decString("second"));
	ASSERT_EQUAL(set.GetAt(2), decString("third"));
	ASSERT_EQUAL(set.GetAt(3), decString("fourth"));
	
	// Reversed copy
	ASSERT_EQUAL(reversed.GetCount(), 4);
	ASSERT_EQUAL(reversed.GetAt(0), decString("fourth"));
	ASSERT_EQUAL(reversed.GetAt(1), decString("third"));
	ASSERT_EQUAL(reversed.GetAt(2), decString("second"));
	ASSERT_EQUAL(reversed.GetAt(3), decString("first"));
}

void detTOrderedSet::TestStringFindReverseOrDefault(){
	SetSubTestNum(133);

	decTOrderedSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	set.Add("date");
	
	auto evaluator1 = [](const decString &val){ return val == "cherry"; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator1, decString("default")), decString("cherry"));
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator2, decString("default")), decString("default"));
	
	// FindReverse should find last matching element
	auto evaluator3 = [](const decString &val){ return val.GetLength() >= 5; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator3, decString("default")), decString("cherry"));
}


// ============================================================================
// New Function Tests - OBJECTREF
// ============================================================================

void detTOrderedSet::TestObjectRefIndexOfMatching(){
	SetSubTestNum(134);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	auto evaluator1 = [obj3](decXmlElementTag::Ref val){ return val == obj3; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_EQUAL(set.IndexOfMatching(evaluator3, 2), 2);
}

void detTOrderedSet::TestObjectRefHasMatching(){
	SetSubTestNum(135);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	auto evaluator1 = [obj2](decXmlElementTag::Ref val){ return val == obj2; };
	ASSERT_TRUE(set.HasMatching(evaluator1));
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_FALSE(set.HasMatching(evaluator2));
	
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_TRUE(set.HasMatching(evaluator3));
}

void detTOrderedSet::TestObjectRefFold(){
	SetSubTestNum(136);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("a"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("b"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("c"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	// Since Fold returns T (which is deTObjectReference<decXmlElementTag>), we can't
	// easily concatenate. Let's use a test that makes sense for object references.
	// We'll just test that it returns the first element since the combiner will
	// keep returning the first value
	auto combiner = [](decXmlElementTag::Ref acc, decXmlElementTag::Ref val){ 
		return acc; 
	};
	ASSERT_EQUAL(set.Fold(combiner), obj1);
	
	decTOrderedSetXmlElementTag emptySet;
	ASSERT_DOES_FAIL(emptySet.Fold(combiner));
}

void detTOrderedSet::TestObjectRefInject(){
	SetSubTestNum(137);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("a"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("b"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("c"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	// Test with string accumulator
	auto combiner = [](decString acc, decXmlElementTag::Ref val){ 
		return acc + val->GetName(); 
	};
	ASSERT_EQUAL(set.Inject(decString("names:"), combiner), decString("names:abc"));
	
	decTOrderedSetXmlElementTag emptySet;
	ASSERT_EQUAL(emptySet.Inject(decString("names:"), combiner), decString("names:"));
	
	// Test with int accumulator (count elements)
	auto counter = [](int acc, decXmlElementTag::Ref val){ 
		return acc + 1; 
	};
	ASSERT_EQUAL(set.Inject(0, counter), 3);
}

void detTOrderedSet::TestObjectRefReverse(){
	SetSubTestNum(138);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	set.Reverse();
	
	ASSERT_EQUAL(set.GetCount(), 4);
	ASSERT_EQUAL(set.GetAt(0), obj4);
	ASSERT_EQUAL(set.GetAt(1), obj3);
	ASSERT_EQUAL(set.GetAt(2), obj2);
	ASSERT_EQUAL(set.GetAt(3), obj1);
}

void detTOrderedSet::TestObjectRefGetReversed(){
	SetSubTestNum(139);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	decTOrderedSetXmlElementTag reversed = set.GetReversed();
	
	// Original unchanged
	ASSERT_EQUAL(set.GetAt(0), obj1);
	ASSERT_EQUAL(set.GetAt(1), obj2);
	ASSERT_EQUAL(set.GetAt(2), obj3);
	ASSERT_EQUAL(set.GetAt(3), obj4);
	
	// Reversed copy
	ASSERT_EQUAL(reversed.GetCount(), 4);
	ASSERT_EQUAL(reversed.GetAt(0), obj4);
	ASSERT_EQUAL(reversed.GetAt(1), obj3);
	ASSERT_EQUAL(reversed.GetAt(2), obj2);
	ASSERT_EQUAL(reversed.GetAt(3), obj1);
}

void detTOrderedSet::TestObjectRefFindReverseOrDefault(){
	SetSubTestNum(140);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	decXmlElementTag::Ref defaultObj(decXmlElementTag::Ref::New("default"));
	
	decTOrderedSetXmlElementTag set;
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	set.Add(obj4);
	
	auto evaluator1 = [obj3](decXmlElementTag::Ref val){ return val == obj3; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator1, defaultObj), obj3);
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator2, defaultObj), defaultObj);
	
	// FindReverse should find last matching element
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_EQUAL(set.FindReverseOrDefault(evaluator3, defaultObj), obj4);
}
