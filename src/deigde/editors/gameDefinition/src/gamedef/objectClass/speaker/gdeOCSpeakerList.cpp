/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCSpeaker.h"
#include "gdeOCSpeakerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCSpeaker
/////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSpeakerList::gdeOCSpeakerList(){
}

gdeOCSpeakerList::gdeOCSpeakerList( const gdeOCSpeakerList &list ) :
pSpeakers( list.pSpeakers ){
}

gdeOCSpeakerList::~gdeOCSpeakerList(){
}



// Management
///////////////

int gdeOCSpeakerList::GetCount() const{
	return pSpeakers.GetCount();
}

gdeOCSpeaker *gdeOCSpeakerList::GetAt( int index ) const{
	return ( gdeOCSpeaker* )pSpeakers.GetAt( index );
}

int gdeOCSpeakerList::IndexOf( gdeOCSpeaker *speaker ) const{
	return pSpeakers.IndexOf( speaker );
}

bool gdeOCSpeakerList::Has( gdeOCSpeaker *speaker ) const{
	return pSpeakers.Has( speaker );
}

void gdeOCSpeakerList::Add( gdeOCSpeaker *speaker ){
	if( ! speaker || Has( speaker ) ){
		DETHROW( deeInvalidParam );
	}
	pSpeakers.Add( speaker );
}

void gdeOCSpeakerList::Remove( gdeOCSpeaker *speaker ){
	pSpeakers.Remove( speaker );
}

void gdeOCSpeakerList::RemoveAll(){
	pSpeakers.RemoveAll();
}



gdeOCSpeakerList &gdeOCSpeakerList::operator=( const gdeOCSpeakerList &list ){
	pSpeakers = list.pSpeakers;
	return *this;
}
