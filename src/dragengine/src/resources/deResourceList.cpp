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
