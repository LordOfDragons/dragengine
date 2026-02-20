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
#include <string.h>

#include "meViewEditorHeightPaint.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../collisions/meCLHeightPaint.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meViewEditorHeightPaint
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorHeightPaint::meViewEditorHeightPaint(meView3D &view) : meViewEditorNavigation(view){
	pPainterElapsedTime = 0.0f;
	pPainting = false;
	
	pHeightPainter = new meCLHeightPaint(view.GetWorld());
}

meViewEditorHeightPaint::~meViewEditorHeightPaint(){
	if(pHeightPainter){
		delete pHeightPainter;
	}
}



// Management
///////////////

void meViewEditorHeightPaint::OnFrameUpdate(float elapsed){
	if(pPainting){
		meCamera &camera = GetActiveCamera();
		
		pPainterElapsedTime += elapsed;
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor(GetViewWidth(), GetViewHeight(), pPaintPosition.x, pPaintPosition.y) * 500.0f; // 500m
		pHeightPainter->SetRay(rayPosition, rayDirection);
		
		pHeightPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		RayTestCollision(pHeightPainter, rayPosition, rayDirection, decCollisionFilter(collisionCategory, collisionFilter));
		
		pHeightPainter->Paint(pPainterElapsedTime);
		
		pPainterElapsedTime = 0.0f;
	}
}



// Callbacks
//////////////

void meViewEditorHeightPaint::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	
	meCamera &camera = GetActiveCamera();
	
	const decDVector rayPosition = GetMatrixView().GetPosition();
	const decVector rayDirection = camera.GetDirectionFor(GetViewWidth(), GetViewHeight(), x, y) * 50.0f; // 50m
	pHeightPainter->SetRay(rayPosition, rayDirection);
	
	pPainterElapsedTime = 0.0f;
	
	pHeightPainter->BeginSession();
	
	pPaintPosition.Set(x, y);
	pPainting = true;
}

void meViewEditorHeightPaint::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonRelease(x, y, shift, control);
	
	if(pPainting){
		pPainting = false;
		pHeightPainter->EndSession();
	}
}

void meViewEditorHeightPaint::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	
	if(pPainting){
		/*
		meCamera *camera = pWorld->GetActiveCamera();
		decLayerMask layerMask;
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor(GetViewWidth(), GetViewHeight(), x, y) * 50.0f; // 50m
		pHeightPainter->SetRay(rayPosition, rayDirection);
		
		pHeightPainter->PreparePaint();
		layerMask.SetBit(meWorld::eclmHeightTerrains);
		TestCollisionWith(pHeightPainter, rayPosition, rayDirection, layerMask, 0);
		pHeightPainter->Paint(pPainterElapsedTime);
		pPainterElapsedTime = 0.0f;
		
		RequestUpdate();
		*/
		
		pPaintPosition.Set(x, y);
	}
}
