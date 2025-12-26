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
	TestIntIndexOfMatching();
	TestIntHasMatching();
	TestIntAllMatching();
	TestIntNoneMatching();
	TestIntFold();
	TestIntInject();
	TestIntVisitVariants();
	TestIntReverse();
	TestIntSortVariants();
	TestIntMoveConstructor();
	TestIntIteratorConstructor();
	TestIntCollectionConstructor();
	// string
	TestStringBasic();
	TestStringOperators();
	TestStringIndexOfMatching();
	TestStringHasMatching();
	TestStringAllMatching();
	TestStringNoneMatching();
	TestStringFold();
	TestStringInject();
	// object ref
	TestObjectRefBasic();
	TestObjectRefOperations();
	TestObjectRefIndexOfMatching();
	TestObjectRefHasMatching();
	TestObjectRefAllMatching();
	TestObjectRefNoneMatching();
	TestObjectRefFold();
	TestObjectRefInject();
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


// ============================================================================
// New Function Tests - INT
// ============================================================================

void detTList::TestIntIndexOfMatching(){
	SetSubTestNum(12);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	list.Add(40);
	
	auto evaluator1 = [](int val){ return val == 30; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](int val){ return val >= 30; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator3, 3), 3);
	ASSERT_EQUAL(list.IndexOfMatching(evaluator3, 2), 2);
}

void detTList::TestIntHasMatching(){
	SetSubTestNum(13);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	
	auto evaluator1 = [](int val){ return val == 20; };
	ASSERT_TRUE(list.HasMatching(evaluator1));
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_FALSE(list.HasMatching(evaluator2));
	
	auto evaluator3 = [](int val){ return val < 15; };
	ASSERT_TRUE(list.HasMatching(evaluator3));
}

void detTList::TestIntAllMatching(){
	SetSubTestNum(131);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	
	// All elements are > 5
	auto evaluator1 = [](int val){ return val > 5; };
	ASSERT_TRUE(list.AllMatching(evaluator1));
	
	// Not all elements are > 15
	auto evaluator2 = [](int val){ return val > 15; };
	ASSERT_FALSE(list.AllMatching(evaluator2));
	
	// All elements are < 50
	auto evaluator3 = [](int val){ return val < 50; };
	ASSERT_TRUE(list.AllMatching(evaluator3));
	
	// Empty list should return true
	decTListInt emptyList;
	auto evaluator4 = [](int val){ return val > 100; };
	ASSERT_TRUE(emptyList.AllMatching(evaluator4));
}

void detTList::TestIntNoneMatching(){
	SetSubTestNum(132);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	
	// None of the elements are > 50
	auto evaluator1 = [](int val){ return val > 50; };
	ASSERT_TRUE(list.NoneMatching(evaluator1));
	
	// Some elements are > 15
	auto evaluator2 = [](int val){ return val > 15; };
	ASSERT_FALSE(list.NoneMatching(evaluator2));
	
	// None are < 5
	auto evaluator3 = [](int val){ return val < 5; };
	ASSERT_TRUE(list.NoneMatching(evaluator3));
	
	// Empty list should return true
	decTListInt emptyList;
	auto evaluator4 = [](int val){ return val > 0; };
	ASSERT_TRUE(emptyList.NoneMatching(evaluator4));
}

void detTList::TestIntFold(){
	SetSubTestNum(14);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(list.Fold(combiner), 60);
	
	decTListInt emptyList;
	ASSERT_DOES_FAIL(emptyList.Fold(combiner));
	
	decTListInt list2;
	list2.Add(2);
	list2.Add(3);
	list2.Add(4);
	auto multiplier = [](int acc, int val){ return acc * val; };
	ASSERT_EQUAL(list2.Fold(multiplier), 24);
}

void detTList::TestIntInject(){
	SetSubTestNum(15);

	decTListInt list;
	list.Add(10);
	list.Add(20);
	list.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(list.Inject(100, combiner), 160);
	
	decTListInt emptyList;
	ASSERT_EQUAL(emptyList.Inject(100, combiner), 100);
	
	// Test with different return type
	auto stringCombiner = [](decString acc, int val){ 
		decString str;
		str.Format("%d", val);
		return acc + str; 
	};
	ASSERT_EQUAL(list.Inject(decString("values:"), stringCombiner), decString("values:102030"));
}


// ============================================================================
// New Function Tests - STRING
// ============================================================================

void detTList::TestStringIndexOfMatching(){
	SetSubTestNum(16);

	decTListString list;
	list.Add("apple");
	list.Add("banana");
	list.Add("cherry");
	list.Add("date");
	
	auto evaluator1 = [](const decString &val){ return val == "cherry"; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](const decString &val){ return val.GetLength() == 4; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator3, 3), 3);
}

void detTList::TestStringHasMatching(){
	SetSubTestNum(17);

	decTListString list;
	list.Add("apple");
	list.Add("banana");
	list.Add("cherry");
	
	auto evaluator1 = [](const decString &val){ return val == "banana"; };
	ASSERT_TRUE(list.HasMatching(evaluator1));
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_FALSE(list.HasMatching(evaluator2));
	
	auto evaluator3 = [](const decString &val){ return val.GetLength() == 5; };
	ASSERT_TRUE(list.HasMatching(evaluator3));
}

void detTList::TestStringAllMatching(){
	SetSubTestNum(171);

	decTListString list;
	list.Add("apple");
	list.Add("banana");
	list.Add("cherry");
	
	// All strings have length > 3
	auto evaluator1 = [](const decString &val){ return val.GetLength() > 3; };
	ASSERT_TRUE(list.AllMatching(evaluator1));
	
	// Not all strings have length > 6
	auto evaluator2 = [](const decString &val){ return val.GetLength() > 6; };
	ASSERT_FALSE(list.AllMatching(evaluator2));
	
	// All strings are non-empty
	auto evaluator3 = [](const decString &val){ return !val.IsEmpty(); };
	ASSERT_TRUE(list.AllMatching(evaluator3));
	
	// Empty list should return true
	decTListString emptyList;
	auto evaluator4 = [](const decString &val){ return val == "never"; };
	ASSERT_TRUE(emptyList.AllMatching(evaluator4));
}

void detTList::TestStringNoneMatching(){
	SetSubTestNum(172);

	decTListString list;
	list.Add("apple");
	list.Add("banana");
	list.Add("cherry");
	
	// None of the strings equal "grape"
	auto evaluator1 = [](const decString &val){ return val == "grape"; };
	ASSERT_TRUE(list.NoneMatching(evaluator1));
	
	// Some strings have length 5
	auto evaluator2 = [](const decString &val){ return val.GetLength() == 5; };
	ASSERT_FALSE(list.NoneMatching(evaluator2));
	
	// None are empty
	auto evaluator3 = [](const decString &val){ return val.IsEmpty(); };
	ASSERT_TRUE(list.NoneMatching(evaluator3));
	
	// Empty list should return true
	decTListString emptyList;
	auto evaluator4 = [](const decString &val){ return !val.IsEmpty(); };
	ASSERT_TRUE(emptyList.NoneMatching(evaluator4));
}

void detTList::TestStringFold(){
	SetSubTestNum(18);

	decTListString list;
	list.Add("a");
	list.Add("b");
	list.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(list.Fold(combiner), decString("abc"));
	
	decTListString emptyList;
	ASSERT_DOES_FAIL(emptyList.Fold(combiner));
}

void detTList::TestStringInject(){
	SetSubTestNum(19);

	decTListString list;
	list.Add("a");
	list.Add("b");
	list.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(list.Inject(decString("start:"), combiner), decString("start:abc"));
	
	decTListString emptyList;
	ASSERT_EQUAL(emptyList.Inject(decString("start:"), combiner), decString("start:"));
	
	// Test with different return type
	auto lengthCombiner = [](int acc, const decString &val){ 
		return acc + val.GetLength(); 
	};
	ASSERT_EQUAL(list.Inject(0, lengthCombiner), 3);
}


// ============================================================================
// New Function Tests - OBJECTREF
// ============================================================================

void detTList::TestObjectRefIndexOfMatching(){
	SetSubTestNum(20);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	decXmlElementTag::Ref obj4(decXmlElementTag::Ref::New("tag4"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	list.Add(obj4);
	
	auto evaluator1 = [obj3](decXmlElementTag::Ref val){ return val == obj3; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator1), 2);
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator2), -1);
	
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_EQUAL(list.IndexOfMatching(evaluator3, 2), 2);
}

void detTList::TestObjectRefHasMatching(){
	SetSubTestNum(21);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	
	auto evaluator1 = [obj2](decXmlElementTag::Ref val){ return val == obj2; };
	ASSERT_TRUE(list.HasMatching(evaluator1));
	
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_FALSE(list.HasMatching(evaluator2));
	
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_TRUE(list.HasMatching(evaluator3));
}

void detTList::TestObjectRefAllMatching(){
	SetSubTestNum(211);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	
	// All tag names have length 4
	auto evaluator1 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_TRUE(list.AllMatching(evaluator1));
	
	// Not all tag names start with "tag2"
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag2"; };
	ASSERT_FALSE(list.AllMatching(evaluator2));
	
	// All are non-null
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val.Pointer() != nullptr; };
	ASSERT_TRUE(list.AllMatching(evaluator3));
	
	// Empty list should return true
	decTListXmlElementTag emptyList;
	auto evaluator4 = [](decXmlElementTag::Ref val){ return val.Pointer() == nullptr; };
	ASSERT_TRUE(emptyList.AllMatching(evaluator4));
}

void detTList::TestObjectRefNoneMatching(){
	SetSubTestNum(212);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("tag1"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("tag2"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("tag3"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	
	// None have name "tag5"
	auto evaluator1 = [](decXmlElementTag::Ref val){ return val->GetName() == "tag5"; };
	ASSERT_TRUE(list.NoneMatching(evaluator1));
	
	// Some have length 4
	auto evaluator2 = [](decXmlElementTag::Ref val){ return val->GetName().GetLength() == 4; };
	ASSERT_FALSE(list.NoneMatching(evaluator2));
	
	// None are null
	auto evaluator3 = [](decXmlElementTag::Ref val){ return val.Pointer() == nullptr; };
	ASSERT_TRUE(list.NoneMatching(evaluator3));
	
	// Empty list should return true
	decTListXmlElementTag emptyList;
	auto evaluator4 = [](decXmlElementTag::Ref val){ return val.Pointer() != nullptr; };
	ASSERT_TRUE(emptyList.NoneMatching(evaluator4));
}

void detTList::TestObjectRefFold(){
	SetSubTestNum(22);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("a"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("b"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("c"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	
	auto combiner = [](decXmlElementTag::Ref acc, decXmlElementTag::Ref val){ 
		return acc; 
	};
	ASSERT_EQUAL(list.Fold(combiner), obj1);
	
	decTListXmlElementTag emptyList;
	ASSERT_DOES_FAIL(emptyList.Fold(combiner));
}

void detTList::TestObjectRefInject(){
	SetSubTestNum(23);

	decXmlElementTag::Ref obj1(decXmlElementTag::Ref::New("a"));
	decXmlElementTag::Ref obj2(decXmlElementTag::Ref::New("b"));
	decXmlElementTag::Ref obj3(decXmlElementTag::Ref::New("c"));
	
	decTListXmlElementTag list;
	list.Add(obj1);
	list.Add(obj2);
	list.Add(obj3);
	
	// Test with string accumulator
	auto combiner = [](decString acc, decXmlElementTag::Ref val){ 
		return acc + val->GetName(); 
	};
	ASSERT_EQUAL(list.Inject(decString("names:"), combiner), decString("names:abc"));
	
	decTListXmlElementTag emptyList;
	ASSERT_EQUAL(emptyList.Inject(decString("names:"), combiner), decString("names:"));
	
	// Test with int accumulator (count elements)
	auto counter = [](int acc, decXmlElementTag::Ref val){ 
		return acc + 1; 
	};
	ASSERT_EQUAL(list.Inject(0, counter), 3);
}


// ============================================================================
// Additional INT Tests - Visit Variants
// ============================================================================

void detTList::TestIntVisitVariants(){
	SetSubTestNum(24);

	decTListInt list;
	for(int i=1; i<=10; i++) list.Add(i * 10); // [10,20,30,...,100]

	// VisitReverse
	decTListInt reversed;
	list.VisitReverse([&reversed](const int &v){ reversed.Add(v); });
	ASSERT_EQUAL(reversed.GetCount(), 10);
	ASSERT_EQUAL(reversed.GetAt(0), 100);
	ASSERT_EQUAL(reversed.GetAt(9), 10);

	// VisitIndexed
	int lastIndex = -1;
	list.VisitIndexed([&lastIndex](int index, const int &v){ 
		ASSERT_EQUAL(v, (index + 1) * 10);
		lastIndex = index;
	});
	ASSERT_EQUAL(lastIndex, 9);

	// VisitIndexed with range
	int sumIndexed = 0;
	list.VisitIndexed([&sumIndexed](int index, const int &v){ 
		sumIndexed += v;
	}, 2, 5);
	ASSERT_EQUAL(sumIndexed, 30 + 40 + 50);

	// VisitIndexed with negative step
	decTListInt reversedIndexed;
	list.VisitIndexed([&reversedIndexed](int index, const int &v){ 
		reversedIndexed.Add(v);
	}, 5, 2, -1);
	ASSERT_EQUAL(reversedIndexed.GetCount(), 4);
	ASSERT_EQUAL(reversedIndexed.GetAt(0), 60);
	ASSERT_EQUAL(reversedIndexed.GetAt(3), 30);

	// VisitReverseIndexed
	int firstIndexReverse = -1;
	list.VisitReverseIndexed([&firstIndexReverse](int index, const int &v){
		if(firstIndexReverse == -1) firstIndexReverse = index;
		ASSERT_EQUAL(v, (index + 1) * 10);
	});
	ASSERT_EQUAL(firstIndexReverse, 9);
}


// ============================================================================
// Additional INT Tests - Reverse
// ============================================================================

void detTList::TestIntReverse(){
	SetSubTestNum(25);

	decTListInt list;
	list.Add(1);
	list.Add(2);
	list.Add(3);
	list.Add(4);
	list.Add(5);

	// GetReversed (non-mutating)
	auto reversed = list.GetReversed();
	ASSERT_EQUAL(reversed.GetCount(), 5);
	ASSERT_EQUAL(reversed.GetAt(0), 5);
	ASSERT_EQUAL(reversed.GetAt(4), 1);
	// Original unchanged
	ASSERT_EQUAL(list.GetAt(0), 1);
	ASSERT_EQUAL(list.GetAt(4), 5);

	// Reverse (mutating)
	list.Reverse();
	ASSERT_EQUAL(list.GetAt(0), 5);
	ASSERT_EQUAL(list.GetAt(4), 1);

	// Test with empty list
	decTListInt emptyList;
	emptyList.Reverse();
	ASSERT_TRUE(emptyList.IsEmpty());

	// Test with single element
	decTListInt singleList;
	singleList.Add(42);
	singleList.Reverse();
	ASSERT_EQUAL(singleList.GetCount(), 1);
	ASSERT_EQUAL(singleList.GetAt(0), 42);
}


// ============================================================================
// Additional INT Tests - Sort Variants
// ============================================================================

void detTList::TestIntSortVariants(){
	SetSubTestNum(26);

	// SortAscending
	decTListInt list1;
	list1.Add(50);
	list1.Add(20);
	list1.Add(80);
	list1.Add(10);
	list1.Add(40);

	list1.SortAscending();
	ASSERT_EQUAL(list1.GetAt(0), 10);
	ASSERT_EQUAL(list1.GetAt(1), 20);
	ASSERT_EQUAL(list1.GetAt(2), 40);
	ASSERT_EQUAL(list1.GetAt(3), 50);
	ASSERT_EQUAL(list1.GetAt(4), 80);

	// SortDescending
	decTListInt list2;
	list2.Add(50);
	list2.Add(20);
	list2.Add(80);
	list2.Add(10);
	list2.Add(40);

	list2.SortDescending();
	ASSERT_EQUAL(list2.GetAt(0), 80);
	ASSERT_EQUAL(list2.GetAt(1), 50);
	ASSERT_EQUAL(list2.GetAt(2), 40);
	ASSERT_EQUAL(list2.GetAt(3), 20);
	ASSERT_EQUAL(list2.GetAt(4), 10);

	// GetSortedAscending (non-mutating)
	decTListInt list3;
	list3.Add(50);
	list3.Add(20);
	list3.Add(80);

	auto sorted3 = list3.GetSortedAscending();
	ASSERT_EQUAL(sorted3.GetAt(0), 20);
	ASSERT_EQUAL(sorted3.GetAt(1), 50);
	ASSERT_EQUAL(sorted3.GetAt(2), 80);
	// Original unchanged
	ASSERT_EQUAL(list3.GetAt(0), 50);

	// GetSortedDescending (non-mutating)
	auto sorted4 = list3.GetSortedDescending();
	ASSERT_EQUAL(sorted4.GetAt(0), 80);
	ASSERT_EQUAL(sorted4.GetAt(1), 50);
	ASSERT_EQUAL(sorted4.GetAt(2), 20);
	// Original still unchanged
	ASSERT_EQUAL(list3.GetAt(0), 50);

	// Empty list edge cases
	decTListInt emptyList;
	emptyList.SortAscending();
	ASSERT_TRUE(emptyList.IsEmpty());

	auto sortedEmpty = emptyList.GetSortedDescending();
	ASSERT_TRUE(sortedEmpty.IsEmpty());
}


// ============================================================================
// Additional INT Tests - Move Constructor
// ============================================================================

void detTList::TestIntMoveConstructor(){
	SetSubTestNum(27);

	decTListInt list1;
	list1.Add(10);
	list1.Add(20);
	list1.Add(30);

	// Move constructor
	decTListInt list2(std::move(list1));
	ASSERT_EQUAL(list2.GetCount(), 3);
	ASSERT_EQUAL(list2.GetAt(0), 10);
	ASSERT_EQUAL(list2.GetAt(1), 20);
	ASSERT_EQUAL(list2.GetAt(2), 30);

	// list1 should be empty after move
	ASSERT_EQUAL(list1.GetCount(), 0);
	ASSERT_TRUE(list1.IsEmpty());

	// Move assignment
	decTListInt list3;
	list3.Add(100);
	list3 = std::move(list2);
	ASSERT_EQUAL(list3.GetCount(), 3);
	ASSERT_EQUAL(list3.GetAt(0), 10);
	ASSERT_EQUAL(list2.GetCount(), 0);
}


// ============================================================================
// Additional INT Tests - Iterator Constructor
// ============================================================================

void detTList::TestIntIteratorConstructor(){
	SetSubTestNum(28);

	decTListInt source;
	source.Add(5);
	source.Add(10);
	source.Add(15);
	source.Add(20);

	// Construct from iterators
	decTListInt list1(source.cbegin(), source.cend());
	ASSERT_EQUAL(list1.GetCount(), 4);
	ASSERT_EQUAL(list1.GetAt(0), 5);
	ASSERT_EQUAL(list1.GetAt(3), 20);

	// Construct from partial range
	auto it1 = source.cbegin();
	auto it2 = source.cbegin();
	++it1; // points to 10
	it2 += 3; // points to 20
	
	decTListInt list2(it1, it2);
	ASSERT_EQUAL(list2.GetCount(), 2);
	ASSERT_EQUAL(list2.GetAt(0), 10);
	ASSERT_EQUAL(list2.GetAt(1), 15);

	// Construct from empty range
	decTListInt list3(source.cend(), source.cend());
	ASSERT_TRUE(list3.IsEmpty());
}


// ============================================================================
// Additional INT Tests - Collection Constructor
// ============================================================================

void detTList::TestIntCollectionConstructor(){
	SetSubTestNum(29);

	decTListInt source;
	source.Add(100);
	source.Add(200);
	source.Add(300);

	// Construct from another collection
	decTListInt list1(source);
	ASSERT_EQUAL(list1.GetCount(), 3);
	ASSERT_EQUAL(list1.GetAt(0), 100);
	ASSERT_EQUAL(list1.GetAt(2), 300);

	// Modify source to ensure deep copy
	source.SetAt(0, 999);
	ASSERT_EQUAL(source.GetAt(0), 999);
	ASSERT_EQUAL(list1.GetAt(0), 100); // list1 unchanged

	// Construct from empty collection
	decTListInt emptySource;
	decTListInt list2(emptySource);
	ASSERT_TRUE(list2.IsEmpty());
}

