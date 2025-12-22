// include only once
#include <stdio.h>
#include <string.h>

#include "detHelperFunctions.h"

#include <dragengine/common/collection/decGlobalFunctions.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decTSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>


// Class detHelperFunctions
/////////////////////////////

detHelperFunctions::detHelperFunctions(){
}

detHelperFunctions::~detHelperFunctions(){
}

void detHelperFunctions::Prepare(){
}

void detHelperFunctions::Run(){
	// DEJoin with decTList<decString>
	TestDEJoinStringListBasic();
	TestDEJoinStringListEmpty();
	TestDEJoinStringListSeparator();
	TestDEJoinStringListEmptyEntries();
	
	// Comparators
	TestComparators();
	
	// DEHash and DECompare for primitive types
	TestDEHashPrimitives();
	TestDEComparePrimitives();
}

void detHelperFunctions::CleanUp(){
}

const char *detHelperFunctions::GetTestName(){
	return "HelperFunctions";
}


// Test DEJoin with decTList<decString>
///////////////////////////////////////

void detHelperFunctions::TestDEJoinStringListBasic(){
	SetSubTestNum(0);

	decTList<decString> list;
	list.Add("hello");
	list.Add("world");
	list.Add("test");

	decString result = DEJoin(list, decString(" "));
	ASSERT_EQUAL(result, "hello world test");
}

void detHelperFunctions::TestDEJoinStringListEmpty(){
	SetSubTestNum(1);

	decTList<decString> list;

	decString result = DEJoin(list, decString(" "));
	ASSERT_EQUAL(result, "");
}

void detHelperFunctions::TestDEJoinStringListSeparator(){
	SetSubTestNum(2);

	decTList<decString> list;
	list.Add("a");
	list.Add("b");
	list.Add("c");

	// Test with different separator
	decString result1 = DEJoin(list, decString(", "));
	ASSERT_EQUAL(result1, "a, b, c");

	// Test with empty separator
	decString result2 = DEJoin(list, decString(""));
	ASSERT_EQUAL(result2, "abc");

	// Test with multichar separator
	decString result3 = DEJoin(list, decString(" :: "));
	ASSERT_EQUAL(result3, "a :: b :: c");

	// Test with single element
	decTList<decString> singleList;
	singleList.Add("only");
	decString result4 = DEJoin(singleList, decString(" "));
	ASSERT_EQUAL(result4, "only");
}

void detHelperFunctions::TestDEJoinStringListEmptyEntries(){
	SetSubTestNum(3);

	decTList<decString> list;
	list.Add("first");
	list.Add("");
	list.Add("second");
	list.Add("");
	list.Add("third");

	decString result = DEJoin(list, decString(", "));
	ASSERT_EQUAL(result, "first, second, third");
}


// Test Comparators
/////////////////////

void detHelperFunctions::TestComparators(){
	SetSubTestNum(6);
	
	{
	decTList<decString> list;
	list.Add("banana");
	list.Add("apple");
	list.Add("cherry");
	
	list.SortAscending();
	ASSERT_EQUAL(list.GetAt(0), "apple");
	ASSERT_EQUAL(list.GetAt(1), "banana");
	ASSERT_EQUAL(list.GetAt(2), "cherry");
	
	list.SortDescending();
	ASSERT_EQUAL(list.GetAt(0), "cherry");
	ASSERT_EQUAL(list.GetAt(1), "banana");
	ASSERT_EQUAL(list.GetAt(2), "apple");
	}

	{
	decTList<decString> list;
	list.Add("banana");
	list.Add("apple");
	list.Add("cherry");
	
	const decTList<decString> list2(list.GetSortedAscending());
	ASSERT_EQUAL(list2.GetAt(0), "apple");
	ASSERT_EQUAL(list2.GetAt(1), "banana");
	ASSERT_EQUAL(list2.GetAt(2), "cherry");
	
	ASSERT_EQUAL(list.GetAt(0), "banana");
	ASSERT_EQUAL(list.GetAt(1), "apple");
	ASSERT_EQUAL(list.GetAt(2), "cherry");
	
	const decTList<decString> list3(list.GetSortedDescending());
	ASSERT_EQUAL(list3.GetAt(0), "cherry");
	ASSERT_EQUAL(list3.GetAt(1), "banana");
	ASSERT_EQUAL(list3.GetAt(2), "apple");
	
	ASSERT_EQUAL(list.GetAt(0), "banana");
	ASSERT_EQUAL(list.GetAt(1), "apple");
	ASSERT_EQUAL(list.GetAt(2), "cherry");
	}

	{
	decTList<decString> list;
	list.Add("banana");
	list.Add("apple");
	list.Add("cherry");
	
	list.Sort(decAscendingComparator<decString>());
	ASSERT_EQUAL(list.GetAt(0), "apple");
	ASSERT_EQUAL(list.GetAt(1), "banana");
	ASSERT_EQUAL(list.GetAt(2), "cherry");
	
	list.Sort(decDesendingComparator<decString>());
	ASSERT_EQUAL(list.GetAt(0), "cherry");
	ASSERT_EQUAL(list.GetAt(1), "banana");
	ASSERT_EQUAL(list.GetAt(2), "apple");
	}

	{
	decTList<int> list;
	list.Add(20);
	list.Add(10);
	list.Add(30);
	list.SortAscending();
	ASSERT_EQUAL(list.GetAt(0), 10);
	ASSERT_EQUAL(list.GetAt(1), 20);
	ASSERT_EQUAL(list.GetAt(2), 30);
	
	list.SortDescending();
	ASSERT_EQUAL(list.GetAt(0), 30);
	ASSERT_EQUAL(list.GetAt(1), 20);
	ASSERT_EQUAL(list.GetAt(2), 10);
	}

	{
	decTList<float> list;
	list.Add(2.5f);
	list.Add(1.5f);
	list.Add(3.5f);
	list.SortAscending();
	ASSERT_EQUAL(list.GetAt(0), 1.5f);
	ASSERT_EQUAL(list.GetAt(1), 2.5f);
	ASSERT_EQUAL(list.GetAt(2), 3.5f);
	
	list.SortDescending();
	ASSERT_EQUAL(list.GetAt(0), 3.5f);
	ASSERT_EQUAL(list.GetAt(1), 2.5f);
	ASSERT_EQUAL(list.GetAt(2), 1.5f);
	}

	{
	decTList<double> list;
	list.Add(2.5);
	list.Add(1.5);
	list.Add(3.5);
	list.SortAscending();
	ASSERT_EQUAL(list.GetAt(0), 1.5);
	ASSERT_EQUAL(list.GetAt(1), 2.5);
	ASSERT_EQUAL(list.GetAt(2), 3.5);
	
	list.SortDescending();
	ASSERT_EQUAL(list.GetAt(0), 3.5);
	ASSERT_EQUAL(list.GetAt(1), 2.5);
	ASSERT_EQUAL(list.GetAt(2), 1.5);
	}
}


// Test DEHash for primitive types
//////////////////////////////////////

void detHelperFunctions::TestDEHashPrimitives(){
	SetSubTestNum(7);
	
	// Test DEHash for int
	{
		int val1 = 42;
		int val2 = 42;
		int val3 = 100;
		
		unsigned int hash1 = DEHash(val1);
		unsigned int hash2 = DEHash(val2);
		unsigned int hash3 = DEHash(val3);
		
		ASSERT_EQUAL(hash1, hash2);  // Same values should have same hash
		ASSERT_TRUE(hash1 != hash3);  // Different values should (likely) have different hash
		ASSERT_EQUAL(hash1, static_cast<unsigned int>(42));  // DEHash(int) returns the int as unsigned int
	}
	
	// Test DEHash for float
	{
		// Note: Float hashing uses the object's Hash() method if available,
		// but for primitive float types, there's no Hash() method.
		// The current implementation doesn't have a DEHash(float) overload,
		// so this tests if one needs to be added or if floats should hash differently.
		// For now, we test that the same float values produce consistent results.
		
		// Skip float hash test for now as DEHash(float) is not directly defined
		// Only DECompare(float) is defined
	}
	
	// Test DEHash for double
	{
		// Similar to float, double doesn't have a direct DEHash implementation
		// Skip for now
	}
}


// Test DECompare for primitive types
/////////////////////////////////////////

void detHelperFunctions::TestDEComparePrimitives(){
	SetSubTestNum(8);
	
	// Test DECompare for int
	{
		int val1 = 10;
		int val2 = 20;
		int val3 = 10;
		
		ASSERT_TRUE(DECompare(val1, val2) < 0);  // 10 < 20
		ASSERT_TRUE(DECompare(val2, val1) > 0);  // 20 > 10
		ASSERT_EQUAL(DECompare(val1, val3), 0);  // 10 == 10
	}
	
	// Test DECompare for float
	{
		float val1 = 1.5f;
		float val2 = 2.5f;
		float val3 = 1.5f;
		
		ASSERT_TRUE(DECompare(val1, val2) < 0);  // 1.5 < 2.5
		ASSERT_TRUE(DECompare(val2, val1) > 0);  // 2.5 > 1.5
		ASSERT_EQUAL(DECompare(val1, val3), 0);  // 1.5 == 1.5
	}
	
	// Test DECompare for double
	{
		double val1 = 1.5;
		double val2 = 2.5;
		double val3 = 1.5;
		
		ASSERT_TRUE(DECompare(val1, val2) < 0);  // 1.5 < 2.5
		ASSERT_TRUE(DECompare(val2, val1) > 0);  // 2.5 > 1.5
		ASSERT_EQUAL(DECompare(val1, val3), 0);  // 1.5 == 1.5
	}
}
