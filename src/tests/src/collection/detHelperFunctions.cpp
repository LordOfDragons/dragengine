// include only once
#include <stdio.h>
#include <string.h>

#include "detHelperFunctions.h"

#include <dragengine/common/collection/decHelperFunctions.h>
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
