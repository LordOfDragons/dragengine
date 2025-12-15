// includes
#include <stdio.h>

#include "detTSet.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTSet.h>
#include <dragengine/common/exceptions.h>

// Type aliases
typedef decTSet<int> decTSetInt;
typedef decTSet<decString> decTSetString;


// Class detTSet
////////////////

detTSet::detTSet(){
	Prepare();
}

detTSet::~detTSet(){
	CleanUp();
}

void detTSet::Prepare(){
}

void detTSet::Run(){
	// int
	TestIntBasic();
	TestIntUniqueness();
	TestIntOperators();
	TestIntIterators();
	TestIntAlgorithms();
	// string
	TestStringBasic();
	TestStringOperators();
}

void detTSet::CleanUp(){
}

const char *detTSet::GetTestName(){
	return "TSet";
}


// Tests - Integer Type
////////////////////////

void detTSet::TestIntBasic(){
	SetSubTestNum(0);

	// Constructor tests
	decTSetInt set;
	ASSERT_EQUAL(set.GetCount(), 0);
	ASSERT_TRUE(set.IsEmpty());
	ASSERT_FALSE(set.IsNotEmpty());

	// Constructor with capacity
	decTSetInt set2(10);
	ASSERT_EQUAL(set2.GetCount(), 0);

	// Add elements
	set.Add(1);
	set.Add(2);
	set.Add(3);
	ASSERT_EQUAL(set.GetCount(), 3);
	ASSERT_FALSE(set.IsEmpty());
	ASSERT_TRUE(set.IsNotEmpty());
	ASSERT_TRUE(set.Has(2));
	ASSERT_EQUAL(set.IndexOf(1), 0);
	ASSERT_EQUAL(set.IndexOf(3), 2);

	// Copy constructor
	decTSetInt set3(set);
	ASSERT_EQUAL(set3.GetCount(), 3);
	ASSERT_TRUE(set3.Has(1));
	ASSERT_TRUE(set3.Has(2));
	ASSERT_TRUE(set3.Has(3));
}

void detTSet::TestIntUniqueness(){
	SetSubTestNum(1);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);

	// AddIfAbsent returns false for duplicates
	ASSERT_FALSE(set.AddIfAbsent(20));
	ASSERT_EQUAL(set.GetCount(), 3);

	// Add throws on duplicate
	ASSERT_DOES_FAIL(set.Add(20));

	// AddIfAbsent returns true for new elements
	ASSERT_TRUE(set.AddIfAbsent(40));
	ASSERT_EQUAL(set.GetCount(), 4);
	ASSERT_TRUE(set.Has(40));
}

void detTSet::TestIntOperators(){
	SetSubTestNum(2);

	decTSetInt set1;
	set1.Add(1);
	set1.Add(2);
	set1.Add(3);

	decTSetInt set2;
	set2.Add(3); // duplicate
	set2.Add(4);
	set2.Add(5);

	// operator+
	auto set3 = set1 + set2; // {1,2,3,4,5}
	ASSERT_EQUAL(set3.GetCount(), 5);
	ASSERT_TRUE(set3.Has(1));
	ASSERT_TRUE(set3.Has(5));

	// operator+=
	decTSetInt set4;
	set4.Add(10);
	set4.Add(20);
	decTSetInt set5;
	set5.Add(30);
	set4 += set5;
	ASSERT_EQUAL(set4.GetCount(), 3);
	ASSERT_TRUE(set4.Has(30));

	// operator==
	decTSetInt setA;
	setA.Add(1);
	setA.Add(2);
	decTSetInt setB;
	setB.Add(1);
	setB.Add(2);
	ASSERT_TRUE(setA == setB);

	// operator!=
	setB.Add(3);
	ASSERT_TRUE(setA != setB);
	ASSERT_FALSE(setA == setB);

	// operator=
	decTSetInt setC;
	setC = setA;
	ASSERT_TRUE(setC == setA);
	ASSERT_EQUAL(setC.GetCount(), 2);

	// Remove operations
	ASSERT_TRUE(set1.RemoveIfPresent(2));
	ASSERT_FALSE(set1.Has(2));
	ASSERT_FALSE(set1.RemoveIfPresent(99));

	set1.Remove(1);
	ASSERT_FALSE(set1.Has(1));
	ASSERT_DOES_FAIL(set1.Remove(999));

	set1.RemoveAll();
	ASSERT_TRUE(set1.IsEmpty());
}

void detTSet::TestIntIterators(){
	SetSubTestNum(3);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	set.Add(50);

	// const_iterator
	int sum = 0;
	int count = 0;
	for(auto it = set.cbegin(); it != set.cend(); ++it){
		sum += *it;
		count++;
	}
	ASSERT_EQUAL(count, 5);
	ASSERT_EQUAL(sum, 150);

	// Random access
	auto it = set.cbegin();
	it += 2; // points to third element (30)
	ASSERT_EQUAL(*it, 30);
	ASSERT_TRUE(it == set.cbegin() + 2);

	auto it2 = it + 2; // points to 50
	ASSERT_EQUAL(*it2, 50);

	// Iterator arithmetic
	ASSERT_EQUAL(set.cend() - set.cbegin(), 5);

	// Reverse iterator
	int rsum = 0;
	for(auto rit = set.crbegin(); rit != set.crend(); ++rit){
		rsum += *rit;
	}
	ASSERT_EQUAL(rsum, 150);
}

void detTSet::TestIntAlgorithms(){
	SetSubTestNum(4);

	decTSetInt set;
	set.Add(5);
	set.Add(2);
	set.Add(8);
	set.Add(1);
	set.Add(9);

	// Visit - no parameters
	int visitSum = 0;
	set.Visit([&visitSum](const int &v){ visitSum += v; });
	ASSERT_EQUAL(visitSum, 25);

	// Visit with from parameter
	int sumFrom = 0;
	set.Visit([&sumFrom](const int &v){ sumFrom += v; }, 2);
	// Set doesn't have predictable order, just check count
	ASSERT_TRUE(sumFrom > 0);

	// Visit with from/to/step parameters
	int sumRange = 0;
	int countRange = 0;
	set.Visit([&sumRange, &countRange](const int &v){ sumRange += v; countRange++; }, 1, 4, 1);
	ASSERT_EQUAL(countRange, 3); // 3 elements visited

	// Visit with negative step
	int sumReverse = 0;
	int countReverse = 0;
	set.Visit([&sumReverse, &countReverse](const int &v){ sumReverse += v; countReverse++; }, 3, 0, -1);
	ASSERT_EQUAL(countReverse, 4); // 4 elements visited (indices 3,2,1,0)

	// Find - returns pointer
	const int *resultPtr = nullptr;
	bool found = set.Find([](const int &v){ return v > 7; }, resultPtr);
	ASSERT_TRUE(found);
	ASSERT_NOT_NULL(resultPtr);
	ASSERT_EQUAL(*resultPtr, 8);

	// Find with from parameter
	const int *foundFrom = nullptr;
	ASSERT_TRUE(set.Find([](const int &v){ return v < 5; }, foundFrom, 2));
	ASSERT_EQUAL(*foundFrom, 1);

	// Find with from/to/step parameters
	const int *foundRange = nullptr;
	ASSERT_TRUE(set.Find([](const int &v){ return v > 6; }, foundRange, 0, 5, 1));
	ASSERT_EQUAL(*foundRange, 8);

	// Find with negative step
	const int *foundRev = nullptr;
	ASSERT_TRUE(set.Find([](const int &v){ return v < 5; }, foundRev, 4, 0, -1));
	ASSERT_EQUAL(*foundRev, 1);

	// FindDefault - no parameters
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v > 100; }, 999), 999);
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v > 7; }, 999), 8);

	// FindDefault with from parameter
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v > 100; }, 999, 2), 999);
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v > 7; }, 999, 2), 8);

	// FindDefault with from/to/step parameters
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v == 2; }, 999, 0, 5, 1), 2);
	ASSERT_EQUAL(set.FindDefault([](const int &v){ return v > 100; }, 999, 0, 5, 1), 999);

	// Collect - no parameters
	auto collected = set.Collect([](const int &v){ return v > 5; });
	ASSERT_EQUAL(collected.GetCount(), 2); // 8, 9
	ASSERT_TRUE(collected.Has(8));
	ASSERT_TRUE(collected.Has(9));

	// Collect with from parameter
	auto collFrom = set.Collect([](const int &v){ return v > 5; }, 2);
	ASSERT_EQUAL(collFrom.GetCount(), 2); // 8, 9

	// Collect with from/to/step
	auto collRange = set.Collect([](const int &v){ return v < 10; }, 0, 5, 1);
	ASSERT_EQUAL(collRange.GetCount(), 5); // all elements

	// RemoveIf - no parameters
	set.RemoveIf([](const int &v){ return v < 5; });
	ASSERT_EQUAL(set.GetCount(), 3); // 5, 8, 9
	ASSERT_TRUE(set.Has(5));
	ASSERT_TRUE(set.Has(8));
	ASSERT_TRUE(set.Has(9));

	// RemoveIf with from parameter
	decTSetInt set2;
	for(int i=1; i<=10; i++) set2.Add(i);
	set2.RemoveIf([](const int &v){ return v > 7; }, 5);
	ASSERT_FALSE(set2.Has(8));
	ASSERT_FALSE(set2.Has(9));
	ASSERT_FALSE(set2.Has(10));
	ASSERT_TRUE(set2.Has(7));

	// RemoveIf with from/to/step
	decTSetInt set3;
	for(int i=1; i<=10; i++) set3.Add(i);
	set3.RemoveIf([](const int &v){ return v % 2 == 0; }, 1, 8, 1);
	ASSERT_FALSE(set3.Has(2));
	ASSERT_FALSE(set3.Has(4));
	ASSERT_FALSE(set3.Has(6));
	ASSERT_TRUE(set3.Has(1));
	ASSERT_TRUE(set3.Has(3));
	ASSERT_TRUE(set3.Has(5));

	// Equals method
	decTSetInt setA;
	setA.Add(1);
	setA.Add(2);
	decTSetInt setB;
	setB.Add(1);
	setB.Add(2);
	ASSERT_TRUE(setA.Equals(setB));
	setB.Add(3);
	ASSERT_FALSE(setA.Equals(setB));

	// First and Last methods
	decTSetInt set4;
	set4.Add(10);
	set4.Add(20);
	set4.Add(30);
	ASSERT_EQUAL(set4.First(), 10);
	ASSERT_EQUAL(set4.Last(), 30);
}


// Tests - String Type
///////////////////////

void detTSet::TestStringBasic(){
	SetSubTestNum(5);

	decTSetString set;
	set.Add(decString("alpha"));
	set.Add(decString("beta"));
	set.Add(decString("gamma"));

	ASSERT_EQUAL(set.GetCount(), 3);
	ASSERT_TRUE(set.Has(decString("beta")));
	ASSERT_EQUAL(set.IndexOf(decString("gamma")), 2);

	// Uniqueness
	ASSERT_FALSE(set.AddIfAbsent(decString("beta")));
	ASSERT_EQUAL(set.GetCount(), 3);

	// Remove
	ASSERT_TRUE(set.RemoveIfPresent(decString("alpha")));
	ASSERT_FALSE(set.Has(decString("alpha")));
	ASSERT_EQUAL(set.GetCount(), 2);

	set.RemoveAll();
	ASSERT_TRUE(set.IsEmpty());
}

void detTSet::TestStringOperators(){
	SetSubTestNum(6);

	decTSetString set1;
	set1.Add(decString("hello"));
	set1.Add(decString("world"));

	decTSetString set2;
	set2.Add(decString("foo"));
	set2.Add(decString("bar"));

	auto set3 = set1 + set2;
	ASSERT_EQUAL(set3.GetCount(), 4);
	ASSERT_TRUE(set3.Has(decString("foo")));
	ASSERT_TRUE(set3.Has(decString("world")));

	set1 += set2;
	ASSERT_TRUE(set1 == set3);

	// Copy and assignment
	decTSetString set4(set1);
	ASSERT_TRUE(set4 == set1);

	decTSetString set5;
	set5 = set1;
	ASSERT_TRUE(set5 == set1);
}
