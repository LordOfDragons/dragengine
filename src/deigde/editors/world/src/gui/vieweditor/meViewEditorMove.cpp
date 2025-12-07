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

#include "meViewEditorMove.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../collisions/meCLSnapPoint.h"
#include "../../configuration/meConfiguration.h"
#include "../../undosys/gui/decal/meUDecalMove.h"
#include "../../undosys/gui/object/meUMoveObject.h"
#include "../../undosys/gui/objectshape/meUObjectShapeMove.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"

#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/gamedefinition/class/snappoint/igdeGDCSnapPoint.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>



// Definitions
////////////////

#define ME_DRAG_MOVE	(1.0 / 50.0)



// Class meViewEditorMove
///////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorMove::meViewEditorMove(meView3D &view) :
meViewEditorNavigation(view),
pCLSnapPoint(NULL)
{
	pCLCollider.TakeOver(view.GetEngine()->GetColliderManager()->CreateColliderVolume());
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmEditing);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmSnapPoint);
	
	pCLCollider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
}

meViewEditorMove::~meViewEditorMove(){
	if(pCLSnapPoint){
		delete pCLSnapPoint;
	}
}



// Management
///////////////



// Callbacks
//////////////

bool meViewEditorMove::OnKeyPress(deInputEvent::eKeyCodes key, bool shift, bool control){
	if(pUndoMove){
		return CheckAxisLocking(key);
		
	}else{
		return meViewEditorNavigation::OnKeyPress(key, shift, control);
	}
}

void meViewEditorMove::OnLeftMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnLeftMouseButtonPress(x, y, shift, control);
	
	const int elementMode = GetElementMode();
	meWorld &world = GetWorld();
	
	if(pCLSnapPoint){
		delete pCLSnapPoint;
		pCLSnapPoint = NULL;
	}
	pUndoMove = NULL;
	
	if(elementMode == meWorldGuiParameters::eemObject){
		const meObjectSelection &selection = world.GetSelectionObject();
		if(selection.GetSelected().GetCount() == 0){
			return;
		}
		
		meObjectList list;
		GetSelectedObjectsWithAttached(list);
		pUndoMove.TakeOver(new meUMoveObject(&world, list));
		
		// create snap point visitor
		meObject * const refObject = selection.GetActive();
		
		pCLSnapPoint = new meCLSnapPoint(world, refObject);
		const int count = list.GetCount();
		int i;
		for(i=0; i<count; i++){
			pCLSnapPoint->AddIgnoreObject(list.GetAt(i));
		}
		
		decShapeList shapeList;
		pCLSnapPoint->CalcBoundingBoxShape(shapeList);
		((deColliderVolume&)(deCollider&)pCLCollider).SetShapes(shapeList);
		
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
			pUndoMove.TakeOver(new meUObjectShapeMove(activeObject, activeProperty, selection.GetSelected()));
		}
		
	}else if(elementMode == meWorldGuiParameters::eemDecal){
		const meDecalSelection &selection = world.GetSelectionDecal();
		
		if(selection.GetSelected().GetCount() > 0){
			pUndoMove.TakeOver(new meUDecalMove(&world));
		}
		
	}else if(elementMode == meWorldGuiParameters::eemNavSpace){
		// TODO
	}
}

void meViewEditorMove::OnLeftMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditor::OnLeftMouseButtonRelease(x, y, shift, control);
	
	if(pUndoMove){
		if(!((meBaseUndoMove&)(igdeUndo&)pUndoMove).GetDistance().IsZero()){
			GetWorld().GetUndoSystem()->Add(pUndoMove, false);
		}
		pUndoMove = NULL;
	}
	
	if(pCLSnapPoint){
		delete pCLSnapPoint;
		pCLSnapPoint = NULL;
	}
}

void meViewEditorMove::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditorNavigation::OnMouseMove(x, y, shift, control);
	
	if(!pUndoMove){
		return;
	}
	
	const meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	const meConfiguration &configuration = GetConfiguration();
	const float sensitivity = configuration.GetSensitivity();
	const decPoint &dragDist = GetDragDistanceStart();
	const decDMatrix &matrixView = GetMatrixView();
	decDVector vector;
	
	vector = matrixView.TransformRight() * (ME_DRAG_MOVE * (double)dragDist.x * (double)sensitivity);
	vector += matrixView.TransformUp() * (ME_DRAG_MOVE * (double)dragDist.y * (double)sensitivity);
	
	if(guiparams.GetUseLocal()){
		vector = GetMatrixViewLocalInverse() * vector;
		
		if(guiparams.GetLockAxisX()){
			vector.x = 0.0;
		}
		if(guiparams.GetLockAxisY()){
			vector.y = 0.0;
		}
		if(guiparams.GetLockAxisZ()){
			vector.z = 0.0;
		}
		if(configuration.GetMoveSnap() != shift){
			vector.Snap((double)configuration.GetMoveStep());
		}
		
		vector = GetMatrixViewLocal() * vector;
		
	}else{
		if(guiparams.GetLockAxisX()){
			vector.x = 0.0;
		}
		if(guiparams.GetLockAxisY()){
			vector.y = 0.0;
		}
		if(guiparams.GetLockAxisZ()){
			vector.z = 0.0;
		}
		if(configuration.GetMoveSnap() != shift){
			vector.Snap((double)configuration.GetMoveStep());
		}
	}
	
	meBaseUndoMove &undo = (meBaseUndoMove&)(igdeUndo&)pUndoMove;
	undo.SetModifyOrientation(false);
	undo.SetDistance(vector);
	undo.ProgressiveRedo();
	
	// snapping
	if(pCLSnapPoint && pCLSnapPoint->IsSnappingEnabled()){
		pCLCollider->SetPosition(pCLSnapPoint->GetObject()->GetPosition());
		pCLCollider->SetOrientation(decQuaternion::CreateFromEuler(
			pCLSnapPoint->GetObject()->GetRotation() * DEG2RAD));
		
		pCLSnapPoint->Reset();
		ColliderTestCollision(pCLSnapPoint, pCLCollider);
		
		if(pCLSnapPoint->GetTargetSnapPoint()){
			if(pCLSnapPoint->GetTargetSnapPoint()->GetSnapToRotation()){
				undo.SetModifyOrientation(true);
				undo.SetMatrix(decDMatrix::CreateTranslation(vector)
					* pCLSnapPoint->CalcCorrectionMatrix() );
				
			}else{
				undo.SetDistance(vector + pCLSnapPoint->CalcCorrectionMatrix().GetPosition());
			}
			
			undo.ProgressiveRedo();
		}
	}
}
