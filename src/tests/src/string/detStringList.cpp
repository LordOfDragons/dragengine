#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detStringList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>

	

// Class detStringList
////////////////////////

// Constructors, destructor
/////////////////////////////

detStringList::detStringList(){
	Prepare();
}

detStringList::~detStringList(){
	CleanUp();
}



// Testing
////////////

void detStringList::Prepare(){
}

void detStringList::Run(){
	TestModify();
	TestTesting();
	TestSort();
	TestOperator();
}

void detStringList::CleanUp(){
}

const char *detStringList::GetTestName(){
	return "StringList";
}



// Tests
//////////

const char * const testString1 = "Hallo";
const char * const testString2 = "Welt";
const char * const testString3 = "This is a test";
const char * const testString4 = "Something else";
const char * const testString5 = "Another test";

void detStringList::TestModify() {
	SetSubTestNum(0);
	
	// decStringList()
	decStringList list1;
	ASSERT_EQUAL(list1.GetCount(), 0);
	
	// Add
	list1.Add(testString1);
	ASSERT_EQUAL(list1.GetCount(), 1);
	ASSERT_EQUAL(list1.GetAt(0), testString1);
	
	list1.Add(decString(testString2));
	ASSERT_EQUAL(list1.GetCount(), 2);
	ASSERT_EQUAL(list1.GetAt(0), testString1);
	ASSERT_EQUAL(list1.GetAt(1), testString2);
	
	// Insert
	list1.Insert(testString3, 0);
	ASSERT_EQUAL(list1.GetCount(), 3);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString2);
	
	list1.Insert(decString(testString4), 2);
	ASSERT_EQUAL(list1.GetCount(), 4);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString2);
	
	list1.Insert(testString5, 4);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString2);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	// SetAt
	list1.SetAt(1, testString5);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString5);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString2);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	list1.SetAt(3, decString(testString1));
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString5);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	list1.SetAt(1, testString2);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString2);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	// Move
	list1.Move(1, 3);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString3);
	ASSERT_EQUAL(list1.GetAt(1), testString4);
	ASSERT_EQUAL(list1.GetAt(2), testString2);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	list1.Move(2, 0);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString3);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	list1.Move(1, 4);
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString4);
	ASSERT_EQUAL(list1.GetAt(2), testString1);
	ASSERT_EQUAL(list1.GetAt(3), testString3);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	// decStringList( const decStringList &list )
	decStringList list2(list1);
	ASSERT_EQUAL(list2.GetCount(), 5);
	ASSERT_EQUAL(list2.GetAt(0), testString2);
	ASSERT_EQUAL(list2.GetAt(1), testString4);
	ASSERT_EQUAL(list2.GetAt(2), testString1);
	ASSERT_EQUAL(list2.GetAt(3), testString3);
	ASSERT_EQUAL(list2.GetAt(4), testString5);
	
	// RemoveFrom
	list1.RemoveFrom(1);
	ASSERT_EQUAL(list1.GetCount(), 4);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString3);
	ASSERT_EQUAL(list1.GetAt(3), testString5);
	
	list1.RemoveFrom(3);
	ASSERT_EQUAL(list1.GetCount(), 3);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString3);
	
	list1.RemoveFrom(0);
	ASSERT_EQUAL(list1.GetCount(), 2);
	ASSERT_EQUAL(list1.GetAt(0), testString1);
	ASSERT_EQUAL(list1.GetAt(1), testString3);
	
	list1.RemoveFrom(0);
	list1.RemoveFrom(0);
	ASSERT_EQUAL(list1.GetCount(), 0);
	
	// RemoveAll
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	ASSERT_EQUAL(list1.GetCount(), 3);
	list1.RemoveAll();
	ASSERT_EQUAL(list1.GetCount(), 0);
}

void detStringList::TestTesting(){
	SetSubTestNum(1);
	
	// IndexOf
	decStringList list1;
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	ASSERT_EQUAL(list1.IndexOf(testString1), 0);
	ASSERT_EQUAL(list1.IndexOf(testString2), 1);
	ASSERT_EQUAL(list1.IndexOf(testString3), 2);
	ASSERT_EQUAL(list1.IndexOf(testString4), -1);
	ASSERT_EQUAL(list1.IndexOf(testString5), -1);
	
	ASSERT_EQUAL(list1.IndexOf(decString(testString1)), 0);
	ASSERT_EQUAL(list1.IndexOf(decString(testString2)), 1);
	ASSERT_EQUAL(list1.IndexOf(decString(testString3)), 2);
	ASSERT_EQUAL(list1.IndexOf(decString(testString4)), -1);
	ASSERT_EQUAL(list1.IndexOf(decString(testString5)), -1);
	
	// Has
	ASSERT_TRUE(list1.Has(testString1));
	ASSERT_TRUE(list1.Has(testString2));
	ASSERT_TRUE(list1.Has(testString3));
	ASSERT_FALSE(list1.Has(testString4));
	ASSERT_FALSE(list1.Has(testString5));
	
	ASSERT_TRUE(list1.Has(decString(testString1)));
	ASSERT_TRUE(list1.Has(decString(testString2)));
	ASSERT_TRUE(list1.Has(decString(testString3)));
	ASSERT_FALSE(list1.Has(decString(testString4)));
	ASSERT_FALSE(list1.Has(decString(testString5)));
}

void detStringList::TestSort(){
	SetSubTestNum(2);
	
	// SortAscending()
	decStringList list1;
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	list1.Add(testString4);
	list1.Add(testString5);
	
	list1.SortAscending();
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString5);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString3);
	ASSERT_EQUAL(list1.GetAt(4), testString2);
	
	list1.SortAscending();
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString5);
	ASSERT_EQUAL(list1.GetAt(1), testString1);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString3);
	ASSERT_EQUAL(list1.GetAt(4), testString2);
	
	// SortDescending()
	list1.RemoveAll();
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	list1.Add(testString4);
	list1.Add(testString5);
	
	list1.SortDescending();
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString3);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
	
	list1.SortDescending();
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString2);
	ASSERT_EQUAL(list1.GetAt(1), testString3);
	ASSERT_EQUAL(list1.GetAt(2), testString4);
	ASSERT_EQUAL(list1.GetAt(3), testString1);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
}

void detStringList::TestOperator(){
	SetSubTestNum(3);
	
	decStringList list1, list2, list3;
	
	// operator[]
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	ASSERT_EQUAL(list1.GetCount(), 3);
	ASSERT_EQUAL(list1[0], testString1);
	ASSERT_EQUAL(list1[1], testString2);
	ASSERT_EQUAL(list1[2], testString3);
	
	// operator=
	list2 = list1;
	ASSERT_EQUAL(list2.GetCount(), 3);
	ASSERT_EQUAL(list2[0], testString1);
	ASSERT_EQUAL(list2[1], testString2);
	ASSERT_EQUAL(list2[2], testString3);
	
	// operator==
	ASSERT_EQUAL(list1, list2);
	
	list2.SetAt(1, testString4);
	ASSERT_NEQUAL(list1, list2);
	
	// operator+(decStringList)
	list1.RemoveAll();
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	list2.RemoveAll();
	list2.Add(testString4);
	list2.Add(testString5);
	
	list3.RemoveAll();
	list3 = list1 + list2;
	ASSERT_EQUAL(list3.GetCount(), 5);
	ASSERT_EQUAL(list3.GetAt(0), testString1);
	ASSERT_EQUAL(list3.GetAt(1), testString2);
	ASSERT_EQUAL(list3.GetAt(2), testString3);
	ASSERT_EQUAL(list3.GetAt(3), testString4);
	ASSERT_EQUAL(list3.GetAt(4), testString5);
	
	// operator+=(decStringList)
	list1.RemoveAll();
	list1.Add(testString1);
	list1.Add(testString2);
	list1.Add(testString3);
	list2.RemoveAll();
	list2.Add(testString4);
	list2.Add(testString5);
	
	list1 += list2;
	ASSERT_EQUAL(list1.GetCount(), 5);
	ASSERT_EQUAL(list1.GetAt(0), testString1);
	ASSERT_EQUAL(list1.GetAt(1), testString2);
	ASSERT_EQUAL(list1.GetAt(2), testString3);
	ASSERT_EQUAL(list1.GetAt(3), testString4);
	ASSERT_EQUAL(list1.GetAt(4), testString5);
}
