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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decPointerLinkedList.h"

#include "../exceptions.h"



// Class decPointerLinkedList::cListEntry
///////////////////////////////////////////

decPointerLinkedList::cListEntry::cListEntry(void *owner) :
pOwner(owner),
pPrev(NULL),
pNext(NULL),
pList(NULL)
{
	if(! owner){
		DETHROW(deeInvalidParam);
	}
}

decPointerLinkedList::cListEntry::~cListEntry(){
	if(pList){
		pList->Remove(this);
	}
}

void decPointerLinkedList::cListEntry::SetPrevious(cListEntry *previous){
	pPrev = previous;
}

void decPointerLinkedList::cListEntry::SetNext(cListEntry *next){
	pNext = next;
}

void decPointerLinkedList::cListEntry::SetList(decPointerLinkedList *list){
	pList = list;
}

void decPointerLinkedList::cListEntry::Clear(){
	pList = NULL;
	pPrev = NULL;
	pNext = NULL;
}



// Class decPointerLinkedList::cIterator
//////////////////////////////////////////

namespace {
	static decPointerLinkedList &decPointerLinkedList_cIterator_List(
	const decPointerLinkedList::cListEntry *entry){
		if(! entry || ! entry->GetList()){
			DETHROW(deeInvalidParam);
		}
		return *entry->GetList();
	}
}

decPointerLinkedList::cIterator::cIterator(decPointerLinkedList &list) :
pList(list),
pEntry(NULL),
pNextEntry(list.GetRoot()),
pLastEntry(list.GetTail()){
}

decPointerLinkedList::cIterator::cIterator(decPointerLinkedList::cListEntry *firstEntry) :
pList(decPointerLinkedList_cIterator_List(firstEntry)),
pEntry(NULL),
pNextEntry(firstEntry),
pLastEntry(pList.GetTail()){
}

decPointerLinkedList::cIterator::cIterator(decPointerLinkedList::cListEntry *firstEntry,
decPointerLinkedList::cListEntry *lastEntry) :
pList(decPointerLinkedList_cIterator_List(firstEntry)),
pEntry(NULL),
pNextEntry(firstEntry),
pLastEntry(lastEntry)
{
	if(! lastEntry || lastEntry->GetList() != firstEntry->GetList()){
		DETHROW(deeInvalidParam);
	}
}

void *decPointerLinkedList::cIterator::Entry() const{
	if(! pEntry){
		DETHROW(deeInvalidParam);
	}
	return pEntry;
}

bool decPointerLinkedList::cIterator::Advance(){
	if(pNextEntry){
		pEntry = pNextEntry->GetOwner();
		
		if(pNextEntry != pLastEntry){
			pNextEntry = pNextEntry->GetNext();
			
		}else{
			pNextEntry = NULL;
		}
		
	}else{
		pEntry = NULL;
	}
	
	return pEntry != NULL;
}

bool decPointerLinkedList::cIterator::HasFinished() const{
	return pEntry == NULL;
}

bool decPointerLinkedList::cIterator::HasNotFinished() const{
	return pEntry != NULL;
}

decPointerLinkedList::cIterator::operator void *() const{
	return Entry();
}

void *decPointerLinkedList::cIterator::operator->() const{
	return Entry();
}

bool decPointerLinkedList::cIterator::operator++(){
	return Advance();
}

bool decPointerLinkedList::cIterator::operator+=(int distance){
	if(distance < 0){
		DETHROW(deeInvalidParam);
	}
	
	bool result = HasNotFinished();
	while(distance-- > 0){
		result = Advance();
	}
	return result;
}

bool decPointerLinkedList::cIterator::operator!() const{
	return pEntry == NULL;
}

decPointerLinkedList::cIterator::operator bool() const{
	return pEntry != NULL;
}



// Class decPointerLinkedList
///////////////////////////////

// Constructor, destructor
////////////////////////////

decPointerLinkedList::decPointerLinkedList() :
pRoot(NULL),
pTail(NULL),
pCount(0){
}

decPointerLinkedList::~decPointerLinkedList(){
	RemoveAll();
}



// Management
///////////////

decPointerLinkedList::cListEntry *decPointerLinkedList::GetAt(int index) const{
	if(index < 0 || index >= pCount){
		DETHROW(deeInvalidParam);
	}
	
	cListEntry *entry = pRoot;
	while(index-- > 0){
		entry = entry->GetNext();
	}
	
	return entry;
}

int decPointerLinkedList::IndexOf(cListEntry *entry) const{
	if(! entry || entry->GetList() != this){
		return -1;
	}
	
	int index = 0;
	cListEntry *find = pRoot;
	while(find != entry){
		index++;
		find = find->GetNext();
	}
	
	return index;
}

bool decPointerLinkedList::Has(cListEntry *entry) const{
	return entry && entry->GetList() == this;
}

void decPointerLinkedList::Add(cListEntry *entry){
	if(! entry || entry->GetList()){
		DETHROW(deeInvalidParam);
	}
	
	if(pTail){
		pTail->SetNext(entry);
		entry->SetPrevious(pTail);
		entry->SetNext(NULL);
		pTail = entry;
		
	}else{ // count is 0, root is NULL, tail is NULL
		pTail = entry;
		pRoot = entry;
		entry->SetPrevious(NULL);
		entry->SetNext(NULL);
	}
	
	entry->SetList(this);
	pCount++;
}

void decPointerLinkedList::InsertAfter(cListEntry *entry, cListEntry *after){
	if(! entry || entry->GetList() || ! after || after->GetList() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(after->GetNext()){
		entry->SetPrevious(after);
		entry->SetNext(after->GetNext());
		after->GetNext()->SetPrevious(entry);
		after->SetNext(entry);
		
	}else{  // after is tail
		pTail->SetNext(entry);
		entry->SetPrevious(pTail);
		entry->SetNext(NULL);
		pTail = entry;
	}
	
	entry->SetList(this);
	pCount++;
}

void decPointerLinkedList::InsertBefore(cListEntry *entry, cListEntry *before){
	if(! entry || entry->GetList() || ! before || before->GetList() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(before->GetPrevious()){
		entry->SetNext(before);
		entry->SetPrevious(before->GetPrevious());
		before->GetPrevious()->SetNext(entry);
		before->SetPrevious(entry);
		
	}else{  // before is root
		pRoot->SetPrevious(entry);
		entry->SetPrevious(NULL);
		entry->SetNext(pRoot);
		pRoot = entry;
	}
	
	entry->SetList(this);
	pCount++;
}

void decPointerLinkedList::Remove(cListEntry *entry){
	if(! entry || entry->GetList() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(entry->GetPrevious()){
		entry->GetPrevious()->SetNext(entry->GetNext());
		
	}else{ // entry is root
		pRoot = entry->GetNext();
		// entry->GetNext() check will set new root previous to NULL
	}
	
	if(entry->GetNext()){
		entry->GetNext()->SetPrevious(entry->GetPrevious());
		
	}else{ // entry is tail
		pTail = entry->GetPrevious();
		// entry->GetPrevious() check has set new tail next to NULL already
	}
	
	entry->Clear();
	pCount--;
}

void decPointerLinkedList::RemoveAll(){
	while(pRoot){
		cListEntry &clearEntry = *pRoot;
		pRoot = pRoot->GetNext();
		clearEntry.Clear();
	}
	
	pTail = NULL;
	pCount = 0;
}



// Operators
//////////////

decPointerLinkedList::cListEntry *decPointerLinkedList::operator[](int index) const{
	return GetAt(index);
}
