/* 
 * Drag[en]gine GUI Launcher
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

#include "deglPatch.h"
#include "deglPatchList.h"

#include <dragengine/common/exceptions.h>



// Class deglPatchList
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglPatchList::deglPatchList(){
}

deglPatchList::deglPatchList( const deglPatchList &list ) :
pPatches( list.pPatches ){
}

deglPatchList::~deglPatchList(){
	RemoveAll();
}



// Management
///////////////

int deglPatchList::GetCount() const{
	return pPatches.GetCount();
}

deglPatch *deglPatchList::GetAt( int index ) const{
	return ( deglPatch* )pPatches.GetAt( index );
}

deglPatch *deglPatchList::GetWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deglPatch * const patch = ( deglPatch* )pPatches.GetAt( i );
		if( patch->GetIdentifier() == id ){
			return patch;
		}
	}
	
	return NULL;
}

bool deglPatchList::Has( deglPatch *patch ) const{
	return pPatches.Has( patch );
}

bool deglPatchList::HasWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglPatch &patch = *( ( deglPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return true;
		}
	}
	
	return false;
}

int deglPatchList::IndexOf( deglPatch *patch ) const{
	return pPatches.IndexOf( patch );
}

int deglPatchList::IndexOfWithID( const decUuid &id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pPatches.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deglPatch &patch = *( ( deglPatch* )pPatches.GetAt( i ) );
		if( patch.GetIdentifier() == id ){
			return i;
		}
	}
	
	return -1;
}

void deglPatchList::Add( deglPatch *patch ){
	if( ! patch || HasWithID( patch->GetIdentifier() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pPatches.Add( patch );
}

void deglPatchList::Remove( deglPatch *patch ){
	const int index = IndexOf( patch );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pPatches.RemoveFrom( index );
}

void deglPatchList::RemoveAll(){
	pPatches.RemoveAll();
}
