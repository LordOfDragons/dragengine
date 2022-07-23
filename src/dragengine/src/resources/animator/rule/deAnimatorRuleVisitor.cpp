/* 
 * Drag[en]gine Game Engine
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
