#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUnicodeStringList.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/common/exceptions.h>

	

// Class detUnicodeStringList
///////////////////////////////

// Constructors, destructor
/////////////////////////////

detUnicodeStringList::detUnicodeStringList(){
	Prepare();
}

detUnicodeStringList::~detUnicodeStringList(){
	CleanUp();
}



// Testing
////////////

void detUnicodeStringList::Prepare(){
}

void detUnicodeStringList::Run(){
	TestModify();
	TestTesting();
	TestSort();
	TestOperator();
}

void detUnicodeStringList::CleanUp(){
}

const char *detUnicodeStringList::GetTestName(){
	return "UnicodeStringList";
}



// Tests
//////////

const decUnicodeString testString1 = decUnicodeString::NewFromUTF8( "Hal\xE2\x82\xAClo" );
const decUnicodeString testString2 = decUnicodeString::NewFromUTF8( "We\xC3\xA4lt" );
const decUnicodeString testString3 = decUnicodeString::NewFromUTF8( "This is a\xE2\x82\xA test" );
const decUnicodeString testString4 = decUnicodeString::NewFromUTF8( "Some\xC3\xA4thing else" );
const decUnicodeString testString5 = decUnicodeString::NewFromUTF8( "An\xE2\x82\xAother \xC3\xA4test" );

void detUnicodeStringList::TestModify() {
	SetSubTestNum( 0 );
	
	// decUnicodeStringList()
	decUnicodeStringList list1;
	ASSERT_EQUAL( list1.GetCount(), 0 );
	
	// Add
	list1.Add( testString1 );
	ASSERT_EQUAL( list1.GetCount(), 1 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	
	list1.Add( decUnicodeString( testString2 ) );
	ASSERT_EQUAL( list1.GetCount(), 2 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	
	// Insert
	list1.InsertAt( testString3, 0 );
	ASSERT_EQUAL( list1.GetCount(), 3 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString2 );
	
	list1.InsertAt( decUnicodeString( testString4 ), 2 );
	ASSERT_EQUAL( list1.GetCount(), 4 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString2 );
	
	list1.InsertAt( testString5, 4 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	// SetAt
	list1.SetAt( 1, testString5 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	list1.SetAt( 3, testString1 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	list1.SetAt( 1, testString2 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	// Move
	list1.Move( 1, 3 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	list1.Move( 2, 0 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	list1.Move( 1, 4 );
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString3 );
	
	// decUnicodeStringList( const decUnicodeStringList &list )
	decUnicodeStringList list2( list1 );
	ASSERT_EQUAL( list2.GetCount(), 5 );
	ASSERT_EQUAL( list2.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list2.GetAt( 1 ), testString4 );
	ASSERT_EQUAL( list2.GetAt( 2 ), testString2 );
	ASSERT_EQUAL( list2.GetAt( 3 ), testString5 );
	ASSERT_EQUAL( list2.GetAt( 4 ), testString3 );
	
	// RemoveFrom
	list1.RemoveFrom( 1 );
	ASSERT_EQUAL( list1.GetCount(), 4 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString3 );
	
	list1.RemoveFrom( 3 );
	ASSERT_EQUAL( list1.GetCount(), 3 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString5 );
	
	list1.RemoveFrom( 0 );
	ASSERT_EQUAL( list1.GetCount(), 2 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString5 );
	
	list1.RemoveFrom( 0 );
	list1.RemoveFrom( 0 );
	ASSERT_EQUAL( list1.GetCount(), 0 );
	
	// RemoveAll
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	ASSERT_EQUAL( list1.GetCount(), 3 );
	list1.RemoveAll();
	ASSERT_EQUAL( list1.GetCount(), 0 );
}

void detUnicodeStringList::TestTesting(){
	SetSubTestNum( 1 );
	
	// IndexOf
	decUnicodeStringList list1;
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	ASSERT_EQUAL( list1.IndexOf( testString1 ), 0 );
	ASSERT_EQUAL( list1.IndexOf( testString2 ), 1 );
	ASSERT_EQUAL( list1.IndexOf( testString3 ), 2 );
	ASSERT_EQUAL( list1.IndexOf( testString4 ), -1 );
	ASSERT_EQUAL( list1.IndexOf( testString5 ), -1 );
	
	// Has
	ASSERT_TRUE( list1.Has( testString1 ) );
	ASSERT_TRUE( list1.Has( testString2 ) );
	ASSERT_TRUE( list1.Has( testString3 ) );
	ASSERT_FALSE( list1.Has( testString4 ) );
	ASSERT_FALSE( list1.Has( testString5 ) );
}

void detUnicodeStringList::TestSort(){
	SetSubTestNum( 2 );
	
	// SortAscending()
	decUnicodeStringList list1;
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	list1.Add( testString4 );
	list1.Add( testString5 );
	
	list1.SortAscending();
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString2 );
	
	list1.SortAscending();
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString5 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString2 );
	
	// SortDescending()
	list1.RemoveAll();
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	list1.Add( testString4 );
	list1.Add( testString5 );
	
	list1.SortDescending();
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	list1.SortDescending();
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
}

void detUnicodeStringList::TestOperator(){
	SetSubTestNum( 3 );
	
	decUnicodeStringList list1, list2, list3;
	
	// operator[]
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	ASSERT_EQUAL( list1.GetCount(), 3 );
	ASSERT_EQUAL( list1[ 0 ], testString1 );
	ASSERT_EQUAL( list1[ 1 ], testString2 );
	ASSERT_EQUAL( list1[ 2 ], testString3 );
	
	// operator=
	list2 = list1;
	ASSERT_EQUAL( list2.GetCount(), 3 );
	ASSERT_EQUAL( list2[ 0 ], testString1 );
	ASSERT_EQUAL( list2[ 1 ], testString2 );
	ASSERT_EQUAL( list2[ 2 ], testString3 );
	
	// operator==
	ASSERT_EQUAL( list1, list2 );
	
	list2.SetAt( 1, testString4 );
	ASSERT_NEQUAL( list1, list2 );
	
	// operator+(decUnicodeStringList)
	list1.RemoveAll();
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	list2.RemoveAll();
	list2.Add( testString4 );
	list2.Add( testString5 );
	
	list3.RemoveAll();
	list3 = list1 + list2;
	ASSERT_EQUAL( list3.GetCount(), 5 );
	ASSERT_EQUAL( list3.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list3.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list3.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list3.GetAt( 3 ), testString4 );
	ASSERT_EQUAL( list3.GetAt( 4 ), testString5 );
	
	// operator+=(decUnicodeStringList)
	list1.RemoveAll();
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	list2.RemoveAll();
	list2.Add( testString4 );
	list2.Add( testString5 );
	
	list1 += list2;
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
	
	// operator+(decUnicodeString)
	list1.RemoveAll();
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	
	list3.RemoveAll();
	list3 = list1 + decUnicodeString( testString4 );
	ASSERT_EQUAL( list3.GetCount(), 4 );
	ASSERT_EQUAL( list3.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list3.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list3.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list3.GetAt( 3 ), testString4 );
	
	list3.RemoveAll();
	list3 = list1 + testString4;
	ASSERT_EQUAL( list3.GetCount(), 4 );
	ASSERT_EQUAL( list3.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list3.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list3.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list3.GetAt( 3 ), testString4 );
	
	// operator+=(decUnicodeString)
	list1.RemoveAll();
	list1.Add( testString1 );
	list1.Add( testString2 );
	list1.Add( testString3 );
	
	list1 += decUnicodeString( testString4 );
	ASSERT_EQUAL( list1.GetCount(), 4 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString4 );
	
	list1 += testString5;
	ASSERT_EQUAL( list1.GetCount(), 5 );
	ASSERT_EQUAL( list1.GetAt( 0 ), testString1 );
	ASSERT_EQUAL( list1.GetAt( 1 ), testString2 );
	ASSERT_EQUAL( list1.GetAt( 2 ), testString3 );
	ASSERT_EQUAL( list1.GetAt( 3 ), testString4 );
	ASSERT_EQUAL( list1.GetAt( 4 ), testString5 );
}
