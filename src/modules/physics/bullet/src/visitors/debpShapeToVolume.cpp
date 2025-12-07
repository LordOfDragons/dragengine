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

#include "debpShapeToVolume.h"
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



// Class debpShapeToVolume
////////////////////////////

// Constructor, destructor
////////////////////////////

debpShapeToVolume::debpShapeToVolume(){
	pSphere = NULL;
	pBox = NULL;
	pCylinder = NULL;
	pCapsule = NULL;
	pVolume = NULL;
	
	try{
		pSphere = new debpDCollisionSphere;
		pBox = new debpDCollisionBox;
		pCylinder = new debpDCollisionCylinder;
		pCapsule = new debpDCollisionCapsule;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debpShapeToVolume::~debpShapeToVolume(){
	pCleanUp();
}



// Management
///////////////

void debpShapeToVolume::SetMatrix(const decDMatrix &matrix){
	pMatrix = matrix;
}



// Convenience Function
/////////////////////////

debpDCollisionVolume *debpShapeToVolume::GetVolumeFor(decShape *shape){
	if(! shape){
		DETHROW(deeInvalidParam);
	}
	
	shape->Visit(*this);
	
	return pVolume;
}



// Visiting
/////////////

void debpShapeToVolume::VisitShape(decShape &shape){
	pSphere->SetCenter(pMatrix.GetPosition());
	pSphere->SetRadius(0.1);
	
	pVolume = pSphere;
}

void debpShapeToVolume::VisitShapeSphere(decShapeSphere &sphere){
	pSphere->SetCenter(pMatrix * sphere.GetPosition());
	pSphere->SetRadius(sphere.GetRadius());
	
	pVolume = pSphere;
}

void debpShapeToVolume::VisitShapeBox(decShapeBox &box){
	pBox->SetCenter(pMatrix * box.GetPosition());
	pBox->SetHalfSize(box.GetHalfExtends());
	pBox->SetOrientation((decDMatrix::CreateFromQuaternion(box.GetOrientation()) * pMatrix).ToQuaternion());
	
	pVolume = pBox;
}

void debpShapeToVolume::VisitShapeCylinder(decShapeCylinder &cylinder){
	pCylinder->SetPosition(pMatrix * cylinder.GetPosition());
	pCylinder->SetOrientation((decDMatrix::CreateFromQuaternion(cylinder.GetOrientation()) * pMatrix).ToQuaternion());
	pCylinder->SetHalfHeight(cylinder.GetHalfHeight());
	pCylinder->SetTopRadius(cylinder.GetTopRadius());
	pCylinder->SetBottomRadius(cylinder.GetBottomRadius());
	
	pVolume = pCylinder;
}

void debpShapeToVolume::VisitShapeCapsule(decShapeCapsule &capsule){
	pCapsule->SetPosition(pMatrix * capsule.GetPosition());
	pCapsule->SetOrientation((decDMatrix::CreateFromQuaternion(capsule.GetOrientation()) * pMatrix).ToQuaternion());
	pCapsule->SetHalfHeight(capsule.GetHalfHeight());
	pCapsule->SetTopRadius(capsule.GetTopRadius());
	pCapsule->SetBottomRadius(capsule.GetBottomRadius());
	
	pVolume = pCapsule;
}

void debpShapeToVolume::VisitShapeHull(decShapeHull &hull) {
	VisitShape(hull);
}



// Private Functions
//////////////////////

void debpShapeToVolume::pCleanUp(){
	if(pCapsule){
		delete pCapsule;
	}
	if(pCylinder){
		delete pCylinder;
	}
	if(pBox){
		delete pBox;
	}
	if(pSphere){
		delete pSphere;
	}
}
