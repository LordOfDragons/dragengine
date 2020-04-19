/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seLink.h"
#include "seLinkList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seLink
/////////////////

// Constructor, destructor
////////////////////////////

seLinkList::seLinkList(){
}

seLinkList::seLinkList( const seLinkList &copy ) :
pLinks( copy.pLinks ){
}

seLinkList::~seLinkList(){
}



// Management
///////////////

int seLinkList::GetCount() const{
	return pLinks.GetCount();
}

seLink *seLinkList::GetAt( int index ) const{
	return ( seLink* )pLinks.GetAt( index );
}

int seLinkList::IndexOf( seLink *link ) const{
	return pLinks.IndexOf( link );
}

bool seLinkList::Has( seLink *link ) const{
	return pLinks.Has( link );
}

void seLinkList::Add( seLink *link ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void seLinkList::Insert( seLink *link, int index ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Insert( link, index );
}

void seLinkList::Move( seLink *link, int index ){
	pLinks.Move( link, index );
}

void seLinkList::Remove( seLink *link ){
	pLinks.Remove( link );
}

void seLinkList::RemoveAll(){
	pLinks.RemoveAll();
}



// Operators
//////////////

seLinkList &seLinkList::operator=( const seLinkList &list ){
	pLinks = list.pLinks;
	return *this;
}

seLinkList &seLinkList::operator+=( const seLinkList &list ){
	pLinks += list.pLinks;
	return *this;
}
