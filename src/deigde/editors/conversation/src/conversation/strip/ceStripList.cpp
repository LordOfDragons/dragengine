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

#include "ceStrip.h"
#include "ceStripList.h"

#include <dragengine/common/exceptions.h>



// Class ceStripList
//////////////////////

// Constructor, destructor
////////////////////////////

ceStripList::ceStripList(){
}

ceStripList::~ceStripList(){
	pList.RemoveAll();
}



// Management
///////////////

int ceStripList::GetCount() const{
	return pList.GetCount();
}

ceStrip *ceStripList::GetAt( int index ) const{
	return ( ceStrip* )pList.GetAt( index );
}

int ceStripList::IndexOf( ceStrip *entry ) const{
	return pList.IndexOf( entry );
}

bool ceStripList::Has( ceStrip *entry ) const{
	return pList.Has( entry );
}

void ceStripList::Add( ceStrip *entry ){
	if( ! entry ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Add( entry );
}

void ceStripList::InsertAt( ceStrip *entry, int index ){
	if( ! entry ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Insert( entry, index );
}

void ceStripList::MoveTo( ceStrip *entry, int index ){
	if( ! entry ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Move( entry, index );
}

void ceStripList::Remove( ceStrip *entry ){
	pList.Remove( entry );
}

void ceStripList::RemoveAll(){
	pList.RemoveAll();
}



void ceStripList::AddCopyFrom( const ceStripList &list ){
	const int count = list.GetCount();
	ceStrip *entry = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			entry = new ceStrip( *list.GetAt( i ) );
			pList.Add( entry );
			entry->FreeReference();
			entry = NULL;
		}
		
	}catch( const deException & ){
		if( entry ){
			entry->FreeReference();
		}
		throw;
	}
}



ceStripList &ceStripList::operator=( const ceStripList &list ){
	pList = list.pList;
	return *this;
}

