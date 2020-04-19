/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalAComponentBone.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponentBone.h>



// Class deoalAComponentBone
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAComponentBone::deoalAComponentBone() :
pIndex( 0 ),
pScale( 1.0f, 1.0f, 1.0f ),
pParent( NULL ){
}

deoalAComponentBone::~deoalAComponentBone(){
}



// Management
///////////////

void deoalAComponentBone::SetIndex( int index ){
	pIndex = index;
}

void deoalAComponentBone::SetName( const char *name ){
	pName = name;
}

void deoalAComponentBone::SetGeometry( const decVector &position,
const decQuaternion &rotation, const decVector &scale ){
	pPosition = position;
	pRotation = rotation;
	pScale = scale;
}

void deoalAComponentBone::SetGeometry( const deComponentBone &bone ){
	pPosition = bone.GetPosition();
	pRotation = bone.GetRotation();
	pScale = bone.GetScale();
}

void deoalAComponentBone::SetOriginalMatrix( const decMatrix &matrix ){
	pOriginalMatrix = matrix;
}

void deoalAComponentBone::SetRigInverseMatrix( const decMatrix &matrix ){
	pRigInverseMatrix = matrix;
}

void deoalAComponentBone::SetParent( deoalAComponentBone *bone ){
	pParent = bone;
}

void deoalAComponentBone::UpdateMatrix(){
	pMatrix = decMatrix::CreateWorld( pPosition, pRotation, pScale ).QuickMultiply( GetOriginalMatrix() );
	if( pParent ){
		pMatrix = pMatrix.QuickMultiply( pParent->pMatrix );
	}
	pWeightMatrix = pRigInverseMatrix.QuickMultiply( pMatrix );
}
