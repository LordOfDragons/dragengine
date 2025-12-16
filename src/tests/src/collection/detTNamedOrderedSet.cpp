// includes
#include <stdio.h>

#include "detTNamedOrderedSet.h"

#include <dragengine/common/collection/decTNamedOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deObject.h>

// Test helper class with GetName()
class TestNamedObject : public deObject {
public:
	typedef deTObjectReference<TestNamedObject> Ref;
	
	decString name;
	int id;
	
	explicit TestNamedObject(const char *aname, int aid = 0) : name(aname), id(aid) {}
	
	const decString &GetName() const { return name; }
	
protected:
	~TestNamedObject() override = default;
};

typedef decTNamedObjectOrderedSet<TestNamedObject> TestNamedSet;


// Class detTNamedOrderedSet
////////////////////////////

detTNamedOrderedSet::detTNamedOrderedSet(){
	Prepare();
}

detTNamedOrderedSet::~detTNamedOrderedSet(){
	CleanUp();
}

void detTNamedOrderedSet::Prepare(){
}

void detTNamedOrderedSet::Run(){
	TestGetNamed();
	TestIndexOfNamed();
	TestHasNamed();
	TestSetAt();
	TestAdd();
	TestAddIfAbsent();
	TestInsert();
}

void detTNamedOrderedSet::CleanUp(){
}

const char *detTNamedOrderedSet::GetTestName(){
	return "TNamedOrderedSet";
}


// Tests
/////////

void detTNamedOrderedSet::TestGetNamed(){
	SetSubTestNum(0);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("alpha", 1);
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("beta", 2);
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("gamma", 3);
	
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	// GetNamed - found
	const TestNamedObject *found = set.GetNamed("beta");
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(found->name, decString("beta"));
	ASSERT_EQUAL(found->id, 2);
	
	// GetNamed - not found (returns null reference)
	ASSERT_NULL(set.GetNamed("delta"));
}

void detTNamedOrderedSet::TestIndexOfNamed(){
	SetSubTestNum(1);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("first", 1);
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("second", 2);
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("third", 3);
	
	set.Add(obj1);
	set.Add(obj2);
	set.Add(obj3);
	
	// IndexOfNamed - found
	ASSERT_EQUAL(set.IndexOfNamed("first"), 0);
	ASSERT_EQUAL(set.IndexOfNamed("second"), 1);
	ASSERT_EQUAL(set.IndexOfNamed("third"), 2);
	
	// IndexOfNamed - not found
	ASSERT_EQUAL(set.IndexOfNamed("fourth"), -1);
	ASSERT_EQUAL(set.IndexOfNamed(""), -1);
}

void detTNamedOrderedSet::TestHasNamed(){
	SetSubTestNum(2);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("item1");
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("item2");
	
	set.Add(obj1);
	set.Add(obj2);
	
	// HasNamed - present
	ASSERT_TRUE(set.HasNamed("item1"));
	ASSERT_TRUE(set.HasNamed("item2"));
	
	// HasNamed - absent
	ASSERT_FALSE(set.HasNamed("item3"));
	ASSERT_FALSE(set.HasNamed(""));
}

void detTNamedOrderedSet::TestSetAt(){
	SetSubTestNum(3);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("name1", 1);
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("name2", 2);
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("name3", 3);
	
	set.Add(obj1);
	set.Add(obj2);
	
	// SetAt with same name - allowed (replace with different instance)
	TestNamedObject::Ref obj1b = TestNamedObject::Ref::New("name1", 10);
	set.SetAt(0, obj1b);
	ASSERT_EQUAL(set.GetAt(0)->id, 10);
	ASSERT_EQUAL(set.GetCount(), 2);
	
	// SetAt with different name already present - should fail
	ASSERT_DOES_FAIL(set.SetAt(0, obj2)); // obj2 with name "name2" already at index 1
	
	// SetAt with new unique name - allowed
	set.SetAt(1, obj3);
	ASSERT_TRUE(set.HasNamed("name3"));
	ASSERT_FALSE(set.HasNamed("name2"));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTNamedOrderedSet::TestAdd(){
	SetSubTestNum(4);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("unique1");
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("unique2");
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("unique1"); // duplicate name
	
	// Add unique names
	set.Add(obj1);
	ASSERT_EQUAL(set.GetCount(), 1);
	ASSERT_TRUE(set.HasNamed("unique1"));
	
	set.Add(obj2);
	ASSERT_EQUAL(set.GetCount(), 2);
	ASSERT_TRUE(set.HasNamed("unique2"));
	
	// Add duplicate name - should fail
	ASSERT_DOES_FAIL(set.Add(obj3));
	ASSERT_EQUAL(set.GetCount(), 2);
	
	// Add same object again - should fail (base class behavior)
	ASSERT_DOES_FAIL(set.Add(obj1));
}

void detTNamedOrderedSet::TestAddIfAbsent(){
	SetSubTestNum(5);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("name1");
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("name2");
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("name1"); // duplicate name
	
	// AddIfAbsent with unique name
	ASSERT_TRUE(set.AddIfAbsent(obj1));
	ASSERT_EQUAL(set.GetCount(), 1);
	
	ASSERT_TRUE(set.AddIfAbsent(obj2));
	ASSERT_EQUAL(set.GetCount(), 2);
	
	// AddIfAbsent with duplicate name - should fail
	ASSERT_DOES_FAIL(set.AddIfAbsent(obj3));
	
	// AddIfAbsent with same object - returns false (not added)
	ASSERT_FALSE(set.AddIfAbsent(obj1));
	ASSERT_EQUAL(set.GetCount(), 2);
}

void detTNamedOrderedSet::TestInsert(){
	SetSubTestNum(6);
	
	TestNamedSet set;
	TestNamedObject::Ref obj1 = TestNamedObject::Ref::New("first");
	TestNamedObject::Ref obj2 = TestNamedObject::Ref::New("second");
	TestNamedObject::Ref obj3 = TestNamedObject::Ref::New("middle");
	TestNamedObject::Ref obj4 = TestNamedObject::Ref::New("first"); // duplicate
	
	set.Add(obj1);
	set.Add(obj2);
	
	// Insert with unique name
	set.Insert(obj3, 1);
	ASSERT_EQUAL(set.GetCount(), 3);
	ASSERT_EQUAL(set.GetAt(0)->name, decString("first"));
	ASSERT_EQUAL(set.GetAt(1)->name, decString("middle"));
	ASSERT_EQUAL(set.GetAt(2)->name, decString("second"));
	
	// Insert with duplicate name - should fail
	ASSERT_DOES_FAIL(set.Insert(obj4, 0));
	ASSERT_EQUAL(set.GetCount(), 3);
}
