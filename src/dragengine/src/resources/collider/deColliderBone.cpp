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
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deColliderBone.h"
#include "deCollider.h"
#include "../../common/exceptions.h"



// class deColliderBone
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderBone::deColliderBone( deCollider *collider, int index ) :
pCollider( collider ),
pIndex( index ),
pMass( 1.0f ),
pDynamic( false )
{
	if( ! collider || index < 0 ){
		DETHROW( deeInvalidParam );
	}
}

deColliderBone::~deColliderBone(){
}



// Management
///////////////

void deColliderBone::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	pCollider->pNotifyBonePositionChanged( pIndex );
}

void deColliderBone::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	pCollider->pNotifyBoneOrientationChanged( pIndex );
}

void deColliderBone::SetLinearVelocity( const decVector &linearVelocity ){
	if( linearVelocity.IsEqualTo( pLinearVelocity ) ){
		return;
	}
	
	pLinearVelocity = linearVelocity;
	pCollider->pNotifyBoneLinearVelocityChanged( pIndex );
}

void deColliderBone::SetAngularVelocity( const decVector &angularVelocity ){
	if( angularVelocity.IsEqualTo( pAngularVelocity ) ){
		return;
	}
	
	pAngularVelocity = angularVelocity;
	pCollider->pNotifyBoneAngularVelocityChanged( pIndex );
}

void deColliderBone::UpdateMatrix(){
	pMatrix.SetWorld( pPosition, pOrientation );
	pInvMatrix = pMatrix.QuickInvert();
}

void deColliderBone::SetMass( float mass ){
	if( mass < 0.0f ){
		mass = 0.0f;
	}
	
	if( fabs( mass - pMass ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMass = mass;
	pCollider->pNotifyBonePropertiesChanged( pIndex );
}

void deColliderBone::SetDynamic( bool dynamic ){
	if( pDynamic == dynamic ){
		return;
	}
	
	pDynamic = dynamic;
	pCollider->pNotifyBoneDynamicChanged( pIndex );
}
