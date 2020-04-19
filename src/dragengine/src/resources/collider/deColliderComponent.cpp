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

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deColliderComponent.h"
#include "deColliderBone.h"
#include "deColliderVisitor.h"
#include "../component/deComponent.h"
#include "../component/deComponentBone.h"
#include "../rig/deRig.h"
#include "../rig/deRigBone.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/physics/deBasePhysicsCollider.h"



// Class deColliderComponent
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderComponent::deColliderComponent( deColliderManager *manager ) :
deColliderRig( manager ){
}

deColliderComponent::~deColliderComponent(){
}



// Management
///////////////

void deColliderComponent::SetComponent( deComponent *component ){
	if( component == pComponent ){
		return;
	}
	
	pComponent = component;
	
	pNotifyComponentChanged();
	
	if( component ){
		SetRig( component->GetRig() );
		
	}else{
		SetRig( NULL );
	}
}



void deColliderComponent::CopyStatesFromComponent(){
	if( ! pComponent ){
		return;
	}
	
	pComponent->PrepareMatrix();
	pComponent->PrepareBones();
	
	const decDMatrix &componentMatrix = pComponent->GetMatrix();
	const decVector &angularVelocity = GetAngularVelocity();
	const decVector &linearVelocity = GetLinearVelocity();
	const deRig * const rig = pComponent->GetRig();
	const int boneCount = GetBoneCount();
	int i;
	
	for( i=0; i <boneCount; i++ ){
		const decDMatrix boneMatrix( decDMatrix( pComponent->GetBoneAt( i ).GetMatrix() )
			.QuickMultiply( componentMatrix ) );
		deColliderBone &bone = GetBoneAt( i );
		
		if( rig ){
			bone.SetPosition( boneMatrix * rig->GetBoneAt( i ).GetCentralMassPoint() );
			
		}else{
			bone.SetPosition( boneMatrix.GetPosition() );
		}
		
		bone.SetOrientation( boneMatrix.ToQuaternion() );
		bone.SetLinearVelocity( linearVelocity );
		bone.SetAngularVelocity( angularVelocity );
		bone.UpdateMatrix();
	}
}

void deColliderComponent::CopyStateFromComponent( int bone ){
	if( ! pComponent || bone < 0 || bone >= GetBoneCount() ){
		return;
	}
	
	pComponent->PrepareMatrix();
	pComponent->PrepareBones();
	
	const decDMatrix &componentMatrix = pComponent->GetMatrix();
	const decVector &angularVelocity = GetAngularVelocity();
	const decVector &linearVelocity = GetLinearVelocity();
	const deRig * const rig = pComponent->GetRig();
	
	const decDMatrix boneMatrix( decDMatrix( pComponent->GetBoneAt( bone ).GetMatrix() )
		.QuickMultiply( componentMatrix ) );
	deColliderBone &colBone = GetBoneAt( bone );
	
	if( rig ){
		colBone.SetPosition( boneMatrix * rig->GetBoneAt( bone ).GetCentralMassPoint() );
		
	}else{
		colBone.SetPosition( boneMatrix.GetPosition() );
	}
	
	colBone.SetOrientation( boneMatrix.ToQuaternion() );
	colBone.SetLinearVelocity( linearVelocity );
	colBone.SetAngularVelocity( angularVelocity );
	colBone.UpdateMatrix();
}

void deColliderComponent::CopyStatesToComponent() const{
	if( ! pComponent ){
		return;
	}
	
	pComponent->PrepareMatrix();
	pComponent->InvalidateBones();
	
	const decDMatrix &invCompMatrix = pComponent->GetInverseMatrix();
	const deRig * const rig = pComponent->GetRig();
	const int boneCount = GetBoneCount();
	int i;
	
	for( i=0; i<boneCount; i++ ){
		deComponentBone &bone = pComponent->GetBoneAt( i );
		if( rig ){
			if( ! rig->GetBoneAt( i ).GetDynamic() ){
				pComponent->UpdateBoneAt( i );
				continue; // state is not defined
			}
		}
		
		decMatrix matrix( GetBoneAt( i ).GetMatrix().QuickMultiply( invCompMatrix ) );
		
		if( rig ){
			const decVector cmpPosition( matrix * -rig->GetBoneAt( i ).GetCentralMassPoint() );
			matrix.a14 = cmpPosition.x;
			matrix.a24 = cmpPosition.y;
			matrix.a34 = cmpPosition.z;
		}
		
		bone.SetMatrix( matrix );
		
		if( bone.GetParentBone() != -1 ){
			matrix = matrix.QuickMultiply( pComponent->GetBoneAt( bone.GetParentBone() ).GetInverseMatrix() );
		}
		matrix = matrix.QuickMultiply( bone.GetInverseOriginalMatrix() );
		
		bone.SetPosition( matrix.GetPosition() );
		bone.SetRotation( matrix.ToQuaternion() );
		bone.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
	}
	
	pComponent->ValidateBones();
}

void deColliderComponent::CopyStateToComponent( int bone ) const{
	if( ! pComponent || bone < 0 || bone >= GetBoneCount() ){
		return;
	}
	
	pComponent->PrepareMatrix();
	pComponent->PrepareBones();
	
	const decDMatrix &inverseComponentMatrix = pComponent->GetInverseMatrix();
	const deRig * const rig = pComponent->GetRig();
	
	decMatrix matrix( GetBoneAt( bone ).GetMatrix().QuickMultiply( inverseComponentMatrix ) );
	if( rig ){
		const decVector cmpPosition( matrix * -rig->GetBoneAt( bone ).GetCentralMassPoint() );
		matrix.a14 = cmpPosition.x;
		matrix.a24 = cmpPosition.y;
		matrix.a34 = cmpPosition.z;
	}
	
	deComponentBone &componentBone = pComponent->GetBoneAt( bone );
	if( componentBone.GetParentBone() != -1 ){
		matrix = matrix.QuickMultiply( pComponent->GetBoneAt( componentBone.GetParentBone() ).GetInverseMatrix() );
	}
	matrix = matrix.QuickMultiply( componentBone.GetInverseOriginalMatrix() );
	
	componentBone.SetPosition( matrix.GetPosition() );
	componentBone.SetRotation( matrix.ToQuaternion() );
	componentBone.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
	
	pComponent->InvalidateBones();
}

void deColliderComponent::InitWeightAttachment( deColliderAttachment &attachment, int face ){
	deBasePhysicsCollider * const peer = GetPeerPhysics();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	peer->InitWeightAttachment( attachment, face );
}



// Visiting
/////////////

void deColliderComponent::Visit( deColliderVisitor &visitor ){
	visitor.VisitComponent( *this );
}
