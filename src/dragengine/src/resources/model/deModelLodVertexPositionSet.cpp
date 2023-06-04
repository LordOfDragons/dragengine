/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deModelLodVertexPositionSet.h"
#include "deModelLodVertexPositionSetPosition.h"
#include "../../common/exceptions.h"



// Class deModelLodVertexPositionSet
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deModelLodVertexPositionSet::deModelLodVertexPositionSet() :
pPositions( nullptr ),
pPositionCount( 0 ){
}

deModelLodVertexPositionSet::~deModelLodVertexPositionSet(){
	if( pPositions ){
		delete [] pPositions;
	}
}



// Management
///////////////

void deModelLodVertexPositionSet::SetPositionCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( pPositions ){
		delete [] pPositions;
		pPositions = nullptr;
		pPositionCount = 0;
	}
	
	if( count > 0 ){
		pPositions = new deModelLodVertexPositionSetPosition[ count ];
		pPositionCount = count;
	}
}

deModelLodVertexPositionSetPosition &deModelLodVertexPositionSet::GetPositionAt( int index ){
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pPositionCount );
	
	return pPositions[ index ];
}

const deModelLodVertexPositionSetPosition &deModelLodVertexPositionSet::GetPositionAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pPositionCount );
	
	return pPositions[ index ];
}
