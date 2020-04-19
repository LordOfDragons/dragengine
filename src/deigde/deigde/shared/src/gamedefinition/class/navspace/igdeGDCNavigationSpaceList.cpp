/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCNavigationSpace.h"
#include "igdeGDCNavigationSpaceList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationSpace
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationSpaceList::igdeGDCNavigationSpaceList(){
}

igdeGDCNavigationSpaceList::igdeGDCNavigationSpaceList( const igdeGDCNavigationSpaceList &list ){
	pNavigationSpaces = list.pNavigationSpaces;
}

igdeGDCNavigationSpaceList::~igdeGDCNavigationSpaceList(){
}



// Management
///////////////

int igdeGDCNavigationSpaceList::GetCount() const{
	return pNavigationSpaces.GetCount();
}

igdeGDCNavigationSpace *igdeGDCNavigationSpaceList::GetAt( int index ) const{
	return ( igdeGDCNavigationSpace* )pNavigationSpaces.GetAt( index );
}

int igdeGDCNavigationSpaceList::IndexOf( igdeGDCNavigationSpace *navSpace ) const{
	return pNavigationSpaces.IndexOf( navSpace );
}

bool igdeGDCNavigationSpaceList::Has( igdeGDCNavigationSpace *navSpace ) const{
	return pNavigationSpaces.Has( navSpace );
}

void igdeGDCNavigationSpaceList::Add( igdeGDCNavigationSpace *navSpace ){
	if( ! navSpace ){
		DETHROW( deeInvalidParam );
	}
	
	pNavigationSpaces.Add( navSpace );
}

void igdeGDCNavigationSpaceList::InsertAt( igdeGDCNavigationSpace *navSpace, int index ){
	if( ! navSpace ){
		DETHROW( deeInvalidParam );
	}
	
	pNavigationSpaces.Insert( navSpace, index );
}

void igdeGDCNavigationSpaceList::MoveTo( igdeGDCNavigationSpace *navSpace, int index ){
	pNavigationSpaces.Move( navSpace, index );
}

void igdeGDCNavigationSpaceList::Remove( igdeGDCNavigationSpace *navSpace ){
	pNavigationSpaces.Remove( navSpace );
}

void igdeGDCNavigationSpaceList::RemoveAll(){
	pNavigationSpaces.RemoveAll();
}



void igdeGDCNavigationSpaceList::SetToDeepCopyFrom( const igdeGDCNavigationSpaceList &list ){
	const int count = list.GetCount();
	igdeGDCNavigationSpace *navSpace = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			navSpace = new igdeGDCNavigationSpace( *list.GetAt( i ) );
			Add( navSpace );
			navSpace->FreeReference();
			navSpace = NULL;
		}
		
	}catch( const deException & ){
		if( navSpace ){
			navSpace->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCNavigationSpaceList &igdeGDCNavigationSpaceList::operator=( const igdeGDCNavigationSpaceList &list ){
	pNavigationSpaces = list.pNavigationSpaces;
	return *this;
}
