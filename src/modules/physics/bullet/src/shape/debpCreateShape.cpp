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

#include "debpCreateShape.h"
#include "debpShapeSphere.h"
#include "debpShapeBox.h"
#include "debpShapeCylinder.h"
#include "debpShapeCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/exceptions.h>



// Class debpCreateShape
//////////////////////////

// Constructor, destructor
////////////////////////////

debpCreateShape::debpCreateShape(){
}

debpCreateShape::~debpCreateShape(){
}



// Management
///////////////

void debpCreateShape::SetCreatedShape(debpShape *shape){
	pShape = shape;
}

void debpCreateShape::Reset(){
	pShape = NULL;
}



// Visiting
/////////////

void debpCreateShape::VisitShape(decShape&){
	pShape = NULL;
}

void debpCreateShape::VisitShapeSphere(decShapeSphere &sphere){
	pShape = debpShapeSphere::Ref::New(&sphere);
}

void debpCreateShape::VisitShapeBox(decShapeBox &box){
	pShape = debpShapeBox::Ref::New(&box);
}

#include "debpShapeHackBox.h"
void debpCreateShape::VisitShapeCylinder(decShapeCylinder &cylinder){
	// hack required because of debpCollisionWorld::contactTest with USE_BULLET_COLLISION disabled
	printf("[TEMPORARY HACK] debpCreateShape::VisitShapeCylinder\n");
	const float radius = cylinder.GetTopRadius() / 1.414213f;
	pShape = debpShapeHackBox::Ref::New(new decShapeBox(
		decVector(radius, cylinder.GetHalfHeight(), radius),
		cylinder.GetPosition(), cylinder.GetOrientation()));
	
	//pShape = debpShapeCylinder::Ref::New(&cylinder);
}

void debpCreateShape::VisitShapeCapsule(decShapeCapsule &capsule){
	pShape = debpShapeCapsule::Ref::New(&capsule);
}

void debpCreateShape::VisitShapeHull(decShapeHull &hull){
	// not supported
	VisitShape(hull);
}
