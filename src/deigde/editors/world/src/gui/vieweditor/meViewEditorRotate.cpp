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

#include "meViewEditorRotate.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../undosys/gui/object/meURotateObject.h"
#include "../../undosys/gui/decal/meUDecalRotate.h"
#include "../../undosys/gui/objectshape/meUObjectShapeRotate.h"
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



// Class meViewEditorRotate
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorRotate::meViewEditorRotate(meView3D &view) : meViewEditorNavigation(view){
}

meViewEditorRotate::~meViewEditorRotate(){
}



// Management
///////////////

void meViewEditorRotate::CalculateRotationAxis(decDVector &axis) const{
	meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	
	if(guiparams.GetUseLocal()){
		axis = GetMatrixViewLocalInverse() * GetMatrixView().TransformView();
		
	}else{
		axis = GetMatrixView().TransformView();
	}
	
	if(guiparams.GetLockAxisX()){
		axis.x = 0.0;
	}
	if(guiparams.GetLockAxisY()){
		axis.y = 0.0;
	}
	if(guiparams.GetLockAxisZ()){
		axis.z = 0.0;
	}
	
	if(guiparams.GetUseLocal()){
		axis = GetMatrixViewLocal() * axis;
	}
	
	if(axis.Length() > 1e-5){
		axis.Normalize();
		
	}else{
		axis.Set(0.0, 0.0, 1.0);
	}
}



// Callbacks
//////////////

bool meViewEditorRotate::OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control){
	if(pUndoRotate){
		return CheckAxisLocking(key);
		
	}else{
		return meViewEditorNavigation::OnKeyPress(key, shift, control);
	}
}

void meViewEditorRotate::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	decDVector center, axis;
	int i, count;
	
	pUndoRotate = NULL;
	
	CalculateRotationAxis(axis);
	if(axis.Length() < 1e-5){
		return;
	}
	
	axis.Normalize();
	center.SetZero();
	
	if(elementMode == meWorldGuiParameters::eemObject){
		const meObjectSelection &selection = world.GetSelectionObject();
		const meObjectList &listSelected = selection.GetSelected();
		
		if(listSelected.GetCount() > 0){
			meObjectList list;
			meObject *object;
			
			GetSelectedObjectsWithAttached(list);
			
			bool modifyPosition = list.GetCount() > 0;
			
			switch(GetWorldGuiParameters().GetRotationPivotCenter()){
			case meWorldGuiParameters::erpcActive:
				center = selection.GetActive()->GetPosition();
				break;
				
			case meWorldGuiParameters::erpcSelected:
				count = listSelected.GetCount();
				for(i=0; i<count; i++){
					object = listSelected.GetAt(i);
					center += object->GetPosition();
				}
				center /= (double)listSelected.GetCount();
				break;
				
			case meWorldGuiParameters::erpcIndividual:
				center = selection.GetActive()->GetPosition();
				modifyPosition = false;
				break;
			}
			
			pUndoRotate.TakeOver(new meURotateObject(&world, list));
			((meURotateObject&)(igdeUndo&)pUndoRotate).SetModifyPosition(modifyPosition);
		}
		
	}else if(elementMode == meWorldGuiParameters::eemObjectShape){
		const meObjectShapeSelection &selection = world.GetSelectionObjectShape();
		meObject *activeObject = world.GetSelectionObject().GetActive();
		decString activeProperty;
		
		if(activeObject){
			activeProperty = activeObject->GetActiveProperty();
			
			if(activeObject->IsPropertyShape(activeProperty.GetString())){
				if(world.GetObjectShapes().GetCount() != 1){
					activeProperty.Empty();
				}
				
			}else if(!activeObject->IsPropertyShapeList(activeProperty.GetString())){
				activeProperty.Empty();
			}
			
			if(selection.GetSelected().GetCount() == 0){
				activeProperty.Empty();
			}
		}
		
		if(!activeProperty.IsEmpty()){
			const decDMatrix matrixParent = decDMatrix::CreateRT(decDVector(activeObject->GetRotation() * DEG2RAD), activeObject->GetPosition());
			const meObjectShapeList &listSelected = selection.GetSelected();
			decDMatrix matrixShape;
			
			bool modifyPosition = listSelected.GetCount() > 0;
			
			switch(GetWorldGuiParameters().GetRotationPivotCenter()){
			case meWorldGuiParameters::erpcActive:
				center = matrixParent * decDVector(selection.GetActive()->GetShape()->GetPosition());
				break;
				
			case meWorldGuiParameters::erpcSelected:
				count = listSelected.GetCount();
				for(i=0; i<count; i++){
					center += matrixParent * decDVector(listSelected.GetAt(i)->GetShape()->GetPosition());
				}
				center /= (double)listSelected.GetCount();
				break;
				
			case meWorldGuiParameters::erpcIndividual:
				center = matrixParent * decDVector(selection.GetActive()->GetShape()->GetPosition());
				modifyPosition = false;
				break;
			}
			
			pUndoRotate.TakeOver(new meUObjectShapeRotate(activeObject, activeProperty, listSelected));
			((meURotateObject&)(igdeUndo&)pUndoRotate).SetModifyPosition(modifyPosition);
		}
		
	}else if(elementMode == meWorldGuiParameters::eemDecal){
		const meDecalSelection &selection = world.GetSelectionDecal();
		const meDecalList &listSelected = selection.GetSelected();
		
		if(listSelected.GetCount() > 0){
			meDecal *decal;
			
			count = listSelected.GetCount();
			
			for(i=0; i<count; i++){
				decal = listSelected.GetAt(i);
				center += decal->GetPosition();
			}
			
			center /= (double)listSelected.GetCount();
			
			pUndoRotate.TakeOver(new meUDecalRotate(&world));
			((meURotateObject&)(igdeUndo&)pUndoRotate).SetModifyPosition(listSelected.GetCount() > 1);
		}
	}
	
	if(pUndoRotate){
		const decPoint screenPoint = decPoint(GetViewWidth() / 2, GetViewHeight() / 2);
		const decPoint screenDirection = GetDragOrigin() - screenPoint;
		
		pNullAngle = atan2f((float)-screenDirection.y, (float)screenDirection.x);
		
		((meURotateObject&)(igdeUndo&)pUndoRotate).SetPivot(center);
		((meURotateObject&)(igdeUndo&)pUndoRotate).SetAxis(axis);
	}
}

void meViewEditorRotate::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditor::OnLeftMouseButtonRelease(x, y, shift, control);
	
	if(pUndoRotate){
		if(fabs(((meURotateObject&)(igdeUndo&)pUndoRotate).GetAngle()) > 1e-5){
			GetWorld().GetUndoSystem()->Add(pUndoRotate, false);
		}
		pUndoRotate = NULL;
	}
}

void meViewEditorRotate::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	
	if(!pUndoRotate){
		return;
	}
	
	const decPoint screenPoint = decPoint(GetViewWidth() / 2, GetViewHeight() / 2);
	const decPoint screenDirection = GetDragCurrent() - screenPoint;
	float angle = (atan2f((float)-screenDirection.y, (float)screenDirection.x) - pNullAngle) * RAD2DEG;
	const meConfiguration &configuration = GetConfiguration();
	decDVector axis;
	
	if(configuration.GetRotateSnap() != shift){
		const float grid = configuration.GetRotateStep();
		angle = grid * ceilf(angle / grid - 0.5f);
	}
	
	// udpate the axis. this is required since due to changes to the locking the rotation axis can change
	CalculateRotationAxis(axis);
	
	meURotateObject &undo = (meURotateObject&)(igdeUndo&)pUndoRotate;
	undo.SetAxis(axis);
	undo.SetAngle(angle);
	undo.ProgressiveRedo();
}
