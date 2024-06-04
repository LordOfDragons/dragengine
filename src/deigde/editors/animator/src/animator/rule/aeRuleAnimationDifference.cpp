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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleAnimationDifference.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationDifference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimationDifference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimationDifference::aeRuleAnimationDifference() :
aeRule( deAnimatorRuleVisitorIdentify::ertAnimationDifference ),
pMove1Name( "idle" ),
pMove1Time( 0.0f ),
pMove2Name( "idle" ),
pMove2Time( 0.0f ),
pUseSameMove( false ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "Animation Difference" );
}

aeRuleAnimationDifference::aeRuleAnimationDifference( const aeRuleAnimationDifference &copy ) :
aeRule( copy ),
pMove1Name( copy.pMove1Name ),
pMove1Time( copy.pMove1Time ),
pMove2Name( copy.pMove2Name ),
pMove2Time( copy.pMove2Time ),
pUseSameMove( copy.pUseSameMove ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetLeadMoveTime( copy.pTargetLeadMoveTime ),
pTargetRefMoveTime( copy.pTargetRefMoveTime ){
}

aeRuleAnimationDifference::~aeRuleAnimationDifference(){
}



// Management
///////////////

void aeRuleAnimationDifference::SetLeadingMoveName( const char *moveName ){
	pMove1Name = moveName;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetLeadingMoveName( moveName );
		NotifyRuleChanged();
	}
}

void aeRuleAnimationDifference::SetLeadingMoveTime( float moveTime ){
	pMove1Time = moveTime;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetLeadingMoveTime( moveTime );
		NotifyRuleChanged();
	}
}

void aeRuleAnimationDifference::SetReferenceMoveName( const char *moveName ){
	pMove2Name = moveName;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetReferenceMoveName( moveName );
		NotifyRuleChanged();
	}
}

void aeRuleAnimationDifference::SetReferenceMoveTime( float moveTime ){
	pMove2Time = moveTime;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetReferenceMoveTime( moveTime );
		NotifyRuleChanged();
	}
}

void aeRuleAnimationDifference::SetUseSameMove( bool useSameMove ){
	pUseSameMove = useSameMove;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetUseSameMove( useSameMove );
		NotifyRuleChanged();
	}
}

void aeRuleAnimationDifference::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationDifference::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationDifference::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationDifference::SetEnableVertexPositionSet( bool enabled ){
	if( enabled != pEnableVertexPositionSet ){
		pEnableVertexPositionSet = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationDifference* )GetEngineRule() )->SetEnableVertexPositionSet( enabled );
			NotifyRuleChanged();
		}
	}
}



void aeRuleAnimationDifference::UpdateTargets(){
	deAnimatorRuleAnimationDifference *rule = ( deAnimatorRuleAnimationDifference* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetLeadMoveTime.UpdateEngineTarget( GetAnimator(), rule->GetTargetLeadingMoveTime() );
		pTargetRefMoveTime.UpdateEngineTarget( GetAnimator(), rule->GetTargetReferenceMoveTime() );
	}
}

int aeRuleAnimationDifference::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetLeadMoveTime.HasLink( link ) ) usageCount++;
	if( pTargetRefMoveTime.HasLink( link ) ) usageCount++;
	
	return usageCount;
}

void aeRuleAnimationDifference::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetLeadMoveTime.HasLink( link ) ){
		pTargetLeadMoveTime.RemoveLink( link );
	}
	
	if( pTargetRefMoveTime.HasLink( link ) ){
		pTargetRefMoveTime.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleAnimationDifference::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetLeadMoveTime.RemoveAllLinks();
	pTargetRefMoveTime.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleAnimationDifference::CreateEngineRule(){
	deAnimatorRuleAnimationDifference *engRule = NULL;
	
	try{
		// create rule
		engRule = new deAnimatorRuleAnimationDifference;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		// init rule
		InitEngineRule( engRule );
		
		engRule->SetLeadingMoveName( pMove1Name.GetString() );
		engRule->SetLeadingMoveTime( pMove1Time );
		engRule->SetReferenceMoveName( pMove2Name.GetString() );
		engRule->SetReferenceMoveTime( pMove2Time );
		engRule->SetUseSameMove( pUseSameMove );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
		pTargetLeadMoveTime.UpdateEngineTarget( GetAnimator(), engRule->GetTargetLeadingMoveTime() );
		pTargetRefMoveTime.UpdateEngineTarget( GetAnimator(), engRule->GetTargetReferenceMoveTime() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleAnimationDifference::CreateCopy() const{
	return new aeRuleAnimationDifference( *this );
}

void aeRuleAnimationDifference::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetLeadMoveTime.AddLinksToList( list );
	pTargetRefMoveTime.AddLinksToList( list );
}



// Operators
//////////////

aeRuleAnimationDifference &aeRuleAnimationDifference::operator=( const aeRuleAnimationDifference &copy ){
	SetLeadingMoveName( copy.pMove1Name );
	SetLeadingMoveTime( copy.pMove1Time );
	SetReferenceMoveName( copy.pMove2Name );
	SetReferenceMoveTime( copy.pMove2Time );
	SetUseSameMove( copy.pUseSameMove );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetLeadMoveTime = copy.pTargetLeadMoveTime;
	pTargetRefMoveTime = copy.pTargetRefMoveTime;
	aeRule::operator=( copy );
	return *this;
}
