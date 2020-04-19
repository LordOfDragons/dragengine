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

#include "igdeGDCNavigationBlocker.h"
#include "igdeGDCNavigationBlockerList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCNavigationBlocker
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCNavigationBlockerList::igdeGDCNavigationBlockerList(){
}

igdeGDCNavigationBlockerList::igdeGDCNavigationBlockerList( const igdeGDCNavigationBlockerList &list ){
	pNavigationBlockers = list.pNavigationBlockers;
}

igdeGDCNavigationBlockerList::~igdeGDCNavigationBlockerList(){
}



// Management
///////////////

int igdeGDCNavigationBlockerList::GetCount() const{
	return pNavigationBlockers.GetCount();
}

igdeGDCNavigationBlocker *igdeGDCNavigationBlockerList::GetAt( int index ) const{
	return ( igdeGDCNavigationBlocker* )pNavigationBlockers.GetAt( index );
}

int igdeGDCNavigationBlockerList::IndexOf( igdeGDCNavigationBlocker *blocker ) const{
	return pNavigationBlockers.IndexOf( blocker );
}

bool igdeGDCNavigationBlockerList::Has( igdeGDCNavigationBlocker *blocker ) const{
	return pNavigationBlockers.Has( blocker );
}

void igdeGDCNavigationBlockerList::Add( igdeGDCNavigationBlocker *blocker ){
	if( ! blocker ){
		DETHROW( deeInvalidParam );
	}
	pNavigationBlockers.Add( blocker );
}

void igdeGDCNavigationBlockerList::InsertAt( igdeGDCNavigationBlocker *blocker, int index ){
	if( ! blocker ){
		DETHROW( deeInvalidParam );
	}
	pNavigationBlockers.Insert( blocker, index );
}

void igdeGDCNavigationBlockerList::MoveTo( igdeGDCNavigationBlocker *blocker, int index ){
	pNavigationBlockers.Move( blocker, index );
}

void igdeGDCNavigationBlockerList::Remove( igdeGDCNavigationBlocker *blocker ){
	pNavigationBlockers.Remove( blocker );
}

void igdeGDCNavigationBlockerList::RemoveAll(){
	pNavigationBlockers.RemoveAll();
}



void igdeGDCNavigationBlockerList::SetToDeepCopyFrom( const igdeGDCNavigationBlockerList &list ){
	igdeGDCNavigationBlocker *blocker = NULL;
	const int count = list.GetCount();
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			blocker = new igdeGDCNavigationBlocker( *list.GetAt( i ) );
			Add( blocker );
			blocker->FreeReference();
			blocker = NULL;
		}
		
	}catch( const deException & ){
		if( blocker ){
			blocker->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCNavigationBlockerList &igdeGDCNavigationBlockerList::operator=( const igdeGDCNavigationBlockerList &list ){
	pNavigationBlockers = list.pNavigationBlockers;
	return *this;
}
