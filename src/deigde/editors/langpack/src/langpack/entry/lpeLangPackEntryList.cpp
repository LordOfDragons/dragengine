/* 
 * Drag[en]gine IGDE Language Pack Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
