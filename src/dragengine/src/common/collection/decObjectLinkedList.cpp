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

#include "decObjectLinkedList.h"

#include "../exceptions.h"
#include "../../deObject.h"



// Class decObjectLinkedList::cListEntry
//////////////////////////////////////////

decObjectLinkedList::cListEntry::cListEntry( deObject *owner ) :
pOwner( owner ),
pPrev( NULL ),
pNext( NULL ),
pList( NULL )
{
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
}

decObjectLinkedList::cListEntry::~cListEntry(){
	if( pList ){
		pList->Remove( this );
	}
}

void decObjectLinkedList::cListEntry::SetPrevious( cListEntry *previous ){
	pPrev = previous;
}

void decObjectLinkedList::cListEntry::SetNext( cListEntry *next ){
	pNext = next;
}

void decObjectLinkedList::cListEntry::SetList( decObjectLinkedList *list ){
	pList = list;
}

void decObjectLinkedList::cListEntry::Clear(){
	pList = NULL;
	pPrev = NULL;
	pNext = NULL;
}



// Class decObjectLinkedList::cIterator
//////////////////////////////////////////

namespace {
	static decObjectLinkedList &decObjectLinkedList_cIterator_List(
	const decObjectLinkedList::cListEntry *entry ){
		if( ! entry || ! entry->GetList() ){
			DETHROW( deeInvalidParam );
		}
		return *entry->GetList();
	}
}

decObjectLinkedList::cIterator::cIterator( decObjectLinkedList &list ) :
pList( list ),
pEntry( NULL ),
pNextEntry( list.GetRoot() ),
pLastEntry( list.GetTail() ){
}

decObjectLinkedList::cIterator::cIterator( decObjectLinkedList::cListEntry *firstEntry ) :
pList( decObjectLinkedList_cIterator_List( firstEntry ) ),
pEntry( NULL ),
pNextEntry( firstEntry ),
pLastEntry( pList.GetTail() ){
}

decObjectLinkedList::cIterator::cIterator( decObjectLinkedList::cListEntry *firstEntry,
decObjectLinkedList::cListEntry *lastEntry ) :
pList( decObjectLinkedList_cIterator_List( firstEntry ) ),
pEntry( NULL ),
pNextEntry( firstEntry ),
pLastEntry( lastEntry )
{
	if( ! lastEntry || lastEntry->GetList() != firstEntry->GetList() ){
		DETHROW( deeInvalidParam );
	}
}

deObject &decObjectLinkedList::cIterator::Entry() const{
	if( ! pEntry ){
		DETHROW( deeInvalidParam );
	}
	return *pEntry;
}

bool decObjectLinkedList::cIterator::Advance(){
	if( pNextEntry ){
		pEntry = pNextEntry->GetOwner();
		
		if( pNextEntry != pLastEntry ){
			pNextEntry = pNextEntry->GetNext();
			
		}else{
			pNextEntry = NULL;
		}
		
	}else{
		pEntry = NULL;
	}
	
	return pEntry != NULL;
}

bool decObjectLinkedList::cIterator::HasFinished() const{
	return pEntry == NULL;
}

bool decObjectLinkedList::cIterator::HasNotFinished() const{
	return pEntry != NULL;
}

decObjectLinkedList::cIterator::operator deObject *() const{
	if( ! pEntry ){
		DETHROW( deeInvalidParam );
	}
	return pEntry;
}

decObjectLinkedList::cIterator::operator deObject &() const{
	if( ! pEntry ){
		DETHROW( deeInvalidParam );
	}
	return *pEntry;
}

deObject *decObjectLinkedList::cIterator::operator->() const{
	if( ! pEntry ){
		DETHROW( deeInvalidParam );
	}
	return pEntry;
}

bool decObjectLinkedList::cIterator::operator++(){
	return Advance();
}

bool decObjectLinkedList::cIterator::operator+=( int distance ){
	if( distance < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	bool result = HasNotFinished();
	while( distance-- > 0 ){
		result = Advance();
	}
	return result;
}

bool decObjectLinkedList::cIterator::operator!() const{
	return pEntry == NULL;
}

decObjectLinkedList::cIterator::operator bool() const{
	return pEntry != NULL;
}



// Class decObjectLinkedList
//////////////////////////////

// Constructor, destructor
////////////////////////////

decObjectLinkedList::decObjectLinkedList() :
pRoot( NULL ),
pTail( NULL ),
pCount( 0 ){
}

decObjectLinkedList::~decObjectLinkedList(){
	RemoveAll();
}



// Management
///////////////

decObjectLinkedList::cListEntry *decObjectLinkedList::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	cListEntry *entry = pRoot;
	while( index-- > 0 ){
		entry = entry->GetNext();
	}
	
	return entry;
}

int decObjectLinkedList::IndexOf( cListEntry *entry ) const{
	if( ! entry || entry->GetList() != this ){
		return -1;
	}
	
	int index = 0;
	cListEntry *find = pRoot;
	while( find != entry ){
		index++;
		find = find->GetNext();
	}
	
	return index;
}

bool decObjectLinkedList::Has( cListEntry *entry ) const{
	return entry && entry->GetList() == this;
}

void decObjectLinkedList::Add( cListEntry *entry ){
	if( ! entry || entry->GetList() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTail ){
		pTail->SetNext( entry );
		entry->SetPrevious( pTail );
		entry->SetNext( NULL );
		pTail = entry;
		
	}else{ // count is 0, root is NULL, tail is NULL
		pTail = entry;
		pRoot = entry;
		entry->SetPrevious( NULL );
		entry->SetNext( NULL );
	}
	
	entry->SetList( this );
	entry->GetOwner()->AddReference();
	pCount++;
}

void decObjectLinkedList::InsertAfter( cListEntry *entry, cListEntry *after ){
	if( ! entry || entry->GetList() || ! after || after->GetList() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( after->GetNext() ){
		entry->SetPrevious( after );
		entry->SetNext( after->GetNext() );
		after->GetNext()->SetPrevious( entry );
		after->SetNext( entry );
		
	}else{  // after is tail
		pTail->SetNext( entry );
		entry->SetPrevious( pTail );
		entry->SetNext( NULL );
		pTail = entry;
	}
	
	entry->SetList( this );
	entry->GetOwner()->AddReference();
	pCount++;
}

void decObjectLinkedList::InsertBefore( cListEntry *entry, cListEntry *before ){
	if( ! entry || entry->GetList() || ! before || before->GetList() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( before->GetPrevious() ){
		entry->SetNext( before );
		entry->SetPrevious( before->GetPrevious() );
		before->GetPrevious()->SetNext( entry );
		before->SetPrevious( entry );
		
	}else{  // before is root
		pRoot->SetPrevious( entry );
		entry->SetPrevious( NULL );
		entry->SetNext( pRoot );
		pRoot = entry;
	}
	
	entry->SetList( this );
	entry->GetOwner()->AddReference();
	pCount++;
}

void decObjectLinkedList::Remove( cListEntry *entry ){
	if( ! entry || entry->GetList() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( entry->GetPrevious() ){
		entry->GetPrevious()->SetNext( entry->GetNext() );
		
	}else{ // entry is root
		pRoot = entry->GetNext();
		// entry->GetNext() check will set new root previous to NULL
	}
	
	if( entry->GetNext() ){
		entry->GetNext()->SetPrevious( entry->GetPrevious() );
		
	}else{ // entry is tail
		pTail = entry->GetPrevious();
		// entry->GetPrevious() check has set new tail next to NULL already
	}
	
	entry->Clear();
	entry->GetOwner()->FreeReference();
	pCount--;
}

void decObjectLinkedList::RemoveAll(){
	while( pRoot ){
		cListEntry &clearEntry = *pRoot;
		pRoot = pRoot->GetNext();
		clearEntry.Clear();
		clearEntry.GetOwner()->FreeReference();
	}
	
	pTail = NULL;
	pCount = 0;
}



// Operators
//////////////

decObjectLinkedList::cListEntry *decObjectLinkedList::operator[]( int index ) const{
	return GetAt( index );
}
