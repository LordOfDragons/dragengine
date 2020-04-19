/* 
 * Drag[en]gine Game Engine
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

#include "deAnimatorControllerTarget.h"
#include "../../../common/exceptions.h"



// Class deAnimatorControllerTarget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorControllerTarget::deAnimatorControllerTarget(){
	pLinks = NULL;
	pLinkCount = 0;
}

deAnimatorControllerTarget::~deAnimatorControllerTarget(){
	if( pLinks ) delete [] pLinks;
}



// Management
///////////////

int deAnimatorControllerTarget::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ) DETHROW( deeInvalidParam );
	
	return pLinks[ index ];
}

int deAnimatorControllerTarget::IndexOfLink( int link ) const{
	int l;
	
	for( l=0; l<pLinkCount; l++ ){
		if( pLinks[ l ] == link ) return l;
	}
	
	return -1;
}

void deAnimatorControllerTarget::AddLink( int link ){
	int l, *newArray = new int[ pLinkCount + 1 ];
	if( ! newArray ) DETHROW( deeOutOfMemory );
	
	if( pLinks ){
		for( l=0; l<pLinkCount; l++ ) newArray[ l ] = pLinks[ l ];
		delete [] pLinks;
	}
	
	pLinks = newArray;
	pLinks[ pLinkCount ] = link;
	pLinkCount++;
}

void deAnimatorControllerTarget::RemoveLink( int link ){
	int l, index = IndexOfLink( link );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	int *newArray = NULL;
	if( pLinkCount > 0 ){
		new int[ pLinkCount - 1 ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
	}
	
	if( pLinks ){
		for( l=0; l<index; l++ ) newArray[ l ] = pLinks[ l ];
		for( l=index+1; l<pLinkCount; l++ ) newArray[ l - 1 ] = pLinks[ l ];
		delete [] pLinks;
	}
	
	pLinks = newArray;
	pLinkCount--;
}

void deAnimatorControllerTarget::RemoveAllLinks(){
	if( pLinks ){
		delete [] pLinks;
		pLinks = NULL;
	}
	
	pLinkCount = 0;
}
