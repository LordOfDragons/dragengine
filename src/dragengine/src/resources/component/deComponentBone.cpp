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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deComponentBone.h"
#include "../../common/exceptions.h"



// class deComponentBone
//////////////////////////

// constructor, destructor
////////////////////////////

deComponentBone::deComponentBone() :
pParentBone( 0 ),
pScale( 1.0f, 1.0f, 1.0f ),
pDirtyInvMatrix( true ){
}

deComponentBone::~deComponentBone(){
}



// management
///////////////

void deComponentBone::SetPosition( const decVector &position ){
	pPosition = position;
}

void deComponentBone::SetRotation( const decQuaternion &rotation ){
	pRotation = rotation;
}

void deComponentBone::SetScale( const decVector &scale ){
	pScale = scale;
}

void deComponentBone::SetMatrix( const decMatrix &matrix ){
	pMatrix = matrix;
	pDirtyInvMatrix = true;
}

const decMatrix &deComponentBone::GetInverseMatrix(){
	if( pDirtyInvMatrix ){
		pInvMatrix = pMatrix.QuickInvert();
		pDirtyInvMatrix = false;
	}
	
	return pInvMatrix;
}

void deComponentBone::SetOriginalMatrix( const decVector &position, const decVector &rotation ){
	pOrgMatrix.SetRT( rotation, position );
	pInvOrgMatrix = pOrgMatrix.Invert();
}

void deComponentBone::SetParentBone( int parentBone ){
	pParentBone = parentBone;
}
