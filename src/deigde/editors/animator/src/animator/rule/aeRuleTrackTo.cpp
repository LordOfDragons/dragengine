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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeRuleTrackTo.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleTrackTo
////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleTrackTo::aeRuleTrackTo() :
aeRule( deAnimatorRuleVisitorIdentify::ertTrackTo ),
pTrackAxis( deAnimatorRuleTrackTo::etaPosZ ),
pUpAxis( deAnimatorRuleTrackTo::etaPosY ),
pUpTarget( deAnimatorRuleTrackTo::eutComponentY ),
pLockedAxis( deAnimatorRuleTrackTo::elaNone )
{
	SetName( "Track To" );
}

aeRuleTrackTo::aeRuleTrackTo( const aeRuleTrackTo &copy ) :
aeRule( copy ),
pTrackBone( copy.pTrackBone ),
pTrackAxis( copy.pTrackAxis ),
pUpAxis( copy.pUpAxis ),
pUpTarget( copy.pUpTarget ),
pLockedAxis( copy.pLockedAxis ),
pTargetPosition( copy.pTargetPosition ),
pTargetUp( copy.pTargetUp ){
}

aeRuleTrackTo::~aeRuleTrackTo(){
}



// Management
///////////////

void aeRuleTrackTo::SetTrackBone( const char *boneName ){
	if( ! boneName ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTrackBone != boneName ){
		deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
		
		pTrackBone = boneName;
		
		if( engRule ){
			engRule->SetTrackBone( boneName );
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetTrackAxis( deAnimatorRuleTrackTo::eTrackAxis axis ){
	if( axis < deAnimatorRuleTrackTo::etaPosX || axis > deAnimatorRuleTrackTo::etaNegZ ){
		DETHROW( deeInvalidParam );
	}
	
	if( axis != pTrackAxis ){
		deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
		
		pTrackAxis = axis;
		
		if( engRule ){
			engRule->SetTrackAxis( axis );
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetUpAxis( deAnimatorRuleTrackTo::eTrackAxis axis ){
	if( axis < deAnimatorRuleTrackTo::etaPosX || axis > deAnimatorRuleTrackTo::etaNegZ ){
		DETHROW( deeInvalidParam );
	}
	
	if( axis != pUpAxis ){
		deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
		
		pUpAxis = axis;
		
		if( engRule ){
			engRule->SetUpAxis( axis );
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetUpTarget( deAnimatorRuleTrackTo::eUpTarget target ){
	if( target < deAnimatorRuleTrackTo::eutWorldX || target > deAnimatorRuleTrackTo::eutController ){
		DETHROW( deeInvalidParam );
	}
	
	if( target != pUpTarget ){
		deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
		
		pUpTarget = target;
		
		if( engRule ){
			engRule->SetUpTarget( target );
		}
		
		NotifyRuleChanged();
	}
}

void aeRuleTrackTo::SetLockedAxis( deAnimatorRuleTrackTo::eLockedAxis axis ){
	if( axis < deAnimatorRuleTrackTo::elaNone || axis > deAnimatorRuleTrackTo::elaZ ){
		DETHROW( deeInvalidParam );
	}
	
	if( axis != pLockedAxis ){
		deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
		
		pLockedAxis = axis;
		
		if( engRule ){
			engRule->SetLockedAxis( axis );
		}
		
		NotifyRuleChanged();
	}
}



void aeRuleTrackTo::UpdateTargets(){
	deAnimatorRuleTrackTo *engRule = ( deAnimatorRuleTrackTo* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( engRule ){
		aeAnimator *animator = GetAnimator();
		
		pTargetPosition.UpdateEngineTarget( animator, engRule->GetTargetPosition() );
		pTargetUp.UpdateEngineTarget( animator, engRule->GetTargetUp() );
	}
}

int aeRuleTrackTo::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetPosition.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetUp.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleTrackTo::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetPosition.HasLink( link ) ){
		pTargetPosition.RemoveLink( link );
	}
	
	if( pTargetUp.HasLink( link ) ){
		pTargetUp.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleTrackTo::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition.RemoveAllLinks();
	pTargetUp.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleTrackTo::CreateEngineRule(){
	deAnimatorRuleTrackTo *engRule = NULL;
	aeAnimator *animator = GetAnimator();
	
	try{
		// create rule
		engRule = new deAnimatorRuleTrackTo;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		// init rule
		InitEngineRule( engRule );
		
		engRule->SetTrackBone( pTrackBone );
		engRule->SetTrackAxis( pTrackAxis );
		engRule->SetUpAxis( pUpAxis );
		engRule->SetUpTarget( pUpTarget );
		engRule->SetLockedAxis( pLockedAxis );
		
		pTargetPosition.UpdateEngineTarget( animator, engRule->GetTargetPosition() );
		pTargetUp.UpdateEngineTarget( animator, engRule->GetTargetUp() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleTrackTo::CreateCopy() const{
	return new aeRuleTrackTo( *this );
}

void aeRuleTrackTo::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetPosition.AddLinksToList( list );
	pTargetUp.AddLinksToList( list );
}



// Operators
///////////////

aeRuleTrackTo &aeRuleTrackTo::operator=( const aeRuleTrackTo &copy ){
	SetTrackBone( copy.pTrackBone );
	SetTrackAxis( copy.pTrackAxis );
	SetUpAxis( copy.pUpAxis );
	SetUpTarget( copy.pUpTarget );
	SetLockedAxis( copy.pLockedAxis );
	pTargetPosition = copy.pTargetPosition;
	pTargetUp = copy.pTargetUp;
	aeRule::operator=( copy );
	return *this;
}
