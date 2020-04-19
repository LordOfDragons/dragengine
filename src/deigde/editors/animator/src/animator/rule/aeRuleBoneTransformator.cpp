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

#include "aeRuleBoneTransformator.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleBoneTransformator
////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleBoneTransformator::aeRuleBoneTransformator() :
aeRule( deAnimatorRuleVisitorIdentify::ertBoneTransformator ),
pMinScaling( 1.0f, 1.0f, 1.0f ),
pMaxScaling( 1.0f, 1.0f, 1.0f ),
pCoordinateFrame( deAnimatorRuleBoneTransformator::ecfComponent ),
pEnablePosition( false ),
pEnableOrientation( true ),
pEnableSize( false )
{
	SetName( "Bone Transformator" );
}

aeRuleBoneTransformator::aeRuleBoneTransformator( const aeRuleBoneTransformator &copy ) :
aeRule( copy ),
pMinTranslation( copy.pMinTranslation ),
pMaxTranslation( copy.pMaxTranslation ),
pMinRotation( copy.pMinRotation ),
pMaxRotation( copy.pMaxRotation ),
pMinScaling( copy.pMinScaling ),
pMaxScaling( copy.pMaxScaling ),
pCoordinateFrame( copy.pCoordinateFrame ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pTargetBone( copy.pTargetBone ),
pTargetTranslation( copy.pTargetTranslation ),
pTargetRotation( copy.pTargetRotation ),
pTargetScaling( copy.pTargetScaling ){
}

aeRuleBoneTransformator::~aeRuleBoneTransformator(){
}



// Management
///////////////

void aeRuleBoneTransformator::SetMinimumTranslation( const decVector &translation ){
	if( translation.IsEqualTo( pMinTranslation ) ){
		return;
	}
	
	pMinTranslation = translation;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMinimumTranslation( translation );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumTranslation( const decVector &translation ){
	if( translation.IsEqualTo( pMaxTranslation ) ){
		return;
	}
	
	pMaxTranslation = translation;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMaximumTranslation( translation );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMinimumRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pMinRotation ) ){
		return;
	}
	
	pMinRotation = rotation;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMinimumRotation( rotation * DEG2RAD );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pMaxRotation ) ){
		return;
	}
	
	pMaxRotation = rotation;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMaximumRotation( rotation * DEG2RAD );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMinimumScaling( const decVector &scaling ){
	if( scaling.IsEqualTo( pMinScaling ) ){
		return;
	}
	
	pMinScaling = scaling;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMinimumScaling( scaling );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetMaximumScaling( const decVector &scaling ){
	if( scaling.IsEqualTo( pMaxScaling ) ){
		return;
	}
	
	pMaxScaling = scaling;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetMaximumScaling( scaling );
		NotifyRuleChanged();
	}
}



void aeRuleBoneTransformator::SetCoordinateFrame( deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame ){
	if( coordinateFrame < deAnimatorRuleBoneTransformator::ecfBoneLocal
	|| coordinateFrame > deAnimatorRuleBoneTransformator::ecfTargetBone ){
		DETHROW( deeInvalidParam );
	}
	
	if( coordinateFrame == pCoordinateFrame ){
		return;
	}
	
	pCoordinateFrame = coordinateFrame;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetCoordinateFrame( coordinateFrame );
		NotifyRuleChanged();
	}
}



void aeRuleBoneTransformator::SetEnablePosition( bool enable ){
	if( enable == pEnablePosition ){
		return;
	}
	
	pEnablePosition = enable;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetEnablePosition( enable );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetEnableOrientation( bool enable ){
	if( enable == pEnableOrientation ){
		return;
	}
	
	pEnableOrientation = enable;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetEnableOrientation( enable );
		NotifyRuleChanged();
	}
}

void aeRuleBoneTransformator::SetEnableSize( bool enable ){
	if( enable == pEnableSize ){
		return;
	}
	
	pEnableSize = enable;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetEnableSize( enable );
		NotifyRuleChanged();
	}
}



void aeRuleBoneTransformator::SetTargetBone( const char *boneName ){
	if( pTargetBone == boneName ){
		return;
	}
	
	pTargetBone = boneName;
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( rule ){
		rule->SetTargetBone( boneName );
		NotifyRuleChanged();
	}
}



void aeRuleBoneTransformator::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleBoneTransformator * const rule = ( deAnimatorRuleBoneTransformator* )GetEngineRule();
	if( ! rule ){
		return;
	}
	
	pTargetTranslation.UpdateEngineTarget( GetAnimator(), rule->GetTargetTranslation() );
	pTargetRotation.UpdateEngineTarget( GetAnimator(), rule->GetTargetRotation() );
	pTargetScaling.UpdateEngineTarget( GetAnimator(), rule->GetTargetScaling() );
}

int aeRuleBoneTransformator::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetTranslation.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetRotation.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetScaling.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void aeRuleBoneTransformator::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetTranslation.HasLink( link ) ){
		pTargetTranslation.RemoveLink( link );
	}
	if( pTargetRotation.HasLink( link ) ){
		pTargetRotation.RemoveLink( link );
	}
	if( pTargetScaling.HasLink( link ) ){
		pTargetScaling.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleBoneTransformator::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetTranslation.RemoveAllLinks();
	pTargetRotation.RemoveAllLinks();
	pTargetScaling.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleBoneTransformator::CreateEngineRule(){
	deAnimatorRuleBoneTransformator *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleBoneTransformator;
		
		InitEngineRule( engRule );
		
		engRule->SetMinimumTranslation( pMinTranslation );
		engRule->SetMaximumTranslation( pMaxTranslation );
		engRule->SetMinimumRotation( pMinRotation * DEG2RAD );
		engRule->SetMaximumRotation( pMaxRotation * DEG2RAD );
		engRule->SetMinimumScaling( pMinScaling );
		engRule->SetMaximumScaling( pMaxScaling );
		engRule->SetCoordinateFrame( pCoordinateFrame );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetTargetBone( pTargetBone.GetString() );
		
		pTargetTranslation.UpdateEngineTarget( GetAnimator(), engRule->GetTargetTranslation() );
		pTargetRotation.UpdateEngineTarget( GetAnimator(), engRule->GetTargetRotation() );
		pTargetScaling.UpdateEngineTarget( GetAnimator(), engRule->GetTargetScaling() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleBoneTransformator::CreateCopy() const{
	return new aeRuleBoneTransformator( *this );
}

void aeRuleBoneTransformator::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetRotation.AddLinksToList( list );
	pTargetScaling.AddLinksToList( list );
	pTargetTranslation.AddLinksToList( list );
}



// Operators
//////////////

aeRuleBoneTransformator &aeRuleBoneTransformator::operator=( const aeRuleBoneTransformator &copy ){
	SetMinimumTranslation( copy.pMinTranslation );
	SetMaximumTranslation( copy.pMaxTranslation );
	SetMinimumRotation( copy.pMinRotation );
	SetMaximumRotation( copy.pMaxRotation );
	SetMinimumScaling( copy.pMinScaling );
	SetMaximumScaling( copy.pMaxScaling );
	SetCoordinateFrame( copy.pCoordinateFrame );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetTargetBone( copy.pTargetBone );
	pTargetTranslation = copy.pTargetTranslation;
	pTargetRotation = copy.pTargetRotation;
	pTargetScaling = copy.pTargetScaling;
	aeRule::operator=( copy );
	return *this;
}
