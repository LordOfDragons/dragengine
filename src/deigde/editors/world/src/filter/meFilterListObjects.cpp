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

#include "meFilterListObjects.h"

#include <dragengine/common/exceptions.h>



// Class meFilterListObjects
//////////////////////////////

// Constructor, destructor
////////////////////////////

meFilterListObjects::meFilterListObjects(){
	pFilters = NULL;
	pFilterCount = 0;
	
	pAcceptAny = true;
}

meFilterListObjects::~meFilterListObjects(){
	RemoveAllFilters();
}



// Management
///////////////

void meFilterListObjects::SetAcceptAny( bool acceptAny ){
	pAcceptAny = acceptAny;
}



void meFilterListObjects::AddFilter( meFilterObjects *filter ){
	if( ! filter ) DETHROW( deeInvalidParam );
	
	meFilterObjects **newArray = new meFilterObjects*[ pFilterCount + 1 ];
	if( ! newArray ) DETHROW( deeOutOfMemory );
	if( pFilters ){
		memcpy( newArray, pFilters, sizeof( meFilterObjects* ) * pFilterCount );
		delete [] pFilters;
	}
	pFilters = newArray;
	
	pFilters[ pFilterCount ] = filter;
	pFilterCount++;
}

void meFilterListObjects::RemoveAllFilters(){
	if( pFilters ){
		while( pFilterCount > 0 ){
			pFilterCount--;
			delete pFilters[ pFilterCount ];
		}
		
		delete [] pFilters;
		pFilters = NULL;
	}
}



bool meFilterListObjects::AcceptObject( meObject *object ) const{
	if( ! object ) DETHROW( deeInvalidParam );
	
	int f;
	
	if( pAcceptAny ){
		for( f=0; f<pFilterCount; f++ ){
			if( pFilters[ f ]->AcceptObject( object ) ){
				return true;
			}
		}
		
		return false;
		
	}else{
		for( f=0; f<pFilterCount; f++ ){
			if( ! pFilters[ f ]->AcceptObject( object ) ){
				return false;
			}
		}
		
		return true;
	}
}
