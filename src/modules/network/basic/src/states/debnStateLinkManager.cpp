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
#include "debnState.h"
#include "debnStateLink.h"
#include "debnStateLinkManager.h"
#include "dragengine/common/exceptions.h"



// Class debnStateLinkManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

debnStateLinkManager::debnStateLinkManager(){
	pHeadLink = NULL;
	pTailLink = NULL;
	pLinkCount = 0;
	pNextIdentifier = 0;
}

debnStateLinkManager::~debnStateLinkManager(){
	RemoveAllLinks();
}



// Management
///////////////

debnStateLink *debnStateLinkManager::GetLinkWithIdentifier( int identifier ) const{
	debnStateLink *link = pHeadLink;
	
	while( link && identifier != link->GetIdentifier() ){
		link = link->GetNextLink();
	}
	
	return link;
}

debnStateLink *debnStateLinkManager::GetLinkWithNetworkState( debnState *networkState ) const{
	debnStateLink *link = pHeadLink;
	
	while( link && networkState != link->GetState() ){
		link = link->GetNextLink();
	}
	
	return link;
}

void debnStateLinkManager::AddLink( debnStateLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	if( pTailLink ){
		pTailLink->SetNextLink( link );
		link->SetPreviousLink( pTailLink );
		pTailLink = link;
		
	}else{
		pHeadLink = link;
		pTailLink = link;
	}
}

void debnStateLinkManager::RemoveLink( debnStateLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	debnStateLink *previous = link->GetPreviousLink();
	debnStateLink *next = link->GetNextLink();
	
	if( ! previous && link != pHeadLink ) DETHROW( deeInvalidParam );
	if( ! next && link != pTailLink ) DETHROW( deeInvalidParam );
	
	if( previous ){
		previous->SetNextLink( next );
		
	}else{
		pHeadLink = next;
	}
	
	if( next ){
		next->SetPreviousLink( previous );
		
	}else{
		pTailLink = previous;
	}
	
	pLinkCount--;
}

void debnStateLinkManager::RemoveAllLinks(){
	debnStateLink *link;
	
	while( pHeadLink ){
		link = pHeadLink;
		
		pHeadLink = link->GetNextLink();
		pLinkCount--;
		
		delete link;
	}
	
	pTailLink = NULL;
}

int debnStateLinkManager::GetFreeIdentifier(){
	int lastIdentifier = pNextIdentifier;
	
	while( GetLinkWithIdentifier( pNextIdentifier ) ){
		pNextIdentifier = ( pNextIdentifier + 1 ) % 65535;
		if( pNextIdentifier == lastIdentifier ) return -1;
	}
	
	return pNextIdentifier;
}
