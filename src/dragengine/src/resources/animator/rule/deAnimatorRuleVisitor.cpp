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

#include "deAnimatorRule.h"
#include "deAnimatorRuleAnimation.h"
#include "deAnimatorRuleAnimationDifference.h"
#include "deAnimatorRuleAnimationSelect.h"
#include "deAnimatorRuleBoneTransformator.h"
#include "deAnimatorRuleInverseKinematic.h"
#include "deAnimatorRuleStateManipulator.h"
#include "deAnimatorRuleStateSnapshot.h"
#include "deAnimatorRuleForeignState.h"
#include "deAnimatorRuleGroup.h"
#include "deAnimatorRuleSubAnimator.h"
#include "deAnimatorRuleVisitor.h"
#include "deAnimatorRuleTrackTo.h"
#include "deAnimatorRuleLimit.h"
#include "deAnimatorRuleMirror.h"

#include "../../../common/exceptions.h"



// Class deAnimatorRuleVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleVisitor::deAnimatorRuleVisitor(){
}

deAnimatorRuleVisitor::~deAnimatorRuleVisitor(){
}



// Visiting
/////////////

void deAnimatorRuleVisitor::VisitRule( deAnimatorRule& ){
}

void deAnimatorRuleVisitor::VisitAnimation( deAnimatorRuleAnimation &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitAnimationDifference( deAnimatorRuleAnimationDifference &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitAnimationSelect( deAnimatorRuleAnimationSelect &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitBoneTransformator( deAnimatorRuleBoneTransformator &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitInverseKinematic( deAnimatorRuleInverseKinematic &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitStateManipulator( deAnimatorRuleStateManipulator &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitStateSnapshot( deAnimatorRuleStateSnapshot &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitForeignState( deAnimatorRuleForeignState &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitGroup( deAnimatorRuleGroup &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitSubAnimator( deAnimatorRuleSubAnimator &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitTrackTo( deAnimatorRuleTrackTo &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitLimit( deAnimatorRuleLimit &rule ){
	VisitRule( rule );
}

void deAnimatorRuleVisitor::VisitMirror( deAnimatorRuleMirror &rule ){
	VisitRule( rule );
}
