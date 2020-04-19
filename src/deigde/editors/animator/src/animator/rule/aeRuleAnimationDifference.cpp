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
pEnableSize( false )
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
	pTargetLeadMoveTime = copy.pTargetLeadMoveTime;
	pTargetRefMoveTime = copy.pTargetRefMoveTime;
	aeRule::operator=( copy );
	return *this;
}
