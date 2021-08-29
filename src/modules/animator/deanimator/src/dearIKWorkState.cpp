/* 
 * Drag[en]gine Animator Module
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
#include <string.h>
#include <unistd.h>

#include "dearBoneState.h"
#include "dearIKWorkState.h"

#include <dragengine/deEngine.h>



// Class dearIKWorkState
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearIKWorkState::dearIKWorkState() :
pBoneStateIndex( 0 ),
pAxisTypeX( eatFree ),
pAxisTypeY( eatFree ),
pAxisTypeZ( eatFree ),
pHasLimits( false ),
pDampening( 1.0f, 1.0f, 1.0f ),
pHasDampening( false ){
}

dearIKWorkState::~dearIKWorkState(){
}



// Management
///////////////

void dearIKWorkState::SetBoneStateIndex( int index ){
	pBoneStateIndex = index;
}

void dearIKWorkState::SetGlobalMatrix( const decMatrix &matrix ){
	pGlobalMatrix = matrix;
}

void dearIKWorkState::SetInverseGlobalMatrix( const decMatrix &matrix ){
	pInvGlobalMatrix = matrix;
}

void dearIKWorkState::SetReferenceRotation( const decVector &rotation ){
	pRefRotation = rotation;
}

void dearIKWorkState::SetAxisTypeX( int type ){
	pAxisTypeX = type;
}

void dearIKWorkState::SetAxisTypeY( int type ){
	pAxisTypeY = type;
}

void dearIKWorkState::SetAxisTypeZ( int type ){
	pAxisTypeZ = type;
}

void dearIKWorkState::SetHasLimits( bool hasLimits ){
	pHasLimits = hasLimits;
}

void dearIKWorkState::SetDampening( const decVector &dampening ){
	pDampening = dampening.Clamped( decVector( 0.0f, 0.0f, 0.0f ), decVector( 1.0f, 1.0f, 1.0f ) );
	pHasDampening = ! pDampening.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ), 0.01f );
}
