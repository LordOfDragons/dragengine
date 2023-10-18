/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "ceLangPack.h"
#include "ceLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class ceLangPack
/////////////////////

// Constructor, destructor
////////////////////////////

ceLangPack::ceLangPack( const char *path ) :
pPath( path ),
pChanged( false ){
}

ceLangPack::~ceLangPack(){
}



// Management
///////////////

void ceLangPack::SetIdentifier( const char *identifier ){
	pIdentifier = identifier;
}

void ceLangPack::SetName( const decUnicodeString &name ){
	pName = name;
}

void ceLangPack::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void ceLangPack::SetMissingText( const decUnicodeString &missingText ){
	pMissingText = missingText;
}

void ceLangPack::SetChanged( bool changed ){
	pChanged = changed;
}



// Entries
/////////////

int ceLangPack::GetEntryCount() const{
	return pEntries.GetCount();
}

ceLangPackEntry *ceLangPack::GetEntryAt( int index ) const{
	return ( ceLangPackEntry* )pEntries.GetAt( index );
}

ceLangPackEntry *ceLangPack::GetEntryNamed ( const char *name ) const{
	const int count = pEntries.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceLangPackEntry * const entry = ( ceLangPackEntry* )pEntries.GetAt( i );
		if( entry->GetName() == name ){
			return entry;
		}
	}
	
	return nullptr;
}

void ceLangPack::AddEntry( ceLangPackEntry *entry ){
	DEASSERT_NOTNULL( entry )
	pEntries.Add( entry );
}

void ceLangPack::RemoveEntry( ceLangPackEntry *entry ){
	pEntries.Remove( entry );
}

void ceLangPack::RemoveAllEntries(){
	pEntries.RemoveAll();
}

void ceLangPack::GetEntryNames( decStringList &list ) const{
	const int count = pEntries.GetCount();
	int i;
	
	list.RemoveAll();
	for( i=0; i<count; i++ ){
		list.Add( ( ( ceLangPackEntry* )pEntries.GetAt( i ) )->GetName() );
	}
	list.SortAscending();
}
