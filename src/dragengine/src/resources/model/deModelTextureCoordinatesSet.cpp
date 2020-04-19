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

#include "deModelTextureCoordinatesSet.h"
#include "../../common/exceptions.h"



// Class deModelTextureCoordinatesSet
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deModelTextureCoordinatesSet::deModelTextureCoordinatesSet(){
	pTextureCoordinates = NULL;
	pTextureCoordinatesCount = 0;
}

deModelTextureCoordinatesSet::~deModelTextureCoordinatesSet(){
	if( pTextureCoordinates ){
		delete [] pTextureCoordinates;
	}
}



// Management
///////////////

void deModelTextureCoordinatesSet::SetTextureCoordinatesCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTextureCoordinates ){
		delete [] pTextureCoordinates;
		pTextureCoordinates = NULL;
		pTextureCoordinatesCount = 0;
	}
	
	if( count > 0 ){
		pTextureCoordinates = new decVector2[ count ];
		pTextureCoordinatesCount = count;
	}
}

const decVector2 &deModelTextureCoordinatesSet::GetTextureCoordinatesAt( int index ) const{
	if( index < 0 || index >= pTextureCoordinatesCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureCoordinates[ index ];
}

void deModelTextureCoordinatesSet::SetTextureCoordinatesAt( int index, const decVector2 &textureCoordinates ){
	if( index < 0 || index >= pTextureCoordinatesCount ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinates[ index ] = textureCoordinates;
}
