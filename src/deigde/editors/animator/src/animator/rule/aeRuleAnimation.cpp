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

#include "aeRuleAnimation.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimation
//////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimation::aeRuleAnimation() :
aeRule( deAnimatorRuleVisitorIdentify::ertAnimation ),
pMoveName( "idle" ),
pMoveTime( 0.0f ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "Animation" );
}

aeRuleAnimation::aeRuleAnimation( const aeRuleAnimation &copy ) : 
aeRule( copy ),
pMoveName( copy.pMoveName ),
pMoveTime( copy.pMoveTime ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetMoveTime( copy.pTargetMoveTime ){
}

aeRuleAnimation::~aeRuleAnimation(){
}



// Management
///////////////

void aeRuleAnimation::SetMoveName( const char *moveName ){
	pMoveName = moveName;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetMoveName( moveName );
		NotifyRuleChanged();
	}
}

void aeRuleAnimation::SetMoveTime( float moveTime ){
	pMoveTime = moveTime;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetMoveTime( moveTime );
		NotifyRuleChanged();
	}
}

void aeRuleAnimation::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableVertexPositionSet( bool enabled ){
	if( enabled != pEnableVertexPositionSet ){
		pEnableVertexPositionSet = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableVertexPositionSet( enabled );
			NotifyRuleChanged();
		}
	}
}



void aeRuleAnimation::UpdateTargets(){
	deAnimatorRuleAnimation *rule = ( deAnimatorRuleAnimation* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), rule->GetTargetMoveTime() );
	}
}

int aeRuleAnimation::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetMoveTime.HasLink( link ) ) usageCount++;
	
	return usageCount;
}

void aeRuleAnimation::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetMoveTime.HasLink( link ) ){
		pTargetMoveTime.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleAnimation::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleAnimation::CreateEngineRule(){
	deAnimatorRuleAnimation *engRule = NULL;
	
	try{
		// create rule
		engRule = new deAnimatorRuleAnimation;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		// init rule
		InitEngineRule( engRule );
		
		engRule->SetMoveName( pMoveName.GetString() );
		engRule->SetMoveTime( pMoveTime );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), engRule->GetTargetMoveTime() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleAnimation::CreateCopy() const{
	return new aeRuleAnimation( *this );
}



void aeRuleAnimation::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetMoveTime.AddLinksToList( list );
}



// Operators
//////////////

aeRuleAnimation &aeRuleAnimation::operator=( const aeRuleAnimation &copy ){
	SetMoveName( copy.pMoveName );
	SetMoveTime( copy.pMoveTime );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetMoveTime = copy.pTargetMoveTime;
	aeRule::operator=( copy );
	return *this;
}
