/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
