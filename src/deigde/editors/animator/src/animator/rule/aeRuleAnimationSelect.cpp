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

#include "aeRuleAnimationSelect.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimationSelect
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimationSelect::aeRuleAnimationSelect() :
aeRule( deAnimatorRuleVisitorIdentify::ertAnimationSelect ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "Animation Select" );
}

aeRuleAnimationSelect::aeRuleAnimationSelect( const aeRuleAnimationSelect &copy ) :
aeRule( copy ),
pMoves( copy.pMoves ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetMoveTime( copy.pTargetMoveTime ),
pTargetSelect( copy.pTargetSelect ){
}

aeRuleAnimationSelect::~aeRuleAnimationSelect(){
}



// Management
///////////////

void aeRuleAnimationSelect::SetMoves( const decStringList &moves ){
	if( pMoves == moves ){
		return;
	}
	
	pMoves = moves;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimationSelect* )GetEngineRule() )->GetMoves() = moves;
		NotifyRuleChanged();
	}
}

void aeRuleAnimationSelect::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationSelect* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationSelect* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationSelect* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimationSelect::SetEnableVertexPositionSet( bool enabled ){
	if( enabled != pEnableVertexPositionSet ){
		pEnableVertexPositionSet = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimationSelect* )GetEngineRule() )->SetEnableVertexPositionSet( enabled );
			NotifyRuleChanged();
		}
	}
}



void aeRuleAnimationSelect::UpdateTargets(){
	deAnimatorRuleAnimationSelect *rule = ( deAnimatorRuleAnimationSelect* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), rule->GetTargetMoveTime() );
		pTargetSelect.UpdateEngineTarget( GetAnimator(), rule->GetTargetSelect() );
	}
}

int aeRuleAnimationSelect::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetMoveTime.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetSelect.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleAnimationSelect::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetMoveTime.HasLink( link ) ){
		pTargetMoveTime.RemoveLink( link );
	}
	
	if( pTargetSelect.HasLink( link ) ){
		pTargetSelect.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleAnimationSelect::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime.RemoveAllLinks();
	pTargetSelect.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleAnimationSelect::CreateEngineRule(){
	deAnimatorRuleAnimationSelect *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleAnimationSelect;
		
		InitEngineRule( engRule );
		
		engRule->GetMoves() = pMoves;
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), engRule->GetTargetMoveTime() );
		pTargetSelect.UpdateEngineTarget( GetAnimator(), engRule->GetTargetSelect() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	return engRule;
}



aeRule *aeRuleAnimationSelect::CreateCopy() const{
	return new aeRuleAnimationSelect( *this );
}

void aeRuleAnimationSelect::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetMoveTime.AddLinksToList( list );
	pTargetSelect.AddLinksToList( list );
}



// Operators
//////////////

aeRuleAnimationSelect &aeRuleAnimationSelect::operator=( const aeRuleAnimationSelect &copy ){
	SetMoves( copy.pMoves );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetMoveTime = copy.pTargetMoveTime;
	pTargetSelect = copy.pTargetSelect;
	aeRule::operator=( copy );
	return *this;
}
