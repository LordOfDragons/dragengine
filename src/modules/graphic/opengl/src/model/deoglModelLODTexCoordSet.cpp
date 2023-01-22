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

#include "deoglModelLODTexCoordSet.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelLODTexCoordSet
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelLODTexCoordSet::deoglModelLODTexCoordSet(){
	pTexCoords = NULL;
	pTexCoordCount = 0;
	
	pTangents = NULL;
	pNegateTangents = NULL;
	pTangentCount = 0;
}

deoglModelLODTexCoordSet::~deoglModelLODTexCoordSet(){
	pCleanUp();
}



// Management
///////////////

void deoglModelLODTexCoordSet::SetTextureCoordinateCount( int count, bool copyContent ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	decVector2 *texCoords = NULL;
	
	if( count > 0 ){
		texCoords = new decVector2[ count ];
		
		if( copyContent && pTexCoords ){
			int copyCount = count;
			
			if( copyCount > pTexCoordCount ){
				copyCount = pTexCoordCount;
			}
			
			memcpy( texCoords, pTexCoords, sizeof( decVector2 ) * copyCount );
		}
	}
	
	if( pTexCoords ){
		delete [] pTexCoords;
	}
	
	pTexCoords = texCoords;
	pTexCoordCount = count;
}

void deoglModelLODTexCoordSet::SetTangentCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	decVector *tangents = NULL;
	bool *negateTangent = NULL;
	
	if( count > 0 ){
		tangents = new decVector[ count ];
		negateTangent = new bool[ count ];
	}
	
	if( pTangents ){
		delete [] pTangents;
	}
	if( pNegateTangents ){
		delete [] pNegateTangents;
	}
	
	pTangents = tangents;
	pNegateTangents = negateTangent;
	
	pTangentCount = count;
}



// Private functions
//////////////////////

void deoglModelLODTexCoordSet::pCleanUp(){
	if( pNegateTangents ){
		delete [] pNegateTangents;
	}
	if( pTangents ){
		delete [] pTangents;
	}
	if( pTexCoords ){
		delete [] pTexCoords;
	}
}
