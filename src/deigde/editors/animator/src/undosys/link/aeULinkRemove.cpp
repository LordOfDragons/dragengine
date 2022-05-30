/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <stdlib.h>
#include <string.h>

#include "aeULinkRemove.h"

#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRuleAnimation.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"
#include "../../animator/rule/aeRuleAnimationSelect.h"
#include "../../animator/rule/aeRuleBoneTransformator.h"
#include "../../animator/rule/aeRuleForeignState.h"
#include "../../animator/rule/aeRuleGroup.h"
#include "../../animator/rule/aeRuleInverseKinematic.h"
#include "../../animator/rule/aeRuleStateManipulator.h"
#include "../../animator/rule/aeRuleTrackTo.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>

#include <dragengine/common/exceptions.h>



// Class aeULinkRemove
/////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkRemove::aeULinkRemove( aeLink *link ) :
pLink( link ),
pTargets( nullptr ),
pTargetCount( 0 )
{
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	pAnimator = link->GetAnimator();
	if( ! pAnimator ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove Link" );
	
	const int targetCount = pAnimator->CountLinkUsage( link );
	if( targetCount == 0 ){
		return;
	}
	
	try{
		pTargets = new sTarget[ targetCount ];
		
		const int ruleCount = pAnimator->GetRules().GetCount();
		int i;
		for( i=0; i<ruleCount; i++ ){
			pAddTargets( targetCount, link, pAnimator->GetRules().GetAt( i ) );
		}
		
		if( pTargetCount != targetCount ){
			DETHROW( deeInvalidParam );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeULinkRemove::~aeULinkRemove(){
	pCleanUp();
}



// Management
///////////////

void aeULinkRemove::Undo(){
	int t;
	
	pAnimator->AddLink( pLink );
	
	for( t=0; t<pTargetCount; t++ ){
		pTargets[ t ].target->AddLink( pLink );
		pTargets[ t ].rule->NotifyRuleChanged();
	}
}

void aeULinkRemove::Redo(){
	int t;
	
	for( t=0; t<pTargetCount; t++ ){
		pTargets[ t ].target->RemoveLink( pLink );
		pTargets[ t ].rule->NotifyRuleChanged();
	}
	
	pAnimator->RemoveLink( pLink );
}



// Private Functions
//////////////////////

void aeULinkRemove::pCleanUp(){
	if( pTargets ){
		delete [] pTargets;
	}
}

void aeULinkRemove::pAddTargets( int targetCount, aeLink *link, aeRule *rule ){
	if( rule->GetTargetBlendFactor().HasLink( link ) ){
		pAddTarget( targetCount, rule, &rule->GetTargetBlendFactor() );
	}
	
	switch( rule->GetType() ){
	case deAnimatorRuleVisitorIdentify::ertAnimation:{
		aeRuleAnimation * const ruleAnim = ( aeRuleAnimation* )rule;
		
		if( ruleAnim->GetTargetMoveTime().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleAnim->GetTargetMoveTime() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationDifference:{
		aeRuleAnimationDifference * const ruleAnimDiff = ( aeRuleAnimationDifference* )rule;
		
		if( ruleAnimDiff->GetTargetLeadingMoveTime().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleAnimDiff->GetTargetLeadingMoveTime() );
		}
		if( ruleAnimDiff->GetTargetReferenceMoveTime().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleAnimDiff->GetTargetReferenceMoveTime() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationSelect:{
		aeRuleAnimationSelect * const ruleAnimSelect = ( aeRuleAnimationSelect* )rule;
		
		if( ruleAnimSelect->GetTargetMoveTime().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleAnimSelect->GetTargetMoveTime() );
		}
		if( ruleAnimSelect->GetTargetSelect().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleAnimSelect->GetTargetSelect() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertBoneTransformator:{
		aeRuleBoneTransformator * const ruleBoneRot = ( aeRuleBoneTransformator* )rule;
		
		if( ruleBoneRot->GetTargetRotation().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleBoneRot->GetTargetRotation() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertForeignState:{
		aeRuleForeignState * const ruleFState = ( aeRuleForeignState* )rule;
		
		if( ruleFState->GetTargetPosition().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleFState->GetTargetPosition() );
		}
		if( ruleFState->GetTargetOrientation().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleFState->GetTargetOrientation() );
		}
		if( ruleFState->GetTargetSize().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleFState->GetTargetSize() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertGroup:{
		aeRuleGroup * const ruleGroup = ( aeRuleGroup* )rule;
		
		if( ruleGroup->GetTargetSelect().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleGroup->GetTargetSelect() );
		}
		
		const int count = ruleGroup->GetRules().GetCount();
		int i;
		for( i=0; i<count; i++ ){
			pAddTargets( targetCount, link, ruleGroup->GetRules().GetAt( i ) );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertInverseKinematic:{
		aeRuleInverseKinematic * const ruleIK = ( aeRuleInverseKinematic* )rule;
		
		if( ruleIK->GetTargetGoalPosition().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetGoalPosition() );
		}
		if( ruleIK->GetTargetGoalOrientation().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetGoalOrientation() );
		}
		if( ruleIK->GetTargetLocalPosition().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetLocalPosition() );
		}
		if( ruleIK->GetTargetLocalOrientation().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetLocalOrientation() );
		}
		if( ruleIK->GetTargetReachRange().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetReachRange() );
		}
		if( ruleIK->GetTargetReachCenter().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleIK->GetTargetReachCenter() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertStateManipulator:{
		aeRuleStateManipulator * const ruleSManip = ( aeRuleStateManipulator* )rule;
		
		if( ruleSManip->GetTargetPosition().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleSManip->GetTargetPosition() );
		}
		if( ruleSManip->GetTargetRotation().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleSManip->GetTargetRotation() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertTrackTo:{
		aeRuleTrackTo * const ruleTrack = ( aeRuleTrackTo* )rule;
		
		if( ruleTrack->GetTargetPosition().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleTrack->GetTargetPosition() );
		}
		if( ruleTrack->GetTargetUp().HasLink( link ) ){
			pAddTarget( targetCount, rule, &ruleTrack->GetTargetUp() );
		}
		}break;
		
	case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
	case deAnimatorRuleVisitorIdentify::ertSubAnimator:
	case deAnimatorRuleVisitorIdentify::ertLimit:
	case deAnimatorRuleVisitorIdentify::ertMirror:
	case deAnimatorRuleVisitorIdentify::ertUnknown:
		break;
	}
}

void aeULinkRemove::pAddTarget( int targetCount, aeRule *rule, aeControllerTarget *target ){
	if( pTargetCount >= targetCount ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets[ pTargetCount ].rule = rule;
	pTargets[ pTargetCount ].target = target;
	pTargetCount++;
}
