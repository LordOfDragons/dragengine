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
