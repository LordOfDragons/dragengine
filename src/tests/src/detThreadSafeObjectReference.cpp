// includes
#include <stdio.h>

#include "detThreadSafeObjectReference.h"

#include <dragengine/threading/deThreadSafeObject.h>
#include <dragengine/threading/deTThreadSafeObjectReference.h>
#include <dragengine/common/exceptions.h>

// Test object class
class TestTSRefObject : public deThreadSafeObject{
public:
	using Ref = deTThreadSafeObjectReference<TestTSRefObject>;
	
	int id;
	static int constructCount;
	static int destructCount;
	
	explicit TestTSRefObject(int nid = 0) : id(nid){
		constructCount++;
	}

protected:
	~TestTSRefObject() override{
		destructCount++;
	}
};

int TestTSRefObject::constructCount = 0;
int TestTSRefObject::destructCount = 0;


// Base and derived classes for casting tests
class TestTSRefBase : public deThreadSafeObject{
public:
	using Ref = deTThreadSafeObjectReference<TestTSRefBase>;
	int baseValue;
	static int constructCount;
	static int destructCount;
	
	explicit TestTSRefBase(int v = 0) : baseValue(v){
		constructCount++;
	}
protected:
	~TestTSRefBase() override{
		destructCount++;
	}
};

int TestTSRefBase::constructCount = 0;
int TestTSRefBase::destructCount = 0;

class TestTSRefDerived : public TestTSRefBase{
public:
	using Ref = deTThreadSafeObjectReference<TestTSRefDerived>;
	int derivedValue;
	static int constructCountDerived;
	static int destructCountDerived;
	
	explicit TestTSRefDerived(int b = 0, int d = 0) : TestTSRefBase(b), derivedValue(d){
		constructCountDerived++;
	}
protected:
	~TestTSRefDerived() override{
		destructCountDerived++;
	}
};

int TestTSRefDerived::constructCountDerived = 0;
int TestTSRefDerived::destructCountDerived = 0;


// Class detThreadSafeObjectReference
///////////////////////////////////////

detThreadSafeObjectReference::detThreadSafeObjectReference(){
	Prepare();
}

detThreadSafeObjectReference::~detThreadSafeObjectReference(){
	CleanUp();
}

void detThreadSafeObjectReference::Prepare(){
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
}

void detThreadSafeObjectReference::Run(){
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

void detThreadSafeObjectReference::CleanUp(){
}

const char *detThreadSafeObjectReference::GetTestName(){
	return "TThreadSafeObjectReference";
}


// Tests
/////////

void detThreadSafeObjectReference::TestConstruction(){
	SetSubTestNum(0);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	// Default constructor
	TestTSRefObject::Ref ref1;
	ASSERT_TRUE(ref1.IsNull());
	ASSERT_FALSE(ref1.IsNotNull());
	ASSERT_NULL(ref1.Pointer());
	
	{
		// Constructor with pointer using .New()
		TestTSRefObject::Ref ref2 = TestTSRefObject::Ref::New(42);
		ASSERT_EQUAL(ref2->GetRefCount(), 1);  // Only held by ref2
		ASSERT_EQUAL(ref2->id, 42);
		
		// Copy constructor
		TestTSRefObject::Ref ref3(ref2);
		ASSERT_EQUAL(ref2->GetRefCount(), 2);  // Held by ref2 and ref3
		ASSERT_EQUAL(ref3.Pointer(), ref2.Pointer());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
}

void detThreadSafeObjectReference::TestAssignment(){
	SetSubTestNum(1);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref ref1;
		TestTSRefObject::Ref obj1 = TestTSRefObject::Ref::New(10);
		TestTSRefObject::Ref obj2 = TestTSRefObject::Ref::New(20);
		
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
		TestTSRefObject::Ref ref2;
		ref2 = ref1;
		ASSERT_EQUAL(obj2->GetRefCount(), 3);  // Held by obj2, ref1, and ref2
		ASSERT_EQUAL(ref2.Pointer(), ref1.Pointer());
		
		// Assign nullptr
		ref1 = nullptr;
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_EQUAL(obj2->GetRefCount(), 2);  // Only held by obj2 and ref2
		
		// Self assignment
		ref2 = ref2;
		ASSERT_EQUAL(obj2->GetRefCount(), 2);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefObject::constructCount, 2);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 2);
}

void detThreadSafeObjectReference::TestComparison(){
	SetSubTestNum(2);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref obj1 = TestTSRefObject::Ref::New(10);
		TestTSRefObject::Ref obj2 = TestTSRefObject::Ref::New(20);
		
		TestTSRefObject::Ref ref1 = obj1;
		TestTSRefObject::Ref ref2 = obj1;
		TestTSRefObject::Ref ref3 = obj2;
		TestTSRefObject::Ref ref4;
		
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
	ASSERT_EQUAL(TestTSRefObject::constructCount, 2);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 2);
}

void detThreadSafeObjectReference::TestConversion(){
	SetSubTestNum(3);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref ref = TestTSRefObject::Ref::New(42);
		
		// Bool conversion
		ASSERT_TRUE((bool)ref);
		ASSERT_FALSE(!ref);
		
		TestTSRefObject::Ref nullRef;
		ASSERT_FALSE((bool)nullRef);
		ASSERT_TRUE(!nullRef);
		
		// Pointer conversion
		TestTSRefObject *ptr = ref;
		ASSERT_EQUAL(ptr, ref.Pointer());
		
		// Reference conversion (non-null)
		const TestTSRefObject &refObj = ref;
		ASSERT_EQUAL(&refObj, ref.Pointer());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
}

void detThreadSafeObjectReference::TestPointerAccess(){
	SetSubTestNum(4);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref ref = TestTSRefObject::Ref::New(123);
		
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
	ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
}

void detThreadSafeObjectReference::TestLifecycle(){
	SetSubTestNum(5);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		// Create object via New
		TestTSRefObject::Ref ref1 = TestTSRefObject::Ref::New(1);
		ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
		ASSERT_EQUAL(TestTSRefObject::destructCount, 0);
		
		{
			// Copy increases ref count but not object count
			TestTSRefObject::Ref ref2(ref1);
			ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
			ASSERT_EQUAL(TestTSRefObject::destructCount, 0);
			ASSERT_EQUAL(ref1.Pointer()->GetRefCount(), 2);  // Held by ref1 and ref2
		}
		// ref2 out of scope, but object still held by ref1
		ASSERT_EQUAL(ref1.Pointer()->GetRefCount(), 1);  // Only ref1 now
		ASSERT_EQUAL(TestTSRefObject::destructCount, 0);
	}
	// ref1 out of scope, object should be destroyed
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
	
	// Test multiple references
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	TestTSRefObject *obj = new TestTSRefObject(2);
	{
		TestTSRefObject::Ref ref1(obj);
		TestTSRefObject::Ref ref2(obj);
		TestTSRefObject::Ref ref3(obj);
		ASSERT_EQUAL(obj->GetRefCount(), 4);  // Object (1) + ref1 (1) + ref2 (1) + ref3 (1)
		obj->FreeReference();  // Release initial reference, now only held by refs
		ASSERT_EQUAL(obj->GetRefCount(), 3);  // Only held by ref1, ref2, ref3
	}
	// All references released, object destroyed
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
}

void detThreadSafeObjectReference::TestMoveSemantics(){
	SetSubTestNum(6);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref ref1 = TestTSRefObject::Ref::New(77);
		TestTSRefObject *ptr = ref1.Pointer();
		ASSERT_EQUAL(ref1->GetRefCount(), 1);
		
		// Move constructor - transfers ownership, source becomes null
		TestTSRefObject::Ref ref2(std::move(ref1));
		ASSERT_TRUE(ref1.IsNull()); // Source should be null after move
		ASSERT_EQUAL(ref2.Pointer(), ptr);
		ASSERT_EQUAL(ref2->id, 77);
		ASSERT_EQUAL(ref2->GetRefCount(), 1);  // Move doesn't change refcount
		
		// Move assignment - transfers ownership, source becomes null
		TestTSRefObject::Ref ref3;
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
	ASSERT_EQUAL(TestTSRefObject::constructCount, 1);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 1);
}

void detThreadSafeObjectReference::TestCasting(){
	SetSubTestNum(7);
	
	// Reset counters
	TestTSRefBase::constructCount = 0;
	TestTSRefBase::destructCount = 0;
	TestTSRefDerived::constructCountDerived = 0;
	TestTSRefDerived::destructCountDerived = 0;
	
	{
		TestTSRefDerived::Ref derivedRef = TestTSRefDerived::Ref::New(10, 20);
		
		// Static cast to base
		TestTSRefBase::Ref baseRef = derivedRef.StaticCast<TestTSRefBase>();
		ASSERT_EQUAL(baseRef.Pointer(), static_cast<TestTSRefBase*>(derivedRef.Pointer()));
		ASSERT_EQUAL(baseRef->baseValue, 10);
		
		// Dynamic cast back to derived
		deTThreadSafeObjectReference<TestTSRefDerived> derivedRef2 = baseRef.DynamicCast<TestTSRefDerived>();
		ASSERT_EQUAL(derivedRef2.Pointer(), derivedRef.Pointer());
		ASSERT_EQUAL(derivedRef2->derivedValue, 20);
		
		// Dynamic cast to unrelated type should return null
		deTThreadSafeObjectReference<TestTSRefObject> objRef = baseRef.DynamicCast<TestTSRefObject>();
		ASSERT_TRUE(objRef.IsNull());
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefBase::constructCount, 1);
	ASSERT_EQUAL(TestTSRefBase::destructCount, 1);
	ASSERT_EQUAL(TestTSRefDerived::constructCountDerived, 1);
	ASSERT_EQUAL(TestTSRefDerived::destructCountDerived, 1);
}

void detThreadSafeObjectReference::TestHash(){
	SetSubTestNum(8);
	
	// Reset counters
	TestTSRefObject::constructCount = 0;
	TestTSRefObject::destructCount = 0;
	
	{
		TestTSRefObject::Ref obj1 = TestTSRefObject::Ref::New(1);
		TestTSRefObject::Ref obj2 = TestTSRefObject::Ref::New(2);
		
		deTThreadSafeObjectReference<TestTSRefObject> ref1 = obj1;
		deTThreadSafeObjectReference<TestTSRefObject> ref2 = obj1;
		deTThreadSafeObjectReference<TestTSRefObject> ref3 = obj2;
		deTThreadSafeObjectReference<TestTSRefObject> nullRef;
		
		// Same object should have same hash
		ASSERT_EQUAL(ref1.Hash(), ref2.Hash());
		
		// Different objects typically have different hashes
		ASSERT_NEQUAL(ref1.Hash(), ref3.Hash());
		
		// Null reference has hash of 0
		ASSERT_EQUAL(nullRef.Hash(), 0U);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefObject::constructCount, 2);
	ASSERT_EQUAL(TestTSRefObject::destructCount, 2);
}

void detThreadSafeObjectReference::TestUpcastDowncast(){
	SetSubTestNum(9);
	
	// Reset counters
	TestTSRefBase::constructCount = 0;
	TestTSRefBase::destructCount = 0;
	TestTSRefDerived::constructCountDerived = 0;
	TestTSRefDerived::destructCountDerived = 0;
	
	{
		// Upcast constructor (derived to base)
		TestTSRefDerived::Ref derivedRef = TestTSRefDerived::Ref::New(100, 200);
		
		deTThreadSafeObjectReference<TestTSRefBase> baseRef(derivedRef);
		ASSERT_EQUAL(baseRef.Pointer(), static_cast<TestTSRefBase*>(derivedRef.Pointer()));
		ASSERT_EQUAL(baseRef->baseValue, 100);
		
		// Upcast assignment
		deTThreadSafeObjectReference<TestTSRefBase> baseRef2;
		baseRef2 = derivedRef;
		ASSERT_EQUAL(baseRef2.Pointer(), static_cast<TestTSRefBase*>(derivedRef.Pointer()));
		
		// Auto-cast to super class
		deTThreadSafeObjectReference<TestTSRefBase> baseRef3 = derivedRef;
		ASSERT_EQUAL(baseRef3.Pointer(), static_cast<TestTSRefBase*>(derivedRef.Pointer()));
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestTSRefBase::constructCount, 1);
	ASSERT_EQUAL(TestTSRefBase::destructCount, 1);
	ASSERT_EQUAL(TestTSRefDerived::constructCountDerived, 1);
	ASSERT_EQUAL(TestTSRefDerived::destructCountDerived, 1);
}
