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

#include "deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Class deoglSPBParameter
////////////////////////////

// Constructor, destructor
////////////////////////////


deoglSPBParameter::deoglSPBParameter() :
pValueType( evtFloat ),
pComponentCount( 1 ),
pVectorCount( 1 ),
pArrayCount( 1 ),

pOffset( 0 ),
pStride( 0 ),
pArrayStride( 0 ),
pDataSize( 0 ){
}

deoglSPBParameter::~deoglSPBParameter(){
}



// Management
///////////////

void deoglSPBParameter::SetValueType( eValueTypes valueType ){
	if( valueType < evtFloat || valueType > evtBool ){
		DETHROW( deeInvalidParam );
	}
	
	pValueType = valueType;
}

void deoglSPBParameter::SetComponentCount( int componentCount ){
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	pComponentCount = componentCount;
}

void deoglSPBParameter::SetVectorCount( int vectorCount ){
	if( vectorCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pVectorCount = vectorCount;
}

void deoglSPBParameter::SetArrayCount( int arrayCount ){
	if( arrayCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pArrayCount = arrayCount;
}

void deoglSPBParameter::SetAll( eValueTypes valueType, int componentCount,
int vectorCount, int arrayCount ){
	SetValueType( valueType );
	SetComponentCount( componentCount );
	SetVectorCount( vectorCount );
	SetArrayCount( arrayCount );
}



void deoglSPBParameter::SetOffset( int offset ){
	if( offset < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pOffset = offset;
}

void deoglSPBParameter::SetStride( int stride ){
	if( stride < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pStride = stride;
}

void deoglSPBParameter::SetArrayStride( int arrayStride ){
	if( arrayStride < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pArrayStride = arrayStride;
}

void deoglSPBParameter::SetDataSize ( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pDataSize = size;
}
