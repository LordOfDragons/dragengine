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

#include "deoglTransformVolume.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionFrustum.h"
#include "../utils/collision/deoglDCollisionCylinder.h"
#include "../utils/collision/deoglDCollisionCapsule.h"

#include <dragengine/common/exceptions.h>



// class deoglTransformVolume
///////////////////////////////

// constructor, destructor
////////////////////////////

deoglTransformVolume::deoglTransformVolume(){
	pSphere = NULL;
	pCylinder = NULL;
	pCapsule = NULL;
	pBox = NULL;
	pTriangle = NULL;
	pFrustum = NULL;
	pVolume = NULL;
	
	pDirtyMatrix = false;
	pScaling.Set(1.0f, 1.0f, 1.0f);
}

deoglTransformVolume::~deoglTransformVolume(){
	pCleanUp();
}



// Management
///////////////

void deoglTransformVolume::SetTranslation(const decDVector &translation){
	pTranslation = translation;
	pDirtyMatrix = true;
}

void deoglTransformVolume::SetRotation(const decQuaternion &rotation){
	pRotation = rotation;
	pDirtyMatrix = true;
}

void deoglTransformVolume::SetScaling(const decVector &scaling){
	pScaling = scaling;
}



// Convenience Function
/////////////////////////

deoglDCollisionVolume *deoglTransformVolume::GetVolumeFor(deoglDCollisionVolume *volume){
	if(!volume) DETHROW(deeInvalidParam);
	volume->Visit(this);
	return pVolume;
}



// Visiting
/////////////

void deoglTransformVolume::VisitSphere(deoglDCollisionSphere *sphere){
	if(!pSphere){
		pSphere = new deoglDCollisionSphere;
	}
	
	pUpdateMatrix();
	
	pSphere->SetCenter(pMatrix * sphere->GetCenter());
	pSphere->SetRadius(sphere->GetRadius());
	
	pVolume = pSphere;
}

void deoglTransformVolume::VisitCylinder(deoglDCollisionCylinder *cylinder){
	if(!pCylinder){
		pCylinder = new deoglDCollisionCylinder;
	}
	
	pUpdateMatrix();
	
	pCylinder->SetPosition(pMatrix * cylinder->GetPosition());
	pCylinder->SetOrientation((decDMatrix::CreateFromQuaternion(cylinder->GetOrientation()) * pMatrix).ToQuaternion());
	pCylinder->SetHalfHeight(cylinder->GetHalfHeight());
	pCylinder->SetTopRadius(cylinder->GetTopRadius());
	pCylinder->SetBottomRadius(cylinder->GetBottomRadius());
	
	pVolume = pCylinder;
}

void deoglTransformVolume::VisitCapsule(deoglDCollisionCapsule *capsule){
	if(!pCapsule){
		pCapsule = new deoglDCollisionCapsule;
	}
	
	pUpdateMatrix();
	
	pCapsule->SetPosition(pMatrix * capsule->GetPosition());
	pCapsule->SetOrientation((decDMatrix::CreateFromQuaternion(capsule->GetOrientation()) * pMatrix).ToQuaternion());
	pCapsule->SetHalfHeight(capsule->GetHalfHeight());
	pCapsule->SetTopRadius(capsule->GetTopRadius());
	pCapsule->SetBottomRadius(capsule->GetBottomRadius());
	
	pVolume = pCapsule;
}

void deoglTransformVolume::VisitBox(deoglDCollisionBox *box){
	if(!pBox){
		pBox = new deoglDCollisionBox;
	}
	
	pUpdateMatrix();
	
	pBox->SetCenter(pMatrix * box->GetCenter());
	pBox->SetHalfSize(box->GetHalfSize());
	pBox->SetOrientation((decDMatrix::CreateFromQuaternion(box->GetOrientation()) * pMatrix).ToQuaternion());
	
	pVolume = pBox;
}

void deoglTransformVolume::VisitTriangle(deoglDCollisionTriangle *triangle){
	if(!pTriangle){
		pTriangle = new deoglDCollisionTriangle;
	}
	
	pUpdateMatrix();
	
	pTriangle->SetCorners(pMatrix * triangle->GetCorner1(), pMatrix * triangle->GetCorner2(),
			pMatrix * triangle->GetCorner3());
	
	pVolume = pTriangle;
}

void deoglTransformVolume::VisitFrustum(deoglDCollisionFrustum *frustum){
	if(!pFrustum){
		pFrustum = new deoglDCollisionFrustum;
	}
	
	pUpdateMatrix();
	
	decDVector normal, position;
	
	position = pMatrix * (frustum->GetLeftNormal() * -frustum->GetLeftDistance());
	normal = pMatrix.TransformNormal(frustum->GetLeftNormal());
	pFrustum->SetLeftPlane(normal, -(normal * position));
	
	position = pMatrix * (frustum->GetRightNormal() * -frustum->GetRightDistance());
	normal = pMatrix.TransformNormal(frustum->GetRightNormal());
	pFrustum->SetRightPlane(normal, -(normal * position));
	
	position = pMatrix * (frustum->GetTopNormal() * -frustum->GetTopDistance());
	normal = pMatrix.TransformNormal(frustum->GetTopNormal());
	pFrustum->SetTopPlane(normal, -(normal * position));
	
	position = pMatrix * (frustum->GetBottomNormal() * -frustum->GetBottomDistance());
	normal = pMatrix.TransformNormal(frustum->GetBottomNormal());
	pFrustum->SetBottomPlane(normal, -(normal * position));
	
	position = pMatrix * (frustum->GetNearNormal() * -frustum->GetNearDistance());
	normal = pMatrix.TransformNormal(frustum->GetNearNormal());
	pFrustum->SetNearPlane(normal, -(normal * position));
	
	position = pMatrix * (frustum->GetFarNormal() * -frustum->GetFarDistance());
	normal = pMatrix.TransformNormal(frustum->GetFarNormal());
	pFrustum->SetFarPlane(normal, -(normal * position));
	
	pVolume = pFrustum;
}



// Private Functions
//////////////////////

void deoglTransformVolume::pCleanUp(){
	if(pFrustum) delete pFrustum;
	if(pTriangle) delete pTriangle;
	if(pBox) delete pBox;
	if(pCapsule) delete pCapsule;
	if(pCylinder) delete pCylinder;
	if(pSphere) delete pSphere;
}

void deoglTransformVolume::pUpdateMatrix(){
	if(pDirtyMatrix){
		pMatrix.SetWorld(pTranslation, pRotation);
		pDirtyMatrix = false;
	}
}
