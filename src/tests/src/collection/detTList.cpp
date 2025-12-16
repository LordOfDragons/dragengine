// includes
#include <stdio.h>

#include "detTList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>

// Type aliases
typedef decTList<int> decTListInt;
typedef decTList<decString> decTListString;
typedef decTObjectList<decXmlElementTag> decTListXmlElementTag;


// Class detTList
/////////////////

detTList::detTList(){
	Prepare();
}

detTList::~detTList(){
	CleanUp();
}

void detTList::Prepare(){
}

void detTList::Run(){
	// int
	TestIntBasic();
	TestIntAddRemove();
	TestIntIndexingAccess();
	TestIntOperators();
	TestIntIterators();
	TestIntAlgorithms();
	// string
	TestStringBasic();
	TestStringOperators();
	// object ref
	TestObjectRefBasic();
	TestObjectRefOperations();
}

void detTList::CleanUp(){
}

const char *detTList::GetTestName(){
	return "TList";
}


// Tests - Integer Type
////////////////////////

void detTList::TestIntBasic(){
	SetSubTestNum(0);

	// Constructor tests
	decTListInt list;
	ASSERT_EQUAL(list.GetCount(), 0);
	ASSERT_TRUE(list.IsEmpty());
	ASSERT_FALSE(list.IsNotEmpty());

	// Constructor with capacity
	decTListInt list2(10);
	ASSERT_EQUAL(list2.GetCount(), 0);
	ASSERT_TRUE(list2.IsEmpty());

	// Add elements
	list.Add(1);
	list.Add(2);
	list.Add(3);
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_FALSE(list.IsEmpty());
	ASSERT_TRUE(list.IsNotEmpty());
	ASSERT_EQUAL(list.First(), 1);
	ASSERT_EQUAL(list.Last(), 3);

	// Copy constructor
	decTListInt list3(list);
	ASSERT_EQUAL(list3.GetCount(), 3);
	ASSERT_EQUAL(list3.GetAt(0), 1);
	ASSERT_EQUAL(list3.GetAt(1), 2);
	ASSERT_EQUAL(list3.GetAt(2), 3);
}

void detTList::TestIntAddRemove(){
	SetSubTestNum(1);

	decTListInt list;
	list.Add(1);
	list.Add(2);
	list.Add(3);
	list.Add(2); // duplicate

	ASSERT_EQUAL(list.CountOccurance(2), 2);
	ASSERT_TRUE(list.Has(2));
	ASSERT_EQUAL(list.IndexOf(2), 1);
	ASSERT_EQUAL(list.IndexOf(2, 2), 3);

	list.RemoveFrom(1); // Remove first 2: [1,3,2]
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_EQUAL(list.GetAt(1), 3);
	ASSERT_EQUAL(list.CountOccurance(2), 1);

	list.RemoveAll();
	ASSERT_TRUE(list.IsEmpty());
	ASSERT_EQUAL(list.GetCount(), 0);
}

void detTList::TestIntIndexingAccess(){
	SetSubTestNum(2);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	list.Add(40);
	list.Add(50);

	// GetAt and operator[]
	ASSERT_EQUAL(list.GetAt(0), 10);
	ASSERT_EQUAL(list[2], 30);
	ASSERT_EQUAL(list.First(), 10);
	ASSERT_EQUAL(list.Last(), 50);

	// SetAt
	list.SetAt(2, 35);
	ASSERT_EQUAL(list.GetAt(2), 35);
	list[2] = 30; // non-const operator[]
	ASSERT_EQUAL(list[2], 30);

	// Insert
	list.Insert(25, 2); // [10,20,25,30,40,50]
	ASSERT_EQUAL(list.GetCount(), 6);
	ASSERT_EQUAL(list.GetAt(0), 10);
	ASSERT_EQUAL(list.GetAt(1), 20);
	ASSERT_EQUAL(list.GetAt(2), 25);
	ASSERT_EQUAL(list.GetAt(3), 30);
	ASSERT_EQUAL(list.GetAt(4), 40);
	ASSERT_EQUAL(list.GetAt(5), 50);

	// Move
	list.Move(0, 3); // [20,25,10,30,40,50]
	ASSERT_EQUAL(list.GetCount(), 6);
	ASSERT_EQUAL(list.GetAt(0), 20);
	ASSERT_EQUAL(list.GetAt(1), 25);
	ASSERT_EQUAL(list.GetAt(2), 10);
	ASSERT_EQUAL(list.GetAt(3), 30);
	ASSERT_EQUAL(list.GetAt(4), 40);
	ASSERT_EQUAL(list.GetAt(5), 50);

	list.Move(5, 1); // [20,50,25,10,30,40]
	ASSERT_EQUAL(list.GetCount(), 6);
	ASSERT_EQUAL(list.GetAt(0), 20);
	ASSERT_EQUAL(list.GetAt(1), 50);
	ASSERT_EQUAL(list.GetAt(2), 25);
	ASSERT_EQUAL(list.GetAt(3), 10);
	ASSERT_EQUAL(list.GetAt(4), 30);
	ASSERT_EQUAL(list.GetAt(5), 40);
}

void detTList::TestIntOperators(){
	SetSubTestNum(3);

	decTListInt list1;
	list1.Add(1);
	list1.Add(2);
	list1.Add(3);

	decTListInt list2;
	list2.Add(4);
	list2.Add(5);

	// operator+
	auto list3 = list1 + list2; // [1,2,3,4,5]
	ASSERT_EQUAL(list3.GetCount(), 5);
	ASSERT_EQUAL(list3.GetAt(3), 4);
	ASSERT_EQUAL(list3.GetAt(4), 5);

	// operator+=
	list1 += list2; // [1,2,3,4,5]
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_TRUE(list1 == list3);

	// operator==
	decTListInt list4(list1);
	ASSERT_TRUE(list1 == list4);

	// operator!=
	list4.Add(6);
	ASSERT_TRUE(list1 != list4);
	ASSERT_FALSE(list1 == list4);

	// operator=
	decTListInt list5;
	list5 = list1;
	ASSERT_TRUE(list5 == list1);
	ASSERT_EQUAL(list5.GetCount(), 5);
}

void detTList::TestIntIterators(){
	SetSubTestNum(4);

	decTListInt list;
	for(int i=0;i<5;i++) list.Add(i+1); // 1..5

	// const_iterator
	int sum = 0;
	int count = 0;
	for(auto it = list.cbegin(); it != list.cend(); ++it){
		sum += *it;
		count++;
	}
	ASSERT_EQUAL(count, 5);
	ASSERT_EQUAL(sum, 15);

	// Random-access ops
	auto it = list.cbegin();
	it += 2; // points to 3
	ASSERT_EQUAL(*it, 3);
	auto it2 = it + 2; // points to 5
	ASSERT_EQUAL(*it2, 5);

	// Iterator arithmetic
	auto it3 = list.cbegin();
	auto it4 = list.cend();
	ASSERT_EQUAL(it4 - it3, 5);

	// Reverse iterator
	int rsum = 0;
	for(auto rit = list.crbegin(); rit != list.crend(); ++rit){
		rsum += *rit;
	}
	ASSERT_EQUAL(rsum, 15);

	// Non-const iterator (mutable)
	for(auto mit = list.begin(); mit != list.end(); ++mit){
		*mit += 10;
	}
	ASSERT_EQUAL(list[0], 11);
	ASSERT_EQUAL(list[4], 15);
}

void detTList::TestIntAlgorithms(){
	SetSubTestNum(5);

	decTListInt list;
	list.Add(5);
	list.Add(2);
	list.Add(8);
	list.Add(1);
	list.Add(9);

	// Visit
	int visitSum = 0;
	list.Visit([&visitSum](const int &v){ visitSum += v; });
	ASSERT_EQUAL(visitSum, 25);

	// Find
	const int *result = nullptr;
	bool found = list.Find([](const int &v){ return v > 7; }, result);
	ASSERT_TRUE(found);
	ASSERT_NOT_NULL(result);
	ASSERT_EQUAL(*result, 8);

	// FindOrDefault
	ASSERT_EQUAL(list.FindOrDefault([](const int &v){ return v > 100; }, 999), 999);

	// Collect
	auto collected = list.Collect([](const int &v){ return v > 5; });
	ASSERT_EQUAL(collected.GetCount(), 2); // 8, 9
	ASSERT_TRUE(collected.Has(8));
	ASSERT_TRUE(collected.Has(9));

	// RemoveIf
	list.RemoveIf([](const int &v){ return v < 5; });
	ASSERT_EQUAL(list.GetCount(), 3); // 5, 8, 9
	ASSERT_EQUAL(list.GetAt(0), 5);

	// Sort
	list.Sort([](const int &a, const int &b){ return a - b; });
	ASSERT_EQUAL(list.GetAt(0), 5);
	ASSERT_EQUAL(list.GetAt(1), 8);
	ASSERT_EQUAL(list.GetAt(2), 9);

	// GetSorted
	decTListInt list2;
	list2.Add(30);
	list2.Add(10);
	list2.Add(20);
	auto sorted = list2.GetSorted([](const int &a, const int &b){ return a - b; });
	ASSERT_EQUAL(sorted.GetAt(0), 10);
	ASSERT_EQUAL(sorted.GetAt(1), 20);
	ASSERT_EQUAL(sorted.GetAt(2), 30);
	ASSERT_EQUAL(list2.GetAt(0), 30); // original unchanged
	ASSERT_EQUAL(list2.GetAt(1), 10);
	ASSERT_EQUAL(list2.GetAt(2), 20);
	
	// GetHead/GetTail/GetMiddle
	decTListInt list3;
	for(int i=1; i<=10; i++) list3.Add(i);
	
	auto head = list3.GetHead(3);
	ASSERT_EQUAL(head.GetCount(), 3);
	ASSERT_EQUAL(head.GetAt(0), 1);
	ASSERT_EQUAL(head.GetAt(2), 3);

	auto tail = list3.GetTail(3);
	ASSERT_EQUAL(tail.GetCount(), 3);
	ASSERT_EQUAL(tail.GetAt(0), 8);
	ASSERT_EQUAL(tail.GetAt(2), 10);

	auto middle = list3.GetMiddle(3, 6);
	ASSERT_EQUAL(middle.GetCount(), 4);
	ASSERT_EQUAL(middle.GetAt(0), 4);
	ASSERT_EQUAL(middle.GetAt(3), 7);

	// GetSliced
	auto sliced = list3.GetSliced(2, 6, 2); // from index 2, count 5, step 2
	ASSERT_EQUAL(sliced.GetCount(), 3);
	ASSERT_EQUAL(sliced.GetAt(0), 3);
	ASSERT_EQUAL(sliced.GetAt(1), 5);
	ASSERT_EQUAL(sliced.GetAt(2), 7);

	// Test output parameter overloads
	decTListInt headOut;
	list3.GetHead(headOut, 2);
	ASSERT_EQUAL(headOut.GetCount(), 2);
	ASSERT_EQUAL(headOut.GetAt(0), 1);
	ASSERT_EQUAL(headOut.GetAt(1), 2);

	decTListInt tailOut;
	list3.GetTail(tailOut, 2);
	ASSERT_EQUAL(tailOut.GetCount(), 2);
	ASSERT_EQUAL(tailOut.GetAt(0), 9);
	ASSERT_EQUAL(tailOut.GetAt(1), 10);

	decTListInt middleOut;
	list3.GetMiddle(middleOut, 2, 4);
	ASSERT_EQUAL(middleOut.GetCount(), 3);
	ASSERT_EQUAL(middleOut.GetAt(0), 3);
	ASSERT_EQUAL(middleOut.GetAt(2), 5);

	decTListInt slicedOut;
	list3.GetSliced(slicedOut, 1, 4, 2);
	ASSERT_EQUAL(slicedOut.GetCount(), 2);
	ASSERT_EQUAL(slicedOut.GetAt(0), 2);
	ASSERT_EQUAL(slicedOut.GetAt(1), 4);

	// Test parameterized algorithm overloads
	// Visit with from parameter
	int sumFrom = 0;
	list3.Visit([&sumFrom](const int &v){ sumFrom += v; }, 5);
	ASSERT_EQUAL(sumFrom, 40); // 6+7+8+9+10

	// Visit with from/to/step parameters
	int sumRange = 0;
	list3.Visit([&sumRange](const int &v){ sumRange += v; }, 2, 5, 1);
	ASSERT_EQUAL(sumRange, 12); // 3+4+5

	// Visit with negative step
	int sumReverse = 0;
	list3.Visit([&sumReverse](const int &v){ sumReverse += v; }, 4, 1, -1);
	ASSERT_EQUAL(sumReverse, 14); // 5+4+3+2

	// Find with from parameter
	const int *foundFrom = nullptr;
	ASSERT_TRUE(list3.Find([](const int &v){ return v > 7; }, foundFrom, 5));
	ASSERT_EQUAL(*foundFrom, 8);

	// Find with from/to/step parameters
	const int *foundRange = nullptr;
	ASSERT_TRUE(list3.Find([](const int &v){ return v == 4; }, foundRange, 0, 5, 1));
	ASSERT_EQUAL(*foundRange, 4);

	// Find with negative step
	const int *foundRev = nullptr;
	ASSERT_TRUE(list3.Find([](const int &v){ return v < 5; }, foundRev, 5, 0, -1));
	ASSERT_EQUAL(*foundRev, 4);

	// FindOrDefault with from parameter
	ASSERT_EQUAL(list3.FindOrDefault([](const int &v){ return v > 100; }, 999, 5), 999);
	ASSERT_EQUAL(list3.FindOrDefault([](const int &v){ return v > 8; }, 999, 8), 9);

	// FindOrDefault with from/to/step parameters
	ASSERT_EQUAL(list3.FindOrDefault([](const int &v){ return v == 5; }, 999, 0, 10, 1), 5);
	ASSERT_EQUAL(list3.FindOrDefault([](const int &v){ return v > 100; }, 999, 0, 10, 1), 999);

	// Collect with from parameter
	auto collFrom = list3.Collect([](const int &v){ return v > 7; }, 5);
	ASSERT_EQUAL(collFrom.GetCount(), 3); // 8, 9, 10

	// Collect with from/to/step
	auto collRange = list3.Collect([](const int &v){ return v % 2 == 0; }, 1, 8, 1);
	ASSERT_EQUAL(collRange.GetCount(), 4); // 2, 4, 6, 8

	// RemoveIf with from parameter
	decTListInt list4;
	for(int i=1; i<=10; i++) list4.Add(i);
	list4.RemoveIf([](const int &v){ return v > 7; }, 5); // removes 8, 9, 10
	ASSERT_EQUAL(list4.GetCount(), 7);
	ASSERT_EQUAL(list4.Last(), 7);

	// RemoveIf with from/to/step
	decTListInt list5;
	for(int i=1; i<=10; i++) list5.Add(i);
	list5.RemoveIf([](const int &v){ return v % 2 == 0; }, 1, 8, 1); // removes 2,4,6,8 from range [1,8)
	ASSERT_EQUAL(list5.GetCount(), 6);
	ASSERT_TRUE(list5.Has(1));
	ASSERT_FALSE(list5.Has(2));
	ASSERT_TRUE(list5.Has(3));
	ASSERT_FALSE(list5.Has(4));
	ASSERT_TRUE(list5.Has(9));
	ASSERT_TRUE(list5.Has(10));

	// Equals method
	decTListInt listA;
	listA.Add(1);
	listA.Add(2);
	decTListInt listB;
	listB.Add(1);
	listB.Add(2);
	ASSERT_TRUE(listA.Equals(listB));
	listB.Add(3);
	ASSERT_FALSE(listA.Equals(listB));
}


// Tests - String Type
///////////////////////

void detTList::TestStringBasic(){
	SetSubTestNum(6);

	decTListString list;
	list.Add(decString("a"));
	list.Add(decString("b"));
	list.Add(decString("c"));
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_TRUE(list.Has(decString("b")));
	ASSERT_EQUAL(list.IndexOf(decString("c")), 2);

	list.Insert(decString("x"), 1);
	ASSERT_EQUAL(list.GetAt(1), decString("x"));
	ASSERT_EQUAL(list.GetCount(), 4);

	list.RemoveFrom(1);
	ASSERT_EQUAL(list.GetAt(1), decString("b"));
	ASSERT_EQUAL(list.GetCount(), 3);

	list.SetAt(0, decString("z"));
	ASSERT_EQUAL(list[0], decString("z"));
}

void detTList::TestStringOperators(){
	SetSubTestNum(7);

	decTListString list1;
	list1.Add(decString("hello"));
	list1.Add(decString("world"));

	decTListString list2;
	list2.Add(decString("foo"));
	list2.Add(decString("bar"));

	auto list3 = list1 + list2;
	ASSERT_EQUAL(list3.GetCount(), 4);
	ASSERT_EQUAL(list3[2], decString("foo"));

	list1 += list2;
	ASSERT_TRUE(list1 == list3);
	ASSERT_EQUAL(list1.GetCount(), 4);
}


// Tests - ObjectRef Type
//////////////////////////

void detTList::TestObjectRefBasic(){
	SetSubTestNum(8);

	decTListXmlElementTag list;
	decXmlElementTag::Ref a = decXmlElementTag::Ref::New("a");
	decXmlElementTag::Ref b = decXmlElementTag::Ref::New("b");
	decXmlElementTag::Ref c = decXmlElementTag::Ref::New("c");

	list.Add(a);
	list.Add(b);
	list.Add(c);
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_TRUE(list.Has(b));
	ASSERT_EQUAL(list.IndexOf(c), 2);

	// SetAt and operator[] should work with Ref as storage
	list.SetAt(1, c);
	ASSERT_TRUE(list[1] == c);

	ASSERT_EQUAL(list.First().Pointer(), a.Pointer());
	ASSERT_EQUAL(list.Last().Pointer(), c.Pointer());

	list.RemoveAll();
	ASSERT_TRUE(list.IsEmpty());
}

void detTList::TestObjectRefOperations(){
	SetSubTestNum(9);

	decTListXmlElementTag list;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	decXmlElementTag::Ref tag3 = decXmlElementTag::Ref::New("tag3");
	decXmlElementTag::Ref tag4 = decXmlElementTag::Ref::New("tag4");

	list.Add(tag1);
	list.Add(tag2);
	list.Add(tag3);

	// Insert
	list.Insert(tag4, 1); // [tag1, tag4, tag2, tag3]
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_TRUE(list.GetAt(1) == tag4);

	// Move
	list.Move(1, 3); // [tag1, tag2, tag4, tag3]
	ASSERT_TRUE(list.GetAt(2) == tag4);

	// RemoveFrom
	list.RemoveFrom(2); // [tag1, tag2, tag3]
	ASSERT_EQUAL(list.GetCount(), 3);

	// Copy constructor
	decTListXmlElementTag list2(list);
	ASSERT_EQUAL(list2.GetCount(), 3);
	ASSERT_TRUE(list2[0] == tag1);

	// operator=
	decTListXmlElementTag list3;
	list3 = list;
	ASSERT_TRUE(list3 == list);

	// Iterators
	int count = 0;
	for(auto it = list.cbegin(); it != list.cend(); ++it){
		ASSERT_NOT_NULL(it->Pointer());
		count++;
	}
	ASSERT_EQUAL(count, 3);
}
