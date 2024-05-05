/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
