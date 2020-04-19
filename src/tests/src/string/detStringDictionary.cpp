#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detStringDictionary.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/exceptions.h>

	

// Class detStringDictionary
//////////////////////////////

// Constructors, destructor
/////////////////////////////

detStringDictionary::detStringDictionary(){
	Prepare();
}

detStringDictionary::~detStringDictionary(){
	CleanUp();
}



// Testing
////////////

void detStringDictionary::Prepare(){
}

void detStringDictionary::Run(){
	TestDictionary();
}

void detStringDictionary::CleanUp(){
}

const char *detStringDictionary::GetTestName(){ return "StringDictionary"; }



// Tests
//////////

void detStringDictionary::TestDictionary(){
	SetSubTestNum( 0 );
	
	const decString key1 = "something";
	const decString key2 = "hello";
	const decString key3 = "oops";
	const decString value1 = "abc";
	const decString value2 = "efg";
	const decString value3 = "hij";
	const decString default1 = "123";
	const decString *result;
	
	decStringDictionary dict1;
	ASSERT_EQUAL( dict1.GetCount(), 0 );
	ASSERT_FALSE( dict1.Has( key1 ) );
	dict1.RemoveIfPresent( key1 );
	
	dict1.SetAt( key1, value1 );
	ASSERT_EQUAL( dict1.GetCount(), 1 );
	ASSERT_TRUE( dict1.Has( key1 ) );
	ASSERT_FALSE( dict1.Has( key2 ) );
	ASSERT_EQUAL( dict1.GetAt( key1 ), value1 );
	ASSERT_TRUE( dict1.GetAt( key1, &result ) );
	ASSERT_EQUAL( *result, value1 );
	ASSERT_FALSE( dict1.GetAt( key2, &result ) );
	
	dict1.SetAt( key2, value2 );
	ASSERT_EQUAL( dict1.GetCount(), 2 );
	ASSERT_TRUE( dict1.Has( key1 ) );
	ASSERT_TRUE( dict1.Has( key2 ) );
	ASSERT_EQUAL( dict1.GetAt( key1 ), value1 );
	ASSERT_EQUAL( dict1.GetAt( key2 ), value2 );
	ASSERT_TRUE( dict1.GetAt( key1, &result ) );
	ASSERT_EQUAL( *result, value1 );
	ASSERT_TRUE( dict1.GetAt( key2, &result ) );
	ASSERT_EQUAL( *result, value2 );
	
	dict1.SetAt( key2, value3 );
	ASSERT_EQUAL( dict1.GetCount(), 2 );
	ASSERT_EQUAL( dict1.GetKeys().GetCount(), 2 );
	ASSERT_EQUAL( dict1.GetValues().GetCount(), 2 );
	
	dict1.SetAt( key2, value2 );
	ASSERT_EQUAL( dict1.GetCount(), 2 );
	ASSERT_EQUAL( dict1.GetKeys().GetCount(), 2 );
	ASSERT_EQUAL( dict1.GetValues().GetCount(), 2 );
	
	decStringDictionary dict2 = decStringDictionary( dict1 );
	ASSERT_EQUAL( dict2.GetAt( key1 ), value1 );
	ASSERT_EQUAL( dict2.GetAt( key2 ), value2 );
	
	SetSubTestNum( 1 );
	dict2.Remove( key1 );
	ASSERT_EQUAL( dict2.GetCount(), 1 );
	ASSERT_FALSE( dict2.Has( key1 ) );
	ASSERT_TRUE( dict2.Has( key2 ) );
	ASSERT_FALSE( dict2.GetAt( key1, &result ) );
	ASSERT_TRUE( dict2.GetAt( key2, &result ) );
	ASSERT_EQUAL( *result, value2 );
	
	dict2 = decStringDictionary( dict1 );
	dict2.RemoveAll();
	ASSERT_EQUAL( dict2.GetCount(), 0 );
	ASSERT_FALSE( dict2.Has( key1 ) );
	
	dict1.SetAt( key3, value3 );
	ASSERT_EQUAL( dict1.GetAt( key1 ), value1 );
	ASSERT_EQUAL( dict1.GetAt( key2 ), value2 );
	ASSERT_EQUAL( dict1.GetAt( key3 ), value3 );
	
	decStringList list = dict1.GetKeys();
	ASSERT_EQUAL( list.GetCount(), 3 );
	ASSERT_TRUE( list.Has( key1 ) );
	ASSERT_TRUE( list.Has( key2 ) );
	ASSERT_TRUE( list.Has( key3 ) );
	
	list = dict1.GetValues();
	ASSERT_EQUAL( list.GetCount(), 3 );
	ASSERT_TRUE( list.Has( value1 ) );
	ASSERT_TRUE( list.Has( value2 ) );
	ASSERT_TRUE( list.Has( value3 ) );
	
	SetSubTestNum( 2 );
	decStringDictionary dict3;
	int i;
	for( i='A'; i<='Z'; i++ ){
		decString key;
		decString value;
		key.AppendCharacter( i );
		value.Format( "%i", i );
		dict3.SetAt( key, value );
	}
	
	ASSERT_EQUAL( dict3.GetCount(), 26 );
	for( i='A'; i<='Z'; i++ ){
		decString key;
		decString value;
		key.AppendCharacter( i );
		value.Format( "%i", i );
		ASSERT_EQUAL( dict3.GetAt( key ), value );
	}
}
