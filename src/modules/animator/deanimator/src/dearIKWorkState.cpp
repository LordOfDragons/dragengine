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

#include "dearBoneState.h"
#include "dearIKWorkState.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRigBone.h>



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
pHasDampening( false ),
pWeight( 1.0f ),
pLength( 0.0f ){
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

void dearIKWorkState::SetAxisTypeX( eAxisTypes type ){
	pAxisTypeX = type;
}

void dearIKWorkState::SetAxisTypeY( eAxisTypes type ){
	pAxisTypeY = type;
}

void dearIKWorkState::SetAxisTypeZ( eAxisTypes type ){
	pAxisTypeZ = type;
}

void dearIKWorkState::SetHasLimits( bool hasLimits ){
	pHasLimits = hasLimits;
}

void dearIKWorkState::SetDampening( const decVector &dampening ){
	pDampening = dampening.Clamped( decVector( 0.0f, 0.0f, 0.0f ), decVector( 1.0f, 1.0f, 1.0f ) );
	pHasDampening = ! pDampening.IsEqualTo( decVector( 1.0f, 1.0f, 1.0f ), 0.01f );
}

void dearIKWorkState::UpdateLimits( const deRigBone &bone ){
	pLimitLower = bone.GetIKLimitsLower();
	pLimitUpper = bone.GetIKLimitsUpper();
	
	if( pAxisTypeX != eatLimited ){
		pLimitUpper.x = pLimitLower.x = 0.0f;
	}
	if( pAxisTypeY != eatLimited ){
		pLimitUpper.y = pLimitLower.y = 0.0f;
	}
	if( pAxisTypeZ != eatLimited ){
		pLimitUpper.z = pLimitLower.z = 0.0f;
	}
	
	pLimitZeroQuatInv.SetFromEuler( ( pLimitLower + pLimitUpper ) * 0.5f );
	pLimitZeroQuat = pLimitZeroQuatInv.Conjugate();
	
	pLimitUpper = ( pLimitUpper - pLimitLower ) * 0.5f;
	pLimitLower = -pLimitUpper;
}

void dearIKWorkState::SetLockedRotation( const decVector &rotation ){
	pLockedRotation = rotation;
}

void dearIKWorkState::SetWeight( float weight ){
	pWeight = weight;
}

void dearIKWorkState::SetLength( float length ){
	pLength = length;
}

void dearIKWorkState::SetEndPosition( const decVector &position ){
	pEndPosition = position;
	pLength = position.Length();
}

decVector dearIKWorkState::GetGlobalEnd() const{
	return pGlobalMatrix * pEndPosition;
}

void dearIKWorkState::SetRigLocalRotation( const decQuaternion &rotation ){
	pRigLocalRot = rotation;
	pInvRigLocalRot = rotation.Conjugate();
}

void dearIKWorkState::SetOrgOrientation( const decQuaternion &orientation ){
	pOrgOrientation = orientation;
}
