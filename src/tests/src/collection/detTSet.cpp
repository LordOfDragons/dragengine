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
	TestIntIndexOf();
	TestIntHasMatching();
	TestIntFindOrDefault();
	TestIntFold();
	TestIntInject();
	// string
	TestStringBasic();
	TestStringOperators();
	TestStringHasMatching();
	TestStringFindOrDefault();
	TestStringFold();
	TestStringInject();
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

	// Find - returns pointer
	const int *resultPtr = nullptr;
	bool found = set.Find([](const int &v){ return v > 7; }, resultPtr);
	ASSERT_TRUE(found);
	ASSERT_NOT_NULL(resultPtr);
	ASSERT_EQUAL(*resultPtr, 8);

	// FindOrDefault - no parameters
	ASSERT_EQUAL(set.FindOrDefault([](const int &v){ return v > 100; }, 999), 999);
	ASSERT_EQUAL(set.FindOrDefault([](const int &v){ return v > 7; }, 999), 8);

	// Collect - no parameters
	auto collected = set.Collect([](const int &v){ return v > 5; });
	ASSERT_EQUAL(collected.GetCount(), 2); // 8, 9
	ASSERT_TRUE(collected.Has(8));
	ASSERT_TRUE(collected.Has(9));

	// RemoveIf - no parameters
	set.RemoveIf([](const int &v){ return v < 5; });
	ASSERT_EQUAL(set.GetCount(), 3); // 5, 8, 9
	ASSERT_TRUE(set.Has(5));
	ASSERT_TRUE(set.Has(8));
	ASSERT_TRUE(set.Has(9));

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


// ============================================================================
// New Function Tests - INT
// ============================================================================

void detTSet::TestIntIndexOf(){
	SetSubTestNum(7);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	set.Add(40);
	
	ASSERT_EQUAL(set.IndexOf(20), 1);
	ASSERT_EQUAL(set.IndexOf(40), 3);
	ASSERT_EQUAL(set.IndexOf(99), -1);
	ASSERT_EQUAL(set.IndexOf(10), 0);
}

void detTSet::TestIntHasMatching(){
	SetSubTestNum(8);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto evaluator1 = [](int val){ return val == 20; };
	ASSERT_TRUE(set.HasMatching(evaluator1));
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_FALSE(set.HasMatching(evaluator2));
	
	auto evaluator3 = [](int val){ return val >= 10; };
	ASSERT_TRUE(set.HasMatching(evaluator3));
}

void detTSet::TestIntFindOrDefault(){
	SetSubTestNum(9);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto evaluator1 = [](int val){ return val == 20; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator1, 99), 20);
	
	auto evaluator2 = [](int val){ return val > 50; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator2, 99), 99);
	
	auto evaluator3 = [](int val){ return val < 15; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator3, 99), 10);
	
	// Test without default value
	ASSERT_EQUAL(set.FindOrDefault(evaluator2), 0);
}

void detTSet::TestIntFold(){
	SetSubTestNum(10);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(set.Fold(combiner), 60);
	
	decTSetInt emptySet;
	ASSERT_DOES_FAIL(emptySet.Fold(combiner));
	
	decTSetInt set2;
	set2.Add(2);
	set2.Add(3);
	set2.Add(4);
	auto multiplier = [](int acc, int val){ return acc * val; };
	ASSERT_EQUAL(set2.Fold(multiplier), 24);
}

void detTSet::TestIntInject(){
	SetSubTestNum(11);

	decTSetInt set;
	set.Add(10);
	set.Add(20);
	set.Add(30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(set.Inject(100, combiner), 160);
	
	decTSetInt emptySet;
	ASSERT_EQUAL(emptySet.Inject(100, combiner), 100);
	
	// Test with different return type
	auto stringCombiner = [](decString acc, int val){ 
		decString str;
		str.Format("%d", val);
		return acc + str; 
	};
	ASSERT_EQUAL(set.Inject(decString("values:"), stringCombiner), decString("values:102030"));
}


// ============================================================================
// New Function Tests - STRING
// ============================================================================

void detTSet::TestStringHasMatching(){
	SetSubTestNum(12);

	decTSetString set;
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

void detTSet::TestStringFindOrDefault(){
	SetSubTestNum(13);

	decTSetString set;
	set.Add("apple");
	set.Add("banana");
	set.Add("cherry");
	
	auto evaluator1 = [](const decString &val){ return val == "banana"; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator1, decString("default")), decString("banana"));
	
	auto evaluator2 = [](const decString &val){ return val == "fig"; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator2, decString("default")), decString("default"));
	
	// Test without explicit default value
	ASSERT_EQUAL(set.FindOrDefault(evaluator2), decString());
	
	auto evaluator3 = [](const decString &val){ return val.GetLength() > 6; };
	ASSERT_EQUAL(set.FindOrDefault(evaluator3, decString("default")), decString("default"));
}

void detTSet::TestStringFold(){
	SetSubTestNum(14);

	decTSetString set;
	set.Add("a");
	set.Add("b");
	set.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(set.Fold(combiner), decString("abc"));
	
	decTSetString emptySet;
	ASSERT_DOES_FAIL(emptySet.Fold(combiner));
}

void detTSet::TestStringInject(){
	SetSubTestNum(15);

	decTSetString set;
	set.Add("a");
	set.Add("b");
	set.Add("c");
	
	auto combiner = [](const decString &acc, const decString &val){ return acc + val; };
	ASSERT_EQUAL(set.Inject(decString("start:"), combiner), decString("start:abc"));
	
	decTSetString emptySet;
	ASSERT_EQUAL(emptySet.Inject(decString("start:"), combiner), decString("start:"));
	
	// Test with different return type
	auto lengthCombiner = [](int acc, const decString &val){ 
		return acc + val.GetLength(); 
	};
	ASSERT_EQUAL(set.Inject(0, lengthCombiner), 3);
}
