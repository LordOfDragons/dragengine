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

#include "meViewEditorNavigation.h"
#include "../meView3D.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/object/meObject.h"
#include "../../undosys/gui/camera/meUCameraMoveObject.h"

#include <deigde/engine/igdeEngineController.h>
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

meViewEditorNavigation::meViewEditorNavigation(meView3D &view) :
meViewEditor(view),
pNavigating(false){
}

meViewEditorNavigation::~meViewEditorNavigation() = default;


// Callbacks
//////////////

void meViewEditorNavigation::OnCameraInteractionBegin(){
	pUndoCameraMove.Clear();
	
	meWorld &world = GetWorld();
	
	const auto &camera = world.GetActiveCamera();
	if(camera && camera->HasHostObject()){
		pUndoCameraMove = meUCameraMoveObject::Ref::New(camera->GetHostObject());
	}
	
	pNavigating = true;
}

void meViewEditorNavigation::OnCameraInteractionUpdate(){
	if(!pNavigating){
		return;
	}
	
	meWorld &world = GetWorld();
	
	const auto &camera = world.GetActiveCamera();
	if(!camera){
		return;
	}
	
	const auto &iacamera = GetView().GetCameraInteractionCamera();
	camera->GetMPDistance() = iacamera->GetMPDistance();
	
	if(camera->HasHostObject()){
		if(pUndoCameraMove){
			auto matrix = camera->GetInvHostMatrix().QuickMultiply(
				decDMatrix::CreateWorld(iacamera->GetMPPosition(), iacamera->GetMPRotation()));
			
			auto undo = pUndoCameraMove.DynamicCast<meUCameraMoveObject>();
			undo->SetNewPosition(matrix.GetPosition());
			undo->SetNewRotation(matrix.GetEulerAngles() * RAD2DEG);
			undo->Redo();
		}
		
	}else{
		camera->GetMPPosition() = iacamera->GetMPPosition();
		camera->GetMPRotation() = iacamera->GetMPRotation();
		world.NotifyCameraChanged(camera);
	}
}

void meViewEditorNavigation::OnCameraInteractionEnd(bool cancelled){
	if(pUndoCameraMove){
		if(pUndoCameraMove.DynamicCast<meUCameraMoveObject>()->HasChanged()){
			if(cancelled){
				pUndoCameraMove->Undo();
				
			}else{
				GetWorld().GetUndoSystem()->Add(pUndoCameraMove, false);
			}
		}
		pUndoCameraMove.Clear();
	}
	pNavigating = false;
}
