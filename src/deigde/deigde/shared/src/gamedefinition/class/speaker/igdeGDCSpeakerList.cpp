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

#include "igdeGDCSpeaker.h"
#include "igdeGDCSpeakerList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCSpeaker
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCSpeakerList::igdeGDCSpeakerList(){
}

igdeGDCSpeakerList::igdeGDCSpeakerList( const igdeGDCSpeakerList &list ){
	pSpeakers = list.pSpeakers;
}

igdeGDCSpeakerList::~igdeGDCSpeakerList(){
}



// Management
///////////////

int igdeGDCSpeakerList::GetCount() const{
	return pSpeakers.GetCount();
}

igdeGDCSpeaker *igdeGDCSpeakerList::GetAt( int index ) const{
	return ( igdeGDCSpeaker* )pSpeakers.GetAt( index );
}

int igdeGDCSpeakerList::IndexOf( igdeGDCSpeaker *speaker ) const{
	return pSpeakers.IndexOf( speaker );
}

bool igdeGDCSpeakerList::Has( igdeGDCSpeaker *speaker ) const{
	return pSpeakers.Has( speaker );
}

void igdeGDCSpeakerList::Add( igdeGDCSpeaker *speaker ){
	if( ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	pSpeakers.Add( speaker );
}

void igdeGDCSpeakerList::InsertAt( igdeGDCSpeaker *speaker, int index ){
	if( ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	pSpeakers.Insert( speaker, index );
}

void igdeGDCSpeakerList::MoveTo( igdeGDCSpeaker *speaker, int index ){
	pSpeakers.Move( speaker, index );
}

void igdeGDCSpeakerList::Remove( igdeGDCSpeaker *speaker ){
	pSpeakers.Remove( speaker );
}

void igdeGDCSpeakerList::RemoveAll(){
	pSpeakers.RemoveAll();
}



void igdeGDCSpeakerList::SetToDeepCopyFrom( const igdeGDCSpeakerList &list ){
	const int count = list.GetCount();
	igdeGDCSpeaker *speaker = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			speaker = new igdeGDCSpeaker( *list.GetAt( i ) );
			Add( speaker );
			speaker->FreeReference();
			speaker = NULL;
		}
		
	}catch( const deException & ){
		if( speaker ){
			speaker->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCSpeakerList &igdeGDCSpeakerList::operator=( const igdeGDCSpeakerList &list ){
	pSpeakers = list.pSpeakers;
	return *this;
}
