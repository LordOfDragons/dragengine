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
