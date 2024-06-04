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

#include "deAnimatorRuleInverseKinematic.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../common/exceptions.h"



// Class deAnimatorRuleInverseKinematic
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleInverseKinematic::deAnimatorRuleInverseKinematic() :
pAdjustPosition( true ),
pAdjustOrientation( false ),
pUseSolverBone( false ),
pReachRange( 0.0f ){
}

deAnimatorRuleInverseKinematic::~deAnimatorRuleInverseKinematic(){
}



// Management
///////////////

void deAnimatorRuleInverseKinematic::SetGoalPosition( const decVector &position ){
	pGoalPosition = position;
}

void deAnimatorRuleInverseKinematic::SetGoalOrientation( const decQuaternion &orientation ){
	pGoalOrientation = orientation;
}

void deAnimatorRuleInverseKinematic::SetLocalPosition( const decVector &position ){
	pLocalPosition = position;
}

void deAnimatorRuleInverseKinematic::SetLocalOrientation( const decQuaternion &orientation ){
	pLocalOrientation = orientation;
}

void deAnimatorRuleInverseKinematic::SetAdjustPosition( bool adjust ){
	pAdjustPosition = adjust;
}

void deAnimatorRuleInverseKinematic::SetAdjustOrientation( bool adjust ){
	pAdjustOrientation = adjust;
}



void deAnimatorRuleInverseKinematic::SetUseSolverBone( bool useSolverBone ){
	pUseSolverBone = useSolverBone;
}

void deAnimatorRuleInverseKinematic::SetSolverBone( const char *boneName ){
	pSolverBone = boneName;
}


void deAnimatorRuleInverseKinematic::SetReachRange( float range ){
	pReachRange = decMath::max( range, 0.0f );
}

void deAnimatorRuleInverseKinematic::SetReachBone( const char *bone ){
	pReachBone = bone;
}

void deAnimatorRuleInverseKinematic::SetReachCenter( const decVector &center ){
	pReachCenter = center;
}



// Visiting
/////////////

void deAnimatorRuleInverseKinematic::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitInverseKinematic( *this );
}
