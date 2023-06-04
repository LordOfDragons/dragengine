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

#include "dearRule.h"
#include "../deDEAnimator.h"
#include "../dearAnimator.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneStateList.h"
#include "../dearVPSStateList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>



// Class dearRule
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearRule::dearRule( dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRule &rule ) :
pInstance( instance ),
pAnimator( animator ),
pRule( rule ),

pBoneMappings( nullptr ),
pBoneMappingCount( 0 ),
pUseAllBones( false ),

pVPSMappings( nullptr ),
pVPSMappingCount( 0 ),
pUseAllVPS( false ),

pTargetBlendFactor( rule.GetTargetBlendFactor(), firstLink ),

pBlendMode( rule.GetBlendMode() ),
pBlendFactor( rule.GetBlendFactor() ),
pInvertBlendFactor( rule.GetInvertBlendFactor() ),
pEnabled( rule.GetEnabled() ){
}

dearRule::~dearRule(){
	if( pVPSMappings ){
		delete [] pVPSMappings;
	}
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
	return pUseAllBones ? pInstance.GetBoneStateList().GetStateCount() : pBoneMappingCount;
}

int dearRule::GetBoneMappingFor( int boneIndex ) const{
	if( pUseAllBones ){
		DEASSERT_TRUE( boneIndex >= 0 )
		DEASSERT_TRUE( boneIndex < pInstance.GetBoneStateList().GetStateCount() )
		return boneIndex;
		
	}else{
		DEASSERT_TRUE( boneIndex >= 0 )
		DEASSERT_TRUE( boneIndex < pBoneMappingCount )
		return pBoneMappings[ boneIndex ];
	}
}



int dearRule::GetVPSMappingCount() const{
	return pUseAllVPS ? pInstance.GetVPSStateList().GetStateCount() : pVPSMappingCount;
}

int dearRule::GetVPSMappingFor( int vpsIndex ) const{
	if( pUseAllVPS ){
		DEASSERT_TRUE( vpsIndex >= 0 )
		DEASSERT_TRUE( vpsIndex < pInstance.GetVPSStateList().GetStateCount() )
		return vpsIndex;
		
	}else{
		DEASSERT_TRUE( vpsIndex >= 0 )
		DEASSERT_TRUE( vpsIndex < pVPSMappingCount )
		return pVPSMappings[ vpsIndex ];
	}
}



float dearRule::GetBlendFactor() const{
	const float blendFactor = pTargetBlendFactor.GetValue( pInstance, pBlendFactor );
	return pInvertBlendFactor ? 1.0f - blendFactor : blendFactor;
}

dearAnimation *dearRule::GetUseAnimation() const{
	return pInstance.GetAnimation() ? pInstance.GetAnimation() : pAnimator.GetAnimation();
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
	pUpdateVPSMappings();
}



// Private functions
//////////////////////

void dearRule::pUpdateBoneMappings(){
	const dearBoneStateList &stalist = pInstance.GetBoneStateList();
	const decStringSet &boneList = pRule.GetListBones();
	const int boneCount = boneList.GetCount();
	int i;
	
	// determine the count of bones
	pUseAllBones = boneCount == 0;
	
	// if the count differs recreate the array
	if( pBoneMappingCount != boneCount ){
		int *newArray = nullptr;
		
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

void dearRule::pUpdateVPSMappings(){
	const dearVPSStateList &stalist = pInstance.GetVPSStateList();
	const decStringSet &vpsList = pRule.GetListVertexPositionSets();
	const int vpsCount = vpsList.GetCount();
	int i;
	
	// determine the count of vertex position sets
	pUseAllVPS = vpsCount == 0;
	
	// if the count differs recreate the array
	if( pVPSMappingCount != vpsCount ){
		int *newArray = nullptr;
		
		if( vpsCount > 0 ){
			newArray = new int[ vpsCount ];
		}
		
		if( pVPSMappings ){
			delete [] pVPSMappings;
		}
		pVPSMappings = newArray;
		pVPSMappingCount = vpsCount;
	}
	
	// update the vertex position set mappings. the vertex position sets in the animator are the
	// same as the vertex position sets in the component model.
	for( i=0; i<pVPSMappingCount; i++ ){
		pVPSMappings[ i ] = stalist.IndexOfStateNamed( vpsList.GetAt( i ) );
	}
}
