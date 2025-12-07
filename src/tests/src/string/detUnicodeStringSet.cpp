#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUnicodeStringSet.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringSet.h>
#include <dragengine/common/exceptions.h>

	

// Class detUnicodeStringSet
//////////////////////////////

// Constructors, destructor
/////////////////////////////

detUnicodeStringSet::detUnicodeStringSet(){
	Prepare();
}

detUnicodeStringSet::~detUnicodeStringSet(){
	CleanUp();
}



// Testing
////////////

void detUnicodeStringSet::Prepare(){
}

void detUnicodeStringSet::Run(){
	TestModify();
	TestTesting();
	TestSort();
	TestOperator();
}

void detUnicodeStringSet::CleanUp(){
}

const char *detUnicodeStringSet::GetTestName(){
	return "UnicodeStringSet";
}



// Tests
//////////

const decUnicodeString testString1 = decUnicodeString::NewFromUTF8("Hal\xE2\x82\xAClo");
const decUnicodeString testString2 = decUnicodeString::NewFromUTF8("We\xC3\xA4lt");
const decUnicodeString testString3 = decUnicodeString::NewFromUTF8("This is a\xE2\x82\xA test");
const decUnicodeString testString4 = decUnicodeString::NewFromUTF8("Some\xC3\xA4thing else");
const decUnicodeString testString5 = decUnicodeString::NewFromUTF8("An\xE2\x82\xAother \xC3\xA4test");

void detUnicodeStringSet::TestModify(){
	SetSubTestNum(0);
	
	// decUnicodeStringSet()
	decUnicodeStringSet set1;
	ASSERT_EQUAL(set1.GetCount(), 0);
	
	// Add
	set1.Add(testString1);
	ASSERT_EQUAL(set1.GetCount(), 1);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_NEQUAL(set1.IndexOf(testString1), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString1)), testString1);
	ASSERT_EQUAL(set1.IndexOf(testString2), -1);
	
	set1.Add(testString2);
	ASSERT_EQUAL(set1.GetCount(), 2);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_NEQUAL(set1.IndexOf(testString1), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString1)), testString1);
	ASSERT_NEQUAL(set1.IndexOf(testString2), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString2)), testString2);
	
	set1.Add(testString3);
	set1.Add(testString4);
	set1.Add(testString5);
	
	// decUnicodeStringSet( const decUnicodeStringSet &set )
	decUnicodeStringSet set2(set1);
	ASSERT_EQUAL(set2.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString5));
	ASSERT_NEQUAL(set1.IndexOf(testString1), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString1)), testString1);
	ASSERT_NEQUAL(set1.IndexOf(testString2), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString2)), testString2);
	ASSERT_NEQUAL(set1.IndexOf(testString3), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString3)), testString3);
	ASSERT_NEQUAL(set1.IndexOf(testString4), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString4)), testString4);
	ASSERT_NEQUAL(set1.IndexOf(testString5), -1);
	ASSERT_EQUAL(set1.GetAt(set1.IndexOf(testString5)), testString5);
	
	// RemoveFrom
	set1.Remove(testString4);
	ASSERT_EQUAL(set1.GetCount(), 4);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString5));
	ASSERT_TRUE(set1.Has(testString3));
	
	set1.Remove(testString3);
	ASSERT_EQUAL(set1.GetCount(), 3);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString5));
	
	set1.Remove(testString1);
	ASSERT_EQUAL(set1.GetCount(), 2);
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString5));
	
	set1.Remove(testString2);
	set1.Remove(testString5);
	ASSERT_EQUAL(set1.GetCount(), 0);
	
	// RemoveAll
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	ASSERT_EQUAL(set1.GetCount(), 3);
	set1.RemoveAll();
	ASSERT_EQUAL(set1.GetCount(), 0);
}

void detUnicodeStringSet::TestTesting(){
	SetSubTestNum(1);
	
	// IndexOf
	decUnicodeStringSet set1;
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	ASSERT_NEQUAL(set1.IndexOf(testString1), -1);
	ASSERT_NEQUAL(set1.IndexOf(testString2), -1);
	ASSERT_NEQUAL(set1.IndexOf(testString3), -1);
	ASSERT_EQUAL(set1.IndexOf(testString4), -1);
	ASSERT_EQUAL(set1.IndexOf(testString5), -1);
	
	ASSERT_NEQUAL(set1.IndexOf(testString1), -1);
	ASSERT_NEQUAL(set1.IndexOf(testString2), -1);
	ASSERT_NEQUAL(set1.IndexOf(testString3), -1);
	ASSERT_EQUAL(set1.IndexOf(testString4), -1);
	ASSERT_EQUAL(set1.IndexOf(testString5), -1);
	
	// Has
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_FALSE(set1.Has(testString4));
	ASSERT_FALSE(set1.Has(testString5));
	
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_FALSE(set1.Has(testString4));
	ASSERT_FALSE(set1.Has(testString5));
}

void detUnicodeStringSet::TestSort(){
	SetSubTestNum(2);
	
	// SortAscending()
	decUnicodeStringSet set1;
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	set1.Add(testString4);
	set1.Add(testString5);
	
	set1.SortAscending();
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString5));
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString2));
	
	set1.SortAscending();
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString5));
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString2));
	
	// SortDescending()
	set1.RemoveAll();
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	set1.Add(testString4);
	set1.Add(testString5);
	
	set1.SortDescending();
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString5));
	
	set1.SortDescending();
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString5));
}

void detUnicodeStringSet::TestOperator(){
	SetSubTestNum(3);
	
	decUnicodeStringSet set1, set2, set3;
	
	// operator[]
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	ASSERT_EQUAL(set1.GetCount(), 3);
	ASSERT_EQUAL(set1[set1.IndexOf(testString1)], testString1);
	ASSERT_EQUAL(set1[set1.IndexOf(testString2)], testString2);
	ASSERT_EQUAL(set1[set1.IndexOf(testString3)], testString3);
	
	// operator=
	set2 = set1;
	ASSERT_EQUAL(set2.GetCount(), 3);
	ASSERT_EQUAL(set2[set1.IndexOf(testString1)], testString1);
	ASSERT_EQUAL(set2[set1.IndexOf(testString2)], testString2);
	ASSERT_EQUAL(set2[set1.IndexOf(testString3)], testString3);
	
	// operator==
	ASSERT_EQUAL(set1, set2);
	
	// operator+(decUnicodeStringSet)
	set1.RemoveAll();
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	set2.RemoveAll();
	set2.Add(testString4);
	set2.Add(testString5);
	
	set3.RemoveAll();
	set3 = set1 + set2;
	ASSERT_EQUAL(set3.GetCount(), 5);
	ASSERT_TRUE(set3.Has(testString1));
	ASSERT_TRUE(set3.Has(testString2));
	ASSERT_TRUE(set3.Has(testString3));
	ASSERT_TRUE(set3.Has(testString4));
	ASSERT_TRUE(set3.Has(testString5));
	
	// operator+=(decUnicodeStringSet)
	set1.RemoveAll();
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	set2.RemoveAll();
	set2.Add(testString4);
	set2.Add(testString5);
	
	set1 += set2;
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString5));
	
	// operator+(decUnicodeString)
	set1.RemoveAll();
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	
	set3.RemoveAll();
	set3 = set1 + testString4;
	ASSERT_EQUAL(set3.GetCount(), 4);
	ASSERT_TRUE(set3.Has(testString1));
	ASSERT_TRUE(set3.Has(testString2));
	ASSERT_TRUE(set3.Has(testString3));
	ASSERT_TRUE(set3.Has(testString4));
	
	set3.RemoveAll();
	set3 = set1 + testString4;
	ASSERT_EQUAL(set3.GetCount(), 4);
	ASSERT_TRUE(set3.Has(testString1));
	ASSERT_TRUE(set3.Has(testString2));
	ASSERT_TRUE(set3.Has(testString3));
	ASSERT_TRUE(set3.Has(testString4));
	
	// operator+=(decUnicodeString)
	set1.RemoveAll();
	set1.Add(testString1);
	set1.Add(testString2);
	set1.Add(testString3);
	
	set1 += testString4;
	ASSERT_EQUAL(set1.GetCount(), 4);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	
	set1 += testString5;
	ASSERT_EQUAL(set1.GetCount(), 5);
	ASSERT_TRUE(set1.Has(testString1));
	ASSERT_TRUE(set1.Has(testString2));
	ASSERT_TRUE(set1.Has(testString3));
	ASSERT_TRUE(set1.Has(testString4));
	ASSERT_TRUE(set1.Has(testString5));
}
