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

#include "deResource.h"
#include "deResourceList.h"
#include "../common/exceptions.h"



// Class deResourceList
/////////////////////////

// Constructor, destructor
////////////////////////////

deResourceList::deResourceList() :
pRoot( NULL ),
pTail( NULL ),
pCount( 0 ){
}

deResourceList::~deResourceList(){
	RemoveAll();
}



// Management
///////////////

bool deResourceList::Has( deResource *resource ) const{
	if( ! resource ){
		DETHROW( deeInvalidParam );
	}
	
	deResource *nextResource = pRoot;
	while( nextResource ){
		if( nextResource == resource ){
			return true;
		}
		nextResource = nextResource->GetLLManagerNext();
	}
	
	return false;
}

void deResourceList::Add( deResource *resource ){
	if( ! resource || resource->GetLLManagerPrev() || resource->GetLLManagerNext() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTail ){
		pTail->SetLLManagerNext( resource );
		resource->SetLLManagerPrev( pTail );
		pTail = resource;
		
	}else{ // it can never happen that root is non-NULL if tail is NULL
		pRoot = resource;
		pTail = resource;
	}
	
	pCount++;
}

void deResourceList::Remove( deResource *resource ){
	// the test for existence in the list is not valid if the resource has been added to a different
	// resource list. resource managers use this list correctly hence the only possible problem occurs
	// if the user messes with the linked list himself. since he is strictly discouraged to do so this
	// problem situation can be ruled out for normal operation to gain speed by dropping a full check
	if( ! resource ){
		DETHROW( deeInvalidParam );
	}
	if( resource != pRoot && ! resource->GetLLManagerNext() && ! resource->GetLLManagerPrev() ){
		DETHROW( deeInvalidParam );
	}
	
	pBareRemove( resource );
}

void deResourceList::RemoveIfPresent( deResource *resource ){
	// the test for existence in the list is not valid if the resource has been added to a different
	// resource list. resource managers use this list correctly hence the only possible problem occurs
	// if the user messes with the linked list himself. since he is strictly discouraged to do so this
	// problem situation can be ruled out for normal operation to gain speed by dropping a full check
	if( ! resource ){
		DETHROW( deeInvalidParam );
	}
	if( resource == pRoot || resource->GetLLManagerNext() || resource->GetLLManagerPrev() ){
		pBareRemove( resource );
	}
}

void deResourceList::RemoveAll(){
	while( pRoot ){
		pRoot->MarkLeaking();
		pRoot = pRoot->GetLLManagerNext();
	}
	pTail = NULL;
	pCount = 0;
}



// Private Functions
//////////////////////

void deResourceList::pBareRemove( deResource *resource ){
	if( pCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( resource == pTail ){
		if( resource->GetLLManagerNext() ){
			DETHROW( deeInvalidParam );
		}
		
		pTail = pTail->GetLLManagerPrev();
		if( pTail ){
			pTail->SetLLManagerNext( NULL );
		}
		
	}else{
		if( ! resource->GetLLManagerNext() ){
			DETHROW( deeInvalidParam );
		}
		
		resource->GetLLManagerNext()->SetLLManagerPrev( resource->GetLLManagerPrev() );
	}
	
	if( resource == pRoot ){
		if( resource->GetLLManagerPrev() ){
			DETHROW( deeInvalidParam );
		}
		
		pRoot = pRoot->GetLLManagerNext();
		if( pRoot ){
			pRoot->SetLLManagerPrev( NULL );
		}
		
	}else{
		if( ! resource->GetLLManagerPrev() ){
			DETHROW( deeInvalidParam );
		}
		
		resource->GetLLManagerPrev()->SetLLManagerNext( resource->GetLLManagerNext() );
	}
	
	resource->SetLLManagerNext( NULL );
	resource->SetLLManagerPrev( NULL );
	pCount--;
	
	resource->MarkLeaking();
}
