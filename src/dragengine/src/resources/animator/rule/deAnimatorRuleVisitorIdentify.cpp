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
#include "deAnimatorRuleTrackTo.h"
#include "deAnimatorRuleVisitorIdentify.h"
#include "deAnimatorRuleLimit.h"
#include "deAnimatorRuleMirror.h"

#include "../../../common/exceptions.h"



// Class deAnimatorRuleVisitorIdentify
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleVisitorIdentify::deAnimatorRuleVisitorIdentify() :
pRule( NULL ),
pType( ertUnknown ){
}

deAnimatorRuleVisitorIdentify::~deAnimatorRuleVisitorIdentify(){
}



// Management
///////////////

deAnimatorRuleAnimation &deAnimatorRuleVisitorIdentify::CastToAnimation() const{
	if( pType != ertAnimation ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleAnimation* )pRule );
}

deAnimatorRuleAnimationDifference &deAnimatorRuleVisitorIdentify::CastToAnimationDifference() const{
	if( pType != ertAnimationDifference ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleAnimationDifference* )pRule );
}

deAnimatorRuleAnimationSelect &deAnimatorRuleVisitorIdentify::CastToAnimationSelect() const{
	if( pType != ertAnimationSelect ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleAnimationSelect* )pRule );
}

deAnimatorRuleBoneTransformator &deAnimatorRuleVisitorIdentify::CastToBoneTransformator() const{
	if( pType != ertBoneTransformator ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleBoneTransformator* )pRule );
}

deAnimatorRuleInverseKinematic &deAnimatorRuleVisitorIdentify::CastToInverseKinematic() const{
	if( pType != ertInverseKinematic ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleInverseKinematic* )pRule );
}

deAnimatorRuleStateManipulator &deAnimatorRuleVisitorIdentify::CastToStateManipulator() const{
	if( pType != ertStateManipulator ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleStateManipulator* )pRule );
}

deAnimatorRuleStateSnapshot &deAnimatorRuleVisitorIdentify::CastToStateSnapshot() const{
	if( pType != ertStateSnapshot ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleStateSnapshot* )pRule );
}

deAnimatorRuleForeignState &deAnimatorRuleVisitorIdentify::CastToForeignState() const{
	if( pType != ertForeignState ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleForeignState* )pRule );
}

deAnimatorRuleGroup &deAnimatorRuleVisitorIdentify::CastToGroup() const{
	if( pType != ertGroup ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleGroup* )pRule );
}

deAnimatorRuleSubAnimator &deAnimatorRuleVisitorIdentify::CastToSubAnimator() const{
	if( pType != ertSubAnimator ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleSubAnimator* )pRule );
}

deAnimatorRuleTrackTo &deAnimatorRuleVisitorIdentify::CastToTrackTo() const{
	if( pType != ertTrackTo ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleTrackTo* )pRule );
}

deAnimatorRuleLimit &deAnimatorRuleVisitorIdentify::CastToLimit() const{
	if( pType != ertLimit ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleLimit* )pRule );
}

deAnimatorRuleMirror &deAnimatorRuleVisitorIdentify::CastToMirror() const{
	if( pType != ertMirror ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deAnimatorRuleMirror* )pRule );
}

void deAnimatorRuleVisitorIdentify::Reset(){
	pRule = NULL;
	pType = ertUnknown;
}



// Visiting
/////////////

void deAnimatorRuleVisitorIdentify::VisitRule( deAnimatorRule &rule ){
	pRule = &rule;
	pType = ertUnknown;
}

void deAnimatorRuleVisitorIdentify::VisitAnimation( deAnimatorRuleAnimation &rule ){
	pRule = &rule;
	pType = ertAnimation;
}

void deAnimatorRuleVisitorIdentify::VisitAnimationDifference( deAnimatorRuleAnimationDifference &rule ){
	pRule = &rule;
	pType = ertAnimationDifference;
}

void deAnimatorRuleVisitorIdentify::VisitAnimationSelect( deAnimatorRuleAnimationSelect &rule ){
	pRule = &rule;
	pType = ertAnimationSelect;
}

void deAnimatorRuleVisitorIdentify::VisitBoneTransformator( deAnimatorRuleBoneTransformator &rule ){
	pRule = &rule;
	pType = ertBoneTransformator;
}

void deAnimatorRuleVisitorIdentify::VisitInverseKinematic( deAnimatorRuleInverseKinematic &rule ){
	pRule = &rule;
	pType = ertInverseKinematic;
}

void deAnimatorRuleVisitorIdentify::VisitStateManipulator( deAnimatorRuleStateManipulator &rule ){
	pRule = &rule;
	pType = ertStateManipulator;
}

void deAnimatorRuleVisitorIdentify::VisitStateSnapshot( deAnimatorRuleStateSnapshot &rule ){
	pRule = &rule;
	pType = ertStateSnapshot;
}

void deAnimatorRuleVisitorIdentify::VisitForeignState( deAnimatorRuleForeignState &rule ){
	pRule = &rule;
	pType = ertForeignState;
}

void deAnimatorRuleVisitorIdentify::VisitGroup( deAnimatorRuleGroup &rule ){
	pRule = &rule;
	pType = ertGroup;
}

void deAnimatorRuleVisitorIdentify::VisitSubAnimator( deAnimatorRuleSubAnimator &rule ){
	pRule = &rule;
	pType = ertSubAnimator;
}

void deAnimatorRuleVisitorIdentify::VisitTrackTo( deAnimatorRuleTrackTo &rule ){
	pRule = &rule;
	pType = ertTrackTo;
}

void deAnimatorRuleVisitorIdentify::VisitLimit( deAnimatorRuleLimit &rule ){
	pRule = &rule;
	pType = ertLimit;
}

void deAnimatorRuleVisitorIdentify::VisitMirror( deAnimatorRuleMirror &rule ){
	pRule = &rule;
	pType = ertMirror;
}
