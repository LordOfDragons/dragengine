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

#include "meViewEditorMaskPaint.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../collisions/meCLMaskPaint.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meViewEditorMaskPaint
////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorMaskPaint::meViewEditorMaskPaint(meView3D &view) : meViewEditorNavigation(view){
	pMaskPainter = new meCLMaskPaint(view.GetWorld());
}

meViewEditorMaskPaint::~meViewEditorMaskPaint(){
	if(pMaskPainter){
		delete pMaskPainter;
	}
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorMaskPaint::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	
	meCamera &camera = GetActiveCamera();
	meWorld &world = GetWorld();
	
	const decPoint3 activeSector; // = world.GetActiveSector();
	meHeightTerrainSector * const htsector = world.GetHeightTerrain()->GetSectorWith(decPoint(activeSector.x, activeSector.z));
	
	if(htsector && htsector->GetActiveTexture()){
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor(GetViewWidth(), GetViewHeight(), x, y) * 500.0f; // 500m
		pMaskPainter->SetRay(rayPosition, rayDirection);
		pMaskPainter->SetTexture(htsector, htsector->GetActiveTexture());
		
		pMaskPainter->BeginSession();
		pMaskPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		
		RayTestCollision(pMaskPainter, rayPosition, rayDirection, decCollisionFilter(collisionCategory, collisionFilter));
		pMaskPainter->Paint();
		
	}else{
		pMaskPainter->SetTexture(NULL, NULL);
	}
}

void meViewEditorMaskPaint::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonRelease(x, y, shift, control);
	
	if(pMaskPainter->CanPaint()){
		pMaskPainter->EndSession();
	}
}

void meViewEditorMaskPaint::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	
	if(pMaskPainter->CanPaint()){
		meCamera &camera = GetActiveCamera();
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor(GetViewWidth(), GetViewHeight(), x, y) * 50.0f; // 50m
		pMaskPainter->SetRay(rayPosition, rayDirection);
		
		pMaskPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		RayTestCollision(pMaskPainter, rayPosition, rayDirection, decCollisionFilter(collisionCategory, collisionFilter));
		pMaskPainter->Paint();
	}
}
