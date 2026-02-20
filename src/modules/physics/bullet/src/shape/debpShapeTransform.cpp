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

#include "debpShapeTransform.h"
#include "debpShapeSphere.h"
#include "debpShapeBox.h"
#include "debpShapeCylinder.h"
#include "debpShapeCapsule.h"

#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>



// Class debpShapeTransform
/////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeTransform::debpShapeTransform(){
}

debpShapeTransform::~debpShapeTransform(){
}



// Management
///////////////

const debpDCollisionVolume &debpShapeTransform::TransformBase(const debpShape *shape, const decDMatrix &transformation){
	switch((debpShape::eShapeTypes)shape->GetType()){
	case debpShape::estSphere:
		return TransformShapeSphere(*(((debpShapeSphere*)shape)->GetShapeSphere()), transformation);
		
	case debpShape::estBox:
		return TransformShapeBox(*(((debpShapeBox*)shape)->GetShapeBox()), transformation);
		
	case debpShape::estCylinder:
		return TransformShapeCylinder(*(((debpShapeCylinder*)shape)->GetShapeCylinder()), transformation);
		
	case debpShape::estCapsule:
		return TransformShapeCapsule(*(((debpShapeCapsule*)shape)->GetShapeCapsule()), transformation);
	}
	
	DETHROW(deeInvalidParam);
}

const debpDCollisionVolume& debpShapeTransform::TransformCurrent(const debpShape *shape, const decDMatrix &transformation){
	switch((debpShape::eShapeTypes)shape->GetType()){
	case debpShape::estSphere:
		return TransformCollisionSphere(((debpShapeSphere*)shape)->GetCollisionSphere(), transformation);
		
	case debpShape::estBox:
		return TransformCollisionBox(((debpShapeBox*)shape)->GetCollisionBox(), transformation);
		
	case debpShape::estCylinder:
		return TransformCollisionCylinder(((debpShapeCylinder*)shape)->GetCollisionCylinder(), transformation);
		
	case debpShape::estCapsule:
		return TransformCollisionCapsule(((debpShapeCapsule*)shape)->GetCollisionCapsule(), transformation);
	}
	
	DETHROW(deeInvalidParam);
}



const debpDCollisionSphere &debpShapeTransform::TransformShapeSphere(const decShapeSphere &sphere, const decDMatrix &transformation){
	pSphere.SetCenter(transformation * sphere.GetPosition());
	pSphere.SetRadius(sphere.GetRadius());
	return pSphere;
}

const debpDCollisionBox &debpShapeTransform::TransformShapeBox(const decShapeBox &box, const decDMatrix &transformation){
	pBox.SetCenter(transformation * box.GetPosition());
	pBox.SetOrientation(box.GetOrientation() * transformation.ToQuaternion());
	pBox.SetHalfSize(box.GetHalfExtends());
	return pBox;
}

const debpDCollisionCylinder &debpShapeTransform::TransformShapeCylinder(const decShapeCylinder &cylinder, const decDMatrix &transformation){
	pCylinder.SetPosition(transformation * cylinder.GetPosition());
	pCylinder.SetOrientation(cylinder.GetOrientation() * transformation.ToQuaternion());
	pCylinder.SetHalfHeight(cylinder.GetHalfHeight());
	pCylinder.SetTopRadius(cylinder.GetTopRadius());
	pCylinder.SetBottomRadius(cylinder.GetBottomRadius());
	return pCylinder;
}

const debpDCollisionCapsule &debpShapeTransform::TransformShapeCapsule(const decShapeCapsule &capsule, const decDMatrix &transformation){
	pCapsule.SetPosition(transformation * capsule.GetPosition());
	pCapsule.SetOrientation(capsule.GetOrientation() * transformation.ToQuaternion());
	pCapsule.SetHalfHeight(capsule.GetHalfHeight());
	pCapsule.SetTopRadius(capsule.GetTopRadius());
	pCapsule.SetBottomRadius(capsule.GetBottomRadius());
	return pCapsule;
}



const debpDCollisionSphere &debpShapeTransform::TransformCollisionSphere(const debpDCollisionSphere &sphere, const decDMatrix &transformation){
	pSphere.SetCenter(transformation * sphere.GetCenter());
	pSphere.SetRadius(sphere.GetRadius());
	return pSphere;
}

const debpDCollisionBox &debpShapeTransform::TransformCollisionBox(const debpDCollisionBox &box, const decDMatrix &transformation){
	pBox.SetCenter(transformation * box.GetCenter());
	pBox.SetOrientation(box.GetOrientation() * transformation.ToQuaternion());
	pBox.SetHalfSize(box.GetHalfSize());
	return pBox;
}

const debpDCollisionCylinder &debpShapeTransform::TransformCollisionCylinder(const debpDCollisionCylinder &cylinder, const decDMatrix &transformation){
	pCylinder.SetPosition(transformation * cylinder.GetPosition());
	pCylinder.SetOrientation(cylinder.GetOrientation() * transformation.ToQuaternion());
	pCylinder.SetHalfHeight(cylinder.GetHalfHeight());
	pCylinder.SetTopRadius(cylinder.GetTopRadius());
	pCylinder.SetBottomRadius(cylinder.GetBottomRadius());
	return pCylinder;
}

const debpDCollisionCapsule &debpShapeTransform::TransformCollisionCapsule(const debpDCollisionCapsule &capsule, const decDMatrix &transformation){
	pCapsule.SetPosition(transformation * capsule.GetPosition());
	pCapsule.SetOrientation(capsule.GetOrientation() * transformation.ToQuaternion());
	pCapsule.SetHalfHeight(capsule.GetHalfHeight());
	pCapsule.SetTopRadius(capsule.GetTopRadius());
	pCapsule.SetBottomRadius(capsule.GetBottomRadius());
	return pCapsule;
}
