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

#include <stdlib.h>

#include "dearAnimatorInstance.h"
#include "dearVPSState.h"
#include "dearVPSStateList.h"
#include "dearMapAnimationVPS.h"
#include "animation/dearAnimation.h"
#include "rule/dearRule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>


// Class dearMapAnimationVPS
//////////////////////////////

// Constructor, destructor
////////////////////////////

dearMapAnimationVPS::dearMapAnimationVPS() :
pIndices( nullptr ),
pCount( 0 ){
}

dearMapAnimationVPS::~dearMapAnimationVPS(){
	if( pIndices ){
		delete [] pIndices;
	}
}



// Management
///////////////

void dearMapAnimationVPS::Clear(){
	if( pIndices ){
		delete [] pIndices;
		pIndices = nullptr;
	}
	pCount = 0;
}

void dearMapAnimationVPS::Init( const dearRule &rule ){
	Clear();
	
	const int count = rule.GetVPSMappingCount();
	if( count == 0 ){
		return;
	}
	
	pIndices = new int[ count ];
	
	const dearAnimation * const animation = rule.GetUseAnimation();
	
	if( animation ){
		const dearVPSStateList &vpsStates = rule.GetInstance().GetVPSStateList();
		const deAnimation &engAnimation = *animation->GetAnimation();
		
		for( pCount=0; pCount<count; pCount++ ){
			const int ruleVPSIndex = rule.GetVPSMappingFor( pCount );
			if( ruleVPSIndex != -1 ){
				pIndices[ pCount ] = engAnimation.GetVertexPositionSets().IndexOf(
					vpsStates.GetStateAt( ruleVPSIndex ).GetName() );
				
			}else{
				pIndices[ pCount ] = -1;
			}
		}
		
	}else{
		for( pCount=0; pCount<count; pCount++ ){
			pIndices[ pCount ] = -1;
		}
	}
}

int dearMapAnimationVPS::GetAt( int ruleVPSIndex ) const{
	DEASSERT_TRUE( ruleVPSIndex >= 0 )
	DEASSERT_TRUE( ruleVPSIndex < pCount )
	
	return pIndices[ ruleVPSIndex ];
}
