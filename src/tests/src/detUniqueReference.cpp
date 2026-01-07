// includes
#include <stdio.h>

#include "detUniqueReference.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/exceptions.h>

// Test object class (non-reference counted)
class TestUniqueObject{
public:
	int id;
	static int constructCount;
	static int destructCount;
	
	explicit TestUniqueObject(int nid = 0) : id(nid){
		constructCount++;
	}
	
	~TestUniqueObject(){
		destructCount++;
	}
};

int TestUniqueObject::constructCount = 0;
int TestUniqueObject::destructCount = 0;


// Base and derived classes for casting tests
class TestUniqueBase{
public:
	int baseValue;
	static int constructCount;
	static int destructCount;
	
	explicit TestUniqueBase(int v = 0) : baseValue(v){
		constructCount++;
	}
	virtual ~TestUniqueBase(){
		destructCount++;
	}
};

int TestUniqueBase::constructCount = 0;
int TestUniqueBase::destructCount = 0;

class TestUniqueDerived : public TestUniqueBase{
public:
	int derivedValue;
	static int constructCountDerived;
	static int destructCountDerived;
	
	explicit TestUniqueDerived(int b = 0, int d = 0) : TestUniqueBase(b), derivedValue(d){
		constructCountDerived++;
	}
	~TestUniqueDerived() override{
		destructCountDerived++;
	}
};

int TestUniqueDerived::constructCountDerived = 0;
int TestUniqueDerived::destructCountDerived = 0;


// Class detUniqueReference
////////////////////////////

detUniqueReference::detUniqueReference(){
	Prepare();
}

detUniqueReference::~detUniqueReference(){
	CleanUp();
}

void detUniqueReference::Prepare(){
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
}

void detUniqueReference::Run(){
	TestConstruction();
	TestAssignment();
	TestComparison();
	TestConversion();
	TestPointerAccess();
	TestLifecycle();
	TestMoveSemantics();
	TestCasting();
	TestHash();
	TestClear();
	TestSwap();
}

void detUniqueReference::CleanUp(){
}

const char *detUniqueReference::GetTestName(){
	return "TUniqueReference";
}


// Tests
/////////

void detUniqueReference::TestConstruction(){
	SetSubTestNum(0);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		// Default constructor
		deTUniqueReference<TestUniqueObject> ref1;
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_FALSE(ref1.IsNotNull());
		ASSERT_NULL(ref1.Pointer());
		
		// Constructor with pointer using New
		auto ref2 = deTUniqueReference<TestUniqueObject>::New(42);
		ASSERT_EQUAL(ref2->id, 42);
		
		// New factory method
		auto ref3 = deTUniqueReference<TestUniqueObject>::New(99);
		ASSERT_TRUE(ref3.IsNotNull());
		ASSERT_EQUAL(ref3->id, 99);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 2);
	
	// Note: Copy constructor is deleted (unique ownership)
	// deTUniqueReference<TestUniqueObject> ref4(ref2); // Should not compile
}

void detUniqueReference::TestAssignment(){
	SetSubTestNum(1);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		// Test move assignment from New
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(10);
		ASSERT_EQUAL(ref1->id, 10);
		ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
		
		// Move assign another (should delete old object)
		ref1 = deTUniqueReference<TestUniqueObject>::New(20);
		ASSERT_EQUAL(ref1->id, 20);
		ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
		ASSERT_EQUAL(TestUniqueObject::destructCount, 1); // obj1 deleted
		
		// Assign nullptr
		ref1 = {};
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_EQUAL(TestUniqueObject::destructCount, 2); // obj2 deleted
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 2);
	
	// Note: Copy assignment is deleted (unique ownership)
	// deTUniqueReference<TestUniqueObject> ref2;
	// ref2 = ref1; // Should not compile
}

void detUniqueReference::TestComparison(){
	SetSubTestNum(2);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(10);
		auto ref2 = deTUniqueReference<TestUniqueObject>::New(20);
		deTUniqueReference<TestUniqueObject> ref3;
		
		TestUniqueObject *obj1 = ref1.Pointer();
		TestUniqueObject *obj2 = ref2.Pointer();
		
		// Equality with pointer
		ASSERT_TRUE(ref1 == obj1);
		ASSERT_FALSE(ref1 == obj2);
		ASSERT_TRUE(ref3.IsNull());
		
		// Inequality with pointer
		ASSERT_FALSE(ref1 != obj1);
		ASSERT_TRUE(ref1 != obj2);
		ASSERT_TRUE(ref3.IsNull());
		
		// Equality with another unique reference (using Compare method)
		deTUniqueReference<TestUniqueObject> ref4;
		// Note: Cannot compare with == operator (only with raw pointers)
		// Compare method requires both to be non-null
		
		// Inequality with another unique reference
		// Note: Unique references can only be compared with raw pointers
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 2);
}

void detUniqueReference::TestConversion(){
	SetSubTestNum(3);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref = deTUniqueReference<TestUniqueObject>::New(42);
		
		// Bool conversion
		ASSERT_TRUE((bool)ref);
		ASSERT_FALSE(!ref);
		
		deTUniqueReference<TestUniqueObject> nullRef;
		ASSERT_FALSE((bool)nullRef);
		ASSERT_TRUE(!nullRef);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
}

void detUniqueReference::TestPointerAccess(){
	SetSubTestNum(4);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref = deTUniqueReference<TestUniqueObject>::New(123);
		const TestUniqueObject *rawPtr = ref.Pointer();
		
		// Arrow operator
		ASSERT_EQUAL(ref->id, 123);
		ref->id = 456;
		ASSERT_EQUAL(rawPtr->id, 456);
		
		// Dereference operator
		ASSERT_EQUAL((*ref).id, 456);
		
		// Pointer() method
		ASSERT_EQUAL(ref.Pointer(), rawPtr);
		
		// Reference() method
		ASSERT_EQUAL(&ref.Reference(), rawPtr);
		ASSERT_EQUAL(ref.Reference().id, 456);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
}

void detUniqueReference::TestLifecycle(){
	SetSubTestNum(5);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		// Create object via New
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(1);
		ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
		ASSERT_EQUAL(TestUniqueObject::destructCount, 0);
		
		{
			// Move to another reference
			auto ref2 = std::move(ref1);
			ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
			ASSERT_EQUAL(TestUniqueObject::destructCount, 0);
			ASSERT_TRUE(ref1.IsNull());
		}
		// ref2 out of scope, object should be destroyed
		ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
	}
	
	// No additional destructions
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
	
	// Test immediate deletion
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref = deTUniqueReference<TestUniqueObject>::New(2);
		ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
	}
	// Object destroyed when ref goes out of scope
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
}

void detUniqueReference::TestMoveSemantics(){
	SetSubTestNum(6);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(77);
		TestUniqueObject *ptr = ref1.Pointer();
		
		// Move constructor
		auto ref2(std::move(ref1));
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_EQUAL(ref2.Pointer(), ptr);
		ASSERT_EQUAL(ref2->id, 77);
		
		// Move assignment
		deTUniqueReference<TestUniqueObject> ref3;
		ref3 = std::move(ref2);
		ASSERT_TRUE(ref2.IsNull());
		ASSERT_EQUAL(ref3.Pointer(), ptr);
		ASSERT_EQUAL(ref3->id, 77);
		
		// Self move assignment (should be safe) - suppress warning with temporary
		auto &ref3Ref = ref3;
		ref3 = std::move(ref3Ref);
		ASSERT_EQUAL(ref3.Pointer(), ptr);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
}

void detUniqueReference::TestCasting(){
	SetSubTestNum(7);
	
	// Reset counters
	TestUniqueBase::constructCount = 0;
	TestUniqueBase::destructCount = 0;
	TestUniqueDerived::constructCountDerived = 0;
	TestUniqueDerived::destructCountDerived = 0;
	
	{
		// Test implicit conversion from derived to base (uses conversion operator)
		deTUniqueReference<TestUniqueDerived> derived = deTUniqueReference<TestUniqueDerived>::New(10, 20);
		TestUniqueDerived *derivedPtr = derived.Pointer();
		
		// Move to base type using implicit conversion
		deTUniqueReference<TestUniqueBase> base = std::move(derived);
		ASSERT_TRUE(derived.IsNull());
		ASSERT_EQUAL(base.Pointer(), static_cast<TestUniqueBase*>(derivedPtr));
		ASSERT_EQUAL(base->baseValue, 10);
		
		// Object will be deleted when base goes out of scope
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueBase::constructCount, 1);
	ASSERT_EQUAL(TestUniqueBase::destructCount, 1);
	ASSERT_EQUAL(TestUniqueDerived::constructCountDerived, 1);
	ASSERT_EQUAL(TestUniqueDerived::destructCountDerived, 1);
}

void detUniqueReference::TestHash(){
	SetSubTestNum(8);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(1);
		auto ref2 = deTUniqueReference<TestUniqueObject>::New(2);
		deTUniqueReference<TestUniqueObject> nullRef;
		
		// Different objects typically have different hashes
		ASSERT_NEQUAL(ref1.Hash(), ref2.Hash());
		
		// Null reference has hash of 0
		ASSERT_EQUAL(nullRef.Hash(), 0U);
		
		// Hash should match pointer hash
		TestUniqueObject *ptr = ref1.Pointer();
		ASSERT_EQUAL(ref1.Hash(), DEHash(ptr));
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 2);
}

void detUniqueReference::TestClear(){
	SetSubTestNum(9);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	auto ref = deTUniqueReference<TestUniqueObject>::New(42);
	ASSERT_FALSE(ref.IsNull());
	ASSERT_EQUAL(TestUniqueObject::destructCount, 0);
	
	ref.Clear();
	ASSERT_TRUE(ref.IsNull());
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
	
	// Clearing null reference should be safe
	ref.Clear();
	ASSERT_TRUE(ref.IsNull());
	ASSERT_EQUAL(TestUniqueObject::destructCount, 1);
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 1);
}

void detUniqueReference::TestSwap(){
	SetSubTestNum(10);
	
	// Reset counters
	TestUniqueObject::constructCount = 0;
	TestUniqueObject::destructCount = 0;
	
	{
		auto ref1 = deTUniqueReference<TestUniqueObject>::New(10);
		auto ref2 = deTUniqueReference<TestUniqueObject>::New(20);
		
		const TestUniqueObject *ptr1 = ref1.Pointer();
		const TestUniqueObject *ptr2 = ref2.Pointer();
		
		ref1.Swap(ref2);
		
		ASSERT_EQUAL(ref1.Pointer(), ptr2);
		ASSERT_EQUAL(ref2.Pointer(), ptr1);
		ASSERT_EQUAL(ref1->id, 20);
		ASSERT_EQUAL(ref2->id, 10);
		
		// Swap with null
		deTUniqueReference<TestUniqueObject> nullRef;
		ref1.Swap(nullRef);
		
		ASSERT_TRUE(ref1.IsNull());
		ASSERT_EQUAL(nullRef.Pointer(), ptr2);
		ASSERT_EQUAL(nullRef->id, 20);
	}
	
	// Verify no leaks
	ASSERT_EQUAL(TestUniqueObject::constructCount, 2);
	ASSERT_EQUAL(TestUniqueObject::destructCount, 2);
}
