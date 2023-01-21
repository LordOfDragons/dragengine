/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delPatch.h"
#include "delPatchList.h"

#include <dragengine/common/exceptions.h>



// Class delPatchList
////////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatchList::delPatchList(){
}

delPatchList::delPatchList( const delPatchList &list ) :
pPatches( list.pPatches ){
}

delPatchList::~delPatchList(){
	RemoveAll();
}



// Management
///////////////

int delPatchList::GetCount() const{
	return pPatches.GetCount();
}

delPatch *delPatchList::GetAt( int index ) const{
	return ( delPatch* )pPatches.GetAt( index );
}

delPatch *delPatchList::GetWithID( const decUuid &id ) const{
	DEASSERT_TRUE( id )
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delPatch * const patch = ( delPatch* )pPatches.GetAt( i );
		if( patch->GetIdentifier() == id ){
			return patch;
		}
	}
	
	return nullptr;
}

bool delPatchList::Has( delPatch *patch ) const{
	return pPatches.Has( patch );
}

bool delPatchList::HasWithID( const decUuid &id ) const{
	DEASSERT_TRUE( id )
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delPatch &patch = *( ( delPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int delPatchList::IndexOf( delPatch *patch ) const{
	return pPatches.IndexOf( patch );
}

int delPatchList::IndexOfWithID( const decUuid &id ) const{
	DEASSERT_TRUE( id )
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delPatch &patch = *( ( delPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void delPatchList::Add( delPatch *patch ){
	DEASSERT_NOTNULL( patch )
	DEASSERT_FALSE( HasWithID( patch->GetIdentifier() ) )
	
	pPatches.Add( patch );
}

void delPatchList::AddAll( const delPatchList &list ){
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delPatch * const patch = ( delPatch* )list.GetAt( i );
		if( ! HasWithID( patch->GetIdentifier() ) ){
			pPatches.Add( patch );
		}
	}
}

void delPatchList::Remove( delPatch *patch ){
	pPatches.RemoveFrom( pPatches.IndexOf( patch ) );
}

void delPatchList::RemoveAll(){
	pPatches.RemoveAll();
}
