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

#include "lpeLangPackEntry.h"
#include "lpeLangPackEntryList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class lpeLangPackEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackEntryList::lpeLangPackEntryList(){
}

lpeLangPackEntryList::lpeLangPackEntryList( const lpeLangPackEntryList &list ) :
pEntries( list.pEntries ){
}

lpeLangPackEntryList::~lpeLangPackEntryList(){
}



// Management
///////////////

int lpeLangPackEntryList::GetCount() const{
	return pEntries.GetCount();
}

lpeLangPackEntry *lpeLangPackEntryList::GetAt( int index ) const{
	return ( lpeLangPackEntry* )pEntries.GetAt( index );
}

lpeLangPackEntry *lpeLangPackEntryList::GetNamed( const char *name ) const{
	const int count = pEntries.GetCount();
	lpeLangPackEntry *entry;
	int p;
	
	for( p=0; p<count; p++ ){
		entry = ( lpeLangPackEntry* )pEntries.GetAt( p );
		
		if( entry->GetName().Equals( name ) ){
			return entry;
		}
	}
	
	return NULL;
}

int lpeLangPackEntryList::IndexOf( lpeLangPackEntry *entry ) const{
	return pEntries.IndexOf( entry );
}

int lpeLangPackEntryList::IndexOfNamed( const char *name ) const{
	const int count = pEntries.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( lpeLangPackEntry* )pEntries.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool lpeLangPackEntryList::Has( lpeLangPackEntry *entry ) const{
	return pEntries.Has( entry );
}

bool lpeLangPackEntryList::HasNamed( const char *name ) const{
	const int count = pEntries.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( lpeLangPackEntry* )pEntries.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void lpeLangPackEntryList::Add( lpeLangPackEntry *entry ){
	if( ! entry || HasNamed( entry->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pEntries.Add( entry );
}

void lpeLangPackEntryList::InsertAt( lpeLangPackEntry *entry, int index ){
	if( ! entry || HasNamed( entry->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pEntries.Insert( entry, index );
}

void lpeLangPackEntryList::MoveTo( lpeLangPackEntry *entry, int index ){
	pEntries.Move( entry, index );
}

void lpeLangPackEntryList::Remove( lpeLangPackEntry *entry ){
	pEntries.Remove( entry );
}

void lpeLangPackEntryList::RemoveAll(){
	pEntries.RemoveAll();
}



lpeLangPackEntryList &lpeLangPackEntryList::operator=( const lpeLangPackEntryList &list ){
	pEntries = list.pEntries;
	return *this;
}
