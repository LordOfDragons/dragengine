/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpBPConstraintHinge.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class debpBPConstraintHinge
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBPConstraintHinge::debpBPConstraintHinge( btRigidBody &rbA, btRigidBody &rbB,
const btTransform &rbAFrame, const btTransform &rbBFrame )
: btHingeConstraint( rbA, rbB, rbAFrame, rbBFrame, true ) // default = false
{
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraintHinge::debpBPConstraintHinge( btRigidBody &rbA, const btTransform &rbAFrame,
const btTransform &rbBFrame )
: btHingeConstraint( rbA, getFixedBody(), rbAFrame, rbBFrame, true ) // default = false
{
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraintHinge::~debpBPConstraintHinge(){
}



// Management
///////////////

#if 0
void debpBPConstraintHinge::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	btHingeConstraint::getInfo2( info );
	SetBreakingParameters( *info );
}

void debpBPConstraintHinge::SetBreakingParameters( btTypedConstraint::btConstraintInfo2 &info ){
	const btVector3 linBreakThreshold( SIMD_INFINITY, SIMD_INFINITY, SIMD_INFINITY );
	const btVector3 angBreakThreshold( SIMD_INFINITY, SIMD_INFINITY, 40.0f );
	
	info.m_upperBreakingThreshold[ 0 ] = linBreakThreshold.x();
	info.m_lowerBreakingThreshold[ 0 ] = -linBreakThreshold.x();
	info.m_breakingMask[ 0 ] = 0x01;
	
	const int stride1 = info.rowskip;
	info.m_upperBreakingThreshold[ stride1 ] = linBreakThreshold.y();
	info.m_lowerBreakingThreshold[ stride1 ] = -linBreakThreshold.y();
	info.m_breakingMask[ stride1 ] = 0x02;
	
	const int stride2 = info.rowskip * 2;
	info.m_upperBreakingThreshold[ stride2 ] = linBreakThreshold.z();
	info.m_lowerBreakingThreshold[ stride2 ] = -linBreakThreshold.z();
	info.m_breakingMask[ stride2 ] = 0x04;
	
	const int stride3 = info.rowskip * 3;
	info.m_upperBreakingThreshold[ stride3 ] = angBreakThreshold.x();
	info.m_lowerBreakingThreshold[ stride3 ] = -angBreakThreshold.x();
	info.m_breakingMask[ stride3 ] = 0x08;
	
	const int stride4 = info.rowskip * 4;
	info.m_upperBreakingThreshold[ stride4 ] = angBreakThreshold.y();
	info.m_lowerBreakingThreshold[ stride4 ] = -angBreakThreshold.y();
	info.m_breakingMask[ stride4 ] = 0x10;
	
	if( getSolveLimit() || getEnableAngularMotor() ){
		const int stride5 = info.rowskip * 5;
		info.m_upperBreakingThreshold[ stride5 ] = angBreakThreshold.z();
		info.m_lowerBreakingThreshold[ stride5 ] = -angBreakThreshold.z();
		info.m_breakingMask[ stride5 ] = 0x20;
	}
}
#endif
