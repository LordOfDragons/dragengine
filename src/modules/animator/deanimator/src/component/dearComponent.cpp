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
