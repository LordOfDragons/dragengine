// includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detTOrderedSet.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>

// Type definition for template with object references
typedef decTOrderedSet<int> decTOrderedSetInt;
typedef decTOrderedSet<decString> decTOrderedSetString;
typedef decTOrderedSet<decXmlElementTag::Ref, decXmlElementTag*> decTOrderedSetXmlElementTag;


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
	
	ASSERT_EQUAL(set.FindDefault(evaluator, -1), 20);
	
	auto evaluator2 = [](int value){ return value > 100; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_EQUAL(set.FindDefault(evaluator2, -1), -1);
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
	
	ASSERT_EQUAL(set.FindDefault(evaluator, ""), "banana");
	
	auto evaluator2 = [](const decString &value){ return value.GetLength() > 10; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_EQUAL(set.FindDefault(evaluator2, ""), "");
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
	
	ASSERT_EQUAL(set.FindDefault(evaluator, {}), obj2);
	
	auto evaluator2 = [](const decXmlElementTag *val){ return val->GetName() == "nonexistent"; };
	ASSERT_FALSE(set.Find(evaluator2, found));
	
	ASSERT_NULL(set.FindDefault(evaluator2, {}));
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
