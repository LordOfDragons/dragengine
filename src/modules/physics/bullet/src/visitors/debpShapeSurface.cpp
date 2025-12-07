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

#include "debpShapeSurface.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionCylinder.h"
#include "../coldet/collision/debpDCollisionCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeSurface
///////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeSurface::debpShapeSurface() :
pUseDirection(false),
pSurface(0.0f){
}

debpShapeSurface::~debpShapeSurface(){
}



// Management
///////////////

void debpShapeSurface::SetDirection(const decVector &direction){
	pDirection = direction;
	pUseDirection = direction.IsZero();
}

void debpShapeSurface::Reset(){
	pSurface = 0.0f;
}



// Visiting
/////////////

void debpShapeSurface::VisitShape(decShape &shape){
}

void debpShapeSurface::VisitShapeSphere(decShapeSphere &sphere){
	pSurface += 4.0f * PI * sphere.GetRadius() * sphere.GetRadius();
}

void debpShapeSurface::VisitShapeBox(decShapeBox &box){
	pSurface += (box.GetHalfExtends().x * box.GetHalfExtends().y
		+ box.GetHalfExtends().x * box.GetHalfExtends().z
		+ box.GetHalfExtends().y * box.GetHalfExtends().z) * 2.0f;
}

void debpShapeSurface::VisitShapeCylinder(decShapeCylinder &cylinder){
	pSurface += 2.0f * PI * cylinder.GetTopRadius() * (cylinder.GetHalfHeight() * 2.0f + cylinder.GetTopRadius());
	// TODO tapered support
}

void debpShapeSurface::VisitShapeCapsule(decShapeCapsule &capsule){
	// surface += surfaceCylinderMantle + surfaceHalfSphere * 2
	// 
	// thus
	// surface += surfaceCylinderMantle + surfaceSphere
	pSurface += 4.0f * PI * capsule.GetTopRadius() * (capsule.GetHalfHeight() + capsule.GetTopRadius());
}

void debpShapeSurface::VisitShapeHull(decShapeHull &hull) {
	// TODO calculate volume
	// use for the time being the surface of a sphere containing all points
	const int count = hull.GetPointCount();
	if(count == 0){
		pSurface = 0.0f;
		return;
	}
	
	decVector center;
	int i;
	
	for(i=0; i<count; i++){
		center += hull.GetPointAt(i);
	}
	center /= (float)count;
	
	float radiusSquared = 0.0f;
	for(i=0; i<count; i++){
		radiusSquared = decMath::max(radiusSquared, (hull.GetPointAt(i) - center).LengthSquared());
	}
	
	pSurface += 4.0f * PI * radiusSquared;
}
