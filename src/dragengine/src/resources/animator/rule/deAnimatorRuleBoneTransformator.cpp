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

#include "deAnimatorRuleBoneTransformator.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleBoneTransformator
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleBoneTransformator::deAnimatorRuleBoneTransformator() :
pMinScaling( 1.0f, 1.0f, 1.0f ),
pMaxScaling( 1.0f, 1.0f, 1.0f ),
pAxis( 0.0f, 0.0f, 1.0f ),
pMinAngle( 0.0f ),
pMaxAngle( 0.0f ),
pCoordinateFrame( ecfComponent ),
pEnablePosition( false ),
pEnableOrientation( true ),
pEnableSize( false ),
pUseAxis( false ){
}

deAnimatorRuleBoneTransformator::~deAnimatorRuleBoneTransformator(){
}



// Management
///////////////

void deAnimatorRuleBoneTransformator::SetMinimumTranslation( const decVector &translation ){
	pMinTranslation = translation;
}

void deAnimatorRuleBoneTransformator::SetMaximumTranslation( const decVector &translation ){
	pMaxTranslation = translation;
}

void deAnimatorRuleBoneTransformator::SetMinimumRotation( const decVector &rotation ){
	pMinRotation = rotation;
}

void deAnimatorRuleBoneTransformator::SetMaximumRotation( const decVector &rotation ){
	pMaxRotation = rotation;
}

void deAnimatorRuleBoneTransformator::SetMinimumScaling( const decVector &scaling ){
	pMinScaling = scaling;
}

void deAnimatorRuleBoneTransformator::SetMaximumScaling( const decVector &scaling ){
	pMaxScaling = scaling;
}

void deAnimatorRuleBoneTransformator::SetAxis( const decVector &axis ){
	pAxis = axis;
}

void deAnimatorRuleBoneTransformator::SetMinimumAngle( float angle ){
	pMinAngle = angle;
}

void deAnimatorRuleBoneTransformator::SetMaximumAngle( float angle ){
	pMaxAngle = angle;
}

void deAnimatorRuleBoneTransformator::SetUseAxis( bool useAxis ){
	pUseAxis = useAxis;
}

void deAnimatorRuleBoneTransformator::SetCoordinateFrame( eCoordinateFrames coordinateFrame ){
	pCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleBoneTransformator::SetEnablePosition( bool enable ){
	pEnablePosition = enable;
}

void deAnimatorRuleBoneTransformator::SetEnableOrientation( bool enable ){
	pEnableOrientation = enable;
}

void deAnimatorRuleBoneTransformator::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}

void deAnimatorRuleBoneTransformator::SetTargetBone( const char *boneName ){
	pTargetBone = boneName;
}



// Visiting
/////////////

void deAnimatorRuleBoneTransformator::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitBoneTransformator( *this );
}
