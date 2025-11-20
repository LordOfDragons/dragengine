/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "meCLHitList.h"
#include "meCLHitListEntry.h"

#include <dragengine/common/exceptions.h>


// Class meCLHitList
//////////////////////

// Management
///////////////

int meCLHitList::GetEntryCount() const{
	return pEntries.GetCount();
}

meCLHitListEntry *meCLHitList::GetEntryAt(int index) const{
	return (meCLHitListEntry*)pEntries.GetAt(index);
}

int meCLHitList::IndexOfEntry(meCLHitListEntry *entry) const{
	return pEntries.IndexOf(entry);
}

bool meCLHitList::HasEntry(meCLHitListEntry *entry) const{
	return pEntries.Has(entry);
}

void meCLHitList::AddEntry(meCLHitListEntry *entry){
	DEASSERT_FALSE(pEntries.Has(entry))
	
	pEntries.Add(entry);
}

void meCLHitList::RemoveEntry(meCLHitListEntry *entry){
	pEntries.Remove(entry);
}

void meCLHitList::RemoveAllEntries(){
	pEntries.RemoveAll();
}


class BridgeComparator : public decObjectComparator{
	meCLHitList::Comparator &pComparator;
	
public:
	BridgeComparator(meCLHitList::Comparator &comparator) : pComparator(comparator){}
	
	int operator() (deObject *a, deObject *b) override{
		return pComparator(*(meCLHitListEntry*)a, *(meCLHitListEntry*)b);
	}
};

class BridgeVisitor : public decObjectVisitor{
	meCLHitList::Visitor &pVisitor;
	
public:
	BridgeVisitor(meCLHitList::Visitor &visitor) : pVisitor(visitor){}
	
	void operator() (deObject *object) override{
		pVisitor(*(meCLHitListEntry*)object);
	}
};

class BridgeEvaluator : public decObjectEvaluator{
	meCLHitList::Evaluator &pEvaluator;
	
public:
	BridgeEvaluator(meCLHitList::Evaluator &evaluator) : pEvaluator(evaluator){}
	
	bool operator() (deObject *object) override{
		return pEvaluator(*(meCLHitListEntry*)object);
	}
};

void meCLHitList::Visit(Visitor &visitor, int from, int to, int step) const{
	BridgeVisitor bridge(visitor);
	pEntries.Visit(bridge, from, to, step);
}

bool meCLHitList::Find(Evaluator &evaluator, deObject *&found, int from, int to, int step) const{
	BridgeEvaluator bridge(evaluator);
	return pEntries.Find(bridge, found, from, to, step);
}

meCLHitListEntry *meCLHitList::FindSame(const meCLHitListEntry &entry) const{
	const int count = pEntries.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meCLHitListEntry * const check = (meCLHitListEntry*)pEntries.GetAt(i);
		if(check->IsSame(entry)){
			return check;
		}
	}
	
	return nullptr;
}

meCLHitList meCLHitList::Collect(Evaluator &evaluator, int from, int to, int step) const{
	meCLHitList list;
	BridgeEvaluator bridge(evaluator);
	list.pEntries = pEntries.Collect(bridge, from, to, step);
	return list;
}

void meCLHitList::RemoveIf(Evaluator &evaluator, int from, int to, int step){
	BridgeEvaluator bridge(evaluator);
	pEntries.RemoveIf(bridge, from, to, step);
}

class RemoveDuplicatesEvaluator : public decObjectEvaluator{
	const decObjectOrderedSet &pList;
	
public:
	RemoveDuplicatesEvaluator(decObjectOrderedSet &list) : pList(list){}
	
	bool operator() (deObject *entry) override{
		const meCLHitListEntry &he = *((meCLHitListEntry*)entry);
		const int count = pList.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			const deObject * const check = pList.GetAt(i);
			if(check == entry){
				return false;
			}
			if(((meCLHitListEntry*)check)->IsSame(he)){
				return true;
			}
		}
		return false;
	}
};

void meCLHitList::RemoveDuplicates(){
	RemoveDuplicatesEvaluator evaluator(pEntries);
	pEntries.RemoveIf(evaluator);
}

void meCLHitList::Sort(Comparator &comparator){
	BridgeComparator bridge(comparator);
	pEntries.Sort(bridge);
}

class DistanceComparator : public decObjectComparator{
public:
	DistanceComparator() = default;
	
	int operator() (deObject *a, deObject *b) override{
		return ((meCLHitListEntry*)a)->CompareTo(*(meCLHitListEntry*)b);
	}
};

void meCLHitList::SortByDistance(){
	DistanceComparator comparator;
	pEntries.Sort(comparator);
}
