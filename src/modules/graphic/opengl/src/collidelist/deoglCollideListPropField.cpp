/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropField
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropField::deoglCollideListPropField(){
	pPropField = NULL;
	
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
}

deoglCollideListPropField::~deoglCollideListPropField(){
	if( pTypes ){
		while( pTypeSize > 0 ){
			pTypeSize--;
			delete pTypes[ pTypeSize ];
		}
		
		delete  [] pTypes;
	}
}



// Management
///////////////

void deoglCollideListPropField::Clear(){
	SetPropField( NULL );
}

bool deoglCollideListPropField::GetIsEmpty() const{
	int t;
	
	for( t=0; t<pTypeCount; t++ ){
		if( pTypes[ t ]->GetIsNotEmpty() ){
			return false;
		}
	}
	
	return true;
}

void deoglCollideListPropField::SetPropField( deoglRPropField *propField ){
	pPropField = propField;
	
	pTypeCount = 0;
	
	if( propField ){
		int typeCount = propField->GetTypeCount();
		
		if( typeCount > pTypeSize ){
			deoglCollideListPropFieldType **newArray = new deoglCollideListPropFieldType*[ typeCount ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pTypes ){
				memcpy( newArray, pTypes, sizeof( deoglCollideListPropFieldType* ) * pTypeSize );
				delete [] pTypes;
			}
			pTypes = newArray;
			while( pTypeSize < typeCount ){
				pTypes[ pTypeSize ] = new deoglCollideListPropFieldType;
				if( ! pTypes[ pTypeSize ] ) DETHROW( deeOutOfMemory );
				pTypeSize++;
			}
		}
		
		while( pTypeCount < typeCount ){
			pTypes[ pTypeCount++ ]->RemoveAllClusters();
		}
	}
}



deoglCollideListPropFieldType *deoglCollideListPropField::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ) DETHROW( deeInvalidParam );
	
	return pTypes[ index ];
}
