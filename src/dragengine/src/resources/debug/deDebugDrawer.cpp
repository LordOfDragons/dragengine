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
#include <string.h>

#include "deDebugDrawer.h"
#include "deDebugDrawerShape.h"
#include "deDebugDrawerManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicDebugDrawer.h"



// Class deDebugDrawer
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawer::deDebugDrawer(deDebugDrawerManager *manager) : deResource(manager){
	pScale.Set(1.0f, 1.0f, 1.0f);
	
	pVisible = true;
	pXRay = false;
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deDebugDrawer::~deDebugDrawer(){
	pCleanUp();
}



// Management
///////////////

void deDebugDrawer::SetPosition(const decDVector &position){
	if(!pPosition.IsEqualTo(position)){
		pPosition = position;
		if(pPeerGraphic){
			pPeerGraphic->PositionChanged();
		}
	}
}

void deDebugDrawer::SetOrientation(const decQuaternion &orientation){
	if(!pOrientation.IsEqualTo(orientation)){
		pOrientation = orientation;
		if(pPeerGraphic){
			pPeerGraphic->OrientationChanged();
		}
	}
}

void deDebugDrawer::SetScale(const decVector &scale){
	if(!pScale.IsEqualTo(scale)){
		pScale = scale;
		if(pPeerGraphic){
			pPeerGraphic->ScalingChanged();
		}
	}
}



void deDebugDrawer::SetVisible(bool visible){
	if(pVisible != visible){
		pVisible = visible;
		if(pPeerGraphic){
			pPeerGraphic->VisibleChanged();
		}
	}
}

void deDebugDrawer::SetXRay(bool xray){
	if(pXRay != xray){
		pXRay = xray;
		if(pPeerGraphic){
			pPeerGraphic->XRayChanged();
		}
	}
}



// Shape Management
//////////////////////

int deDebugDrawer::GetShapeCount() const{
	return pShapes.GetCount();
}

deDebugDrawerShape *deDebugDrawer::GetShapeAt(int index) const{
	return (deDebugDrawerShape*)pShapes.GetAt(index);
}

int deDebugDrawer::IndexOfShape(deDebugDrawerShape *shape) const{
	return pShapes.IndexOf(shape);
}

bool deDebugDrawer::HasShape(deDebugDrawerShape *shape) const{
	return pShapes.Has(shape);
}

void deDebugDrawer::AddShape(deDebugDrawerShape *shape){
	if(!shape || HasShape(shape)){
		DETHROW(deeInvalidParam);
	}
	pShapes.Add(shape);
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::RemoveShape(deDebugDrawerShape *shape){
	RemoveShapeFrom(IndexOfShape(shape));
}

void deDebugDrawer::RemoveShapeFrom(int index){
	deDebugDrawerShape * const shape = GetShapeAt(index);
	pShapes.RemoveFrom(index);
	delete shape;
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::RemoveAllShapes(){
	if(pShapes.GetCount() == 0){
		return;
	}
	
	const int count = pShapes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deDebugDrawerShape*)pShapes.GetAt(i);
	}
	pShapes.RemoveAll();
	
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::NotifyShapeColorChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeColorChanged();
	}
}

void deDebugDrawer::NotifyShapeGeometryChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeGeometryChanged();
	}
}

void deDebugDrawer::NotifyShapeContentChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeContentChanged();
	}
}

void deDebugDrawer::NotifyShapeLayoutChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeLayoutChanged();
	}
}



// System Peers
/////////////////

void deDebugDrawer::SetPeerGraphic(deBaseGraphicDebugDrawer *peer){
	if(peer != pPeerGraphic){
		if(pPeerGraphic){
			delete pPeerGraphic;
		}
		pPeerGraphic = peer;
	}
}



// Linked List
////////////////

void deDebugDrawer::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deDebugDrawer::SetLLWorldPrev(deDebugDrawer *debugDrawer){
	pLLWorldPrev = debugDrawer;
}

void deDebugDrawer::SetLLWorldNext(deDebugDrawer *debugDrawer){
	pLLWorldNext = debugDrawer;
}



// Private functions
//////////////////////

void deDebugDrawer::pCleanUp(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	RemoveAllShapes();
}
