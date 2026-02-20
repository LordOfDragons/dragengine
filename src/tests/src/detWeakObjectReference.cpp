// includes
#include <stdio.h>

#include "detWeakObjectReference.h"

#include <dragengine/deObject.h>
#include <dragengine/deTWeakObjectReference.h>
#include <dragengine/common/exceptions.h>

// Test object class
class TestWeakRefObject : public deObject{
public:
	using Ref = deTObjectReference<TestWeakRefObject>;
	using WeakRef = deTWeakObjectReference<TestWeakRefObject>;
	
	int id;
	static int constructCount;
	static int destructCount;
	
	explicit TestWeakRefObject(int nid = 0) : id(nid){
		constructCount++;
	}

protected:
	~TestWeakRefObject() override{
		destructCount++;
	}
};

int TestWeakRefObject::constructCount = 0;
int TestWeakRefObject::destructCount = 0;


// Base and derived classes for casting tests
class TestWeakRefBase : public deObject{
public:
	using Ref = deTObjectReference<TestWeakRefBase>;
	using WeakRef = deTWeakObjectReference<TestWeakRefBase>;
	int baseValue;
	explicit TestWeakRefBase(int v = 0) : baseValue(v){}
protected:
	~TestWeakRefBase() override = default;
};

class TestWeakRefDerived : public TestWeakRefBase{
public:
	using Ref = deTObjectReference<TestWeakRefDerived>;
	using WeakRef = deTWeakObjectReference<TestWeakRefDerived>;
	int derivedValue;
	explicit TestWeakRefDerived(int b = 0, int d = 0) : TestWeakRefBase(b), derivedValue(d){}
protected:
	~TestWeakRefDerived() override = default;
};


// Class detWeakObjectReference
/////////////////////////////////

detWeakObjectReference::detWeakObjectReference(){
	Prepare();
}

detWeakObjectReference::~detWeakObjectReference(){
	CleanUp();
}

void detWeakObjectReference::Prepare(){
	TestWeakRefObject::constructCount = 0;
	TestWeakRefObject::destructCount = 0;
}

void detWeakObjectReference::Run(){
	TestConstruction();
	TestAssignment();
	TestComparison();
	TestConversion();
	TestPointerAccess();
	TestLifecycleWeakRef();
	TestObjectDeletion();
}

void detWeakObjectReference::CleanUp(){
}

const char *detWeakObjectReference::GetTestName(){
	return "TWeakObjectReference";
}


// Tests
/////////

void detWeakObjectReference::TestConstruction(){
	SetSubTestNum(0);
	
	// Default constructor
	TestWeakRefObject::WeakRef wref1;
	ASSERT_TRUE(wref1.IsNull());
	ASSERT_FALSE(wref1.IsNotNull());
	ASSERT_NULL(wref1.Pointer());
	
	// Constructor with pointer (object kept alive by strong ref)
	TestWeakRefObject::Ref strongRef = TestWeakRefObject::Ref::New(42);
	TestWeakRefObject::WeakRef wref2(strongRef.Pointer());
	ASSERT_FALSE(wref2.IsNull());
	ASSERT_EQUAL(wref2.Pointer(), strongRef.Pointer());
	ASSERT_EQUAL(wref2->id, 42);
	
	// Copy constructor
	TestWeakRefObject::WeakRef wref3(wref2);
	ASSERT_FALSE(wref3.IsNull());
	ASSERT_EQUAL(wref3.Pointer(), strongRef.Pointer());
}

void detWeakObjectReference::TestAssignment(){
	SetSubTestNum(1);
	
	TestWeakRefObject::Ref strong1 = TestWeakRefObject::Ref::New(10);
	TestWeakRefObject::Ref strong2 = TestWeakRefObject::Ref::New(20);
	
	TestWeakRefObject::WeakRef wref1;
	
	// Assign pointer
	wref1 = strong1.Pointer();
	ASSERT_FALSE(wref1.IsNull());
	ASSERT_EQUAL(wref1->id, 10);
	
	// Assign another pointer
	wref1 = strong2.Pointer();
	ASSERT_FALSE(wref1.IsNull());
	ASSERT_EQUAL(wref1->id, 20);
	
	// Assign weak reference
	TestWeakRefObject::WeakRef wref2;
	wref2 = wref1;
	ASSERT_EQUAL(wref2.Pointer(), wref1.Pointer());
	
	// Assign nullptr
	wref1 = nullptr;
	ASSERT_TRUE(wref1.IsNull());
}

void detWeakObjectReference::TestComparison(){
	SetSubTestNum(2);
	
	TestWeakRefObject::Ref strong1 = TestWeakRefObject::Ref::New(10);
	TestWeakRefObject::Ref strong2 = TestWeakRefObject::Ref::New(20);
	
	TestWeakRefObject::WeakRef wref1(strong1.Pointer());
	TestWeakRefObject::WeakRef wref2(strong1.Pointer());
	TestWeakRefObject::WeakRef wref3(strong2.Pointer());
	TestWeakRefObject::WeakRef wref4;
	
	// Equality with reference
	ASSERT_TRUE(wref1 == wref2);
	ASSERT_FALSE(wref1 == wref3);
	ASSERT_FALSE(wref1 == wref4);
	
	// Equality with pointer
	ASSERT_TRUE(wref1 == strong1.Pointer());
	ASSERT_FALSE(wref1 == strong2.Pointer());
	ASSERT_TRUE(wref4.IsNull());
	
	// Inequality
	ASSERT_FALSE(wref1 != wref2);
	ASSERT_TRUE(wref1 != wref3);
	ASSERT_TRUE(wref1 != strong2.Pointer());
}

void detWeakObjectReference::TestConversion(){
	SetSubTestNum(3);
	
	TestWeakRefObject::Ref strongRef = TestWeakRefObject::Ref::New(42);
	TestWeakRefObject::WeakRef wref(strongRef.Pointer());
	
	// Bool conversion
	ASSERT_TRUE((bool)wref);
	ASSERT_FALSE(!wref);
	
	TestWeakRefObject::WeakRef nullWRef;
	ASSERT_FALSE((bool)nullWRef);
	ASSERT_TRUE(!nullWRef);
	
	// Pointer conversion
	TestWeakRefObject *ptr = wref;
	ASSERT_EQUAL(ptr, strongRef.Pointer());
	
	// Reference conversion (non-null)
	const TestWeakRefObject &refObj = wref;
	ASSERT_EQUAL(&refObj, strongRef.Pointer());
}

void detWeakObjectReference::TestPointerAccess(){
	SetSubTestNum(4);
	
	TestWeakRefObject::Ref strongRef = TestWeakRefObject::Ref::New(123);
	TestWeakRefObject::WeakRef wref(strongRef.Pointer());
	
	// Arrow operator
	ASSERT_EQUAL(wref->id, 123);
	wref->id = 456;
	ASSERT_EQUAL(strongRef->id, 456);
	
	// Pointer() method
	ASSERT_EQUAL(wref.Pointer(), strongRef.Pointer());
	
	// Reference() method
	ASSERT_EQUAL(&wref.Reference(), strongRef.Pointer());
	ASSERT_EQUAL(wref.Reference().id, 456);
}

void detWeakObjectReference::TestLifecycleWeakRef(){
	SetSubTestNum(5);
	
	// Reset counters
	TestWeakRefObject::constructCount = 0;
	TestWeakRefObject::destructCount = 0;
	
	TestWeakRefObject::WeakRef wref;
	
	{
		// Create object via strong ref
		TestWeakRefObject::Ref strongRef = TestWeakRefObject::Ref::New(1);
		ASSERT_EQUAL(TestWeakRefObject::constructCount, 1);
		ASSERT_EQUAL(TestWeakRefObject::destructCount, 0);
		
		// Create weak ref
		wref = strongRef.Pointer();
		ASSERT_FALSE(wref.IsNull());
		ASSERT_EQUAL(TestWeakRefObject::destructCount, 0);
	}
	// Strong ref out of scope, object should be destroyed
	ASSERT_EQUAL(TestWeakRefObject::destructCount, 1);
	
	// Weak ref should now be null (object deleted)
	ASSERT_TRUE(wref.IsNull());
}

void detWeakObjectReference::TestObjectDeletion(){
	SetSubTestNum(7);
	
	TestWeakRefObject::WeakRef wref;
	
	{
		TestWeakRefObject::Ref strong = TestWeakRefObject::Ref::New(99);
		wref = strong.Pointer();
		
		ASSERT_FALSE(wref.IsNull());
		ASSERT_EQUAL(wref->id, 99);
		
		// Multiple weak refs to same object
		TestWeakRefObject::WeakRef wref2 = wref;
		TestWeakRefObject::WeakRef wref3(strong.Pointer());
		
		ASSERT_FALSE(wref2.IsNull());
		ASSERT_FALSE(wref3.IsNull());
	}
	// Strong ref destroyed, all weak refs should become null
	ASSERT_TRUE(wref.IsNull());
}
