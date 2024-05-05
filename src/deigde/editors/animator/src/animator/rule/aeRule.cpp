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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeRule.h"
#include "aeRuleAnimation.h"
#include "aeRuleAnimationDifference.h"
#include "aeRuleAnimationSelect.h"
#include "aeRuleBoneTransformator.h"
#include "aeRuleForeignState.h"
#include "aeRuleGroup.h"
#include "aeRuleInverseKinematic.h"
#include "aeRuleLimit.h"
#include "aeRuleStateManipulator.h"
#include "aeRuleStateSnapshot.h"
#include "aeRuleSubAnimator.h"
#include "aeRuleTrackTo.h"
#include "aeRuleMirror.h"
#include "../aeAnimator.h"
#include "../link/aeLinkList.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeRule
/////////////////

// Constructor, destructor
////////////////////////////

aeRule::aeRule( deAnimatorRuleVisitorIdentify::eRuleTypes type ) :
pAnimator( NULL ),
pParentGroup( NULL ),
pEngRule( NULL ),
pName( "Rule" ),
pType( type ),
pBlendMode( deAnimatorRule::ebmBlend ),
pBlendFactor( 1.0f ),
pInvertBlendFactor( false ),
pEnabled( true ){
}

aeRule::aeRule( const aeRule &copy ) :
pAnimator( NULL ),
pParentGroup( NULL ),
pEngRule( NULL ),
pName( copy.pName ),
pType( copy.pType ),
pListBones( copy.pListBones ),
pListVertexPositionSets( copy.pListVertexPositionSets ),
pBlendMode( copy.pBlendMode ),
pBlendFactor( copy.pBlendFactor ),
pInvertBlendFactor( copy.pInvertBlendFactor ),
pEnabled( copy.pEnabled ),
pTargetBlendFactor( copy.pTargetBlendFactor ){
}

aeRule::~aeRule(){
	SetAnimator( NULL );
}



// Management
///////////////

aeAnimator *aeRule::GetAnimator() const{
	if( pParentGroup ){
		return pParentGroup->GetAnimator();
		
	}else{
		return pAnimator;
	}
}

void aeRule::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	pEngRule = NULL;
	pAnimator = animator;
	
	OnParentAnimatorChanged();
}



void aeRule::SetEngineRule( deAnimatorRule *engRule ){
	pEngRule = engRule;
}

void aeRule::InitEngineRule( deAnimatorRule *engRule ) const{
	if( ! engRule ){
		DETHROW( deeInvalidParam );
	}
	
	aeAnimator * const animator = GetAnimator();
	
	engRule->SetEnabled( pEnabled );
	engRule->SetBlendMode( pBlendMode );
	engRule->SetBlendFactor( pBlendFactor );
	engRule->SetInvertBlendFactor( pInvertBlendFactor );
	engRule->GetListBones() = pListBones;
	engRule->GetListVertexPositionSets() = pListVertexPositionSets;
	
	pTargetBlendFactor.UpdateEngineTarget( animator, engRule->GetTargetBlendFactor() );
}



void aeRule::SetParentGroup( aeRuleGroup *group ){
	pParentGroup = group;
}



void aeRule::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = name;
	
	aeAnimator * const animator = GetAnimator();
	if( animator ){
		animator->NotifyRuleNameChanged( this );
	}
}

void aeRule::SetEnabled( bool enabled ){
	if( enabled != pEnabled ){
		pEnabled = enabled;
		
		if( pEngRule ){
			pEngRule->SetEnabled( enabled );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::SetBlendMode( deAnimatorRule::eBlendModes mode ){
	if( mode < deAnimatorRule::ebmBlend || mode > deAnimatorRule::deAnimatorRule::ebmOverlay ){
		DETHROW( deeInvalidParam );
	}
	
	if( mode != pBlendMode ){
		pBlendMode = mode;
		
		if( pEngRule ){
			pEngRule->SetBlendMode( mode );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::SetBlendFactor( float factor ){
	if( fabsf( factor - pBlendFactor ) > 1e-5f ){
		pBlendFactor = factor;
		
		if( pEngRule ){
			pEngRule->SetBlendFactor( factor );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::SetInvertBlendFactor( bool invert ){
	if( invert == pInvertBlendFactor ){
		return;
	}
	
	pInvertBlendFactor = invert;
	
	if( pEngRule ){
		pEngRule->SetInvertBlendFactor( invert );
	}
	
	NotifyRuleChanged();
}



void aeRule::UpdateCompAnim(){
}

void aeRule::UpdateTargets(){
	aeAnimator * const animator = GetAnimator();
	if( pEngRule && animator ){
		pTargetBlendFactor.UpdateEngineTarget( animator, pEngRule->GetTargetBlendFactor() );
	}
}

int aeRule::CountLinkUsage( aeLink *link ) const{
	int usageCount = 0;
	
	if( pTargetBlendFactor.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void aeRule::RemoveLinkFromTargets( aeLink *link ){
	if( pTargetBlendFactor.HasLink( link ) ){
		pTargetBlendFactor.RemoveLink( link );
	}
}

void aeRule::RemoveLinksFromAllTargets(){
	pTargetBlendFactor.RemoveAllLinks();
}



void aeRule::ListLinks( aeLinkList &list ){
	pTargetBlendFactor.AddLinksToList( list );
}



void aeRule::NotifyRuleChanged(){
	aeAnimator * const animator = GetAnimator();
	if( animator ){
		UpdateTargets();
		
		if( pEngRule ){
			animator->GetEngineAnimator()->NotifyRulesChanged();
		}
		
		animator->NotifyRuleChanged( this );
	}
}



void aeRule::OnParentAnimatorChanged(){
}



// Bone Management
////////////////////

void aeRule::SetListBones( const decStringSet &bones ){
	if( pListBones == bones ){
		return;
	}
	
	pListBones = bones;
	
	if( pEngRule ){
		pEngRule->GetListBones() = bones;
	}
	
	NotifyRuleChanged();
}

void aeRule::AddBone( const char *bone ){
	if( ! bone ) DETHROW( deeInvalidParam );
	
	if( ! pListBones.Has( bone ) ){
		pListBones.Add( bone );
		
		if( pEngRule ){
			pEngRule->GetListBones().Add( bone );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::RemoveBone( const char *bone ){
	if( pListBones.Has( bone ) ){
		pListBones.Remove( bone );
		
		if( pEngRule ){
			pEngRule->GetListBones().Remove( bone );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::RemoveAllBones(){
	if( pListBones.GetCount() > 0 ){
		pListBones.RemoveAll();
		
		if( pEngRule ){
			pEngRule->GetListBones().RemoveAll();
		}
		
		NotifyRuleChanged();
	}
}



// Vertex position set management
///////////////////////////////////

void aeRule::SetListVertexPositionSets( const decStringSet &sets ){
	if( pListVertexPositionSets == sets ){
		return;
	}
	
	pListVertexPositionSets = sets;
	
	if( pEngRule ){
		pEngRule->GetListVertexPositionSets() = sets;
	}
	
	NotifyRuleChanged();
}

void aeRule::AddVertexPositionSet( const char *vertexPositionSet ){
	DEASSERT_NOTNULL( vertexPositionSet )
	
	if( ! pListVertexPositionSets.Has( vertexPositionSet ) ){
		pListVertexPositionSets.Add( vertexPositionSet );
		
		if( pEngRule ){
			pEngRule->GetListVertexPositionSets().Add( vertexPositionSet );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::RemoveVertexPositionSet( const char *vertexPositionSet ){
	if( pListVertexPositionSets.Has( vertexPositionSet ) ){
		pListVertexPositionSets.Remove( vertexPositionSet );
		
		if( pEngRule ){
			pEngRule->GetListVertexPositionSets().Remove( vertexPositionSet );
		}
		
		NotifyRuleChanged();
	}
}

void aeRule::RemoveAllVertexPositionSets(){
	if( pListVertexPositionSets.GetCount() > 0 ){
		pListVertexPositionSets.RemoveAll();
		
		if( pEngRule ){
			pEngRule->GetListVertexPositionSets().RemoveAll();
		}
		
		NotifyRuleChanged();
	}
}



// Operators
//////////////

aeRule &aeRule::operator=( const aeRule &copy ){
	SetName( copy.pName );
	pListBones = copy.pListBones;
	pListVertexPositionSets = copy.pListVertexPositionSets;
	SetBlendMode( copy.pBlendMode );
	SetBlendFactor( copy.pBlendFactor );
	SetInvertBlendFactor( copy.pInvertBlendFactor );
	SetEnabled( copy.pEnabled );
	pTargetBlendFactor = copy.pTargetBlendFactor;
	NotifyRuleChanged();
	return *this;
}



// Helper
///////////

aeRule *aeRule::CreateRuleFromType( deAnimatorRuleVisitorIdentify::eRuleTypes type ){
	switch( type ){
	case deAnimatorRuleVisitorIdentify::ertAnimation:
		return new aeRuleAnimation;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationDifference:
		return new aeRuleAnimationDifference;
		
	case deAnimatorRuleVisitorIdentify::ertAnimationSelect:
		return new aeRuleAnimationSelect;
		
	case deAnimatorRuleVisitorIdentify::ertBoneTransformator:
		return new aeRuleBoneTransformator;
		
	case deAnimatorRuleVisitorIdentify::ertForeignState:
		return new aeRuleForeignState;
		
	case deAnimatorRuleVisitorIdentify::ertGroup:
		return new aeRuleGroup;
		
	case deAnimatorRuleVisitorIdentify::ertInverseKinematic:
		return new aeRuleInverseKinematic;
		
	case deAnimatorRuleVisitorIdentify::ertLimit:
		return new aeRuleLimit;
		
	case deAnimatorRuleVisitorIdentify::ertStateManipulator:
		return new aeRuleStateManipulator;
		
	case deAnimatorRuleVisitorIdentify::ertStateSnapshot:
		return new aeRuleStateSnapshot;
		
	case deAnimatorRuleVisitorIdentify::ertSubAnimator:
		return new aeRuleSubAnimator;
		
	case deAnimatorRuleVisitorIdentify::ertTrackTo:
		return new aeRuleTrackTo;
		
	case deAnimatorRuleVisitorIdentify::ertMirror:
		return aeRuleMirror::CreateDefault();
		
	default:
		DETHROW( deeInvalidParam );
	}
}
