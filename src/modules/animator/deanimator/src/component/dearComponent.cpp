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

#include "dearComponent.h"
#include "dearComponentBoneState.h"
#include "dearComponentVPSState.h"
#include "../deDEAnimator.h"
#include "../dearBoneState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>



// Class dearComponent
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearComponent::dearComponent( deDEAnimator &module, deComponent &component ) :
pModule( module ),
pComponent( component ),

pBoneStates( nullptr ),
pBoneStateCount( 0 ),

pVPSStates( nullptr ),
pVPSStateCount( 0 )
{
	ModelChanged();
	RigChanged();
}

dearComponent::~dearComponent(){
	if( pVPSStates ){
		delete [] pVPSStates;
	}
	if( pBoneStates ){
		delete [] pBoneStates;
	}
}



// Management
///////////////

dearComponentBoneState &dearComponent::GetBoneStateAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pBoneStateCount )
	
	return pBoneStates[ index ];
}

dearComponentVPSState &dearComponent::GetVPSStateAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVPSStateCount )
	
	return pVPSStates[ index ];
}

void dearComponent::PrepareBones(){
	int i;
	
	for( i=0; i<pBoneStateCount; i++ ){
		dearComponentBoneState &bone = pBoneStates[ i ];
		dearComponentBoneState * const parent = bone.GetParent();
		
		if( parent ){
			bone.SetMatrix( decMatrix::CreateWorld( bone.GetPosition(), bone.GetRotation(), bone.GetScale() )
				.QuickMultiply( bone.GetOriginalMatrix() )
				.QuickMultiply( parent->GetMatrix() ) );
			
		}else{
			bone.SetMatrix( decMatrix::CreateWorld( bone.GetPosition(), bone.GetRotation(), bone.GetScale() )
				.QuickMultiply( bone.GetOriginalMatrix() ) );
		}
	}
}

void dearComponent::UpdateFromComponent(){
	int i;
	
	for( i=0; i<pBoneStateCount; i++ ){
		const deComponentBone &bone = pComponent.GetBoneAt( i );
		pBoneStates[ i ].SetPosition( bone.GetPosition() );
		pBoneStates[ i ].SetRotation( bone.GetRotation() );
		pBoneStates[ i ].SetScale( bone.GetScale() );
	}
	
	for( i=0; i<pVPSStateCount; i++ ){
		pVPSStates[ i ].SetWeight( pComponent.GetVertexPositionSetWeightAt( i ) );
	}
	
	pMatrix = pComponent.GetMatrix();
}

void dearComponent::UpdateMatrixFromComponent(){
	pMatrix = pComponent.GetMatrix();
}

void dearComponent::UpdateComponent(){
	int i;
	
	for( i=0; i<pBoneStateCount; i++ ){
		deComponentBone &bone = pComponent.GetBoneAt( i );
		bone.SetPosition( pBoneStates[ i ].GetPosition() );
		bone.SetRotation( pBoneStates[ i ].GetRotation() );
		bone.SetScale( pBoneStates[ i ].GetScale() );
	}
	
	for( i=0; i<pVPSStateCount; i++ ){
		pComponent.SetVertexPositionSetWeightAt( i, pVPSStates[ i ].GetWeight() );
	}
}

void dearComponent::UpdateComponentPrepareBones(){
	int i;
	
	for( i=0; i<pBoneStateCount; i++ ){
		pComponent.GetBoneAt( i ).SetMatrix( pBoneStates[ i ].GetMatrix() );
	}
	
	pComponent.ValidateBones();
}



void dearComponent::ModelChanged(){
	const int vpsCount = pComponent.GetVertexPositionSetCount();
	if( vpsCount == pVPSStateCount ){
		return;
	}
	
	if( pVPSStates ){
		delete [] pVPSStates;
		pVPSStates = nullptr;
		pVPSStateCount = 0;
	}
	
	if( vpsCount > 0 ){
		pVPSStates = new dearComponentVPSState[ vpsCount ];
		pVPSStateCount = vpsCount;
	}
}

void dearComponent::RigChanged(){
	const int boneCount = pComponent.GetBoneCount();
	if( boneCount == pBoneStateCount ){
		return;
	}
	
	if( pBoneStates ){
		delete [] pBoneStates;
		pBoneStates = nullptr;
		pBoneStateCount = 0;
	}
	
	if( boneCount > 0 ){
		pBoneStates = new dearComponentBoneState[ boneCount ];
		pBoneStateCount = boneCount;
		
		int i;
		for( i=0; i<boneCount; i++ ){
			const deComponentBone &bone = pComponent.GetBoneAt( i );
			if( bone.GetParentBone() != -1 ){
				pBoneStates[ i ].SetParent( pBoneStates + bone.GetParentBone() );
			}
			pBoneStates[ i ].SetOriginalMatrix( bone.GetOriginalMatrix() );
		}
	}
}
