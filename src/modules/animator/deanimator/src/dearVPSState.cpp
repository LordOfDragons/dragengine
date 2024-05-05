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

#include "dearVPSState.h"
#include "dearAnimationVPSState.h"
#include "component/dearComponentVPSState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>


// class dearVPSState
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dearVPSState::dearVPSState() :
pIndex( 0 ),
pModelIndex( -1 ),
pWeight( 0.0f ),
pProtect( false ),
pDirty( true ){
}

dearVPSState::~dearVPSState(){
}



// Management
///////////////

void dearVPSState::SetName( const char *name ){
	pName = name;
}

void dearVPSState::SetIndex( int index ){
	DEASSERT_TRUE( index >= 0 )
	
	pIndex = index;
}

void dearVPSState::SetModelIndex( int index ){
	pModelIndex = index;
}

void dearVPSState::SetWeight( float weight ){
	pWeight = weight;
	SetDirty( true );
}

void dearVPSState::SetProtected( bool prot ){
	pProtect = prot;
}

void dearVPSState::SetDirty( bool dirty ){
	pDirty = dirty;
}



void dearVPSState::SetFrom( const dearVPSState &state ){
	pWeight = state.pWeight;
	SetDirty( true );
}

void dearVPSState::SetFrom( const deComponent &component ){
	pWeight = component.GetVertexPositionSetWeightAt( pModelIndex );
	SetDirty( true );
}

void dearVPSState::SetFrom( const dearComponentVPSState &state ){
	pWeight = state.GetWeight();
	SetDirty( true );
}

void dearVPSState::BlendWithDefault( deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enableWeight ){
	BlendWith( 0.0f, blendMode, blendFactor, enableWeight );
}

void dearVPSState::BlendWith( const dearVPSState &state,
deAnimatorRule::eBlendModes blendMode, float blendFactor, bool enableWeight ){
	BlendWith( state.GetWeight(), blendMode, blendFactor, enableWeight );
}

void dearVPSState::BlendWith( const dearAnimationVPSState &state,
deAnimatorRule::eBlendModes blendMode, float blendFactor, bool enableWeight ){
	BlendWith( state.GetWeight(), blendMode, blendFactor, enableWeight );
}

void dearVPSState::BlendWith( const deComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor, bool enableWeight ){
	BlendWith( component.GetVertexPositionSetWeightAt( pModelIndex ),
		blendMode, blendFactor, enableWeight );
}

void dearVPSState::BlendWith( const dearComponentVPSState &state,
deAnimatorRule::eBlendModes blendMode, float blendFactor, bool enableWeight ){
	BlendWith( state.GetWeight(), blendMode, blendFactor, enableWeight );
}

void dearVPSState::BlendWith( float weight, deAnimatorRule::eBlendModes blendMode,
float blendFactor, bool enableWeight ){
	switch( blendMode ){
	case deAnimatorRule::ebmBlend:
		// blend new state over old state
		if( fabsf( blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 0
			// keep the old state
			return;
			
		}else if( fabsf( 1.0f - blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 1
			// apply the new state
			if( enableWeight ){
				pWeight = weight;
			}
			
		}else{
			// blend the state
			if( enableWeight ){
				pWeight = pWeight * ( 1.0f - blendFactor ) + weight * blendFactor;
			}
		}
		break;
		
	case deAnimatorRule::ebmOverlay:
		// overlay new state over the old state
		if( fabsf( blendFactor ) < FLOAT_SAFE_EPSILON ){ // blendFactor = 0
			// keep the old state
			return;
			
		}else{
			// add new state to the old state
			if( enableWeight ){
				pWeight += weight * blendFactor;
			}
		}
	}
	
	SetDirty( true );
}

dearVPSState &dearVPSState::operator=( const dearVPSState &state ){
	pIndex = state.pIndex;
	pModelIndex = state.pModelIndex;
	pWeight = state.pWeight;
	pProtect = state.pProtect;
	pDirty = state.pDirty;
	return *this;
}
