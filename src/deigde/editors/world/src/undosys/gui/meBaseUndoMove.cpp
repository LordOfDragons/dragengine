/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
