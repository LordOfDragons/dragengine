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
#include "dearAnimationState.h"
#include "component/dearComponentBoneState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponentBone.h>



// class dearBoneState
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearBoneState::dearBoneState(){
	pRigBone = NULL;
	pRigBoneName = NULL;
	pIndex = 0;
	pRigIndex = -1;
	pAnimBone = -1;
	pParentState = NULL;
	pChildStates = NULL;
	pChildStateCount = 0;
	pChildStateSize = 0;
	pScale.Set( 1.0f, 1.0f, 1.0f );
	pProtect = false;
	pDirty = true;
}

dearBoneState::~dearBoneState(){
	if( pChildStates ){
		delete [] pChildStates;
	}
}



// Management
///////////////

void dearBoneState::SetParentState( dearBoneState *boneState ){
	pParentState = boneState;
	SetDirty( true );
}

void dearBoneState::SetRigBone( deRigBone *bone ){
	pRigBone = bone;
}

void dearBoneState::SetRigBoneName( const char *name ){
	pRigBoneName = name;
}

void dearBoneState::SetIndex( int index ){
	if( index < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = index;
}

void dearBoneState::SetRigIndex( int index ){
	pRigIndex = index;
}

void dearBoneState::SetAnimationBone( int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pAnimBone = index;
}

void dearBoneState::SetPosition( const decVector &position ){
	pPosition = position;
	SetDirty( true );
}

void dearBoneState::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
	SetDirty( true );
}

void dearBoneState::SetScale( const decVector &size ){
	pScale = size;
	SetDirty( true );
}

void dearBoneState::SetPosOrient( const decVector &position, const decQuaternion &orientation ){
	pPosition = position;
	pOrientation = orientation;
	SetDirty( true );
}

void dearBoneState::SetRigLocalMatrix( const decMatrix &matrix ){
	pRigLocalMatrix = matrix;
	pInvRigLocalMatrix = matrix.QuickInvert();
}

void dearBoneState::SetLocalMatrix( const decMatrix &matrix ){
	pLocalMatrix = matrix;
}

void dearBoneState::SetInverseLocalMatrix( const decMatrix &matrix ){
	pInvLocalMatrix = matrix;
}

void dearBoneState::SetGlobalMatrix( const decMatrix &matrix ){
	pGlobalMatrix = matrix;
}

void dearBoneState::SetInverseGlobalMatrix( const decMatrix &matrix ){
	pInvGlobalMatrix = matrix;
}

void dearBoneState::SetProtected( bool prot ){
	pProtect = prot;
}

void dearBoneState::SetDirty( bool dirty ){
	if( dirty == pDirty ){
		return;
	}
	
	pDirty = dirty;
	
	if( dirty ){
		int i;
		
		for( i=0; i<pChildStateCount; i++ ){
			pChildStates[ i ]->SetDirty( true );
		}
	}
}

void dearBoneState::UpdateMatrices(){
	if( ! pDirty ){
		return;
	}
	
	// local matrices
	pLocalMatrix.SetWorld( pPosition, pOrientation, pScale );
	pInvLocalMatrix = pLocalMatrix.QuickInvert();
	
	// global matrices
	if( pParentState ){
		pParentState->UpdateMatrices();
		pGlobalMatrix = pLocalMatrix.QuickMultiply( pRigLocalMatrix )
			.QuickMultiply( pParentState->GetGlobalMatrix() );
		
	}else{
		pGlobalMatrix = pLocalMatrix.QuickMultiply( pRigLocalMatrix );
	}
	
	pInvGlobalMatrix = pGlobalMatrix.QuickInvert();
	
	// no more dirty
	pDirty = false;
}

void dearBoneState::UpdateFromGlobalMatrix(){
	const decMatrix matrix = CalcLocalFromGlobal( pGlobalMatrix );
	SetPosition( matrix.GetPosition() );
	SetOrientation( matrix.ToQuaternion() );
}

decMatrix dearBoneState::CalcLocalFromGlobal( const decMatrix &globalMatrix ) const{
	if( pParentState ){
		return globalMatrix.QuickMultiply( pParentState->GetInverseGlobalMatrix() )
			.QuickMultiply( pInvRigLocalMatrix );
		
	}else{
		return globalMatrix.QuickMultiply( pInvRigLocalMatrix );
	}
}

void dearBoneState::UpdateMatricesKeepGlobal(){
	if( ! pDirty ){
		return;
	}
	
	pLocalMatrix.SetWorld( pPosition, pOrientation, pScale );
	pInvLocalMatrix = pLocalMatrix.QuickInvert();
	
	pInvGlobalMatrix = pGlobalMatrix.QuickInvert();
	
	pDirty = false;
}

void dearBoneState::ResetMatrices(){
	pRigLocalMatrix.SetIdentity();
	pInvRigLocalMatrix.SetIdentity();
	pLocalMatrix.SetIdentity();
	pInvLocalMatrix.SetIdentity();
	pGlobalMatrix.SetIdentity();
	pInvGlobalMatrix.SetIdentity();
}



void dearBoneState::SetFrom( const dearBoneState &state ){
	pPosition = state.pPosition;
	pOrientation = state.pOrientation;
	pScale = state.pScale;
	SetDirty( true );
}

void dearBoneState::SetFrom( const deComponentBone &bone ){
	pPosition = bone.GetPosition();
	pOrientation = bone.GetRotation();
	pScale = bone.GetScale();
	SetDirty( true );
}

void dearBoneState::SetFrom( const dearComponentBoneState &state ){
	pPosition = state.GetPosition();
	pOrientation = state.GetRotation();
	pScale = state.GetScale();
	SetDirty( true );
}

void dearBoneState::BlendWithDefault( deAnimatorRule::eBlendModes blendMode, float blendFactor,
bool enablePosition, bool enableOrientation, bool enableScale ){
	BlendWith( decVector(), decQuaternion(), decVector( 1.0f, 1.0f, 1.0f ),
		blendMode, blendFactor, enablePosition, enableOrientation, enableScale );
}

void dearBoneState::BlendWith( const dearBoneState& state, deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale ){
	BlendWith( state.GetPosition(), state.GetOrientation(), state.GetScale(),
		blendMode, blendFactor, enablePosition, enableOrientation, enableScale );
}

void dearBoneState::BlendWith( const dearAnimationState& state, deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale ){
	BlendWith( state.GetPosition(), state.GetOrientation(), state.GetSize(),
		blendMode, blendFactor, enablePosition, enableOrientation, enableScale );
}

void dearBoneState::BlendWith( const deComponentBone& bone, deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale ){
	BlendWith( bone.GetPosition(), bone.GetRotation(), bone.GetScale(),
		blendMode, blendFactor, enablePosition, enableOrientation, enableScale );
}

void dearBoneState::BlendWith( const dearComponentBoneState &state, deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enablePosition, bool enableOrientation, bool enableScale ){
	BlendWith( state.GetPosition(), state.GetRotation(), state.GetScale(),
		blendMode, blendFactor, enablePosition, enableOrientation, enableScale );
}

void dearBoneState::BlendWith( const decVector& position, const decQuaternion& orientation,
deAnimatorRule::eBlendModes blendMode, float blendFactor, bool enablePosition, bool enableOrientation ){
	BlendWith( position, orientation, decVector( 1.0f, 1.0f, 1.0f ),
		blendMode, blendFactor, enablePosition, enableOrientation, false );
}

void dearBoneState::BlendWith( const decVector& position, const decQuaternion& orientation,
const decVector& scale, deAnimatorRule::eBlendModes blendMode, float blendFactor,
bool enablePosition, bool enableOrientation, bool enableScale ){
	// blend new state over old state
	if( blendMode == deAnimatorRule::ebmBlend ){
		if( fabsf( blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 0
			// keep the old state
			return;
			
		}else if( fabsf( 1.0f - blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 1
			// apply the new state
			if( enablePosition ){
				pPosition = position;
			}
			
			if( enableOrientation ){
				pOrientation = orientation;
			}
			
			if( enableScale ){
				pScale = scale;
			}
			
		}else{
			// blend the state
			if( enablePosition ){
				pPosition = pPosition * ( 1.0f - blendFactor ) + position * blendFactor;
			}
			
			if( enableOrientation ){
				pOrientation = pOrientation.Slerp( orientation, blendFactor );
				
				//pOrientation =
				//	( decQuaternion( 0.0f, 0.0f, 0.0f, blendFactor ) + pOrientation * ( 1.0f - blendFactor ) ) *
				//	( decQuaternion( 0.0f, 0.0f, 0.0f, 1.0f - blendFactor ) + orientation * blendFactor );
				
				// pOrientation =
				//    decQuaternion().Slerp( pOrientation, 1.0f - blendFactor ) *
				//    decQuaternion().Slerp( orientation, blendFactor ) );
			}
			
			if( enableScale ) {
				pScale = pScale * ( 1.0f - blendFactor ) + scale * blendFactor;
			}
		}
		
	// overlay new state over the old state
	}else if( blendMode == deAnimatorRule::ebmOverlay ){
		if( fabsf( blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 0
			// keep the old state
			return;
			
		}else{
			// add new state to the old state
			if( enablePosition ){
				pPosition += position * blendFactor;
			}
			
			if( enableOrientation ){
				pOrientation *= decQuaternion().Slerp( orientation, blendFactor );
				
				//pOrientation =
				//	( decQuaternion( 0.0f, 0.0f, 0.0f, blendFactor ) + pOrientation * ( 1.0f - blendFactor ) ) *
				//	( decQuaternion( 0.0f, 0.0f, 0.0f, 1.0f - blendFactor ) + orientation * blendFactor );
				
				// pOrientation =
				//    decQuaternion().Slerp( pOrientation, 1.0f - blendFactor ) *
				//    decQuaternion().Slerp( orientation, blendFactor ) );
			}
			
			if( enableScale ) {
				pScale += ( scale - decVector( 1.0f, 1.0f, 1.0f ) ) * blendFactor;
			}
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	SetDirty( true );
}



// Child states
/////////////////

dearBoneState *dearBoneState::GetChildStateAt( int index ) const{
	if( index < 0 || index >= pChildStateCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pChildStates[ index ];
}

void dearBoneState::AddChildState( dearBoneState *boneState ){
	if( ! boneState ){
		DETHROW( deeInvalidParam );
	}
	
	if( pChildStateCount == pChildStateSize ){
		const int newSize = pChildStateSize * 3 / 2 + 1;
		dearBoneState ** const newArray = new dearBoneState*[ newSize ];
		
		if( pChildStates ){
			int i;
			for( i=0; i<pChildStateSize; i++ ){
				newArray[ i ] = pChildStates[ i ];
			}
			delete [] pChildStates;
		}
		pChildStates = newArray;
		pChildStateSize = newSize;
	}
	
	pChildStates[ pChildStateCount ] = boneState;
	pChildStateCount++;
}

void dearBoneState::RemoveAllChildStates(){
	pChildStateCount = 0;
}
