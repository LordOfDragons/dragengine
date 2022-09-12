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

#include <stdlib.h>

#include "dearAnimatorInstance.h"
#include "dearBoneState.h"
#include "dearBoneStateList.h"
#include "dearMapAnimationBones.h"
#include "animation/dearAnimation.h"
#include "rule/dearRule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>


// Class dearMapAnimationBones
////////////////////////////////

// Constructor, destructor
////////////////////////////

dearMapAnimationBones::dearMapAnimationBones() :
pIndices( nullptr ),
pCount( 0 ){
}

dearMapAnimationBones::~dearMapAnimationBones(){
	if( pIndices ){
		delete [] pIndices;
	}
}



// Management
///////////////

void dearMapAnimationBones::Clear(){
	if( pIndices ){
		delete [] pIndices;
		pIndices = nullptr;
	}
	pCount = 0;
}

void dearMapAnimationBones::Init( const deAnimation &animation, const dearBoneStateList &boneStates ){
	Clear();
	
	const int count = boneStates.GetStateCount();
	if( count == 0 ){
		return;
	}
	
	pIndices = new int[ count ];
	for( pCount=0; pCount<count; pCount++ ){
		pIndices[ pCount ] = animation.FindBone( boneStates.GetStateAt( pCount )->GetRigBoneName() );
	}
}
#include <stdio.h>
void dearMapAnimationBones::Init( const dearRule &rule ){
	Clear();
	
	const int count = rule.GetBoneMappingCount();
	if( count == 0 ){
		return;
	}
	
	const dearAnimation * const animation = rule.GetUseAnimation();
	if( ! animation ){
		return;
	}
	
	const dearBoneStateList &boneStates = rule.GetInstance().GetBoneStateList();
	const deAnimation &engAnimation = *animation->GetAnimation();
	
	pIndices = new int[ count ];
	for( pCount=0; pCount<count; pCount++ ){
		const int boneStateIndex = rule.GetBoneMappingFor( pCount );
		if( boneStateIndex != -1 ){
			pIndices[ pCount ] = engAnimation.FindBone(
				boneStates.GetStateAt( boneStateIndex )->GetRigBoneName() );
			
		}else{
			pIndices[ pCount ] = -1;
		}
	}
}

int dearMapAnimationBones::GetAt( int boneStateIndex ) const{
	DEASSERT_TRUE( boneStateIndex >= 0 )
	DEASSERT_TRUE( boneStateIndex < pCount )
	
	return pIndices[ boneStateIndex ];
}
