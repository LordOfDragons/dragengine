/* 
 * Drag[en]gine IGDE World Editor
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
