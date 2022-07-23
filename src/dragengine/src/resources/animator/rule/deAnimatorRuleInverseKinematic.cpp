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
