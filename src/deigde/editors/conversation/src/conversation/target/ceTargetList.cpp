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

#include "ceTarget.h"
#include "ceTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceTarget
///////////////////

// Constructor, destructor
////////////////////////////

ceTargetList::ceTargetList(){
}

ceTargetList::~ceTargetList(){
}



// Management
///////////////

int ceTargetList::GetCount() const{
	return pTargets.GetCount();
}

ceTarget *ceTargetList::GetAt( int index ) const{
	return ( ceTarget* )pTargets.GetAt( index );
}

ceTarget *ceTargetList::GetNamed( const char *name ) const{
	const int count = pTargets.GetCount();
	ceTarget *target;
	int i;
	
	for( i=0; i<count; i++ ){
		target = ( ceTarget* )pTargets.GetAt( i );
		
		if( target->GetName().Equals( name ) ){
			return target;
		}
	}
	
	return NULL;
}

int ceTargetList::IndexOf( ceTarget *target ) const{
	return pTargets.IndexOf( target );
}

int ceTargetList::IndexOfNamed( const char *name ) const{
	const int count = pTargets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceTarget* )pTargets.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceTargetList::Has( ceTarget *target ) const{
	return pTargets.Has( target );
}

bool ceTargetList::HasNamed( const char *name ) const{
	const int count = pTargets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceTarget* )pTargets.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceTargetList::Add( ceTarget *target ){
	if( ! target || HasNamed( target->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pTargets.Add( target );
}

void ceTargetList::Remove( ceTarget *target ){
	pTargets.Remove( target );
}

void ceTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



ceTargetList &ceTargetList::operator=( const ceTargetList &list ){
	pTargets = list.pTargets;
	return *this;
}
