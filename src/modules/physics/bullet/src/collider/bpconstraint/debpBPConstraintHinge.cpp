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

#include "debpBPConstraintHinge.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class debpBPConstraintHinge
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBPConstraintHinge::debpBPConstraintHinge(btRigidBody &rbA, btRigidBody &rbB,
const btTransform &rbAFrame, const btTransform &rbBFrame)
: btHingeConstraint(rbA, rbB, rbAFrame, rbBFrame, true) // default = false
{
	setUserConstraintPtr((debpBPConstraintBase*)this);
}

debpBPConstraintHinge::debpBPConstraintHinge(btRigidBody &rbA, const btTransform &rbAFrame,
const btTransform &rbBFrame)
: btHingeConstraint(rbA, getFixedBody(), rbAFrame, rbBFrame, true) // default = false
{
	setUserConstraintPtr((debpBPConstraintBase*)this);
}

debpBPConstraintHinge::~debpBPConstraintHinge(){
}



// Management
///////////////

#if 0
void debpBPConstraintHinge::getInfo2(btTypedConstraint::btConstraintInfo2 *info){
	btHingeConstraint::getInfo2(info);
	SetBreakingParameters(*info);
}

void debpBPConstraintHinge::SetBreakingParameters(btTypedConstraint::btConstraintInfo2 &info){
	const btVector3 linBreakThreshold(SIMD_INFINITY, SIMD_INFINITY, SIMD_INFINITY);
	const btVector3 angBreakThreshold(SIMD_INFINITY, SIMD_INFINITY, 40.0f);
	
	info.m_upperBreakingThreshold[0] = linBreakThreshold.x();
	info.m_lowerBreakingThreshold[0] = -linBreakThreshold.x();
	info.m_breakingMask[0] = 0x01;
	
	const int stride1 = info.rowskip;
	info.m_upperBreakingThreshold[stride1] = linBreakThreshold.y();
	info.m_lowerBreakingThreshold[stride1] = -linBreakThreshold.y();
	info.m_breakingMask[stride1] = 0x02;
	
	const int stride2 = info.rowskip * 2;
	info.m_upperBreakingThreshold[stride2] = linBreakThreshold.z();
	info.m_lowerBreakingThreshold[stride2] = -linBreakThreshold.z();
	info.m_breakingMask[stride2] = 0x04;
	
	const int stride3 = info.rowskip * 3;
	info.m_upperBreakingThreshold[stride3] = angBreakThreshold.x();
	info.m_lowerBreakingThreshold[stride3] = -angBreakThreshold.x();
	info.m_breakingMask[stride3] = 0x08;
	
	const int stride4 = info.rowskip * 4;
	info.m_upperBreakingThreshold[stride4] = angBreakThreshold.y();
	info.m_lowerBreakingThreshold[stride4] = -angBreakThreshold.y();
	info.m_breakingMask[stride4] = 0x10;
	
	if(getSolveLimit() || getEnableAngularMotor()){
		const int stride5 = info.rowskip * 5;
		info.m_upperBreakingThreshold[stride5] = angBreakThreshold.z();
		info.m_lowerBreakingThreshold[stride5] = -angBreakThreshold.z();
		info.m_breakingMask[stride5] = 0x20;
	}
}
#endif
