/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deAnimatorRuleLimit.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleLimit
//////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleLimit::deAnimatorRuleLimit(){
	pCoordinateFrame = ecfComponent;
	pMinScaling.Set( 1.0f, 1.0f, 1.0f );
	pMaxScaling.Set( 1.0f, 1.0f, 1.0f );
	
	pEnablePositionXMin = false;
	pEnablePositionXMax = false;
	pEnablePositionYMin = false;
	pEnablePositionYMax = false;
	pEnablePositionZMin = false;
	pEnablePositionZMax = false;
	
	pEnableRotationXMin = false;
	pEnableRotationXMax = false;
	pEnableRotationYMin = false;
	pEnableRotationYMax = false;
	pEnableRotationZMin = false;
	pEnableRotationZMax = false;
	
	pEnableScalingXMin = false;
	pEnableScalingXMax = false;
	pEnableScalingYMin = false;
	pEnableScalingYMax = false;
	pEnableScalingZMin = false;
	pEnableScalingZMax = false;
}

deAnimatorRuleLimit::~deAnimatorRuleLimit(){
}



// Management
///////////////

void deAnimatorRuleLimit::SetMinimumPosition( const decVector &position ){
	pMinPosition = position;
}

void deAnimatorRuleLimit::SetMaximumPosition( const decVector &position ){
	pMaxPosition = position;
}

void deAnimatorRuleLimit::SetMinimumRotation( const decVector &rotation ){
	pMinRotation = rotation;
}

void deAnimatorRuleLimit::SetMaximumRotation( const decVector &rotation ){
	pMaxRotation = rotation;
}

void deAnimatorRuleLimit::SetMinimumScaling( const decVector &scaling ){
	pMinScaling = scaling;
}

void deAnimatorRuleLimit::SetMaximumScaling( const decVector &scaling ){
	pMaxScaling = scaling;
}



void deAnimatorRuleLimit::SetCoordinateFrame( eCoordinateFrames coordinateFrame ){
	if( coordinateFrame < ecfBoneLocal || coordinateFrame > ecfTargetBone ){
		DETHROW( deeInvalidParam );
	}
	pCoordinateFrame = coordinateFrame;
}



void deAnimatorRuleLimit::SetEnablePositionXMin( bool enabled ){
	pEnablePositionXMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionXMax( bool enabled ){
	pEnablePositionXMax = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionYMin( bool enabled ){
	pEnablePositionYMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionYMax( bool enabled ){
	pEnablePositionYMax = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionZMin( bool enabled ){
	pEnablePositionZMin = enabled;
}

void deAnimatorRuleLimit::SetEnablePositionZMax( bool enabled ){
	pEnablePositionZMax = enabled;
}



void deAnimatorRuleLimit::SetEnableRotationXMin( bool enabled ){
	pEnableRotationXMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationXMax( bool enabled ){
	pEnableRotationXMax = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationYMin( bool enabled ){
	pEnableRotationYMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationYMax( bool enabled ){
	pEnableRotationYMax = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationZMin( bool enabled ){
	pEnableRotationZMin = enabled;
}

void deAnimatorRuleLimit::SetEnableRotationZMax( bool enabled ){
	pEnableRotationZMax = enabled;
}



void deAnimatorRuleLimit::SetEnableScalingXMin( bool enabled ){
	pEnableScalingXMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingXMax( bool enabled ){
	pEnableScalingXMax = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingYMin( bool enabled ){
	pEnableScalingYMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingYMax( bool enabled ){
	pEnableScalingYMax = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingZMin( bool enabled ){
	pEnableScalingZMin = enabled;
}

void deAnimatorRuleLimit::SetEnableScalingZMax( bool enabled ){
	pEnableScalingZMax = enabled;
}



void deAnimatorRuleLimit::SetTargetBone( const char *boneName ){
	pTargetBone = boneName;
}



// Visiting
/////////////

void deAnimatorRuleLimit::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitLimit( *this );
}
