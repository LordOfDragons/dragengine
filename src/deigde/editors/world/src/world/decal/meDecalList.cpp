/* 
 * Drag[en]gine IGDE World Editor
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

#include "meDecal.h"
#include "meDecalList.h"

#include <dragengine/common/exceptions.h>



// Class meDecalList
//////////////////////

// Constructor, destructor
////////////////////////////

meDecalList::meDecalList(){
}

meDecalList::~meDecalList(){
}



// Management
///////////////

int meDecalList::GetCount() const{
	return pObjects.GetCount();
}

meDecal *meDecalList::GetAt( int position ) const{
	return ( meDecal* )pObjects.GetAt( position );
}

int meDecalList::IndexOf( meDecal *decal ) const{
	return pObjects.IndexOf( decal );
}

bool meDecalList::Has( meDecal *decal ) const{
	return pObjects.Has( decal );
}

void meDecalList::Add( meDecal *decal ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects.Add( decal );
}

void meDecalList::AddIfAbsent( meDecal *decal ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects.AddIfAbsent( decal );
}

void meDecalList::Remove( meDecal *decal ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects.Remove( decal );
}

void meDecalList::RemoveIfPresent( meDecal *decal ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects.RemoveIfPresent( decal );
}

void meDecalList::RemoveAll(){
	pObjects.RemoveAll();
}



meDecalList &meDecalList::operator=( const meDecalList &list ){
	pObjects = list.pObjects;
	return *this;
}
