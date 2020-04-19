/* 
 * Drag[en]gine Basic Network Module
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnStateLink.h"
#include "debnStateLinkList.h"
#include "dragengine/common/exceptions.h"



// Class debnStateLinkList
////////////////////////////

// Constructor, destructor
////////////////////////////

debnStateLinkList::debnStateLinkList(){
	pLinks = NULL;
	pLinkCount = 0;
	pLinkSize = 0;
}

debnStateLinkList::~debnStateLinkList(){
	if( pLinks ) delete [] pLinks;
}



// Management
///////////////

debnStateLink *debnStateLinkList::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ) DETHROW( deeInvalidParam );
	
	return pLinks[ index ];
}

int debnStateLinkList::IndexOfLink( debnStateLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( link == pLinks[ i ] ) return i;
	}
	
	return -1;
}

bool debnStateLinkList::HasLink( debnStateLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( link == pLinks[ i ] ) return true;
	}
	
	return false;
}

void debnStateLinkList::AddLink( debnStateLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	if( pLinkCount == pLinkSize ){
		int newSize = pLinkSize * 3 / 2 + 1;
		debnStateLink **newArray = new debnStateLink*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLinks ){
			memcpy( newArray, pLinks, sizeof( debnStateLink* ) * pLinkSize );
			delete pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[ pLinkCount ] = link;
	pLinkCount++;
}

void debnStateLinkList::RemoveLink( debnStateLink *link ){
	int i, index = IndexOfLink( link );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLinkCount; i++ ){
		pLinks[ i - 1 ] = pLinks[ i ];
	}
	pLinkCount--;
}

void debnStateLinkList::RemoveLinkIfExisting( debnStateLink *link ){
	int i, index = IndexOfLink( link );
	
	if( index != -1 ){
		for( i=index+1; i<pLinkCount; i++ ){
			pLinks[ i - 1 ] = pLinks[ i ];
		}
		pLinkCount--;
	}
}

void debnStateLinkList::RemoveAllLinks(){
	pLinkCount = 0;
}
