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
#include <stdlib.h>
#include <string.h>

#include "dearCreateRuleVisitor.h"
#include "dearRule.h"
#include "dearRuleAnimation.h"
#include "dearRuleAnimationDifference.h"
#include "dearRuleAnimationSelect.h"
#include "dearRuleBoneTransformator.h"
#include "dearRuleInverseKinematic.h"
#include "dearRuleStateManipulator.h"
#include "dearRuleStateSnapshot.h"
#include "dearRuleForeignState.h"
#include "dearRuleGroup.h"
#include "dearRuleSubAnimator.h"
#include "dearRuleTrackTo.h"
#include "dearRuleLimit.h"
#include "dearRuleMirror.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleInverseKinematic.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/common/exceptions.h>



// Class dearCreateRuleVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

dearCreateRuleVisitor::dearCreateRuleVisitor( dearAnimatorInstance &instance,
const dearAnimator &animator, const decIntList &controllerMapping, int firstLink ) :
pInstance( instance ),
pAnimator( animator ),
pControllerMapping( controllerMapping ),
pFirstLink( firstLink ),
pCreatedRule( nullptr ){
}

dearCreateRuleVisitor::~dearCreateRuleVisitor(){
}



// Management
///////////////

void dearCreateRuleVisitor::Reset(){
	pCreatedRule = nullptr;
}

bool dearCreateRuleVisitor::HasCreatedRule() const{
	return pCreatedRule;
}

dearRule *dearCreateRuleVisitor::CreateRuleFrom( deAnimatorRule &engRule ){
	Reset();
	engRule.Visit( *this );
	return pCreatedRule;
}



// Visiting
/////////////

void dearCreateRuleVisitor::VisitRule( deAnimatorRule& ){
	pCreatedRule = NULL;
}

void dearCreateRuleVisitor::VisitAnimation( deAnimatorRuleAnimation &rule ){
	pCreatedRule = new dearRuleAnimation( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitAnimationDifference( deAnimatorRuleAnimationDifference &rule ){
	pCreatedRule = new dearRuleAnimationDifference( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitAnimationSelect( deAnimatorRuleAnimationSelect &rule ){
	pCreatedRule = new dearRuleAnimationSelect( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitBoneTransformator( deAnimatorRuleBoneTransformator &rule ){
	pCreatedRule = new dearRuleBoneTransformator( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitInverseKinematic( deAnimatorRuleInverseKinematic &rule ){
	pCreatedRule = new dearRuleInverseKinematic( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitStateManipulator( deAnimatorRuleStateManipulator &rule ){
	pCreatedRule = new dearRuleStateManipulator( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitStateSnapshot( deAnimatorRuleStateSnapshot &rule ){
	pCreatedRule = new dearRuleStateSnapshot( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitForeignState( deAnimatorRuleForeignState &rule ){
	pCreatedRule = new dearRuleForeignState( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitGroup( deAnimatorRuleGroup &rule ){
	pCreatedRule = new dearRuleGroup( pInstance, pAnimator, pFirstLink, rule, pControllerMapping );
}

void dearCreateRuleVisitor::VisitSubAnimator( deAnimatorRuleSubAnimator &rule ){
	pCreatedRule = new dearRuleSubAnimator( pInstance, pAnimator, pFirstLink, rule, pControllerMapping );
}

void dearCreateRuleVisitor::VisitTrackTo( deAnimatorRuleTrackTo &rule ){
	pCreatedRule = new dearRuleTrackTo( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitLimit( deAnimatorRuleLimit &rule ){
	pCreatedRule = new dearRuleLimit( pInstance, pAnimator, pFirstLink, rule );
}

void dearCreateRuleVisitor::VisitMirror( deAnimatorRuleMirror &rule ){
	pCreatedRule = new dearRuleMirror( pInstance, pAnimator, pFirstLink, rule );
}
