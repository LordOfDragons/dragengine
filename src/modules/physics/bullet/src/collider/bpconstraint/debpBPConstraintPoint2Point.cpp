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

#include "debpBPConstraintPoint2Point.h"
#include "../debpColliderConstraint.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderConstraint.h>



// Class debpBPConstraintPoint2Point
////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBPConstraintPoint2Point::debpBPConstraintPoint2Point( debpColliderConstraint &constraint,
btRigidBody &rbA, btRigidBody &rbB, const btVector3 &pivotInA, const btVector3 &pivotInB ) :
btPoint2PointConstraint( rbA, rbB, pivotInA, pivotInB ),
pConstraint( constraint )
{
	(void)pConstraint; // for future use
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraintPoint2Point::debpBPConstraintPoint2Point( debpColliderConstraint &constraint,
btRigidBody &rbA, const btVector3 &pivotInA, const btVector3 &pivotInB ) :
btPoint2PointConstraint( rbA, getFixedBody(), pivotInA, pivotInB ),
pConstraint( constraint )
{
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraintPoint2Point::~debpBPConstraintPoint2Point(){
}



// Management
///////////////

void debpBPConstraintPoint2Point::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	btPoint2PointConstraint::getInfo2( info );
}
