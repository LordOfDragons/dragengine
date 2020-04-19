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

#include "meBaseUndoMove.h"



// Class meBaseUndoMove
/////////////////////////

// Constructor, destructor
////////////////////////////

meBaseUndoMove::meBaseUndoMove() :
pModifyOrientation( false ){
}

meBaseUndoMove::~meBaseUndoMove(){
}



// Management
///////////////

void meBaseUndoMove::SetDistance( const decDVector &distance ){
	decString info;
	
	pDistance = distance;
	
	info.Format( "distance (%g,%g,%g)", distance.x, distance.y, distance.z );
	SetLongInfo( info.GetString() );
}

void meBaseUndoMove::SetModifyOrientation( bool modify ){
	pModifyOrientation = modify;
}

void meBaseUndoMove::SetMatrix( const decDMatrix &matrix ){
	pMatrix = matrix;
	
	const decDVector displacement( matrix.GetPosition() );
	const decDVector rotation( matrix.GetEulerAngles() / DEG2RAD );
	
	decString info;
	info.Format( "distance(%g,%g,%g) rotation=(%g,%g,%g)",
		displacement.x, displacement.y, displacement.z,
		rotation.x, rotation.y, rotation.z );
}



// Undo and Redo actions
//////////////////////////

void meBaseUndoMove::TransformElement( decDVector &position, decDVector &rotation ){
	const decDMatrix matrix = decDMatrix::CreateRT( rotation * DEG2RAD, position ) * pMatrix;
	
	// orientation
	const decDVector view( matrix.TransformView().Normalized() );
	const decDVector up( matrix.TransformUp().Normalized() );
	
	rotation = decDMatrix::CreateWorld( decDVector(), view, up ).GetEulerAngles() / DEG2RAD;
	
	if( fabs( rotation.x ) < FLOAT_SAFE_EPSILON ){
		rotation.x = 0.0;
	}
	if( fabs( rotation.y ) < FLOAT_SAFE_EPSILON ){
		rotation.y = 0.0;
	}
	if( fabs( rotation.z ) < FLOAT_SAFE_EPSILON ){
		rotation.z = 0.0;
	}
	
	// position
	position = matrix.GetPosition();
	
	if( fabs( position.x ) < FLOAT_SAFE_EPSILON ){
		position.x = 0.0;
	}
	if( fabs( position.y ) < FLOAT_SAFE_EPSILON ){
		position.y = 0.0;
	}
	if( fabs( position.z ) < FLOAT_SAFE_EPSILON ){
		position.z = 0.0;
	}
}

void meBaseUndoMove::ProgressiveRedo(){
	Undo();
	Redo();
}
