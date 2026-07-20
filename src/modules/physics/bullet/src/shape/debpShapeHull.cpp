/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "debpShapeHull.h"
#include "../dePhysicsBullet.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>


// Class debpShapeHull
////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeHull::debpShapeHull(decShapeHull *shape) :
debpShape(estHull, shape),
pSHull(shape)
{
	pCHull.SetPosition(shape->GetPosition());
	pCHull.SetOrientation(shape->GetOrientation());
	
	debpDCollisionHull::PointList points(shape->GetPoints().GetCount());
	shape->GetPoints().Visit([&](const decVector &point){
		points.Add(decDVector(point));
	});
	pCHull.SetPoints(points);
	
	SetCollisionVolume(&pCHull);
}

debpShapeHull::~debpShapeHull() = default;


// Management
///////////////

void debpShapeHull::UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale){
	pCHull.SetPosition(transformation * pSHull->GetPosition());
	pCHull.SetOrientation(pSHull->GetOrientation() * transformation.ToQuaternion());
	
	pCHull.GetPoints().Visit([&](decDVector &point){
		point = point.Multiply(scale);
	});
}

void debpShapeHull::PrintDebug(dePhysicsBullet &module){
	const decDVector &p = pCHull.GetPosition();
	const decVector o(pCHull.GetOrientation().GetEulerAngles() * RAD2DEG);
	module.LogInfoFormat("hull: p=(%.4g,%.4g,%.4g) o=(%.2g,%.2g,%.2g) pc=%d",
		p.x, p.y, p.z, o.x, o.y, o.z, pCHull.GetPoints().GetCount());
}
