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

#include "deDebugDrawer.h"
#include "deDebugDrawerShape.h"
#include "deDebugDrawerManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicDebugDrawer.h"



// Class deDebugDrawer
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawer::deDebugDrawer(deDebugDrawerManager *manager) : deResource(manager),
pLLWorld(this)
{
	pScale.Set(1.0f, 1.0f, 1.0f);
	
	pVisible = true;
	pXRay = false;
	
	pPeerGraphic = nullptr;
	
	pParentWorld = nullptr;
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

void deDebugDrawer::AddShape(deDebugDrawerShape::Ref &&shape){
	DEASSERT_NOTNULL(shape)
	
	pShapes.Add(std::move(shape));
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::RemoveShape(deDebugDrawerShape *shape){
	pShapes.Remove(shape);
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::RemoveShapeFrom(int index){
	pShapes.RemoveFrom(index);
	NotifyShapeLayoutChanged();
}

void deDebugDrawer::RemoveAllShapes(){
	if(pShapes.IsEmpty()){
		return;
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



// Private functions
//////////////////////

void deDebugDrawer::pCleanUp(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
}
