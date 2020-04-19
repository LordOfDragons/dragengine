/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceControllerValue.h"
#include "ceControllerValueList.h"

#include <dragengine/common/exceptions.h>



// Class ceControllerValueList
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceControllerValueList::ceControllerValueList(){
}

ceControllerValueList::~ceControllerValueList(){
	pList.RemoveAll();
}



// Management
///////////////

int ceControllerValueList::GetCount() const{
	return pList.GetCount();
}

ceControllerValue *ceControllerValueList::GetAt( int index ) const{
	return ( ceControllerValue* )pList.GetAt( index );
}

ceControllerValue *ceControllerValueList::GetWith( int controller ) const{
	const int count = pList.GetCount();
	ceControllerValue *entry;
	int i;
	
	for( i=0; i<count; i++ ){
		entry = ( ceControllerValue* )pList.GetAt( i );
		
		if( entry->GetController() == controller ){
			return entry;
		}
	}
	
	return NULL;
}

int ceControllerValueList::IndexOf( ceControllerValue *entry ) const{
	return pList.IndexOf( entry );
}

int ceControllerValueList::IndexOfWith( int controller ) const{
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceControllerValue* )pList.GetAt( i ) )->GetController() == controller ){
			return i;
		}
	}
	
	return -1;
}

bool ceControllerValueList::Has( ceControllerValue *entry ) const{
	return pList.Has( entry );
}

bool ceControllerValueList::HasWith( int controller ) const{
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceControllerValue* )pList.GetAt( i ) )->GetController() == controller ){
			return true;
		}
	}
	
	return false;
}

void ceControllerValueList::Add( ceControllerValue *entry ){
	if( ! entry || HasWith( entry->GetController() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Add( entry );
}

void ceControllerValueList::Remove( ceControllerValue *entry ){
	pList.Remove( entry );
}

void ceControllerValueList::RemoveAll(){
	pList.RemoveAll();
}



ceControllerValueList &ceControllerValueList::operator=( const ceControllerValueList &list ){
	pList = list.pList;
	return *this;
}
