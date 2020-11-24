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

#include "ceGesture.h"
#include "ceGestureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceGesture
////////////////////

// Constructor, destructor
////////////////////////////

ceGestureList::ceGestureList(){
}

ceGestureList::~ceGestureList(){
}



// Management
///////////////

int ceGestureList::GetCount() const{
	return pGestures.GetCount();
}

ceGesture *ceGestureList::GetAt( int index ) const{
	return ( ceGesture* )pGestures.GetAt( index );
}

ceGesture *ceGestureList::GetNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	ceGesture *gesture;
	int i;
	
	for( i=0; i<count; i++ ){
		gesture = ( ceGesture* )pGestures.GetAt( i );
		
		if( gesture->GetName().Equals( name ) ){
			return gesture;
		}
	}
	
	return NULL;
}

int ceGestureList::IndexOf( ceGesture *gesture ) const{
	return pGestures.IndexOf( gesture );
}

int ceGestureList::IndexOfNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceGesture* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceGestureList::Has( ceGesture *gesture ) const{
	return pGestures.Has( gesture );
}

bool ceGestureList::HasNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceGesture* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceGestureList::Add( ceGesture *gesture ){
	if( ! gesture || HasNamed( gesture->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pGestures.Add( gesture );
}

void ceGestureList::Remove( ceGesture *gesture ){
	pGestures.Remove( gesture );
}

void ceGestureList::RemoveAll(){
	pGestures.RemoveAll();
}



ceGestureList &ceGestureList::operator=( const ceGestureList &list ){
	pGestures = list.pGestures;
	return *this;
}

ceGestureList &ceGestureList::operator+=( const ceGestureList &list ){
	pGestures += list.pGestures;
	return *this;
}
