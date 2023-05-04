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

#include "aeRuleStateManipulator.h"
#include "../aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Class aeRuleStateManipulator
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleStateManipulator::aeRuleStateManipulator() :
aeRule( deAnimatorRuleVisitorIdentify::ertStateManipulator ),
pMinSize( 1.0f, 1.0f, 1.0f ),
pMaxSize( 1.0f, 1.0f, 1.0f ),
pMinVertexPositionSet( 0.0f ),
pMaxVertexPositionSet( 1.0f ),
pEnablePosition( false ),
pEnableRotation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "State Manipulator" );
}

aeRuleStateManipulator::aeRuleStateManipulator( const aeRuleStateManipulator &copy ) :
aeRule( copy ),
pMinPosition( copy.pMinPosition ),
pMaxPosition( copy.pMaxPosition ),
pMinRotation( copy.pMinRotation ),
pMaxRotation( copy.pMaxRotation ),
pMinSize( copy.pMinSize ),
pMaxSize( copy.pMaxSize ),
pMinVertexPositionSet( copy.pMinVertexPositionSet ),
pMaxVertexPositionSet( copy.pMaxVertexPositionSet ),
pEnablePosition( copy.pEnablePosition ),
pEnableRotation( copy.pEnableRotation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetPosition( copy.pTargetPosition ),
pTargetRotation( copy.pTargetRotation ),
pTargetSize( copy.pTargetSize ),
pTargetVertexPositionSet( copy.pTargetVertexPositionSet ){
}

aeRuleStateManipulator::~aeRuleStateManipulator(){
}



// Management
///////////////

void aeRuleStateManipulator::SetMinimumPosition( const decVector &position ){
	if( position.IsEqualTo( pMinPosition ) ){
		return;
	}
	
	pMinPosition = position;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMinimumPosition( position );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMaximumPosition( const decVector &position ){
	if( position.IsEqualTo( pMaxPosition ) ){
		return;
	}
	
	pMaxPosition = position;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMaximumPosition( position );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMinimumRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pMinRotation ) ){
		return;
	}
	
	pMinRotation = rotation;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMinimumRotation( rotation * DEG2RAD );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMaximumRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pMaxRotation ) ){
		return;
	}
	
	pMaxRotation = rotation;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMaximumRotation( rotation * DEG2RAD );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMinimumSize( const decVector &size ){
	if( size.IsEqualTo( pMinSize ) ){
		return;
	}
	
	pMinSize = size;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMinimumSize( size );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMaximumSize( const decVector &size ){
	if( size.IsEqualTo( pMaxSize ) ){
		return;
	}
	
	pMaxSize = size;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMaximumSize( size );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMinimumVertexPositionSet( float weight ){
	if( fabsf( weight - pMinVertexPositionSet ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinVertexPositionSet = weight;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMinimumVertexPositionSet( weight );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetMaximumVertexPositionSet( float weight ){
	if( fabsf( weight - pMaxVertexPositionSet ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxVertexPositionSet = weight;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetMaximumVertexPositionSet( weight );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetEnablePosition( bool enable ){
	if( enable == pEnablePosition ){
		return;
	}
	
	pEnablePosition = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetEnablePosition( enable );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetEnableRotation( bool enable ){
	if( enable == pEnableRotation ){
		return;
	}
	
	pEnableRotation = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetEnableRotation( enable );
	}
	
	NotifyRuleChanged();
}

void aeRuleStateManipulator::SetEnableSize( bool enable ){
	if( enable == pEnableSize ){
		return;
	}
	
	pEnableSize = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetEnableSize( enable );
		NotifyRuleChanged();
	}
}

void aeRuleStateManipulator::SetEnableVertexPositionSet( bool enable ){
	if( enable == pEnableVertexPositionSet ){
		return;
	}
	
	pEnableVertexPositionSet = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateManipulator* )GetEngineRule() )->SetEnableVertexPositionSet( enable );
		NotifyRuleChanged();
	}
}



void aeRuleStateManipulator::UpdateTargets(){
	deAnimatorRuleStateManipulator *rule = ( deAnimatorRuleStateManipulator* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetPosition.UpdateEngineTarget( GetAnimator(), rule->GetTargetPosition() );
		pTargetRotation.UpdateEngineTarget( GetAnimator(), rule->GetTargetRotation() );
		pTargetSize.UpdateEngineTarget( GetAnimator(), rule->GetTargetSize() );
		pTargetVertexPositionSet.UpdateEngineTarget( GetAnimator(), rule->GetTargetVertexPositionSet() );
	}
}

int aeRuleStateManipulator::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetPosition.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetRotation.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetSize.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetVertexPositionSet.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleStateManipulator::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetPosition.HasLink( link ) ){
		pTargetPosition.RemoveLink( link );
	}
	if( pTargetRotation.HasLink( link ) ){
		pTargetRotation.RemoveLink( link );
	}
	if( pTargetSize.HasLink( link ) ){
		pTargetSize.RemoveLink( link );
	}
	if( pTargetVertexPositionSet.HasLink( link ) ){
		pTargetVertexPositionSet.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleStateManipulator::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition.RemoveAllLinks();
	pTargetRotation.RemoveAllLinks();
	pTargetSize.RemoveAllLinks();
	pTargetVertexPositionSet.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleStateManipulator::CreateEngineRule(){
	deAnimatorRuleStateManipulator *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleStateManipulator;
		
		InitEngineRule( engRule );
		
		engRule->SetMinimumPosition( pMinPosition );
		engRule->SetMaximumPosition( pMaxPosition );
		engRule->SetMinimumRotation( pMinRotation * DEG2RAD );
		engRule->SetMaximumRotation( pMaxRotation * DEG2RAD );
		engRule->SetMinimumSize( pMinSize );
		engRule->SetMaximumSize( pMaxSize );
		engRule->SetMinimumVertexPositionSet( pMinVertexPositionSet );
		engRule->SetMaximumVertexPositionSet( pMaxVertexPositionSet );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableRotation( pEnableRotation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
		pTargetPosition.UpdateEngineTarget( GetAnimator(), engRule->GetTargetPosition() );
		pTargetRotation.UpdateEngineTarget( GetAnimator(), engRule->GetTargetRotation() );
		pTargetSize.UpdateEngineTarget( GetAnimator(), engRule->GetTargetSize() );
		pTargetVertexPositionSet.UpdateEngineTarget( GetAnimator(), engRule->GetTargetVertexPositionSet() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleStateManipulator::CreateCopy() const{
	return new aeRuleStateManipulator( *this );
}

void aeRuleStateManipulator::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetPosition.AddLinksToList( list );
	pTargetRotation.AddLinksToList( list );
	pTargetSize.AddLinksToList( list );
	pTargetVertexPositionSet.AddLinksToList( list );
}



// Operators
//////////////

aeRuleStateManipulator &aeRuleStateManipulator::operator=( const aeRuleStateManipulator &copy ){
	SetMinimumPosition( copy.pMinPosition );
	SetMaximumPosition( copy.pMaxPosition );
	SetMinimumRotation( copy.pMinRotation );
	SetMaximumRotation( copy.pMaxRotation );
	SetMinimumSize( copy.pMinSize );
	SetMaximumSize( copy.pMaxSize );
	SetMinimumVertexPositionSet( copy.pMinVertexPositionSet );
	SetMaximumVertexPositionSet( copy.pMaxVertexPositionSet );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableRotation( copy.pEnableRotation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetPosition = copy.pTargetPosition;
	pTargetRotation = copy.pTargetRotation;
	pTargetSize = copy.pTargetSize;
	pTargetVertexPositionSet = copy.pTargetVertexPositionSet;
	aeRule::operator=( copy );
	return *this;
}
