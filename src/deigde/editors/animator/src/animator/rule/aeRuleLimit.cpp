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

#include "aeRuleLimit.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleLimit
//////////////////////

// Constructor, destructor
////////////////////////////

aeRuleLimit::aeRuleLimit() :
aeRule( deAnimatorRuleVisitorIdentify::ertLimit ),
pMinScaling( 1.0f, 1.0f, 1.0f ),
pMaxScaling( 1.0f, 1.0f, 1.0f ),
pCoordinateFrame( deAnimatorRuleLimit::ecfComponent ),
pEnablePositionXMin( false ),
pEnablePositionXMax( false ),
pEnablePositionYMin( false ),
pEnablePositionYMax( false ),
pEnablePositionZMin( false ),
pEnablePositionZMax( false ),
pEnableRotationXMin( false ),
pEnableRotationXMax( false ),
pEnableRotationYMin( false ),
pEnableRotationYMax( false ),
pEnableRotationZMin( false ),
pEnableRotationZMax( false ),
pEnableScalingXMin( false ),
pEnableScalingXMax( false ),
pEnableScalingYMin( false ),
pEnableScalingYMax( false ),
pEnableScalingZMin( false ),
pEnableScalingZMax( false )
{
	SetName( "Limit" );
}

aeRuleLimit::aeRuleLimit( const aeRuleLimit &copy ) :
aeRule( copy ),
pMinPosition( copy.pMinPosition ),
pMaxPosition( copy.pMaxPosition ),
pMinRotation( copy.pMinRotation ),
pMaxRotation( copy.pMaxRotation ),
pMinScaling( copy.pMinScaling ),
pMaxScaling( copy.pMaxScaling ),
pCoordinateFrame( copy.pCoordinateFrame ),
pEnablePositionXMin( copy.pEnablePositionXMin ),
pEnablePositionXMax( copy.pEnablePositionXMax ),
pEnablePositionYMin( copy.pEnablePositionYMin ),
pEnablePositionYMax( copy.pEnablePositionYMax ),
pEnablePositionZMin( copy.pEnablePositionZMin ),
pEnablePositionZMax( copy.pEnablePositionZMax ),
pEnableRotationXMin( copy.pEnableRotationXMin ),
pEnableRotationXMax( copy.pEnableRotationXMax ),
pEnableRotationYMin( copy.pEnableRotationYMin ),
pEnableRotationYMax( copy.pEnableRotationYMax ),
pEnableRotationZMin( copy.pEnableRotationZMin ),
pEnableRotationZMax( copy.pEnableRotationZMax ),
pEnableScalingXMin( copy.pEnableScalingXMin ),
pEnableScalingXMax( copy.pEnableScalingXMax ),
pEnableScalingYMin( copy.pEnableScalingYMin ),
pEnableScalingYMax( copy.pEnableScalingYMax ),
pEnableScalingZMin( copy.pEnableScalingZMin ),
pEnableScalingZMax( copy.pEnableScalingZMax ),
pTargetBone( copy.pTargetBone ){
}

aeRuleLimit::~aeRuleLimit(){
}



// Management
///////////////

void aeRuleLimit::SetMinimumPosition( const decVector &position ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMinPosition = position;
	
	if( rule ){
		rule->SetMinimumPosition( position );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumPosition( const decVector &position ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMaxPosition = position;
	
	if( rule ){
		rule->SetMaximumPosition( position );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMinimumRotation( const decVector &rotation ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMinRotation = rotation;
	
	if( rule ){
		rule->SetMinimumRotation( rotation * DEG2RAD );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumRotation( const decVector &rotation ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMaxRotation = rotation;
	
	if( rule ){
		rule->SetMaximumRotation( rotation * DEG2RAD );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMinimumScaling( const decVector &scaling ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMinScaling = scaling;
	
	if( rule ){
		rule->SetMinimumScaling( scaling );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetMaximumScaling( const decVector &scaling ){
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pMaxScaling = scaling;
	
	if( rule ){
		rule->SetMaximumScaling( scaling );
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetCoordinateFrame( deAnimatorRuleLimit::eCoordinateFrames coordinateFrame ){
	if( coordinateFrame < deAnimatorRuleLimit::ecfBoneLocal || coordinateFrame > deAnimatorRuleLimit::ecfTargetBone ){
		DETHROW( deeInvalidParam );
	}
	
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pCoordinateFrame = coordinateFrame;
	
	if( rule ){
		rule->SetCoordinateFrame( coordinateFrame );
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnablePositionXMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionXMin = enabled;
	
	if( rule ){
		rule->SetEnablePositionXMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionXMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionXMax = enabled;
	
	if( rule ){
		rule->SetEnablePositionXMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionYMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionYMin = enabled;
	
	if( rule ){
		rule->SetEnablePositionYMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionYMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionYMax = enabled;
	
	if( rule ){
		rule->SetEnablePositionYMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionZMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionZMin = enabled;
	
	if( rule ){
		rule->SetEnablePositionZMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnablePositionZMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnablePositionZMax = enabled;
	
	if( rule ){
		rule->SetEnablePositionZMax( enabled );
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnableRotationXMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationXMin = enabled;
	
	if( rule ){
		rule->SetEnableRotationXMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationXMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationXMax = enabled;
	
	if( rule ){
		rule->SetEnableRotationXMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationYMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationYMin = enabled;
	
	if( rule ){
		rule->SetEnableRotationYMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationYMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationYMax = enabled;
	
	if( rule ){
		rule->SetEnableRotationYMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationZMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationZMin = enabled;
	
	if( rule ){
		rule->SetEnableRotationZMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableRotationZMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableRotationZMax = enabled;
	
	if( rule ){
		rule->SetEnableRotationZMax( enabled );
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetEnableScalingXMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingXMin = enabled;
	
	if( rule ){
		rule->SetEnableScalingXMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingXMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingXMax = enabled;
	
	if( rule ){
		rule->SetEnableScalingXMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingYMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingYMin = enabled;
	
	if( rule ){
		rule->SetEnableScalingYMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingYMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingYMax = enabled;
	
	if( rule ){
		rule->SetEnableScalingYMax( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingZMin( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingZMin = enabled;
	
	if( rule ){
		rule->SetEnableScalingZMin( enabled );
		NotifyRuleChanged();
	}
}

void aeRuleLimit::SetEnableScalingZMax( bool enabled ){
	deAnimatorRuleLimit * const rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pEnableScalingZMax = enabled;
	
	if( rule ){
		rule->SetEnableScalingZMax( enabled );
		NotifyRuleChanged();
	}
}



void aeRuleLimit::SetTargetBone( const char *boneName ){
	if( ! boneName ){
		DETHROW( deeInvalidParam );
	}
	
	deAnimatorRuleLimit *rule = ( deAnimatorRuleLimit* )GetEngineRule();
	
	pTargetBone = boneName;
	
	if( rule ){
		rule->SetTargetBone( boneName );
		NotifyRuleChanged();
	}
}



deAnimatorRule *aeRuleLimit::CreateEngineRule(){
	deAnimatorRuleLimit *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleLimit;
		
		InitEngineRule( engRule );
		
		engRule->SetMinimumPosition( pMinPosition );
		engRule->SetMaximumPosition( pMaxPosition );
		engRule->SetMinimumRotation( pMinRotation * DEG2RAD );
		engRule->SetMaximumRotation( pMaxRotation * DEG2RAD );
		engRule->SetMinimumScaling( pMinScaling );
		engRule->SetMaximumScaling( pMaxScaling );
		engRule->SetCoordinateFrame( pCoordinateFrame );
		engRule->SetTargetBone( pTargetBone.GetString() );
		
		engRule->SetEnablePositionXMin( pEnablePositionXMin );
		engRule->SetEnablePositionXMax( pEnablePositionXMax );
		engRule->SetEnablePositionYMin( pEnablePositionYMin );
		engRule->SetEnablePositionYMax( pEnablePositionYMax );
		engRule->SetEnablePositionZMin( pEnablePositionZMin );
		engRule->SetEnablePositionZMax( pEnablePositionZMax );
		
		engRule->SetEnableRotationXMin( pEnableRotationXMin );
		engRule->SetEnableRotationXMax( pEnableRotationXMax );
		engRule->SetEnableRotationYMin( pEnableRotationYMin );
		engRule->SetEnableRotationYMax( pEnableRotationYMax );
		engRule->SetEnableRotationZMin( pEnableRotationZMin );
		engRule->SetEnableRotationZMax( pEnableRotationZMax );
		
		engRule->SetEnableScalingXMin( pEnableScalingXMin );
		engRule->SetEnableScalingXMax( pEnableScalingXMax );
		engRule->SetEnableScalingYMin( pEnableScalingYMin );
		engRule->SetEnableScalingYMax( pEnableScalingYMax );
		engRule->SetEnableScalingZMin( pEnableScalingZMin );
		engRule->SetEnableScalingZMax( pEnableScalingZMax );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleLimit::CreateCopy() const{
	return new aeRuleLimit( *this );
}

void aeRuleLimit::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
}



// Operators
//////////////

aeRuleLimit &aeRuleLimit::operator=( const aeRuleLimit &copy ){
	SetMinimumPosition( copy.pMinPosition );
	SetMaximumPosition( copy.pMaxPosition );
	SetMinimumRotation( copy.pMinRotation );
	SetMaximumRotation( copy.pMaxRotation );
	SetMinimumScaling( copy.pMinScaling );
	SetMaximumScaling( copy.pMaxScaling );
	SetCoordinateFrame( copy.pCoordinateFrame );
	SetEnablePositionXMin( copy.pEnablePositionXMin );
	SetEnablePositionXMax( copy.pEnablePositionXMax );
	SetEnablePositionYMin( copy.pEnablePositionYMin );
	SetEnablePositionYMax( copy.pEnablePositionYMax );
	SetEnablePositionZMin( copy.pEnablePositionZMin );
	SetEnablePositionZMax( copy.pEnablePositionZMax );
	SetEnableRotationXMin( copy.pEnableRotationXMin );
	SetEnableRotationXMax( copy.pEnableRotationXMax );
	SetEnableRotationYMin( copy.pEnableRotationYMin );
	SetEnableRotationYMax( copy.pEnableRotationYMax );
	SetEnableRotationZMin( copy.pEnableRotationZMin );
	SetEnableRotationZMax( copy.pEnableRotationZMax );
	SetEnableScalingXMin( copy.pEnableScalingXMin );
	SetEnableScalingXMax( copy.pEnableScalingXMax );
	SetEnableScalingYMin( copy.pEnableScalingYMin );
	SetEnableScalingYMax( copy.pEnableScalingYMax );
	SetEnableScalingZMin( copy.pEnableScalingZMin );
	SetEnableScalingZMax( copy.pEnableScalingZMax );
	SetTargetBone( copy.pTargetBone );
	aeRule::operator=( copy );
	return *this;
}
