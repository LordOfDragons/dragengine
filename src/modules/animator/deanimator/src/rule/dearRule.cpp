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

#include "dearRule.h"
#include "../dearBoneStateList.h"
#include "../deDEAnimator.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>



// Class dearRule
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearRule::dearRule( dearAnimatorInstance &instance, int firstLink, const deAnimatorRule &rule ) :
pInstance( instance ),
pRule( rule ),

pBoneMappings( NULL ),
pBoneMappingCount( 0 ),
pUseAllBones( false ),

pTargetBlendFactor( rule.GetTargetBlendFactor(), firstLink ),

pBlendMode( rule.GetBlendMode() ),
pBlendFactor( rule.GetBlendFactor() ),
pEnabled( rule.GetEnabled() )
{
}

dearRule::~dearRule(){
	if( pBoneMappings ){
		delete [] pBoneMappings;
	}
}



// Management
///////////////

deDEAnimator &dearRule::GetModule() const{
	return pInstance.GetModule();
}

int dearRule::GetBoneMappingCount() const{
	if( pUseAllBones ){
		return pInstance.GetBoneStateList().GetStateCount();
		
	}else{
		return pBoneMappingCount;
	}
}

int dearRule::GetBoneMappingFor( int boneIndex ) const{
	if( pUseAllBones ){
		if( boneIndex < 0 || boneIndex >= pInstance.GetBoneStateList().GetStateCount() ){
			DETHROW( deeInvalidParam );
		}
		return boneIndex;
		
	}else{
		if( boneIndex < 0 || boneIndex >= pBoneMappingCount ){
			DETHROW( deeInvalidParam );
		}
		return pBoneMappings[ boneIndex ];
	}
}

float dearRule::GetBlendFactor() const{
	return pTargetBlendFactor.GetValue( pInstance, pBlendFactor );
}



void dearRule::CaptureStateInto( int ){
}

void dearRule::StoreFrameInto( int, const char *, float ){
}

bool dearRule::RebuildInstance() const{
	return false;
}

void dearRule::ControllerChanged( int ){
}

void dearRule::RuleChanged(){
	pUpdateBoneMappings();
}



// Private functions
//////////////////////

void dearRule::pUpdateBoneMappings(){
	const dearBoneStateList &stalist = pInstance.GetBoneStateList();
	const decStringSet &boneList = pRule.GetListBones();
	const int boneCount = boneList.GetCount();
	int i;
	
	// determine the count of bones
	pUseAllBones = ( boneCount == 0 );
	
	// if the count differs recreate the array
	if( pBoneMappingCount != boneCount ){
		int *newArray = NULL;
		
		if( boneCount > 0 ){
			newArray = new int[ boneCount ];
		}
		
		if( pBoneMappings ){
			delete [] pBoneMappings;
		}
		pBoneMappings = newArray;
		pBoneMappingCount = boneCount;
	}
	
	// update the bone mappings. the bones in the animator are the
	// same as the bones in the component rig.
	for( i=0; i<pBoneMappingCount; i++ ){
		pBoneMappings[ i ] = stalist.IndexOfStateNamed( boneList.GetAt( i ) );
	}
}
