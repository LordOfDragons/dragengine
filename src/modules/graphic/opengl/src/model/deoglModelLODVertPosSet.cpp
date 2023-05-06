/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglModelLODVertPosSet.h"

#include <dragengine/common/exceptions.h>


// Class deoglModelLODVertPosSet
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelLODVertPosSet::deoglModelLODVertPosSet() :
pVBOOffset( 0 ),
pPositions( nullptr ),
pPositionCount( 0 ){
}

deoglModelLODVertPosSet::~deoglModelLODVertPosSet(){
	pCleanUp();
}



// Management
///////////////

void deoglModelLODVertPosSet::SetVBOOffset( int offset ){
	pVBOOffset = offset;
}

void deoglModelLODVertPosSet::SetPositionCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	sPosition *positions = nullptr;
	if( count > 0 ){
		positions = new sPosition[ count ];
	}
	
	if( pPositions ){
		delete [] pPositions;
	}
	
	pPositions = positions;
	pPositionCount = count;
}



// Private functions
//////////////////////

void deoglModelLODVertPosSet::pCleanUp(){
	if( pPositions ){
		delete [] pPositions;
	}
}
