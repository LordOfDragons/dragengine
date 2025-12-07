#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "detUnicodeStringDictionary.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/common/string/unicode/decUnicodeStringDictionary.h>
#include <dragengine/common/exceptions.h>

	

// Class detUnicodeStringDictionary
/////////////////////////////////////

// Constructors, destructor
/////////////////////////////

detUnicodeStringDictionary::detUnicodeStringDictionary(){
	Prepare();
}

detUnicodeStringDictionary::~detUnicodeStringDictionary(){
	CleanUp();
}



// Testing
////////////

void detUnicodeStringDictionary::Prepare(){
}

void detUnicodeStringDictionary::Run(){
	TestDictionary();
}

void detUnicodeStringDictionary::CleanUp(){
}

const char *detUnicodeStringDictionary::GetTestName(){
	return "UnicodeStringDictionary";
}



// Tests
//////////

const decUnicodeString key1 = decUnicodeString::NewFromUTF8("Hal\xE2\x82\xAClo");
const decUnicodeString key2 = decUnicodeString::NewFromUTF8("We\xC3\xA4lt");
const decUnicodeString key3 = decUnicodeString::NewFromUTF8("This is a\xE2\x82\xA test");
const decUnicodeString value1 = decUnicodeString::NewFromUTF8("Some\xC3\xA4thing else");
const decUnicodeString value2 = decUnicodeString::NewFromUTF8("An\xE2\x82\xAother \xC3\xA4test");
const decUnicodeString value3 = decUnicodeString::NewFromUTF8("Su\xE2\x82\xAper");
const decUnicodeString default1 = decUnicodeString::NewFromUTF8("12\xC3\xA4g45");

void detUnicodeStringDictionary::TestDictionary(){
	SetSubTestNum(0);
	
	decUnicodeString *result;
	
	decUnicodeStringDictionary dict1;
	ASSERT_EQUAL(dict1.GetCount(), 0);
	ASSERT_FALSE(dict1.Has(key1));
	dict1.RemoveIfPresent(key1);
	
	dict1.SetAt(key1, value1);
	ASSERT_EQUAL(dict1.GetCount(), 1);
	ASSERT_TRUE(dict1.Has(key1));
	ASSERT_FALSE(dict1.Has(key2));
	ASSERT_EQUAL(dict1.GetAt(key1), value1);
	ASSERT_TRUE(dict1.GetAt(key1, &result));
	ASSERT_EQUAL(*result, value1);
	ASSERT_FALSE(dict1.GetAt(key2, &result));
	
	dict1.SetAt(key2, value2);
	ASSERT_EQUAL(dict1.GetCount(), 2);
	ASSERT_TRUE(dict1.Has(key1));
	ASSERT_TRUE(dict1.Has(key2));
	ASSERT_EQUAL(dict1.GetAt(key1), value1);
	ASSERT_EQUAL(dict1.GetAt(key2), value2);
	ASSERT_TRUE(dict1.GetAt(key1, &result));
	ASSERT_EQUAL(*result, value1);
	ASSERT_TRUE(dict1.GetAt(key2, &result));
	ASSERT_EQUAL(*result, value2);
	
	decUnicodeStringDictionary dict2 = decUnicodeStringDictionary(dict1);
	ASSERT_EQUAL(dict2.GetAt(key1), value1);
	ASSERT_EQUAL(dict2.GetAt(key2), value2);
	
	SetSubTestNum(1);
	dict2.Remove(key1);
	ASSERT_EQUAL(dict2.GetCount(), 1);
	ASSERT_FALSE(dict2.Has(key1));
	ASSERT_TRUE(dict2.Has(key2));
	ASSERT_FALSE(dict2.GetAt(key1, &result));
	ASSERT_TRUE(dict2.GetAt(key2, &result));
	ASSERT_EQUAL(*result, value2);
	
	dict2 = decUnicodeStringDictionary(dict1);
	dict2.RemoveAll();
	ASSERT_EQUAL(dict2.GetCount(), 0);
	ASSERT_FALSE(dict2.Has(key1));
	
	dict1.SetAt(key3, value3);
	ASSERT_EQUAL(dict1.GetAt(key1), value1);
	ASSERT_EQUAL(dict1.GetAt(key2), value2);
	ASSERT_EQUAL(dict1.GetAt(key3), value3);
	
	decUnicodeStringList list = dict1.GetKeys();
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_TRUE(list.Has(key1));
	ASSERT_TRUE(list.Has(key2));
	ASSERT_TRUE(list.Has(key3));
	
	list = dict1.GetValues();
	ASSERT_TRUE(list.Has(value1));
	ASSERT_TRUE(list.Has(value2));
	ASSERT_TRUE(list.Has(value3));
	
	SetSubTestNum(2);
	decUnicodeStringDictionary dict3;
	int i;
	for(i='A'; i<='Z'; i++){
		decUnicodeString key;
		decUnicodeString value;
		key.AppendCharacter(i);
		value.Format("%i", i);
		dict3.SetAt(key, value);
	}
	
	ASSERT_EQUAL(dict3.GetCount(), 26);
	for(i='A'; i<='Z'; i++){
		decUnicodeString key;
		decUnicodeString value;
		key.AppendCharacter(i);
		value.Format("%i", i);
		ASSERT_EQUAL(dict3.GetAt(key), value);
	}
}
