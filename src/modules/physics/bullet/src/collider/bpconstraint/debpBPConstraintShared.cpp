#if 0

/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2014, Plüss Roland ( roland@rptd.ch )
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
#include <stddef.h>
#include <stdlib.h>

#include "debpBPConstraintShared.h"
#include "../debpColliderConstraint.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class debpBPConstraintShared
/////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBPConstraintShared::debpBPConstraintShared(debpColliderConstraint *constraint){
	if(! constraint){
		DETHROW(deeInvalidParam);
	}
	
	pConstraint = constraint;
	
	ResetAxisBreaking();
}

debpBPConstraintShared::~debpBPConstraintShared(){
}



// Management
///////////////

bool debpBPConstraintShared::IsAxisBroke(eBreakingAxes axis) const{
	if(axis < ebaLinearX || axis > ebaAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	return pAxisBroke[axis];
}

void debpBPConstraintShared::SetAxisBroke(eBreakingAxes axis, bool broke){
	if(axis < ebaLinearX || axis > ebaAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	pAxisBroke[axis] = broke;
}

void debpBPConstraintShared::SetLinearXBroke(bool broke){
	pAxisBroke[ebaLinearX] = broke;
}

void debpBPConstraintShared::SetLinearYBroke(bool broke){
	pAxisBroke[ebaLinearY] = broke;
}

void debpBPConstraintShared::SetLinearZBroke(bool broke){
	pAxisBroke[ebaLinearZ] = broke;
}

void debpBPConstraintShared::SetAngularXBroke(bool broke){
	pAxisBroke[ebaAngularX] = broke;
}

void debpBPConstraintShared::SetAngularYBroke(bool broke){
	pAxisBroke[ebaAngularY] = broke;
}

void debpBPConstraintShared::SetAngularZBroke(bool broke){
	pAxisBroke[ebaAngularZ] = broke;
}

void debpBPConstraintShared::ResetAxisBreaking(){
	pAxisBroke[ebaLinearX] = false;
	pAxisBroke[ebaLinearY] = false;
	pAxisBroke[ebaLinearZ] = false;
	pAxisBroke[ebaAngularX] = false;
	pAxisBroke[ebaAngularY] = false;
	pAxisBroke[ebaAngularZ] = false;
}



void debpBPConstraintShared::SolverPrepareBreaking(eBreakingAxes axis,
btTypedConstraint::btConstraintInfo2 &info, int row) const{
	if(axis < ebaLinearX || axis > ebaAngularZ){
		DETHROW(deeInvalidParam);
	}
	
	const int offset = info.rowskip * row;
	
	btScalar breakingImpulse = SIMD_INFINITY;
	if(axis == ebaAngularZ){
		breakingImpulse = (btScalar)40.0;
	}
	
	if(info.m_upperLimit[offset] >= breakingImpulse){
		info.m_upperLimit[offset] = breakingImpulse;
	}
	if(info.m_lowerLimit[offset] <= -breakingImpulse){
		info.m_lowerLimit[offset] = -breakingImpulse;
	}
}

bool debpBPConstraintShared::TestBreaking(eBreakingAxes axis, btTypedConstraint::btConstraintInfo2 &info, int row){
	return false;
}

debpBPConstraintShared *debpBPConstraintShared::BreakConstraint() const{
	return NULL;
}

#endif
