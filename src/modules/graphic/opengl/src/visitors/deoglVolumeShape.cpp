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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoglVolumeShape.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../shapes/deoglShapeBox.h"
#include "../shapes/deoglShapeSphere.h"
#include "../shapes/deoglShapeCylinder.h"
#include "../shapes/deoglShapeCapsule.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>



// Class deoglVolumeShape
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVolumeShape::deoglVolumeShape(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pShape(NULL){
}

deoglVolumeShape::~deoglVolumeShape(){
}



// Management
///////////////

void deoglVolumeShape::Reset(){
	pShape = NULL;
}



// Visiting
/////////////

void deoglVolumeShape::VisitShape(decShape &shape){
	Reset();
}

void deoglVolumeShape::VisitShapeSphere(decShapeSphere &sphere){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt(deoglRTBufferObject::esSphere);
	
	((deoglShapeSphere*)pShape)->CalcMatrix(pMatrix1, sphere.GetPosition(), sphere.GetRadius());
	pMatrix2.SetIdentity();
}

void deoglVolumeShape::VisitShapeBox(decShapeBox &box){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt(deoglRTBufferObject::esBox);
	
	((deoglShapeBox*)pShape)->CalcMatrix(pMatrix1, box.GetPosition(),
		box.GetOrientation(), box.GetHalfExtends());
	pMatrix2.SetIdentity();
}

void deoglVolumeShape::VisitShapeCylinder(decShapeCylinder &cylinder){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt(deoglRTBufferObject::esCylinder);
	
	((deoglShapeCylinder*)pShape)->CalcMatrices(pMatrix1, pMatrix2, cylinder.GetPosition(),
		cylinder.GetOrientation(), cylinder.GetHalfHeight(), cylinder.GetTopRadius(),
		cylinder.GetBottomRadius());
}

void deoglVolumeShape::VisitShapeCapsule(decShapeCapsule &capsule){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt(deoglRTBufferObject::esCapsule);
	
	((deoglShapeCapsule*)pShape)->CalcMatrices(pMatrix1, pMatrix2, capsule.GetPosition(),
		capsule.GetOrientation(), capsule.GetHalfHeight(), capsule.GetTopRadius(),
		capsule.GetBottomRadius());
}

void deoglVolumeShape::VisitShapeHull(decShapeHull &hull){
	// not supported yet
	VisitShape(hull);
}
