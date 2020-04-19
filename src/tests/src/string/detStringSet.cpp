#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detStringSet.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/exceptions.h>

	

// Class detStringSet
////////////////////////

// Constructors, destructor
/////////////////////////////

detStringSet::detStringSet(){
	Prepare();
}

detStringSet::~detStringSet(){
	CleanUp();
}



// Testing
////////////

void detStringSet::Prepare(){
}

void detStringSet::Run(){
	TestModify();
	TestTesting();
	TestSort();
	TestOperator();
}

void detStringSet::CleanUp(){
}

const char *detStringSet::GetTestName(){
	return "StringSet";
}



// Tests
//////////

const char * const testString1 = "Hallo";
const char * const testString2 = "Welt";
const char * const testString3 = "This is a test";
const char * const testString4 = "Something else";
const char * const testString5 = "Another test";

void detStringSet::TestModify(){
	SetSubTestNum( 0 );
	
	// decStringSet()
	decStringSet set1;
	ASSERT_EQUAL( set1.GetCount(), 0 );
	
	// Add
	set1.Add( testString1 );
	ASSERT_EQUAL( set1.GetCount(), 1 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_NEQUAL( set1.IndexOf( testString1 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString1 ) ), testString1 );
	ASSERT_EQUAL( set1.IndexOf( testString2 ), -1 );
	
	set1.Add( decString( testString2 ) );
	ASSERT_EQUAL( set1.GetCount(), 2 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_NEQUAL( set1.IndexOf( testString1 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString1 ) ), testString1 );
	ASSERT_NEQUAL( set1.IndexOf( testString2 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString2 ) ), testString2 );
	
	set1.Add( testString3 );
	set1.Add( testString4 );
	set1.Add( testString5 );
	
	// decStringSet( const decStringSet &set )
	decStringSet set2( set1 );
	ASSERT_EQUAL( set2.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	ASSERT_NEQUAL( set1.IndexOf( testString1 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString1 ) ), testString1 );
	ASSERT_NEQUAL( set1.IndexOf( testString2 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString2 ) ), testString2 );
	ASSERT_NEQUAL( set1.IndexOf( testString3 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString3 ) ), testString3 );
	ASSERT_NEQUAL( set1.IndexOf( testString4 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString4 ) ), testString4 );
	ASSERT_NEQUAL( set1.IndexOf( testString5 ), -1 );
	ASSERT_EQUAL( set1.GetAt( set1.IndexOf( testString5 ) ), testString5 );
	
	// RemoveFrom
	set1.Remove( testString4 );
	ASSERT_EQUAL( set1.GetCount(), 4 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	
	set1.Remove( decString( testString3 ) );
	ASSERT_EQUAL( set1.GetCount(), 3 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	
	set1.Remove( testString1 );
	ASSERT_EQUAL( set1.GetCount(), 2 );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	
	set1.Remove( testString2 );
	set1.Remove( testString5 );
	ASSERT_EQUAL( set1.GetCount(), 0 );
	
	// RemoveAll
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	ASSERT_EQUAL( set1.GetCount(), 3 );
	set1.RemoveAll();
	ASSERT_EQUAL( set1.GetCount(), 0 );
}

void detStringSet::TestTesting(){
	SetSubTestNum( 1 );
	
	// IndexOf
	decStringSet set1;
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	ASSERT_NEQUAL( set1.IndexOf( testString1 ), -1 );
	ASSERT_NEQUAL( set1.IndexOf( testString2 ), -1 );
	ASSERT_NEQUAL( set1.IndexOf( testString3 ), -1 );
	ASSERT_EQUAL( set1.IndexOf( testString4 ), -1 );
	ASSERT_EQUAL( set1.IndexOf( testString5 ), -1 );
	
	ASSERT_NEQUAL( set1.IndexOf( decString( testString1 ) ), -1 );
	ASSERT_NEQUAL( set1.IndexOf( decString( testString2 ) ), -1 );
	ASSERT_NEQUAL( set1.IndexOf( decString( testString3 ) ), -1 );
	ASSERT_EQUAL( set1.IndexOf( decString( testString4 ) ), -1 );
	ASSERT_EQUAL( set1.IndexOf( decString( testString5 ) ), -1 );
	
	// Has
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_FALSE( set1.Has( testString4 ) );
	ASSERT_FALSE( set1.Has( testString5 ) );
	
	ASSERT_TRUE( set1.Has( decString( testString1 ) ) );
	ASSERT_TRUE( set1.Has( decString( testString2 ) ) );
	ASSERT_TRUE( set1.Has( decString( testString3 ) ) );
	ASSERT_FALSE( set1.Has( decString( testString4 ) ) );
	ASSERT_FALSE( set1.Has( decString( testString5 ) ) );
}

void detStringSet::TestSort(){
	SetSubTestNum( 2 );
	
	// SortAscending()
	decStringSet set1;
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	set1.Add( testString4 );
	set1.Add( testString5 );
	
	set1.SortAscending();
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString5 ) );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	
	set1.SortAscending();
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString5 ) );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	
	// SortDescending()
	set1.RemoveAll();
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	set1.Add( testString4 );
	set1.Add( testString5 );
	
	set1.SortDescending();
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	
	set1.SortDescending();
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
}

void detStringSet::TestOperator(){
	SetSubTestNum( 3 );
	
	decStringSet set1, set2, set3;
	
	// operator[]
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	ASSERT_EQUAL( set1.GetCount(), 3 );
	ASSERT_EQUAL( set1[ set1.IndexOf( testString1 ) ], testString1 );
	ASSERT_EQUAL( set1[ set1.IndexOf( testString2 ) ], testString2 );
	ASSERT_EQUAL( set1[ set1.IndexOf( testString3 ) ], testString3 );
	
	// operator=
	set2 = set1;
	ASSERT_EQUAL( set2.GetCount(), 3 );
	ASSERT_EQUAL( set2[ set1.IndexOf( testString1 ) ], testString1 );
	ASSERT_EQUAL( set2[ set1.IndexOf( testString2 ) ], testString2 );
	ASSERT_EQUAL( set2[ set1.IndexOf( testString3 ) ], testString3 );
	
	// operator==
	ASSERT_EQUAL( set1, set2 );
	
	// operator+(decStringSet)
	set1.RemoveAll();
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	set2.RemoveAll();
	set2.Add( testString4 );
	set2.Add( testString5 );
	
	set3.RemoveAll();
	set3 = set1 + set2;
	ASSERT_EQUAL( set3.GetCount(), 5 );
	ASSERT_TRUE( set3.Has( testString1 ) );
	ASSERT_TRUE( set3.Has( testString2 ) );
	ASSERT_TRUE( set3.Has( testString3 ) );
	ASSERT_TRUE( set3.Has( testString4 ) );
	ASSERT_TRUE( set3.Has( testString5 ) );
	
	// operator+=(decStringSet)
	set1.RemoveAll();
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	set2.RemoveAll();
	set2.Add( testString4 );
	set2.Add( testString5 );
	
	set1 += set2;
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
	
	// operator+(decString)
	set1.RemoveAll();
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	
	set3.RemoveAll();
	set3 = set1 + decString( testString4 );
	ASSERT_EQUAL( set3.GetCount(), 4 );
	ASSERT_TRUE( set3.Has( testString1 ) );
	ASSERT_TRUE( set3.Has( testString2 ) );
	ASSERT_TRUE( set3.Has( testString3 ) );
	ASSERT_TRUE( set3.Has( testString4 ) );
	
	set3.RemoveAll();
	set3 = set1 + testString4;
	ASSERT_EQUAL( set3.GetCount(), 4 );
	ASSERT_TRUE( set3.Has( testString1 ) );
	ASSERT_TRUE( set3.Has( testString2 ) );
	ASSERT_TRUE( set3.Has( testString3 ) );
	ASSERT_TRUE( set3.Has( testString4 ) );
	
	// operator+=(decString)
	set1.RemoveAll();
	set1.Add( testString1 );
	set1.Add( testString2 );
	set1.Add( testString3 );
	
	set1 += decString( testString4 );
	ASSERT_EQUAL( set1.GetCount(), 4 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	
	set1 += testString5;
	ASSERT_EQUAL( set1.GetCount(), 5 );
	ASSERT_TRUE( set1.Has( testString1 ) );
	ASSERT_TRUE( set1.Has( testString2 ) );
	ASSERT_TRUE( set1.Has( testString3 ) );
	ASSERT_TRUE( set1.Has( testString4 ) );
	ASSERT_TRUE( set1.Has( testString5 ) );
}
