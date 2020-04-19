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

#include "meNavigationSpace.h"
#include "meNavigationSpaceList.h"

#include <dragengine/common/exceptions.h>



// Class meNavigationSpaceList
////////////////////////////////

// Constructor, destructor
////////////////////////////

meNavigationSpaceList::meNavigationSpaceList(){
}

meNavigationSpaceList::~meNavigationSpaceList(){
}



// Management
///////////////

int meNavigationSpaceList::GetCount() const{
	return pNavSpaces.GetCount();
}

meNavigationSpace *meNavigationSpaceList::GetAt( int position ) const{
	return ( meNavigationSpace* )pNavSpaces.GetAt( position );
}

int meNavigationSpaceList::IndexOf( meNavigationSpace *navspaces ) const{
	return pNavSpaces.IndexOf( navspaces );
}

bool meNavigationSpaceList::Has( meNavigationSpace *navspaces ){
	return pNavSpaces.Has( navspaces );
}

void meNavigationSpaceList::Add( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.Add( navspaces );
}

void meNavigationSpaceList::AddIfAbsent( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.AddIfAbsent( navspaces );
}

void meNavigationSpaceList::Remove( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.Remove( navspaces );
}

void meNavigationSpaceList::RemoveIfPresent( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.RemoveIfPresent( navspaces );
}

void meNavigationSpaceList::RemoveAll(){
	pNavSpaces.RemoveAll();
}
