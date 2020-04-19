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

#include "meUHTNavSpaceFaceRemove.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceType.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceFaceRemove
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceFaceRemove::meUHTNavSpaceFaceRemove( const decObjectOrderedSet &faces ) :
pFaces( NULL ),
pFaceCount( 0 )
{
	if( faces.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = faces.GetCount();
	
	if( count == 1 ){
		SetShortInfo( "Height terrain nav-space remove face" );
		
	}else{
		SetShortInfo( "Height terrain nav-space remove faces" );
	}
	
	try{
		pFaces = new sFace[ count ];
		
		for( pFaceCount=0; pFaceCount<count; pFaceCount++ ){
			sFace &face = pFaces[ pFaceCount ];
			
			face.face = ( meHeightTerrainNavSpaceFace* )faces.GetAt( pFaceCount );
			face.face->AddReference();
			
			face.type = face.face->GetType();
			if( ! face.type ){
				DETHROW( deeInvalidParam );
			}
			face.type->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUHTNavSpaceFaceRemove::~meUHTNavSpaceFaceRemove(){
	pCleanUp();
}



// Management
///////////////

void meUHTNavSpaceFaceRemove::Undo(){
	int i;
	
	for( i=0; i<pFaceCount; i++ ){
		pFaces[ i ].type->AddFace( pFaces[ i ].face );
	}
}

void meUHTNavSpaceFaceRemove::Redo(){
	int i;
	
	for( i=0; i<pFaceCount; i++ ){
		pFaces[ i ].type->RemoveFace( pFaces[ i ].face );
	}
}



// Private functions
//////////////////////

void meUHTNavSpaceFaceRemove::pCleanUp(){
	if( ! pFaces ){
		return;
	}
	
	int i;
	
	for( i=0; i<pFaceCount; i++ ){
		if( pFaces[ i ].face ){
			pFaces[ i ].face->FreeReference();
		}
		if( pFaces[ i ].type ){
			pFaces[ i ].type->FreeReference();
		}
	}
}
