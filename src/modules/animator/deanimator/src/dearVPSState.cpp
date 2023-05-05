/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
