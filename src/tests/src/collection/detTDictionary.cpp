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
	TestGetAtOrDefault();
	TestHasMatching();
	TestVisit();
	TestFind();
	TestFindOrDefault();
	TestCollect();
	TestFold();
	TestInject();
	TestRemoveIf();
	TestConstIterator();
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
	const int *value = 0;
	ASSERT_TRUE(dict.GetAt(decString("b"), value));
	ASSERT_NOT_NULL(value);
	ASSERT_EQUAL(*value, 2);
	ASSERT_FALSE(dict.GetAt(decString("missing"), value));

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


// Tests for new functions in commit 1d5552384445f75f7cf67ed21a59f55c7127cae5
/////////////////////////////////////////////////////////////////////////

void detTDictionary::TestGetAtOrDefault(){
	SetSubTestNum(5);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);

	// GetAtOrDefault with existing key
	ASSERT_EQUAL(dict.GetAtOrDefault(decString("a"), 999), 10);
	ASSERT_EQUAL(dict.GetAtOrDefault(decString("b"), -1), 20);

	// GetAtOrDefault with missing key (returns default)
	ASSERT_EQUAL(dict.GetAtOrDefault(decString("missing"), 999), 999);
	ASSERT_EQUAL(dict.GetAtOrDefault(decString("x"), 0), 0);

	// Verify dictionary not modified
	ASSERT_EQUAL(dict.GetCount(), 3);
	ASSERT_FALSE(dict.Has(decString("missing")));

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref defaultTag = decXmlElementTag::Ref::New("default");
	dictObj.SetAt(decString("key1"), tag1);

	// GetAtOrDefault with pointer types (VP) for ObjectRef
	ASSERT_TRUE(dictObj.GetAtOrDefault(decString("key1"), defaultTag) == tag1);
	ASSERT_TRUE(dictObj.GetAtOrDefault(decString("missing"), defaultTag) == defaultTag);
}

void detTDictionary::TestVisit(){
	SetSubTestNum(6);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 1);
	dict.SetAt(decString("b"), 2);
	dict.SetAt(decString("c"), 3);

	// Test Visit with lambda (lvalue reference)
	int sum = 0;
	auto visitor = [&sum](const decString &key, const int &value){
		sum += value;
	};
	dict.Visit(visitor);
	ASSERT_EQUAL(sum, 6);

	// Test Visit with rvalue lambda
	int count = 0;
	dict.Visit([&count](const decString &key, const int &value){
		count++;
	});
	ASSERT_EQUAL(count, 3);

	// Test Visit with key checking
	bool hasKeyB = false;
	dict.Visit([&hasKeyB](const decString &key, const int &value){
		if(key == "b"){
			hasKeyB = true;
		}
	});
	ASSERT_TRUE(hasKeyB);

	// Test Visit with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);

	int objCount = 0;
	dictObj.Visit([&objCount](const decString &key, const decXmlElementTag::Ref &value){
		objCount++;
	});
	ASSERT_EQUAL(objCount, 2);
}

void detTDictionary::TestFind(){
	SetSubTestNum(7);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);

	// Find existing element (lvalue evaluator)
	const int *found = nullptr;
	auto evaluator = [](const decString &key, const int &value) -> bool {
		return value == 20;
	};
	ASSERT_TRUE(dict.Find(evaluator, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(*found, 20);

	// Find with rvalue evaluator
	const int *found2 = nullptr;
	ASSERT_TRUE(dict.Find([](const decString &key, const int &value) -> bool {
		return key == "c";
	}, found2));
	ASSERT_NOT_NULL(found2);
	ASSERT_EQUAL(*found2, 30);

	// Find non-existent element
	const int *notFound = nullptr;
	ASSERT_FALSE(dict.Find([](const decString &key, const int &value) -> bool {
		return value == 999;
	}, notFound));
	ASSERT_TRUE(notFound == nullptr);

	// Find with key pattern
	const int *foundA = nullptr;
	ASSERT_TRUE(dict.Find([](const decString &key, const int &value) -> bool {
		return key.BeginsWith("a");
	}, foundA));
	ASSERT_EQUAL(*foundA, 10);

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);

	const decXmlElementTag::Ref *foundTag = nullptr;
	ASSERT_TRUE(dictObj.Find([](const decString &key, const decXmlElementTag::Ref &value) -> bool {
		return value->GetName() == "tag2";
	}, foundTag));
	ASSERT_TRUE(*foundTag == tag2);
}

void detTDictionary::TestFindOrDefault(){
	SetSubTestNum(8);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);

	// FindOrDefault with existing element (lvalue evaluator)
	auto evaluator = [](const decString &key, const int &value) -> bool {
		return value == 20;
	};
	ASSERT_EQUAL(dict.FindOrDefault(evaluator, 999), 20);

	// FindOrDefault with rvalue evaluator
	ASSERT_EQUAL(dict.FindOrDefault([](const decString &key, const int &value) -> bool {
		return key == "a";
	}, -1), 10);

	// FindOrDefault with non-existent element (returns default)
	ASSERT_EQUAL(dict.FindOrDefault([](const decString &key, const int &value) -> bool {
		return value == 999;
	}, 777), 777);

	// FindOrDefault with complex condition
	ASSERT_EQUAL(dict.FindOrDefault([](const decString &key, const int &value) -> bool {
		return value > 25;
	}, 0), 30);

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	decXmlElementTag::Ref defaultTag = decXmlElementTag::Ref::New("default");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);

	auto result = dictObj.FindOrDefault([](const decString &key, const decXmlElementTag::Ref &value) -> bool {
		return value->GetName() == "tag1";
	}, defaultTag);
	ASSERT_TRUE(result == tag1);

	auto notFound = dictObj.FindOrDefault([](const decString &key, const decXmlElementTag::Ref &value) -> bool {
		return value->GetName() == "missing";
	}, defaultTag);
	ASSERT_TRUE(notFound == defaultTag);
}

void detTDictionary::TestCollect(){
	SetSubTestNum(9);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);
	dict.SetAt(decString("d"), 40);

	// Collect with lvalue evaluator (filter values >= 20)
	auto evaluator = [](const decString &key, const int &value) -> bool {
		return value >= 20;
	};
	auto collected = dict.Collect(evaluator);
	ASSERT_EQUAL(collected.GetCount(), 3);
	ASSERT_TRUE(collected.Has(decString("b")));
	ASSERT_TRUE(collected.Has(decString("c")));
	ASSERT_TRUE(collected.Has(decString("d")));
	ASSERT_FALSE(collected.Has(decString("a")));
	ASSERT_EQUAL(collected.GetAt(decString("b")), 20);

	// Collect with rvalue evaluator (filter by key pattern)
	auto collected2 = dict.Collect([](const decString &key, const int &value) -> bool {
		return key.BeginsWith("c") || key.BeginsWith("d");
	});
	ASSERT_EQUAL(collected2.GetCount(), 2);
	ASSERT_TRUE(collected2.Has(decString("c")));
	ASSERT_TRUE(collected2.Has(decString("d")));

	// Collect with no matches
	auto empty = dict.Collect([](const decString &key, const int &value) -> bool {
		return value > 100;
	});
	ASSERT_TRUE(empty.IsEmpty());

	// Collect all elements
	auto all = dict.Collect([](const decString &key, const int &value) -> bool {
		return true;
	});
	ASSERT_EQUAL(all.GetCount(), 4);
	ASSERT_TRUE(all == dict);

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	decXmlElementTag::Ref tag3 = decXmlElementTag::Ref::New("tag3");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);
	dictObj.SetAt(decString("k3"), tag3);

	auto collectedObj = dictObj.Collect([](const decString &key, const decXmlElementTag::Ref &value) -> bool {
		return value->GetName() == "tag2";
	});
	ASSERT_EQUAL(collectedObj.GetCount(), 1);
	ASSERT_TRUE(collectedObj.GetAt(decString("k2")) == tag2);
}

void detTDictionary::TestRemoveIf(){
	SetSubTestNum(10);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);
	dict.SetAt(decString("d"), 40);

	// RemoveIf with lvalue evaluator (remove values < 25)
	auto evaluator = [](const decString &key, const int &value) -> bool {
		return value < 25;
	};
	dict.RemoveIf(evaluator);
	ASSERT_EQUAL(dict.GetCount(), 2);
	ASSERT_FALSE(dict.Has(decString("a")));
	ASSERT_FALSE(dict.Has(decString("b")));
	ASSERT_TRUE(dict.Has(decString("c")));
	ASSERT_TRUE(dict.Has(decString("d")));

	// RemoveIf with rvalue evaluator (remove specific key)
	dict.RemoveIf([](const decString &key, const int &value) -> bool {
		return key == "c";
	});
	ASSERT_EQUAL(dict.GetCount(), 1);
	ASSERT_FALSE(dict.Has(decString("c")));
	ASSERT_TRUE(dict.Has(decString("d")));

	// RemoveIf with no matches
	dict.SetAt(decString("e"), 50);
	dict.RemoveIf([](const decString &key, const int &value) -> bool {
		return value > 100;
	});
	ASSERT_EQUAL(dict.GetCount(), 2);

	// RemoveIf all elements
	dict.RemoveIf([](const decString &key, const int &value) -> bool {
		return true;
	});
	ASSERT_TRUE(dict.IsEmpty());

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	decXmlElementTag::Ref tag3 = decXmlElementTag::Ref::New("tag3");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);
	dictObj.SetAt(decString("k3"), tag3);

	dictObj.RemoveIf([](const decString &key, const decXmlElementTag::Ref &value) -> bool {
		return value->GetName() != "tag2";
	});
	ASSERT_EQUAL(dictObj.GetCount(), 1);
	ASSERT_TRUE(dictObj.GetAt(decString("k2")) == tag2);
}

void detTDictionary::TestConstIterator(){
	SetSubTestNum(11);

	decTStringIntDict dict;
	dict.SetAt(decString("a"), 10);
	dict.SetAt(decString("b"), 20);
	dict.SetAt(decString("c"), 30);

	// Test cbegin/cend iteration
	int sum = 0;
	int count = 0;
	for(auto it = dict.cbegin(); it != dict.cend(); ++it){
		const auto &pair = *it;
		sum += pair.second;
		count++;
	}
	ASSERT_EQUAL(count, 3);
	ASSERT_EQUAL(sum, 60);

	// Test begin/end iteration (should also be const for const dict)
	const decTStringIntDict &constDict = dict;
	int sum2 = 0;
	for(auto it = constDict.begin(); it != constDict.end(); ++it){
		const auto &pair = *it;
		sum2 += pair.second;
	}
	ASSERT_EQUAL(sum2, 60);

	// Test iterator dereferencing to std::pair<const K&, const V&>
	auto it = dict.cbegin();
	const auto &pair = *it;
	// pair.first is const decString&, pair.second is const int&
	ASSERT_TRUE(dict.Has(pair.first));
	ASSERT_EQUAL(dict.GetAt(pair.first), pair.second);

	// Test iterator dereferencing again
	auto it2 = dict.cbegin();
	const auto &pair2 = *it2;
	ASSERT_TRUE(dict.Has(pair2.first));

	// Test post-increment
	auto it3 = dict.cbegin();
	auto it4 = it3++;
	ASSERT_TRUE(it3 != it4);
	ASSERT_TRUE(it4 == dict.cbegin());

	// Test iterator equality
	auto it5 = dict.cbegin();
	auto it6 = dict.cbegin();
	ASSERT_TRUE(it5 == it6);
	++it6;
	ASSERT_FALSE(it5 == it6);
	ASSERT_TRUE(it5 != it6);

	// Test end iterator
	auto itEnd = dict.cend();
	auto itEnd2 = dict.cend();
	ASSERT_TRUE(itEnd == itEnd2);

	// Test empty dictionary iteration
	decTStringIntDict emptyDict;
	ASSERT_TRUE(emptyDict.cbegin() == emptyDict.cend());
	int emptyCount = 0;
	for(auto it7 = emptyDict.cbegin(); it7 != emptyDict.cend(); ++it7){
		emptyCount++;
	}
	ASSERT_EQUAL(emptyCount, 0);

	// Test with ObjectRef values
	decTStringXmlTagDict dictObj;
	decXmlElementTag::Ref tag1 = decXmlElementTag::Ref::New("tag1");
	decXmlElementTag::Ref tag2 = decXmlElementTag::Ref::New("tag2");
	dictObj.SetAt(decString("k1"), tag1);
	dictObj.SetAt(decString("k2"), tag2);

	int objCount = 0;
	for(auto it7 = dictObj.cbegin(); it7 != dictObj.cend(); ++it7){
		const auto &pair3 = *it7;
		objCount++;
		ASSERT_NOT_NULL(pair3.second);
	}
	ASSERT_EQUAL(objCount, 2);
}


// ============================================================================
// New Function Tests
// ============================================================================

void detTDictionary::TestHasMatching(){
	SetSubTestNum(8);

	decTStringIntDict dict;
	dict.SetAt("a", 10);
	dict.SetAt("b", 20);
	dict.SetAt("c", 30);
	
	auto evaluator1 = [](const decString &key, int val){ return val == 20; };
	ASSERT_TRUE(dict.HasMatching(evaluator1));
	
	auto evaluator2 = [](const decString &key, int val){ return val > 50; };
	ASSERT_FALSE(dict.HasMatching(evaluator2));
	
	auto evaluator3 = [](const decString &key, int val){ return key == "c"; };
	ASSERT_TRUE(dict.HasMatching(evaluator3));
}

void detTDictionary::TestFold(){
	SetSubTestNum(13);

	decTStringIntDict dict;
	dict.SetAt("a", 10);
	dict.SetAt("b", 20);
	dict.SetAt("c", 30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(dict.Fold(combiner), 60);
	
	decTStringIntDict emptyDict;
	ASSERT_DOES_FAIL(emptyDict.Fold(combiner));
	
	decTStringIntDict dict2;
	dict2.SetAt("x", 2);
	dict2.SetAt("y", 3);
	dict2.SetAt("z", 4);
	auto multiplier = [](int acc, int val){ return acc * val; };
	ASSERT_EQUAL(dict2.Fold(multiplier), 24);
}

void detTDictionary::TestInject(){
	SetSubTestNum(14);

	decTStringIntDict dict;
	dict.SetAt("a", 10);
	dict.SetAt("b", 20);
	dict.SetAt("c", 30);
	
	auto combiner = [](int acc, int val){ return acc + val; };
	ASSERT_EQUAL(dict.Inject(100, combiner), 160);
	
	decTStringIntDict emptyDict;
	ASSERT_EQUAL(emptyDict.Inject(100, combiner), 100);
	
	// Test with different return type
	auto stringCombiner = [](decString acc, int val){ 
		decString str;
		str.Format("%d,", val);
		return acc + str; 
	};
	decString result = dict.Inject(decString("values:"), stringCombiner);
	ASSERT_TRUE(result.BeginsWith("values:"));
	ASSERT_TRUE(result.Find("10,") >= 0);
	ASSERT_TRUE(result.Find("20,") >= 0);
	ASSERT_TRUE(result.Find("30,") >= 0);
}
