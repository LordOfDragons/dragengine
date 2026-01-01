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

#include "meViewEditorNavigation.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/object/meObject.h"
#include "../../undosys/gui/camera/meUCameraMoveObject.h"
#include "../../undosys/gui/camera/meUCameraRotateObject.h"

#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define ME_DRAG_MOVE	(1.0f / 50.0f)
#define ME_DRAG_TURN	(1.0f / 5.0f)



// Class meViewEditorNavigation
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorNavigation::meViewEditorNavigation(meView3D &view) : meViewEditor(view){
	pNavigating = false;
}

meViewEditorNavigation::~meViewEditorNavigation(){
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorNavigation::OnRightMouseButtonPress(int x, int y, bool shift, bool control){
	meViewEditor::OnRightMouseButtonPress(x, y, shift, control);
	
	meWorld &world = GetWorld();
	meCamera * const activeCamera = world.GetActiveCamera();
	
	pUndoCameraMove = nullptr;
	pUndoCameraRotate = nullptr;
	
	// distance
	if(shift && control){
		pOldDist = activeCamera->GetDistance();
		pNavigating = true;
		
		if(activeCamera->HasHostObject()){
			pUndoCameraMove = meUCameraMoveObject::Ref::New(activeCamera->GetHostObject());
		}
		
	// move
	}else if(shift || control){
		pNavigating = true;
		
		if(activeCamera->HasHostObject()){
			pUndoCameraMove = meUCameraMoveObject::Ref::New(activeCamera->GetHostObject());
		}
		
	// rotate
	}else{
		pOldRotation = activeCamera->GetOrientation();
		pOldOrientation.SetFromEuler(pOldRotation * DEG2RAD);
		pNavigating = true;
		
		if(activeCamera->HasHostObject()){
			pUndoCameraRotate = meUCameraRotateObject::Ref::New(activeCamera->GetHostObject());
		}
	}
}

void meViewEditorNavigation::OnRightMouseButtonRelease(int x, int y, bool shift, bool control){
	meViewEditor::OnRightMouseButtonRelease(x, y, shift, control);
	
	if(pUndoCameraMove){
		if(((meUCameraMoveObject&)(igdeUndo&)pUndoCameraMove).HasChanged()){
			GetWorld().GetUndoSystem()->Add(pUndoCameraMove, false);
		}
		pUndoCameraMove = nullptr;
	}
	
	if(pUndoCameraRotate){
		if(((meUCameraRotateObject&)(igdeUndo&)pUndoCameraRotate).HasChanged()){
			GetWorld().GetUndoSystem()->Add(pUndoCameraRotate, false);
		}
		pUndoCameraRotate = nullptr;
	}
	
	if(pNavigating){
		pNavigating = false;
	}
}

void meViewEditorNavigation::OnMouseMove(int x, int y, bool shift, bool control){
	meViewEditor::OnMouseMove(x, y, shift, control);
	
	meWorld &world = GetWorld();
	
	if(GetDragRightMouseButton()){
		const float sensitivity = GetConfiguration().GetSensitivity();
		const decPoint &dragDistStart = GetDragDistanceStart();
		const decPoint &dragDistLast = GetDragDistanceLast();
		const decDMatrix &matrixView = GetMatrixView();
		const bool controlStart = GetControlStart();
		const bool shiftStart = GetShiftStart();
		meCamera *camera;
		
		// distance
		if(shiftStart && controlStart){
			const float newDistance = decMath::max(0.0f,
				pOldDist - ME_DRAG_MOVE * (float)dragDistStart.y * sensitivity);
			
			camera = world.GetFreeRoamingCamera();
			camera->SetDistance(newDistance);
			
			world.NotifyCameraChanged(camera);
			
		// move left-right
		}else if(shiftStart){
			camera = world.GetActiveCamera();
			
			if(camera->HasHostObject()){
				if(pUndoCameraMove){
					decDVector position(camera->GetPosition()
						- matrixView.TransformRight() * (ME_DRAG_MOVE * (float)dragDistLast.x * sensitivity)
						- matrixView.TransformUp() * (ME_DRAG_MOVE * (float)dragDistLast.y) * sensitivity);
					position = camera->GetInvHostMatrix().
						QuickMultiply(decDMatrix::CreateRT(camera->GetOrientation() * DEG2RAD, position)).
						GetPosition();
					
					((meUCameraMoveObject&)(igdeUndo&)pUndoCameraMove).SetNewPosition(position);
					pUndoCameraMove->Redo();
				}
				
			}else{
				camera->SetPosition(camera->GetPosition()
					- matrixView.TransformRight() * (ME_DRAG_MOVE * (float)dragDistLast.x * sensitivity)
					- matrixView.TransformUp() * (ME_DRAG_MOVE * (float)dragDistLast.y) * sensitivity);
			}
			
			world.NotifyCameraChanged(camera);
			
		// move forward-backward
		}else if(controlStart){
			camera = world.GetActiveCamera();
			
			if(camera->HasHostObject()){
				if(pUndoCameraMove){
					decDVector position(camera->GetPosition()
						+ matrixView.TransformView() * (ME_DRAG_MOVE * (float)dragDistLast.y * sensitivity));
					position = camera->GetInvHostMatrix().
						QuickMultiply(decDMatrix::CreateRT(camera->GetOrientation() * DEG2RAD, position)).
						GetPosition();
					
					((meUCameraMoveObject&)(igdeUndo&)pUndoCameraMove).SetNewPosition(position);
					pUndoCameraMove->Redo();
				}
				
			}else{
				camera->SetPosition(camera->GetPosition()
					+ matrixView.TransformView() * (ME_DRAG_MOVE * (float)dragDistLast.y * sensitivity));
			}
			
			world.NotifyCameraChanged(camera);
			
		// rotate
		}else{
			camera = world.GetActiveCamera();
			
			if(camera->HasHostObject()){
				decQuaternion rotation;
					
				// which one of the next two lines to use depends on the use case.
				//
				// using this version the camera rotation works similar to the world camera
				// rotation handling. this will reset any tilting if present.
				rotation.SetFromEuler(
					decMath::normalize(pOldRotation.x - ME_DRAG_TURN * (float)dragDistStart.y, 0.0f, 360.0f) * DEG2RAD,
					decMath::normalize(pOldRotation.y - ME_DRAG_TURN * (float)dragDistStart.x, 0.0f, 360.0f) * DEG2RAD,
					0.0f);
				
				// using this version the camera rotation works similar to the world camera
				// rotation handling with the exception that the tilt (z axis rotation) is kept.
				// this is similar to the line above but the tilt is retained which might be
				// desireable in certain situations
				/*
				rotation.SetFromEuler(-ME_DRAG_TURN * (float)dragDistStart.y * DEG2RAD,
					-ME_DRAG_TURN * (float)dragDistStart.x * DEG2RAD, 0.0f);
				rotation = pOldOrientation * rotation;
				*/
				
				// using this version the camera rotation works more like in-game meaning the
				// rotation is relative to the current camera orientation
				/*
				rotation.SetFromEuler(-ME_DRAG_TURN * (float)dragDistStart.y * DEG2RAD,
					-ME_DRAG_TURN * (float)dragDistStart.x * DEG2RAD, 0.0f);
				rotation *= pOldOrientation;
				*/
				
				if(pUndoCameraRotate){
					rotation = camera->GetInvHostMatrix().
						QuickMultiply(decDMatrix::CreateWorld(camera->GetPosition(), rotation)).
						ToQuaternion();
					
					((meUCameraRotateObject&)(igdeUndo&)pUndoCameraRotate).SetNewRotation(
						rotation.GetEulerAngles() * RAD2DEG);
					pUndoCameraRotate->Redo();
				}
				
			}else{
				const decVector rotation(
					decMath::normalize(pOldRotation.x - ME_DRAG_TURN * (float)dragDistStart.y, 0.0f, 360.0f),
					decMath::normalize(pOldRotation.y - ME_DRAG_TURN * (float)dragDistStart.x, 0.0f, 360.0f),
					0.0f);
				
				camera->SetOrientation(rotation);
				world.NotifyCameraChanged(camera);
			}
		}
	}
}

void meViewEditorNavigation::OnMouseWheel(int steps, bool shift, bool control){
	meViewEditor::OnMouseWheel(steps, shift, control);
}
