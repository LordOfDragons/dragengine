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

#include "meViewEditorScale.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../undosys/gui/object/meUScaleObject.h"
#include "../../undosys/gui/decal/meUDecalScale.h"
#include "../../undosys/gui/objectshape/meUObjectShapeScale.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"
#include "../../world/decal/meDecal.h"
#include "../../world/objectshape/meObjectShape.h"

#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Definitions
////////////////

#define ME_DRAG_SCALE	(1.0 / 250.0)



// Class meViewEditorScale
////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorScale::meViewEditorScale(meView3D &view) : meViewEditorNavigation(view){
}

meViewEditorScale::~meViewEditorScale(){
}



// Management
///////////////



// Callbacks
//////////////

bool meViewEditorScale::OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control){
	if(pUndoScale){
		return CheckAxisLocking(key);
		
	}else{
		return meViewEditorNavigation::OnKeyPress(key, shift, control);
	}
}

void meViewEditorScale::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	
	pUndoScale = nullptr;
	
	if(elementMode == meWorldGuiParameters::eemObject){
		const meObjectSelection &selection = world.GetSelectionObject();
		const meObject::List &listSelected = selection.GetSelected();
		
		if(listSelected.IsNotEmpty()){
			pUndoScale = meUScaleObject::Ref::New(&world, GetWorld().GetSelectionObject().GetSelected());
		}
		
	}else if(elementMode == meWorldGuiParameters::eemObjectShape){
		meObject * const activeObject = world.GetSelectionObject().GetActive();
		if(!activeObject){
			return;
		}
		
		const decString activeProperty(activeObject->GetActiveProperty());
		if(activeProperty.IsEmpty()){
			return;
		}
		
		if(!activeObject->IsPropertyShape(activeProperty)
		&& !activeObject->IsPropertyShapeList(activeProperty)){
			return;
		}
		
		const meObjectShapeSelection &selection = world.GetSelectionObjectShape();
		if(selection.GetSelected().IsNotEmpty()){
			pUndoScale = meUObjectShapeScale::Ref::New(activeObject, activeProperty, selection.GetSelected());
		}
		
	}else if(elementMode == meWorldGuiParameters::eemDecal){
		const meDecalSelection &selection = world.GetSelectionDecal();
		
		if(selection.GetSelected().IsNotEmpty()){
			pUndoScale = meUDecalScale::Ref::New(&world);
		}
	}
	
	if(pUndoScale){
		pUndoScale->SetModifyPosition(false);
	}
}

void meViewEditorScale::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditor::OnLeftMouseButtonRelease(x, y, shift, control);
	
	if(pUndoScale){
		if(!pUndoScale->GetFactors().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
			GetWorld().GetUndoSystem()->Add(pUndoScale, false);
		}
		pUndoScale = nullptr;
	}
}

void meViewEditorScale::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	
	if(pUndoScale){
		const meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		const meConfiguration &configuration = GetConfiguration();
		const float sensitivity = configuration.GetSensitivity();
		const int halfHeight = GetViewHeight() >> 1;
		const int halfWidth = GetViewWidth() >> 1;
		const decPoint &dragOrigin = GetDragOrigin();
		const decPoint &dragCurrent = GetDragCurrent();
		const decPoint &dragDist = GetDragDistanceStart();
		const float dx1 = (float)(dragOrigin.x - halfWidth);
		const float dy1 = (float)(dragOrigin.y - halfHeight);
		const float dx2 = (float)(dragCurrent.x - halfWidth);
		const float dy2 = (float)(dragCurrent.y - halfHeight);
		const decDMatrix &matrixView = GetMatrixView();
		float uniformFactor = 1.0f;
		decDVector vector;
		
		// determine new scaling
		/*
		float fx = 1.0f, fy = 1.0f;
		
		if(dx1 != 0.0f){
			fx = dx2 / dx1;
		}
		if(dy1 != 0.0f){
			fy = dy2 / dy1;
		}
		*/
		
		// determine new scaling (camera-relative but applied to world axes)
		// extract camera view direction to determine which world axes to scale
		const decDVector viewRight = matrixView.TransformRight();
		const decDVector viewUp = matrixView.TransformUp();
		
		// apply drag distance to world axes based on camera orientation
		// dragDist.x: right is positive (increase scale)
		// dragDist.y: down is positive, but we want up to increase scale, so negate
		const double scaleX = ME_DRAG_SCALE * (double)dragDist.x * (double)sensitivity;
		const double scaleY = ME_DRAG_SCALE * (double)dragDist.y * (double)sensitivity;
		
		// map camera movement to world axes using absolute values
		vector.x = (fabs(viewRight.x) * scaleX + fabs(viewUp.x) * scaleY);
		vector.y = (fabs(viewRight.y) * scaleX + fabs(viewUp.y) * scaleY);
		vector.z = (fabs(viewRight.z) * scaleX + fabs(viewUp.z) * scaleY);
		
		if(guiparams.GetLockAxisX()){
			vector.x = 0.0;
		}
		if(guiparams.GetLockAxisY()){
			vector.y = 0.0;
		}
		if(guiparams.GetLockAxisZ()){
			vector.z = 0.0;
		}
		if(configuration.GetScaleSnap() != shift){
			vector.Snap((double)configuration.GetScaleStep());
		}
		
		const float oldDistance = sqrtf(dx1 * dx1 + dy1 * dy1);
		if(oldDistance > 0.0f){
			uniformFactor = sqrtf(dx2 * dx2 + dy2 * dy2) / oldDistance;
		}
		
		if(configuration.GetScaleSnap() != shift){
			const float grid = configuration.GetScaleStep();
			const float halfGrid = grid * 0.5f;
			const float integral = grid * (float)floorf(uniformFactor / grid);
			
			if(uniformFactor - integral < halfGrid){
				uniformFactor = integral;
				
			}else{
				uniformFactor = integral + grid;
			}
		}
		
		pUndoScale->SetFactors(decVector(1.0f, 1.0f, 1.0f) + vector);
		pUndoScale->SetUniformFactor(uniformFactor);
		pUndoScale->SetScaleUniform(control);
		pUndoScale->ProgressiveRedo();
	}
}
