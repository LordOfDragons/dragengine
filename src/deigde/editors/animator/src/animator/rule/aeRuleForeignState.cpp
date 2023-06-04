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

#include "aeRuleForeignState.h"
#include "../aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Class aeRuleForeignState
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleForeignState::aeRuleForeignState() :
aeRule( deAnimatorRuleVisitorIdentify::ertForeignState ),
pForeignBone( "bone" ),
pSourceCoordinateFrame( deAnimatorRuleForeignState::ecfBoneLocal ),
pDestCoordinateFrame( deAnimatorRuleForeignState::ecfBoneLocal ),
pScalePosition( 1.0f ),
pScaleOrientation( 1.0f ),
pScaleSize( 1.0f ),
pScaleVertexPositionSet( 1.0f ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "Foreign State" );
}

aeRuleForeignState::aeRuleForeignState( const aeRuleForeignState &copy ) :
aeRule( copy ),
pForeignBone( copy.pForeignBone ),
pForeignVertexPositionSet( copy.pForeignVertexPositionSet ),
pSourceCoordinateFrame( copy.pSourceCoordinateFrame ),
pDestCoordinateFrame( copy.pDestCoordinateFrame ),
pScalePosition( copy.pScalePosition ),
pScaleOrientation( copy.pScaleOrientation ),
pScaleSize( copy.pScaleSize ),
pScaleVertexPositionSet( copy.pScaleVertexPositionSet ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetPosition( copy.pTargetPosition ),
pTargetOrientation( copy.pTargetOrientation ),
pTargetSize( copy.pTargetSize ),
pTargetVertexPositionSet( copy.pTargetVertexPositionSet ){
}

aeRuleForeignState::~aeRuleForeignState(){
}



// Management
///////////////

void aeRuleForeignState::SetForeignBone( const char *boneName ){
	pForeignBone = boneName;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetForeignBone( boneName );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetForeignVertexPositionSet( const char *vertexPositionSet ){
	pForeignVertexPositionSet = vertexPositionSet;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetForeignVertexPositionSet( vertexPositionSet );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetScalePosition( float scalePosition ){
	pScalePosition = scalePosition;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetScalePosition( scalePosition );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetScaleOrientation( float scaleOrientation ){
	pScaleOrientation = scaleOrientation;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetScaleOrientation( scaleOrientation );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetScaleSize( float scaleSize ){
	pScaleSize = scaleSize;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetScaleSize( scaleSize );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetScaleVertexPositionSet( float scale ){
	pScaleVertexPositionSet = scale;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetScaleVertexPositionSet( scale );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetEnablePosition( bool enable ){
	pEnablePosition = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetEnablePosition( enable );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetEnableOrientation( bool enable ){
	pEnableOrientation = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetEnableOrientation( enable );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetEnableSize( bool enable ){
	pEnableSize = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetEnableSize( enable );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetEnableVertexPositionSet( bool enable ){
	pEnableVertexPositionSet = enable;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetEnableVertexPositionSet( enable );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetSourceCoordinateFrame( deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame ){
	pSourceCoordinateFrame = coordinateFrame;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetSourceCoordinateFrame( coordinateFrame );
		NotifyRuleChanged();
	}
}

void aeRuleForeignState::SetDestCoordinateFrame( deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame ){
	pDestCoordinateFrame = coordinateFrame;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleForeignState* )GetEngineRule() )->SetDestCoordinateFrame( coordinateFrame );
		NotifyRuleChanged();
	}
}



void aeRuleForeignState::UpdateTargets(){
	deAnimatorRuleForeignState *rule = ( deAnimatorRuleForeignState* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetPosition.UpdateEngineTarget( GetAnimator(), rule->GetTargetPosition() );
		pTargetOrientation.UpdateEngineTarget( GetAnimator(), rule->GetTargetOrientation() );
		pTargetSize.UpdateEngineTarget( GetAnimator(), rule->GetTargetSize() );
		pTargetVertexPositionSet.UpdateEngineTarget( GetAnimator(), rule->GetTargetVertexPositionSet() );
	}
}

int aeRuleForeignState::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetPosition.HasLink( link ) ) usageCount++;
	if( pTargetOrientation.HasLink( link ) ) usageCount++;
	if( pTargetSize.HasLink( link ) ) usageCount++;
	if( pTargetVertexPositionSet.HasLink( link ) ) usageCount++;
	
	return usageCount;
}

void aeRuleForeignState::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetPosition.HasLink( link ) ){
		pTargetPosition.RemoveLink( link );
	}
	if( pTargetOrientation.HasLink( link ) ){
		pTargetOrientation.RemoveLink( link );
	}
	if( pTargetSize.HasLink( link ) ){
		pTargetSize.RemoveLink( link );
	}
	if( pTargetVertexPositionSet.HasLink( link ) ){
		pTargetVertexPositionSet.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleForeignState::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetPosition.RemoveAllLinks();
	pTargetOrientation.RemoveAllLinks();
	pTargetSize.RemoveAllLinks();
	pTargetVertexPositionSet.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleForeignState::CreateEngineRule(){
	deAnimatorRuleForeignState *engRule = NULL;
	
	try{
		// create rule
		engRule = new deAnimatorRuleForeignState;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		// init rule
		InitEngineRule( engRule );
		
		engRule->SetForeignBone( pForeignBone );
		engRule->SetForeignVertexPositionSet( pForeignBone );
		engRule->SetScalePosition( pScalePosition );
		engRule->SetScaleOrientation( pScaleOrientation );
		engRule->SetScaleSize( pScaleSize );
		engRule->SetScaleVertexPositionSet( pScaleVertexPositionSet );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		engRule->SetSourceCoordinateFrame( pSourceCoordinateFrame );
		engRule->SetDestCoordinateFrame( pDestCoordinateFrame );
		
		pTargetPosition.UpdateEngineTarget( GetAnimator(), engRule->GetTargetPosition() );
		pTargetOrientation.UpdateEngineTarget( GetAnimator(), engRule->GetTargetOrientation() );
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



aeRule *aeRuleForeignState::CreateCopy() const{
	return new aeRuleForeignState( *this );
}

void aeRuleForeignState::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetOrientation.AddLinksToList( list );
	pTargetPosition.AddLinksToList( list );
	pTargetSize.AddLinksToList( list );
	pTargetVertexPositionSet.AddLinksToList( list );
}



// Operators
//////////////

aeRuleForeignState &aeRuleForeignState::operator=( const aeRuleForeignState &copy ){
	SetForeignBone( copy.pForeignBone );
	SetForeignVertexPositionSet( copy.pForeignVertexPositionSet );
	SetScalePosition( copy.pScalePosition );
	SetScaleOrientation( copy.pScaleOrientation );
	SetScaleSize( copy.pScaleSize );
	SetScaleVertexPositionSet( copy.pScaleVertexPositionSet );
	SetSourceCoordinateFrame( copy.pSourceCoordinateFrame );
	SetDestCoordinateFrame( copy.pDestCoordinateFrame );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetPosition = copy.pTargetPosition;
	pTargetOrientation = copy.pTargetOrientation;
	pTargetSize = copy.pTargetSize;
	pTargetVertexPositionSet = copy.pTargetVertexPositionSet;
	aeRule::operator=( copy );
	return *this;
}
