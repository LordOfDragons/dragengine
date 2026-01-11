// includes
#include <stdio.h>

#include <dragengine/deObject.h>
#include <dragengine/common/exceptions.h>

#include "detObjectReference.h"

// Test object class
class TestRefObject : public deObject{
public:
	using Ref = deTObjectReference<TestRefObject>;
	
	int id;
	static int constructCount;
	static int destructCount;
	
	explicit TestRefObject(int nid = 0) : id(nid){
		constructCount++;
	}

protected:
	~TestRefObject() override{
		destructCount++;
	}
};

int TestRefObject::constructCount = 0;
int TestRefObject::destructCount = 0;


// Base and derived classes for casting tests
class TestRefBase : public deObject{
public:
	using Ref = deTObjectReference<TestRefBase>;
	int baseValue;
	static int constructCount;
	static int destructCount;
	
	explicit TestRefBase(int v = 0) : baseValue(v){
		constructCount++;
	}
protected:
	~TestRefBase() override{
		destructCount++;
	}
};

int TestRefBase::constructCount = 0;
int TestRefBase::destructCount = 0;

class TestRefDerived : public TestRefBase{
public:
	using Ref = deTObjectReference<TestRefDerived>;
	int derivedValue;
	static int constructCountDerived;
	static int destructCountDerived;
	
	explicit TestRefDerived(int b = 0, int d = 0) : TestRefBase(b), derivedValue(d){
		constructCountDerived++;
	}
protected:
	~TestRefDerived() override{
		destructCountDerived++;
	}
};

int TestRefDerived::constructCountDerived = 0;
int TestRefDerived::destructCountDerived = 0;

// Typedefs for easier use
using TestRefObjectRef = deTObjectReference<TestRefObject>;
using TestRefBaseRef = deTObjectReference<TestRefBase>;
using TestRefDerivedRef = deTObjectReference<TestRefDerived>;


// Class detObjectReference
////////////////////////////

detObjectReference::detObjectReference(){
	Prepare();
}

detObjectReference::~detObjectReference(){
	CleanUp();
}

void detObjectReference::Prepare(){
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
}

void detObjectReference::Run(){
	TestConstruction();
	TestAssignment();
	TestComparison();
	TestConversion();
	TestPointerAccess();
	TestLifecycle();
	TestMoveSemantics();
	TestCasting();
	TestHash();
	TestUpcastDowncast();
}

void detObjectReference::CleanUp(){
}

const char *detObjectReference::GetTestName(){
	return "TObjectReference";
}


// Tests
/////////

void detObjectReference::TestConstruction(){
	SetSubTestNum(0);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	// Default constructor
	TestRefObject::Ref ref1;
	ASSERT_TRUE(ref1.IsNull());
	ASSERT_FALSE(ref1.IsNotNull());
	ASSERT_NULL(ref1.Pointer());
	
	{
		// Constructor with pointer using .New()
		TestRefObject::Ref ref2 = TestRefObject::Ref::New(42);
		ASSERT_EQUAL(ref2->GetRefCount(), 1);  // Only held by ref2
		ASSERT_EQUAL(ref2->id, 42);
		
		// Copy constructor
		TestRefObject::Ref ref3(ref2);
		ASSERT_EQUAL(ref2.Pointer(), ref3.Pointer());
		ASSERT_EQUAL(ref2->GetRefCount(), 2);  // Held by ref2 and ref3
	}
	
	// Verify no leaks (after all references go out of scope)
	ASSERT_EQUAL(TestRefObject::constructCount, 1);
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
}

void detObjectReference::TestAssignment(){
	SetSubTestNum(1);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref ref1;
		TestRefObject::Ref obj1 = TestRefObject::Ref::New(10);
		TestRefObject::Ref obj2 = TestRefObject::Ref::New(20);
		
		// Assign reference
		ref1 = obj1;
		ASSERT_EQUAL(obj1->GetRefCount(), 2);  // Held by obj1 and ref1
		ASSERT_EQUAL(ref1->id, 10);
		
		// Assign another reference (should release old reference)
		ref1 = obj2;
		ASSERT_EQUAL(obj1->GetRefCount(), 1);  // Only held by obj1 now
		ASSERT_EQUAL(obj2->GetRefCount(), 2);  // Held by obj2 and ref1
		ASSERT_EQUAL(ref1->id, 20);
		
		// Assign reference
		TestRefObject::Ref ref2;
		ref2 = ref1;
		ASSERT_EQUAL(obj2->GetRefCount(), 3);  // Held by obj2, ref1, and ref2
		ASSERT_EQUAL(ref2.Pointer(), ref1.Pointer());
		
		// Assign nullptr
		ref1 = nullptr;
		ASSERT_EQUAL(obj2->GetRefCount(), 2);  // Only held by obj2 and ref2
		ASSERT_TRUE(ref1.IsNull());
		ref1 = nullptr;  // Assign nullptr again (no change)
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_EQUAL(obj2->GetRefCount(), 2);
		
		// Self assignment
		ref2 = ref2;
		ASSERT_EQUAL(obj2->GetRefCount(), 2);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 2);
	ASSERT_EQUAL(TestRefObject::destructCount, 2);
}

void detObjectReference::TestComparison(){
	SetSubTestNum(2);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref obj1 = TestRefObject::Ref::New(10);
		TestRefObject::Ref obj2 = TestRefObject::Ref::New(20);
		
		TestRefObject::Ref ref1 = obj1;
		TestRefObject::Ref ref2 = obj1;
		TestRefObject::Ref ref3 = obj2;
		TestRefObject::Ref ref4;
		
		// Equality with reference
		ASSERT_TRUE(ref1 == ref2);
		ASSERT_FALSE(ref1 == ref3);
		ASSERT_FALSE(ref1 == ref4);
		
		// Equality with pointer
		ASSERT_TRUE(ref1 == obj1.Pointer());
		ASSERT_FALSE(ref1 == obj2.Pointer());
		ASSERT_TRUE(ref4.IsNull());
		ASSERT_TRUE(ref4.Pointer() == nullptr);
		
		// Inequality
		ASSERT_FALSE(ref1 != ref2);
		ASSERT_TRUE(ref1 != ref3);
		ASSERT_TRUE(ref1 != obj2.Pointer());
		ASSERT_FALSE(ref1 != obj1.Pointer());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 2);
	ASSERT_EQUAL(TestRefObject::destructCount, 2);
}

void detObjectReference::TestConversion(){
	SetSubTestNum(3);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref ref = TestRefObject::Ref::New(42);
		
		// Bool conversion
		ASSERT_TRUE((bool)ref);
		ASSERT_FALSE(!ref);
		
		TestRefObject::Ref nullRef;
		ASSERT_FALSE((bool)nullRef);
		ASSERT_TRUE(!nullRef);
		
		// Pointer conversion
		TestRefObject *ptr = ref;
		ASSERT_EQUAL(ptr, ref.Pointer());
		
		// Reference conversion (non-null)
		const TestRefObject &refObj = ref;
		ASSERT_EQUAL(&refObj, ref.Pointer());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 1);
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
}

void detObjectReference::TestPointerAccess(){
	SetSubTestNum(4);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref ref = TestRefObject::Ref::New(123);
		
		// Arrow operator
		ASSERT_EQUAL(ref->id, 123);
		ref->id = 456;
		ASSERT_EQUAL(ref->id, 456);
		
		// Pointer() method
		ASSERT_EQUAL(ref.Pointer()->id, 456);
		
		// Reference() method
		ASSERT_EQUAL(&ref.Reference(), ref.Pointer());
		ASSERT_EQUAL(ref.Reference().id, 456);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 1);
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
}

void detObjectReference::TestLifecycle(){
	SetSubTestNum(5);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		// Create object via New
		TestRefObject::Ref ref1 = TestRefObject::Ref::New(1);
		ASSERT_EQUAL(TestRefObject::constructCount, 1);
		ASSERT_EQUAL(TestRefObject::destructCount, 0);
		
		{
			// Copy increases ref count but not object count
			TestRefObject::Ref ref2(ref1);
			ASSERT_EQUAL(TestRefObject::constructCount, 1);
			ASSERT_EQUAL(TestRefObject::destructCount, 0);
			ASSERT_EQUAL(ref1.Pointer()->GetRefCount(), 2);  // Held by ref1 and ref2
		}
		// ref2 out of scope, but object still held by ref1
		 ASSERT_EQUAL(ref1.Pointer()->GetRefCount(), 1);  // Only ref1 now
		ASSERT_EQUAL(TestRefObject::destructCount, 0);
	}
	// ref1 out of scope, object should be destroyed
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
	
	// Test multiple references
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	TestRefObject *obj = new TestRefObject(2);
	{
		TestRefObject::Ref ref1(obj);
		TestRefObject::Ref ref2(obj);
		TestRefObject::Ref ref3(obj);
		ASSERT_EQUAL(obj->GetRefCount(), 4);  // Object (1) + ref1 (1) + ref2 (1) + ref3 (1)
		obj->FreeReference();  // Release initial reference, now only held by refs
		ASSERT_EQUAL(obj->GetRefCount(), 3);  // Only held by ref1, ref2, ref3
	}
	// All references released, object destroyed
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
}

void detObjectReference::TestMoveSemantics(){
	SetSubTestNum(6);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref ref1 = TestRefObject::Ref::New(77);
		TestRefObject *ptr = ref1.Pointer();
		ASSERT_EQUAL(ref1->GetRefCount(), 1);
		
		// Move constructor - transfers ownership, source becomes null
		TestRefObject::Ref ref2(std::move(ref1));
		ASSERT_TRUE(ref1.IsNull()); // Source should be null after move
		ASSERT_EQUAL(ref2.Pointer(), ptr);
		ASSERT_EQUAL(ref2->id, 77);
		ASSERT_EQUAL(ref2->GetRefCount(), 1);  // Move doesn't change refcount
		
		// Move assignment - transfers ownership, source becomes null
		TestRefObject::Ref ref3;
		ref3 = std::move(ref2);
		ASSERT_TRUE(ref2.IsNull()); // Source should be null after move
		ASSERT_EQUAL(ref3.Pointer(), ptr);
		ASSERT_EQUAL(ref3->id, 77);
		ASSERT_EQUAL(ref3->GetRefCount(), 1);
		
		// Self move assignment (should be safe)
		auto &ref3Ref = ref3;
		ref3 = std::move(ref3Ref);
		ASSERT_EQUAL(ref3.Pointer(), ptr);
		ASSERT_EQUAL(ref3->GetRefCount(), 1);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 1);
	ASSERT_EQUAL(TestRefObject::destructCount, 1);
}

void detObjectReference::TestCasting(){
	SetSubTestNum(7);
	
	// Reset counters
	TestRefBase::constructCount = 0;
	TestRefBase::destructCount = 0;
	TestRefDerived::constructCountDerived = 0;
	TestRefDerived::destructCountDerived = 0;
	
	{
		TestRefDerived::Ref derivedRef = TestRefDerived::Ref::New(10, 20);
		
		// Static cast to base
		TestRefBase::Ref baseRef = derivedRef.StaticCast<TestRefBase>();
		ASSERT_EQUAL(baseRef.Pointer(), static_cast<TestRefBase*>(derivedRef.Pointer()));
		ASSERT_EQUAL(baseRef->baseValue, 10);
		
		// Dynamic cast back to derived
		TestRefDerivedRef derivedRef2 = baseRef.DynamicCast<TestRefDerived>();
		ASSERT_EQUAL(derivedRef2.Pointer(), derivedRef.Pointer());
		ASSERT_EQUAL(derivedRef2->derivedValue, 20);
		
		// Dynamic cast to unrelated type should return null
		TestRefObjectRef objRef = baseRef.DynamicCast<TestRefObject>();
		ASSERT_TRUE(objRef.IsNull());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefBase::constructCount, 1);
	ASSERT_EQUAL(TestRefBase::destructCount, 1);
	ASSERT_EQUAL(TestRefDerived::constructCountDerived, 1);
	ASSERT_EQUAL(TestRefDerived::destructCountDerived, 1);
}

void detObjectReference::TestHash(){
	SetSubTestNum(8);
	
	// Reset counters
	TestRefObject::constructCount = 0;
	TestRefObject::destructCount = 0;
	
	{
		TestRefObject::Ref obj1 = TestRefObject::Ref::New(1);
		TestRefObject::Ref obj2 = TestRefObject::Ref::New(2);
		
		TestRefObjectRef ref1 = obj1;
		TestRefObjectRef ref2 = obj1;
		TestRefObjectRef ref3 = obj2;
		TestRefObjectRef nullRef;
		
		// Same object should have same hash
		ASSERT_EQUAL(ref1.Hash(), ref2.Hash());
		
		// Different objects typically have different hashes
		ASSERT_NEQUAL(ref1.Hash(), ref3.Hash());
		
		// Null reference has hash of 0
		ASSERT_EQUAL(nullRef.Hash(), 0U);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefObject::constructCount, 2);
	ASSERT_EQUAL(TestRefObject::destructCount, 2);
}

void detObjectReference::TestUpcastDowncast(){
	SetSubTestNum(9);
	
	// Reset counters
	TestRefBase::constructCount = 0;
	TestRefBase::destructCount = 0;
	TestRefDerived::constructCountDerived = 0;
	TestRefDerived::destructCountDerived = 0;
	
	{
		// Upcast constructor (derived to base)
		TestRefDerivedRef derivedRef = TestRefDerived::Ref::New(100, 200);
		
		TestRefBaseRef baseRef(derivedRef);
		ASSERT_EQUAL(baseRef.Pointer(), static_cast<TestRefBase*>(derivedRef.Pointer()));
		ASSERT_EQUAL(baseRef->baseValue, 100);
		
		// Upcast assignment
		TestRefBaseRef baseRef2;
		baseRef2 = derivedRef;
		ASSERT_EQUAL(baseRef2.Pointer(), static_cast<TestRefBase*>(derivedRef.Pointer()));
		
		// Auto-cast to super class
		TestRefBaseRef baseRef3 = derivedRef;
		ASSERT_EQUAL(baseRef3.Pointer(), static_cast<TestRefBase*>(derivedRef.Pointer()));
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestRefBase::constructCount, 1);
	ASSERT_EQUAL(TestRefBase::destructCount, 1);
	ASSERT_EQUAL(TestRefDerived::constructCountDerived, 1);
	ASSERT_EQUAL(TestRefDerived::destructCountDerived, 1);
}
