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

#include "debpShapeCylinder.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeCylinder
////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeCylinder::debpShapeCylinder(decShapeCylinder *shape) : debpShape(estCylinder, shape){
	pSCylinder = shape;
	
	SetCollisionVolume(&pCCylinder);
}

debpShapeCylinder::~debpShapeCylinder(){
}



// Management
///////////////

void debpShapeCylinder::UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale){
	pCCylinder.SetPosition(transformation * pSCylinder->GetPosition());
	pCCylinder.SetOrientation(pSCylinder->GetOrientation() * transformation.ToQuaternion());
	
	const float scaleRadius = (float)((scale.x + scale.z) * 0.5);
	pCCylinder.SetHalfHeight(pSCylinder->GetHalfHeight() * (float)scale.y);
	pCCylinder.SetTopRadius(pSCylinder->GetTopRadius() * scaleRadius);
	pCCylinder.SetBottomRadius(pSCylinder->GetBottomRadius() * scaleRadius);
}

void debpShapeCylinder::PrintDebug(dePhysicsBullet &module){
	const decDVector &p = pCCylinder.GetPosition();
	double h = pCCylinder.GetHalfHeight();
	double t = pCCylinder.GetTopRadius();
	double b = pCCylinder.GetBottomRadius();
	const decQuaternion &o = pCCylinder.GetOrientation();
	
	module.LogInfoFormat("cylinder: p=(%.4g,%.4g,%.4g) h=%.4g t=%.4g b=%.4g o=(%.4g,%.4g,%.4g,%.4g)", p.x, p.y, p.z, h, t, b, o.x, o.y, o.z, o.w);
}
