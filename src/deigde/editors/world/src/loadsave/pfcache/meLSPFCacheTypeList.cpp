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

#include "meLSPFCacheTypeList.h"

#include <dragengine/common/exceptions.h>



// Class meLSPFCacheTypeList
//////////////////////////////

// Constructor, destructor
////////////////////////////

meLSPFCacheTypeList::meLSPFCacheTypeList(){
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
}

meLSPFCacheTypeList::~meLSPFCacheTypeList(){
	if( pTypes ) delete [] pTypes;
}



// Management
///////////////

meLSPFCacheTypeList::sType &meLSPFCacheTypeList::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ) DETHROW( deeInvalidParam );
	
	return pTypes[ index ];
}

int meLSPFCacheTypeList::IndexOfTypeWith( int vlayer, int variation ) const{
	int t;
	
	for( t=0; t<pTypeCount; t++ ){
		if( pTypes[ t ].vlayer == vlayer && pTypes[ t ].variation == variation ){
			return t;
		}
	}
	
	return -1;
}

void meLSPFCacheTypeList::AddType( int vlayer, int variation ){
	if( IndexOfTypeWith( vlayer, variation ) == -1 ){
		if( pTypeCount == pTypeSize ){
			int newSize = pTypeSize * 3 / 2 + 1;
			sType *newArray = new sType[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pTypes ){
				memcpy( newArray, pTypes, sizeof( sType ) * pTypeSize );
				delete [] pTypes;
			}
			pTypes = newArray;
			pTypeSize = newSize;
		}
		
		pTypes[ pTypeCount ].vlayer = vlayer;
		pTypes[ pTypeCount ].variation = variation;
		pTypeCount++;
	}
}

void meLSPFCacheTypeList::RemoveAllTypes(){
	pTypeCount = 0;
}
