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

	// DEFindNamed tests
	TestDEFindNamedFound();
	TestDEFindNamedNotFound();
	
	// DEAppend tests
	TestDEAppend();
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


// Test DEFindNamed
////////////////////

// Helper class for testing DEFindNamed
class TestNamedElement {
public:
	decString name;
	int value;

	TestNamedElement() : value(0) {}
	TestNamedElement(const char *n, int v) : name(n), value(v) {}

	const decString &GetName() const { return name; }
};

void detHelperFunctions::TestDEFindNamedFound(){
	SetSubTestNum(4);

	decTList<TestNamedElement> list;
	list.Add(TestNamedElement("first", 1));
	list.Add(TestNamedElement("second", 2));
	list.Add(TestNamedElement("third", 3));

	const TestNamedElement *found = DEFindNamed<decTList<TestNamedElement>, TestNamedElement>(list, "second");
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(found->value, 2);
	ASSERT_EQUAL(found->GetName(), "second");
}

void detHelperFunctions::TestDEFindNamedNotFound(){
	SetSubTestNum(5);

	decTList<TestNamedElement> list;
	list.Add(TestNamedElement("first", 1));
	list.Add(TestNamedElement("second", 2));
	list.Add(TestNamedElement("third", 3));

	const TestNamedElement *found = DEFindNamed<decTList<TestNamedElement>, TestNamedElement>(list, "notfound");
	ASSERT_NULL(found);
}


// Test DEAppend
//////////////////

void detHelperFunctions::TestDEAppend(){
	SetSubTestNum(6);

	// Test appending to decStringList
	{
	decTList<decString> collection;
	collection.Add("one");
	collection.Add("two");
	collection.Add("three");
	
	decStringList list;
	DEAppend(list, collection);
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_TRUE(list.Has("one"));
	ASSERT_TRUE(list.Has("two"));
	ASSERT_TRUE(list.Has("three"));
	}
	
	// Test appending decStringList to decTList<decString>
	{
	decStringList list;
	list.Add("alpha");
	list.Add("beta");
	list.Add("gamma");

	decTList<decString> collection;
	DEAppend(collection, list);
	ASSERT_EQUAL(collection.GetCount(), 3);
	ASSERT_EQUAL(collection.GetAt(0), "alpha");
	ASSERT_EQUAL(collection.GetAt(1), "beta");
	ASSERT_EQUAL(collection.GetAt(2), "gamma");
	}

	// Test appending to decStringSet
	{
	decTSet<decString> collection;
	collection.Add("one");
	collection.Add("two");
	collection.Add("three");

	decStringSet set;
	DEAppend(set, collection);
	ASSERT_EQUAL(set.GetCount(), 3);
	ASSERT_TRUE(set.Has("one"));
	ASSERT_TRUE(set.Has("two"));
	ASSERT_TRUE(set.Has("three"));
	}
	
	// Test appending decStringSet to decTSet<decString>
	{
	decStringSet set;
	set.Add("alpha");
	set.Add("beta");
	set.Add("gamma");

	decTSet<decString> collection;
	DEAppend(collection, set);
	ASSERT_EQUAL(collection.GetCount(), 3);
	ASSERT_EQUAL(collection.GetAt(0), "alpha");
	ASSERT_EQUAL(collection.GetAt(1), "beta");
	ASSERT_EQUAL(collection.GetAt(2), "gamma");
	}
}
