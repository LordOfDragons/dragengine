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
#include "../../animator/link/aeLink.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleAnimation.h"
#include "../../animator/rule/aeRuleAnimationDifference.h"
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

aeULinkRemove::aeULinkRemove( aeLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	aeAnimator *animator = link->GetAnimator();
	if( ! animator ) DETHROW( deeInvalidParam );
	
	int targetCount = animator->CountLinkUsage( link );
	
	int type, r, ruleCount = animator->GetRules().GetCount();
	aeRuleAnimation *ruleAnim;
	aeRuleAnimationDifference *ruleAnimDiff;
	aeRuleBoneTransformator *ruleBoneRot;
	aeRuleForeignState *ruleFState;
	aeRuleGroup *ruleGroup;
	aeRuleInverseKinematic *ruleIK;
	aeRuleStateManipulator *ruleSManip;
	aeRuleTrackTo *ruleTrack;
	aeRule *rule;
	
	pAnimator = NULL;
	pLink = NULL;
	
	pTargets = NULL;
	pTargetCount = 0;
	
	SetShortInfo( "Remove Link" );
	
	if( targetCount > 0 ){
		try{
			pTargets = new sTarget[ targetCount ];
			
			for( r=0; r<ruleCount; r++ ){
				rule = animator->GetRules().GetAt( r );
				type = rule->GetType();
				
				if( rule->GetTargetBlendFactor().HasLink( link ) ){
					pAddTarget( targetCount, rule, &rule->GetTargetBlendFactor() );
				}
					
				if( type == deAnimatorRuleVisitorIdentify::ertAnimation ){
					ruleAnim = ( aeRuleAnimation* )rule;
					
					if( ruleAnim->GetTargetMoveTime().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleAnim->GetTargetMoveTime() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertAnimationDifference ){
					ruleAnimDiff = ( aeRuleAnimationDifference* )rule;
					
					if( ruleAnimDiff->GetTargetLeadingMoveTime().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleAnimDiff->GetTargetLeadingMoveTime() );
					}
					if( ruleAnimDiff->GetTargetReferenceMoveTime().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleAnimDiff->GetTargetReferenceMoveTime() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertBoneTransformator ){
					ruleBoneRot = ( aeRuleBoneTransformator* )rule;
					
					if( ruleBoneRot->GetTargetRotation().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleBoneRot->GetTargetRotation() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertForeignState ){
					ruleFState = ( aeRuleForeignState* )rule;
					
					if( ruleFState->GetTargetPosition().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleFState->GetTargetPosition() );
					}
					if( ruleFState->GetTargetOrientation().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleFState->GetTargetOrientation() );
					}
					if( ruleFState->GetTargetSize().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleFState->GetTargetSize() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertGroup ){
					ruleGroup = ( aeRuleGroup* )rule;
					
					if( ruleGroup->GetTargetSelect().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleGroup->GetTargetSelect() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertInverseKinematic ){
					ruleIK = ( aeRuleInverseKinematic* )rule;
					
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
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertStateManipulator ){
					ruleSManip = ( aeRuleStateManipulator* )rule;
					
					if( ruleSManip->GetTargetPosition().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleSManip->GetTargetPosition() );
					}
					if( ruleSManip->GetTargetRotation().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleSManip->GetTargetRotation() );
					}
					
				}else if( type == deAnimatorRuleVisitorIdentify::ertTrackTo ){
					ruleTrack = ( aeRuleTrackTo* )rule;
					
					if( ruleTrack->GetTargetPosition().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleTrack->GetTargetPosition() );
					}
					if( ruleTrack->GetTargetUp().HasLink( link ) ){
						pAddTarget( targetCount, rule, &ruleTrack->GetTargetUp() );
					}
				}
			}
			
			if( pTargetCount != targetCount ) DETHROW( deeInvalidParam );
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	pAnimator = animator;
	pAnimator->AddReference();
	
	pLink = link;
	pLink->AddReference();
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
	int t;
	
	for( t=0; t<pTargetCount; t++ ){
		pTargets[ t ].rule->FreeReference();
	}
	
	if( pLink ) pLink->FreeReference();
	if( pAnimator ) pAnimator->FreeReference();
}

void aeULinkRemove::pAddTarget( int targetCount, aeRule *rule, aeControllerTarget *target ){
	if( pTargetCount >= targetCount ) DETHROW( deeInvalidParam );
	
	pTargets[ pTargetCount ].rule = rule;
	pTargets[ pTargetCount ].target = target;
	
	rule->AddReference();
	
	pTargetCount++;
}
