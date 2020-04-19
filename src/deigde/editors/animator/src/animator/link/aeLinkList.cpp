/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeLink.h"
#include "aeLinkList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeLink
/////////////////

// Constructor, destructor
////////////////////////////

aeLinkList::aeLinkList(){
}

aeLinkList::aeLinkList( const aeLinkList &copy ) :
pLinks( copy.pLinks ){
}

aeLinkList::~aeLinkList(){
}



// Management
///////////////

int aeLinkList::GetCount() const{
	return pLinks.GetCount();
}

aeLink *aeLinkList::GetAt( int index ) const{
	return ( aeLink* )pLinks.GetAt( index );
}

aeLink *aeLinkList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		aeLink * const link = ( aeLink* )pLinks.GetAt( i );
		if( link->GetName() == name ){
			return link;
		}
	}
	
	return NULL;
}

int aeLinkList::IndexOf( aeLink *link ) const{
	return pLinks.IndexOf( link );
}

bool aeLinkList::Has( aeLink *link ) const{
	return pLinks.Has( link );
}

bool aeLinkList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void aeLinkList::Add( aeLink *link ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void aeLinkList::Insert( aeLink *link, int index ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Insert( link, index );
}

void aeLinkList::Move( aeLink *link, int index ){
	pLinks.Move( link, index );
}

void aeLinkList::Remove( aeLink *link ){
	pLinks.Remove( link );
}

void aeLinkList::RemoveAll(){
	pLinks.RemoveAll();
}



// Operators
//////////////

aeLinkList &aeLinkList::operator=( const aeLinkList &list ){
	pLinks = list.pLinks;
	return *this;
}

aeLinkList &aeLinkList::operator+=( const aeLinkList &list ){
	pLinks += list.pLinks;
	return *this;
}
