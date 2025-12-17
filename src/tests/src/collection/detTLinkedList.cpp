// includes
#include <stdio.h>

#include "detTLinkedList.h"

#include <dragengine/common/collection/decTLinkedList.h>
#include <dragengine/deObject.h>

// TestLLObject implementation
class TestLLObject : public deObject{
public:
	typedef deTObjectReference<TestLLObject> Ref;

	int id;
	decTObjectLinkedList<TestLLObject>::Element entry;

	explicit TestLLObject(int nid) : id(nid), entry(this) {}
protected:
	~TestLLObject() override = default;
};

typedef decTObjectLinkedList<TestLLObject> TestLLList;


// Class detTLinkedList
///////////////////////

detTLinkedList::detTLinkedList(){
	Prepare();
}

detTLinkedList::~detTLinkedList(){
	CleanUp();
}

void detTLinkedList::Prepare(){
}

void detTLinkedList::Run(){
	TestBasicAddRemove();
	TestInsertions();
	TestIndexingAccess();
	TestIterators();
	TestElementIteratorAdvanced();
	TestStability();
	TestHasMatching();
	TestVisit();
	TestFind();
	TestFindReverse();
	TestFindOrNull();
	TestFindReverseOrNull();
	TestInject();
	TestRemoveIf();
	TestBidirectionalIterator();
}

void detTLinkedList::CleanUp(){
}

const char *detTLinkedList::GetTestName(){
	return "TLinkedList";
}


// Tests
/////////

void detTLinkedList::TestBasicAddRemove(){
	SetSubTestNum(0);

	TestLLObject::Ref a = TestLLObject::Ref::New(1);
	TestLLObject::Ref b = TestLLObject::Ref::New(2);
	TestLLObject::Ref c = TestLLObject::Ref::New(3);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);

	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_NOT_NULL(list.GetRoot());
	ASSERT_NOT_NULL(list.GetTail());
	ASSERT_EQUAL(list.GetRoot()->GetOwner()->id, 1);
	ASSERT_EQUAL(list.GetTail()->GetOwner()->id, 3);

	ASSERT_TRUE(list.Has(&b->entry));
	ASSERT_EQUAL(list.IndexOf(&b->entry), 1);

	list.Remove(&b->entry);
	ASSERT_EQUAL(list.GetCount(), 2);
	ASSERT_FALSE(list.Has(&b->entry));

	list.RemoveAll();
	ASSERT_TRUE(list.IsEmpty());
}

void detTLinkedList::TestInsertions(){
	SetSubTestNum(1);

	TestLLObject::Ref a = TestLLObject::Ref::New(1);
	TestLLObject::Ref b = TestLLObject::Ref::New(2);
	TestLLObject::Ref c = TestLLObject::Ref::New(3);
	TestLLObject::Ref d = TestLLObject::Ref::New(4);
	
	TestLLList list;
	list.Add(&a->entry); // [a]
	list.InsertAfter(&b->entry, list.GetRoot()); // [a,b]
	list.InsertBefore(&c->entry, list.GetRoot()); // [c,a,b]

	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_EQUAL(list.GetRoot()->GetOwner()->id, 3);
	ASSERT_EQUAL(list.GetAt(1)->GetOwner()->id, 1);
	ASSERT_EQUAL(list.GetTail()->GetOwner()->id, 2);

	// InsertAfter at tail
	list.InsertAfter(&d->entry, list.GetTail()); // [c,a,b,d]
	ASSERT_EQUAL(list.GetCount(), 4);
	ASSERT_EQUAL(list.GetTail()->GetOwner()->id, 4);

	// Verify chain integrity
	ASSERT_EQUAL(list.GetRoot()->GetNext()->GetOwner()->id, 1);
	ASSERT_EQUAL(list.GetTail()->GetPrevious()->GetOwner()->id, 2);
}

void detTLinkedList::TestIndexingAccess(){
	SetSubTestNum(2);

	TestLLObject::Ref obj1 = TestLLObject::Ref::New(10);
	TestLLObject::Ref obj2 = TestLLObject::Ref::New(20);
	TestLLObject::Ref obj3 = TestLLObject::Ref::New(30);
	TestLLObject::Ref obj4 = TestLLObject::Ref::New(40);
	
	TestLLList list;
	list.Add(&obj1->entry);
	list.Add(&obj2->entry);
	list.Add(&obj3->entry);
	list.Add(&obj4->entry);

	// GetAt
	ASSERT_EQUAL(list.GetAt(0)->GetOwner()->id, 10);
	ASSERT_EQUAL(list.GetAt(2)->GetOwner()->id, 30);
	ASSERT_EQUAL(list[3]->GetOwner()->id, 40);

	// IndexOf
	ASSERT_EQUAL(list.IndexOf(&obj2->entry), 1);
	ASSERT_EQUAL(list.IndexOf(&obj4->entry), 3);

	// Has
	ASSERT_TRUE(list.Has(&obj3->entry));

	// GetRoot and GetTail
	ASSERT_EQUAL(list.GetRoot()->GetOwner()->id, 10);
	ASSERT_EQUAL(list.GetTail()->GetOwner()->id, 40);

	// Previous/Next navigation
	auto elem = list.GetRoot();
	ASSERT_NULL(elem->GetPrevious());
	ASSERT_NOT_NULL(elem->GetNext());
	ASSERT_EQUAL(elem->GetNext()->GetOwner()->id, 20);

	elem = list.GetTail();
	ASSERT_NULL(elem->GetNext());
	ASSERT_NOT_NULL(elem->GetPrevious());
	ASSERT_EQUAL(elem->GetPrevious()->GetOwner()->id, 30);
}

void detTLinkedList::TestIterators(){
	SetSubTestNum(3);

	TestLLObject::Ref a = TestLLObject::Ref::New(1);
	TestLLObject::Ref b = TestLLObject::Ref::New(2);
	TestLLObject::Ref c = TestLLObject::Ref::New(3);
	TestLLObject::Ref d = TestLLObject::Ref::New(4);
	TestLLObject::Ref e = TestLLObject::Ref::New(5);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	list.Add(&d->entry);
	list.Add(&e->entry);

	// ElementIterator
	TestLLList::ElementIterator it(list);
	int sum = 0;
	while(it.Advance()){
		sum += it->id;
	}
	ASSERT_EQUAL(sum, 15);

	// ElementIterator with operator++
	TestLLList::ElementIterator it2(list);
	int count = 0;
	while(++it2){
		count++;
	}
	ASSERT_EQUAL(count, 5);

	// ElementIterator +=
	TestLLList::ElementIterator it3(list);
	it3.Advance(); // to first
	it3 += 2; // skip 2 more
	ASSERT_TRUE(it3.HasNotFinished());
	ASSERT_EQUAL(it3->id, 3);

	// std-like const_iterator
	count = 0;
	int sum2 = 0;
	for(auto ci = list.cbegin(); ci != list.cend(); ++ci){
		sum2 += ci->id;
		count++;
	}
	ASSERT_EQUAL(count, 5);
	ASSERT_EQUAL(sum2, 15);

	// const_iterator post-increment
	auto ci = list.cbegin();
	auto ci_old = ci++;
	ASSERT_EQUAL(ci_old->id, 1);
	ASSERT_EQUAL(ci->id, 2);
}

void detTLinkedList::TestElementIteratorAdvanced(){
	SetSubTestNum(4);

	TestLLObject::Ref obj1 = TestLLObject::Ref::New(10);
	TestLLObject::Ref obj2 = TestLLObject::Ref::New(20);
	TestLLObject::Ref obj3 = TestLLObject::Ref::New(30);
	TestLLObject::Ref obj4 = TestLLObject::Ref::New(40);
	
	TestLLList list;
	list.Add(&obj1->entry);
	list.Add(&obj2->entry);
	list.Add(&obj3->entry);
	list.Add(&obj4->entry);

	// Iterator from specific entry to end
	TestLLList::ElementIterator it(&obj2->entry);
	int sum = 0;
	while(it.Advance()){
		sum += it->id;
	}
	ASSERT_EQUAL(sum, 90); // 20+30+40

	// Iterator from first to specific entry
	TestLLList::ElementIterator it2(&obj1->entry, &obj3->entry);
	sum = 0;
	while(it2.Advance()){
		sum += it2->id;
	}
	ASSERT_EQUAL(sum, 60); // 10+20+30

	// Entry() method
	TestLLList::ElementIterator it3(list);
	it3.Advance();
	ASSERT_EQUAL(it3.Entry().id, 10);

	// HasFinished
	TestLLList::ElementIterator it4(list);
	ASSERT_TRUE(it4.HasFinished());
	it4.Advance();
	ASSERT_FALSE(it4.HasFinished());

	// HasNotFinished
	TestLLList::ElementIterator it5(list);
	ASSERT_FALSE(it5.HasNotFinished());
	it5.Advance();
	ASSERT_TRUE(it5.HasNotFinished());

	// operator! (inverted HasNotFinished)
	TestLLList::ElementIterator it6(list);
	ASSERT_TRUE(!it6); // !HasNotFinished = true initially (not advanced yet)
	it6.Advance();
	ASSERT_FALSE(!it6); // !HasNotFinished = false after advance

	// operator= (assignment)
	TestLLList::ElementIterator it7(list);
	TestLLList::ElementIterator it8(list);
	it7.Advance(); // move to first element
	it8 = it7; // copy state
	ASSERT_EQUAL(it8->id, 10);
	it8.Advance();
	ASSERT_EQUAL(it8->id, 20);
	ASSERT_EQUAL(it7->id, 10); // original unchanged

	// Iterate and remove safely
	TestLLObject::Ref r1 = TestLLObject::Ref::New(1);
	TestLLObject::Ref r2 = TestLLObject::Ref::New(2);
	TestLLObject::Ref r3 = TestLLObject::Ref::New(3);
	
	TestLLList list2;
	list2.Add(&r1->entry);
	list2.Add(&r2->entry);
	list2.Add(&r3->entry);

	// Remove middle element during iteration
	TestLLList::ElementIterator itRem(list2);
	while(itRem.Advance()){
		if(itRem->id == 2){
			list2.Remove(&r2->entry);
		}
	}
	ASSERT_EQUAL(list2.GetCount(), 2);
	ASSERT_FALSE(list2.Has(&r2->entry));
}

void detTLinkedList::TestStability(){
	SetSubTestNum(5);
	
	TestLLObject::Ref r1 = TestLLObject::Ref::New(1);
	TestLLObject::Ref r2 = TestLLObject::Ref::New(2);
	TestLLObject::Ref r3 = TestLLObject::Ref::New(3);
	ASSERT_EQUAL(r2->GetRefCount(), 1);
	
	TestLLList list;
	list.Add(&r1->entry);
	list.Add(&r2->entry);
	list.Add(&r3->entry);
	ASSERT_EQUAL(r2->GetRefCount(), 2);
	
	TestLLObject * const stillAlive = r2;
	r2 = nullptr; // null the reference. the object is still kept alive by the list entry
	ASSERT_EQUAL(stillAlive->GetRefCount(), 1);
	
	ASSERT_NOT_NULL(stillAlive->entry.GetOwner());
	ASSERT_NOT_NULL(stillAlive->entry.GetList());
	
	ASSERT_EQUAL(list.GetCount(), 3);
	ASSERT_TRUE(list.Has(&r1->entry));
	ASSERT_TRUE(list.Has(&stillAlive->entry));
	ASSERT_TRUE(list.Has(&r3->entry));
}


// Tests for new functions in commit 1d5552384445f75f7cf67ed21a59f55c7127cae5
/////////////////////////////////////////////////////////////////////////

void detTLinkedList::TestVisit(){
	SetSubTestNum(6);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);

	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);

	// Test Visit with lvalue visitor
	int sum = 0;
	auto visitor = [&sum](TestLLObject *obj){
		sum += obj->id;
	};
	list.Visit(visitor);
	ASSERT_EQUAL(sum, 60);

	// Test Visit with rvalue visitor
	int count = 0;
	list.Visit([&count](TestLLObject *obj){
		count++;
	});
	ASSERT_EQUAL(count, 3);

	// Test Visit with condition check
	bool hasId20 = false;
	list.Visit([&hasId20](TestLLObject *obj){
		if(obj->id == 20){
			hasId20 = true;
		}
	});
	ASSERT_TRUE(hasId20);

	// Test Visit on empty list
	TestLLList emptyList;
	int emptyCount = 0;
	emptyList.Visit([&emptyCount](TestLLObject *obj){
		emptyCount++;
	});
	ASSERT_EQUAL(emptyCount, 0);
}

void detTLinkedList::TestFind(){
	SetSubTestNum(7);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);

	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);

	// Find with lvalue evaluator
	TestLLObject *found = nullptr;
	auto evaluator = [](TestLLObject *obj) -> bool {
		return obj->id == 20;
	};
	ASSERT_TRUE(list.Find(evaluator, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(found->id, 20);

	// Find with rvalue evaluator
	TestLLObject *found2 = nullptr;
	ASSERT_TRUE(list.Find([](TestLLObject *obj) -> bool {
		return obj->id == 30;
	}, found2));
	ASSERT_NOT_NULL(found2);
	ASSERT_EQUAL(found2->id, 30);

	// Find non-existent element
	TestLLObject *notFound = nullptr;
	ASSERT_FALSE(list.Find([](TestLLObject *obj) -> bool {
		return obj->id == 999;
	}, notFound));
	ASSERT_TRUE(notFound == nullptr);

	// Find first matching element (when multiple could match)
	TestLLObject *foundFirst = nullptr;
	ASSERT_TRUE(list.Find([](TestLLObject *obj) -> bool {
		return obj->id >= 10;
	}, foundFirst));
	ASSERT_EQUAL(foundFirst->id, 10); // first element matches

	// Find on empty list
	TestLLList emptyList;
	TestLLObject *emptyFound = nullptr;
	ASSERT_FALSE(emptyList.Find([](TestLLObject *obj) -> bool {
		return true;
	}, emptyFound));
	ASSERT_TRUE(emptyFound == nullptr);
}

void detTLinkedList::TestFindOrNull(){
	SetSubTestNum(8);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);

	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);

	// FindOrNull with lvalue evaluator
	auto evaluator = [](TestLLObject *obj) -> bool {
		return obj->id == 20;
	};
	const TestLLObject *found = list.FindOrNull(evaluator);
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(found->id, 20);

	// FindOrNull with rvalue evaluator
	TestLLObject *found2 = list.FindOrNull([](TestLLObject *obj) -> bool {
		return obj->id == 10;
	});
	ASSERT_NOT_NULL(found2);
	ASSERT_EQUAL(found2->id, 10);

	// FindOrNull with non-existent element
	TestLLObject *notFound = list.FindOrNull([](TestLLObject *obj) -> bool {
		return obj->id == 999;
	});
	ASSERT_TRUE(notFound == nullptr);

	// FindOrNull with complex condition
	TestLLObject *foundGreater = list.FindOrNull([](TestLLObject *obj) -> bool {
		return obj->id > 25;
	});
	ASSERT_NOT_NULL(foundGreater);
	ASSERT_EQUAL(foundGreater->id, 30);

	// FindOrNull on empty list
	TestLLList emptyList;
	TestLLObject *emptyFound = emptyList.FindOrNull([](TestLLObject *obj) -> bool {
		return true;
	});
	ASSERT_TRUE(emptyFound == nullptr);
}

void detTLinkedList::TestRemoveIf(){
	SetSubTestNum(9);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);
	TestLLObject::Ref d = TestLLObject::Ref::New(40);

	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	list.Add(&d->entry);
	ASSERT_EQUAL(list.GetCount(), 4);

	// RemoveIf with lvalue evaluator (remove elements with id < 25)
	auto evaluator = [](TestLLObject *obj) -> bool {
		return obj->id < 25;
	};
	list.RemoveIf(evaluator);
	ASSERT_EQUAL(list.GetCount(), 2);
	ASSERT_FALSE(list.Has(&a->entry));
	ASSERT_FALSE(list.Has(&b->entry));
	ASSERT_TRUE(list.Has(&c->entry));
	ASSERT_TRUE(list.Has(&d->entry));

	// RemoveIf with rvalue evaluator (remove specific element)
	list.RemoveIf([](TestLLObject *obj) -> bool {
		return obj->id == 30;
	});
	ASSERT_EQUAL(list.GetCount(), 1);
	ASSERT_FALSE(list.Has(&c->entry));
	ASSERT_TRUE(list.Has(&d->entry));

	// RemoveIf with no matches
	TestLLObject::Ref e = TestLLObject::Ref::New(50);
	list.Add(&e->entry);
	list.RemoveIf([](TestLLObject *obj) -> bool {
		return obj->id > 100;
	});
	ASSERT_EQUAL(list.GetCount(), 2);

	// RemoveIf all elements
	list.RemoveIf([](TestLLObject *obj) -> bool {
		return true;
	});
	ASSERT_TRUE(list.IsEmpty());

	// RemoveIf on empty list (should not crash)
	list.RemoveIf([](TestLLObject *obj) -> bool {
		return true;
	});
	ASSERT_TRUE(list.IsEmpty());

	// Test removing while iterating (important edge case)
	TestLLObject::Ref r1 = TestLLObject::Ref::New(1);
	TestLLObject::Ref r2 = TestLLObject::Ref::New(2);
	TestLLObject::Ref r3 = TestLLObject::Ref::New(3);
	TestLLObject::Ref r4 = TestLLObject::Ref::New(4);
	TestLLObject::Ref r5 = TestLLObject::Ref::New(5);
	TestLLList list2;
	list2.Add(&r1->entry);
	list2.Add(&r2->entry);
	list2.Add(&r3->entry);
	list2.Add(&r4->entry);
	list2.Add(&r5->entry);

	// Remove even IDs
	list2.RemoveIf([](TestLLObject *obj) -> bool {
		return obj->id % 2 == 0;
	});
	ASSERT_EQUAL(list2.GetCount(), 3);
	ASSERT_TRUE(list2.Has(&r1->entry));
	ASSERT_FALSE(list2.Has(&r2->entry));
	ASSERT_TRUE(list2.Has(&r3->entry));
	ASSERT_FALSE(list2.Has(&r4->entry));
	ASSERT_TRUE(list2.Has(&r5->entry));
}

void detTLinkedList::TestBidirectionalIterator(){
	SetSubTestNum(10);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);

	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);

	// Test basic forward iteration
	int count = 0;
	for(auto it = list.cbegin(); it != list.cend(); ++it){
		count++;
	}
	ASSERT_EQUAL(count, 3);

	// Test basic operator-> access
	auto it = list.cbegin();
	ASSERT_EQUAL(it->id, 10);

	// Test post-increment
	auto it2 = list.cbegin();
	auto it3 = it2++;
	ASSERT_TRUE(it3 == list.cbegin());
	ASSERT_TRUE(it2 != list.cbegin());

	// Test const list iteration
	const TestLLList &constList = list;
	int count2 = 0;
	for(auto it4 = constList.begin(); it4 != constList.end(); ++it4){
		count2++;
	}
	ASSERT_EQUAL(count2, 3);

	// Test post-decrement
	it2 = list.cend();
	it3 = it2--;
	ASSERT_TRUE(it3 == list.cend());
	ASSERT_TRUE(it2 != list.cend());

	// Test const list iteration
	count2 = 0;
	for(auto it4 = constList.end(); it4 != constList.begin(); --it4){
		count2++;
	}
	ASSERT_EQUAL(count2, 3);
}


// ============================================================================
// New Function Tests
// ============================================================================

void detTLinkedList::TestHasMatching(){
	SetSubTestNum(11);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	
	auto evaluator1 = [](TestLLObject *obj){ return obj->id == 20; };
	ASSERT_TRUE(list.HasMatching(evaluator1));
	
	auto evaluator2 = [](TestLLObject *obj){ return obj->id > 50; };
	ASSERT_FALSE(list.HasMatching(evaluator2));
	
	auto evaluator3 = [](TestLLObject *obj){ return obj->id < 15; };
	ASSERT_TRUE(list.HasMatching(evaluator3));
}

void detTLinkedList::TestFindReverse(){
	SetSubTestNum(12);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	
	TestLLObject *found = nullptr;
	auto evaluator1 = [](const TestLLObject *obj){ return obj->id == 20; };
	ASSERT_TRUE(list.FindReverse(evaluator1, found));
	ASSERT_NOT_NULL(found);
	ASSERT_EQUAL(found->id, 20);
	
	found = nullptr;
	auto evaluator2 = [](const TestLLObject *obj){ return obj->id > 50; };
	ASSERT_FALSE(list.FindReverse(evaluator2, found));
	ASSERT_NULL(found);
	
	// FindReverse should find last matching element
	found = nullptr;
	auto evaluator3 = [](const TestLLObject *obj){ return obj->id >= 10; };
	ASSERT_TRUE(list.FindReverse(evaluator3, found));
	ASSERT_EQUAL(found->id, 30);
}

void detTLinkedList::TestFindReverseOrNull(){
	SetSubTestNum(13);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	
	auto evaluator1 = [](TestLLObject *obj){ return obj->id == 20; };
	TestLLObject *found1 = list.FindReverseOrNull(evaluator1);
	ASSERT_NOT_NULL(found1);
	ASSERT_EQUAL(found1->id, 20);
	
	auto evaluator2 = [](TestLLObject *obj){ return obj->id > 50; };
	TestLLObject *found2 = list.FindReverseOrNull(evaluator2);
	ASSERT_NULL(found2);
	
	// FindReverse should find last matching element
	auto evaluator3 = [](TestLLObject *obj){ return obj->id >= 10; };
	TestLLObject *found3 = list.FindReverseOrNull(evaluator3);
	ASSERT_NOT_NULL(found3);
	ASSERT_EQUAL(found3->id, 30);
	
	// Test with default value
	TestLLObject::Ref defaultObj = TestLLObject::Ref::New(999);
	TestLLObject *found4 = list.FindReverseOrNull(evaluator2, defaultObj);
	ASSERT_NOT_NULL(found4);
	ASSERT_EQUAL(found4->id, 999);
}

void detTLinkedList::TestInject(){
	SetSubTestNum(15);

	TestLLObject::Ref a = TestLLObject::Ref::New(10);
	TestLLObject::Ref b = TestLLObject::Ref::New(20);
	TestLLObject::Ref c = TestLLObject::Ref::New(30);
	
	TestLLList list;
	list.Add(&a->entry);
	list.Add(&b->entry);
	list.Add(&c->entry);
	
	// Test with int accumulator
	auto combiner = [](int acc, TestLLObject *val){ 
		return acc + val->id; 
	};
	ASSERT_EQUAL(list.Inject(100, combiner), 160);
	
	TestLLList emptyList;
	ASSERT_EQUAL(emptyList.Inject(100, combiner), 100);
	
	// Test counting elements
	auto counter = [](int acc, TestLLObject *){ 
		return acc + 1; 
	};
	ASSERT_EQUAL(list.Inject(0, counter), 3);
}