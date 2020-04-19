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

#include "ceActorGesture.h"
#include "ceActorGestureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceActorGesture
/////////////////////////

// Constructor, destructor
////////////////////////////

ceActorGestureList::ceActorGestureList(){
}

ceActorGestureList::~ceActorGestureList(){
}



// Management
///////////////

int ceActorGestureList::GetCount() const{
	return pGestures.GetCount();
}

ceActorGesture *ceActorGestureList::GetAt( int index ) const{
	return ( ceActorGesture* )pGestures.GetAt( index );
}

ceActorGesture *ceActorGestureList::GetNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	ceActorGesture *gesture;
	int i;
	
	for( i=0; i<count; i++ ){
		gesture = ( ceActorGesture* )pGestures.GetAt( i );
		
		if( gesture->GetName().Equals( name ) ){
			return gesture;
		}
	}
	
	return NULL;
}

int ceActorGestureList::IndexOf( ceActorGesture *gesture ) const{
	return pGestures.IndexOf( gesture );
}

int ceActorGestureList::IndexOfNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorGesture* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceActorGestureList::Has( ceActorGesture *gesture ) const{
	return pGestures.Has( gesture );
}

bool ceActorGestureList::HasNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorGesture* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceActorGestureList::Add( ceActorGesture *gesture ){
	if( ! gesture || HasNamed( gesture->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pGestures.Add( gesture );
}

void ceActorGestureList::Remove( ceActorGesture *gesture ){
	pGestures.Remove( gesture );
}

void ceActorGestureList::RemoveAll(){
	pGestures.RemoveAll();
}



ceActorGestureList &ceActorGestureList::operator=( const ceActorGestureList &list ){
	pGestures = list.pGestures;
	return *this;
}
