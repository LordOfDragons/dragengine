// includes
#include <stdio.h>

#include "detTDictionary.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTDictionary.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>

// Type aliases
typedef decTStringDictionary<int> decTStringIntDict;
typedef decTStringDictionary<decXmlElementTag::Ref, decXmlElementTag*> decTStringXmlTagDict;


// Class detTDictionary
///////////////////////

detTDictionary::detTDictionary(){
	Prepare();
}

detTDictionary::~detTDictionary(){
	CleanUp();
}

void detTDictionary::Prepare(){
}

void detTDictionary::Run(){
	TestStringIntBasic();
	TestStringIntOperators();
	TestStringIntKeysValues();
	TestStringObjectRefBasic();
	TestStringObjectRefOperations();
}

void detTDictionary::CleanUp(){
}

const char *detTDictionary::GetTestName(){
	return "TDictionary";
}


// Tests - String->Int
///////////////////////

void detTDictionary::TestStringIntBasic(){
	SetSubTestNum(0);

	// Constructor tests
	decTStringIntDict dict;
	ASSERT_EQUAL(dict.GetCount(), 0);
	ASSERT_TRUE(dict.IsEmpty());
	ASSERT_FALSE(dict.IsNotEmpty());

	// Constructor with bucket count
	decTStringIntDict dict2(16);
	ASSERT_EQUAL(dict2.GetCount(), 0);
	dict2.SetAt(decString("test"), 42);
	ASSERT_EQUAL(dict2.GetCount(), 1);

	// SetAt and GetAt
	dict.SetAt(decString("a"), 1);
	dict.SetAt(decString("b"), 2);
	dict.SetAt(decString("c"), 3);
	ASSERT_EQUAL(dict.GetCount(), 3);
	ASSERT_FALSE(dict.IsEmpty());
	ASSERT_TRUE(dict.IsNotEmpty());
	ASSERT_TRUE(dict.Has(decString("a")));
	
	// GetAt const version (returns const V&)
	ASSERT_EQUAL(dict.GetAt(decString("a")), 1);
	
	// operator[] const version
	ASSERT_EQUAL(dict[decString("b")], 2);

	// Update existing key
	dict.SetAt(decString("a"), 10);
	ASSERT_EQUAL(dict.GetAt(decString("a")), 10);
	ASSERT_EQUAL(dict.GetCount(), 3); // count unchanged

	// GetAt with output parameter
	int value = 0;
	ASSERT_TRUE(dict.GetAt(decString("b"), &value));
	ASSERT_EQUAL(value, 2);
	ASSERT_FALSE(dict.GetAt(decString("missing"), &value));

	// GetAt non-const version (returns V&)
	decTStringIntDict dict4;
	dict4.SetAt(decString("x"), 100);
	int &ref = dict4.GetAt(decString("x"));
	ref = 200;
	ASSERT_EQUAL(dict4.GetAt(decString("x")), 200);

	// Copy constructor
	decTStringIntDict dict3(dict);
	ASSERT_EQUAL(dict3.GetCount(), 3);
	ASSERT_EQUAL(dict3.GetAt(decString("c")), 3);

	// Equals method
	decTStringIntDict dictA;
	dictA.SetAt(decString("k1"), 1);
	dictA.SetAt(decString("k2"), 2);
	decTStringIntDict dictB;
	dictB.SetAt(decString("k1"), 1);
	dictB.SetAt(decString("k2"), 2);
	ASSERT_TRUE(dictA.Equals(dictB));
	dictB.SetAt(decString("k3"), 3);
	ASSERT_FALSE(dictA.Equals(dictB));
	dictA.SetAt(decString("k2"), 999);
	ASSERT_FALSE(dictA.Equals(dictB)); // different value for same key
}

void detTDictionary::TestStringIntOperators(){
	SetSubTestNum(1);

	decTStringIntDict dict1;
	dict1.SetAt(decString("a"), 1);
	dict1.SetAt(decString("b"), 2);

	decTStringIntDict dict2;
	dict2.SetAt(decString("c"), 3);
	dict2.SetAt(decString("d"), 4);

	// operator+ (merge)
	auto dict3 = dict1 + dict2;
	ASSERT_EQUAL(dict3.GetCount(), 4);
	ASSERT_EQUAL(dict3[decString("a")], 1);
	ASSERT_EQUAL(dict3[decString("d")], 4);

	// operator+= (merge in place)
	dict1 += dict2;
	ASSERT_EQUAL(dict1.GetCount(), 4);
	ASSERT_TRUE(dict1 == dict3);

	// operator== and !=
	decTStringIntDict dict4;
	dict4.SetAt(decString("a"), 1);
	dict4.SetAt(decString("b"), 2);
	dict4.SetAt(decString("c"), 3);
	dict4.SetAt(decString("d"), 4);
	ASSERT_TRUE(dict1 == dict4);
	ASSERT_FALSE(dict1 != dict4);

	dict4.SetAt(decString("e"), 5);
	ASSERT_FALSE(dict1 == dict4);
	ASSERT_TRUE(dict1 != dict4);

	// operator=
	decTStringIntDict dict5;
	dict5 = dict1;
	ASSERT_TRUE(dict5 == dict1);
	ASSERT_EQUAL(dict5.GetCount(), 4);

	// operator[] for modification
	dict5[decString("a")] = 100;
	ASSERT_EQUAL(dict5[decString("a")], 100);
}

void detTDictionary::TestStringIntKeysValues(){
	SetSubTestNum(2);

	decTStringIntDict dict;
	dict.SetAt(decString("key1"), 10);
	dict.SetAt(decString("key2"), 20);
	dict.SetAt(decString("key3"), 30);

	// GetKeys
	auto keys = dict.GetKeys();
	ASSERT_EQUAL(keys.GetCount(), 3);
	ASSERT_TRUE(keys.Has(decString("key1")));
	ASSERT_TRUE(keys.Has(decString("key2")));
	ASSERT_TRUE(keys.Has(decString("key3")));

	// GetValues
	auto values = dict.GetValues();
	ASSERT_EQUAL(values.GetCount(), 3);
	ASSERT_TRUE(values.Has(10));
	ASSERT_TRUE(values.Has(20));
	ASSERT_TRUE(values.Has(30));

	// Remove
	dict.Remove(decString("key2"));
	ASSERT_EQUAL(dict.GetCount(), 2);
	ASSERT_FALSE(dict.Has(decString("key2")));

	// RemoveIfPresent
	ASSERT_TRUE(dict.RemoveIfPresent(decString("key1")));
	ASSERT_FALSE(dict.RemoveIfPresent(decString("key1"))); // already removed
	ASSERT_EQUAL(dict.GetCount(), 1);

	// RemoveAll
	dict.RemoveAll();
	ASSERT_TRUE(dict.IsEmpty());
	ASSERT_EQUAL(dict.GetCount(), 0);

	// CheckLoad (stress test)
	for(int i=0; i<100; i++){
		decString key;
		key.Format("key%d", i);
		dict.SetAt(key, i);
	}
	ASSERT_EQUAL(dict.GetCount(), 100);
	dict.CheckLoad(); // should internally rehash
	ASSERT_EQUAL(dict.GetCount(), 100); // count preserved
}


// Tests - String->ObjectRef
/////////////////////////////

void detTDictionary::TestStringObjectRefBasic(){
	SetSubTestNum(3);

	decTStringXmlTagDict dict;
	decXmlElementTag::Ref tagA = decXmlElementTag::Ref::New("tagA");
	decXmlElementTag::Ref tagB = decXmlElementTag::Ref::New("tagB");
	decXmlElementTag::Ref tagC = decXmlElementTag::Ref::New("tagC");

	// SetAt with pointer input
	dict.SetAt(decString("ka"), tagA);
	dict.SetAt(decString("kb"), tagB);
	dict.SetAt(decString("kc"), tagC);
	ASSERT_EQUAL(dict.GetCount(), 3);
	ASSERT_TRUE(dict.Has(decString("ka")));
	ASSERT_TRUE(dict.GetAt(decString("kb")) == tagB);

	// Update existing key
	dict.SetAt(decString("ka"), tagB);
	ASSERT_TRUE(dict.GetAt(decString("ka")) == tagB);
	ASSERT_EQUAL(dict.GetCount(), 3);

	// Remove
	dict.Remove(decString("ka"));
	ASSERT_FALSE(dict.Has(decString("ka")));
	ASSERT_EQUAL(dict.GetCount(), 2);

	// RemoveIfPresent
	ASSERT_TRUE(dict.RemoveIfPresent(decString("kb")));
	ASSERT_FALSE(dict.RemoveIfPresent(decString("kb")));

	dict.RemoveAll();
	ASSERT_TRUE(dict.IsEmpty());
}

void detTDictionary::TestStringObjectRefOperations(){
	SetSubTestNum(4);

	decTStringXmlTagDict dict1;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	decXmlElementTag::Ref tag3 = decXmlElementTag::Ref::New("tag3");
	decXmlElementTag::Ref tag4 = decXmlElementTag::Ref::New("tag4");

	dict1.SetAt(decString("first"), tag1);
	dict1.SetAt(decString("second"), tag2);

	// Copy constructor
	decTStringXmlTagDict dict2(dict1);
	ASSERT_EQUAL(dict2.GetCount(), 2);
	ASSERT_TRUE(dict2.GetAt(decString("first")) == tag1);

	// operator=
	decTStringXmlTagDict dict3;
	dict3 = dict1;
	ASSERT_TRUE(dict3 == dict1);

	// operator+
	decTStringXmlTagDict dict4;
	dict4.SetAt(decString("third"), tag3);
	dict4.SetAt(decString("fourth"), tag4);
	auto dict5 = dict1 + dict4;
	ASSERT_EQUAL(dict5.GetCount(), 4);
	ASSERT_TRUE(dict5.Has(decString("third")));

	// operator+=
	dict1 += dict4;
	ASSERT_TRUE(dict1 == dict5);

	// GetKeys and GetValues
	auto keys = dict1.GetKeys();
	ASSERT_EQUAL(keys.GetCount(), 4);
	ASSERT_TRUE(keys.Has(decString("first")));

	auto values = dict1.GetValues();
	ASSERT_EQUAL(values.GetCount(), 4);
	ASSERT_TRUE(values.Has(tag3));

	// operator[] access
	ASSERT_TRUE(dict1[decString("second")] == tag2);
	dict1[decString("second")] = tag4;
	ASSERT_TRUE(dict1[decString("second")] == tag4);
}
