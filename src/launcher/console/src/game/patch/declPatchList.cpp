/* 
 * Drag[en]gine Console Launcher
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

#include "declPatch.h"
#include "declPatchList.h"

#include <dragengine/common/exceptions.h>



// Class declPatchList
////////////////////////

// Constructors and Destructors
/////////////////////////////////

declPatchList::declPatchList(){
}

declPatchList::declPatchList( const declPatchList &list ) :
pPatches( list.pPatches ){
}

declPatchList::~declPatchList(){
	RemoveAll();
}



// Management
///////////////

int declPatchList::GetCount() const{
	return pPatches.GetCount();
}

declPatch *declPatchList::GetAt( int index ) const{
	return ( declPatch* )pPatches.GetAt( index );
}

declPatch *declPatchList::GetWithID( const decUuid &id ) const{
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		declPatch * const patch = ( declPatch* )pPatches.GetAt( i );
		if( patch->GetIdentifier() == id ){
			return patch;
		}
	}
	
	return NULL;
}

bool declPatchList::Has( declPatch *patch ) const{
	return pPatches.Has( patch );
}

bool declPatchList::HasWithID( const decUuid &id ) const{
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declPatch &patch = *( ( declPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int declPatchList::IndexOf( declPatch *patch ) const{
	return pPatches.IndexOf( patch );
}

int declPatchList::IndexOfWithID( const decUuid &id ) const{
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const declPatch &patch = *( ( declPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void declPatchList::Add( declPatch *patch ){
	if( ! patch || HasWithID( patch->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pPatches.Add( patch );
}

void declPatchList::Remove( declPatch *patch ){
	const int index = IndexOf( patch );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPatches.RemoveFrom( index );
}

void declPatchList::RemoveAll(){
	pPatches.RemoveAll();
}
