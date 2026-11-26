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

#include "igdeCameraInteractionListener.h"
#include "../igdeCamera.h"
#include "../igdeViewRenderWindow.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"


// Class igdeCameraInteractionListener::MouseDragListener
///////////////////////////////////////////////////////////

igdeCameraInteractionListener::MouseDragListener::MouseDragListener(
	igdeCameraInteractionListener *aowner) :
owner(aowner){
}

igdeCameraInteractionListener::MouseDragListener::~MouseDragListener() = default;

bool igdeCameraInteractionListener::MouseDragListener::OnDragBegin(){
	return owner ? owner->OnDragBegin() : false;
}

void igdeCameraInteractionListener::MouseDragListener::OnDragUpdate(){
	if(owner){
		owner->OnDragUpdate();
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnDragFinish(bool cancelled){
	if(owner){
		owner->OnDragFinish(cancelled);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnButtonPress(
	igdeWidget *widget, int button, const decPoint &position, int modifiers){
	if(owner){
		owner->OnButtonPress(widget, button, position, modifiers);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnMouseMoved(
	igdeWidget *widget, const decPoint &position, int modifiers){
	if(owner){
		owner->OnMouseMoved(widget, position, modifiers);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnButtonRelease(
igdeWidget *widget, int button, const decPoint &position, int modifiers){
	if(owner){
		owner->OnButtonRelease(widget, button, position, modifiers);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnMouseWheeled(
igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers){
	if(owner){
		owner->OnMouseWheeled(widget, position, change, modifiers);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnKeyPress(
	igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key){
	if(owner){
		owner->OnKeyPress(widget, keyCode, key);
	}
}

void igdeCameraInteractionListener::MouseDragListener::OnKeyRelease(
	igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key){
	if(owner){
		owner->OnKeyRelease(widget, keyCode, key);
	}
}


// Class igdeCameraInteractionListener::EnvironmentListener
/////////////////////////////////////////////////////////////

igdeCameraInteractionListener::EnvironmentListener::EnvironmentListener(
	igdeCameraInteractionListener *aowner) :
owner(aowner){
}

igdeCameraInteractionListener::EnvironmentListener::~EnvironmentListener() = default;

void igdeCameraInteractionListener::EnvironmentListener::OnFrameUpdate(float elapsed){
	if(owner){
		owner->OnFrameUpdate(elapsed);
	}
}


// Class igdeCameraInteractionListener
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeCameraInteractionListener::igdeCameraInteractionListener(igdeEnvironment &environment) :
pEnvironment(environment),
pEnableRotate(true),
pEnablePan(true),
pEnableMove(true),
pEnableZoom(true),
pSpeedRotate(0.2f),
pSpeedPan(1.0f / 50.0f),
pSpeedMove(1.0f / 50.0f),
pSpeedZoom(0.5f / 50.0f),
pSpeedFly(3.0f),
pFlySpeedModifier(0),
pInteraction(eiNone),
pZoomOrigin(0.0f),
pZoomCurrent(0.0f),
pFlyForward(false),
pFlyBackward(false),
pFlyLeft(false),
pFlyRight(false),
pFlyUp(false),
pFlyDown(false),
pRightMouseButtonPressed(false),
pMouseDragListener(deTObjectReference<MouseDragListener>::New(this)),
pEnvironmentListener(deTObjectReference<EnvironmentListener>::New(this))
{
	pFlyVelocity.SetAdjustRange(pSpeedFly);
	pFlyVelocity.SetAdjustTime(0.5f);
}

igdeCameraInteractionListener::igdeCameraInteractionListener(
	igdeEnvironment &environment, igdeCamera *camera) :
igdeCameraInteractionListener(environment)
{
	pCamera = camera;
}

igdeCameraInteractionListener::~igdeCameraInteractionListener(){
	if(pMouseDragListener){
		pMouseDragListener->owner = nullptr;
	}
	if(pEnvironmentListener){
		pEnvironmentListener->owner = nullptr;
	}
}


// Management
///////////////

void igdeCameraInteractionListener::SetCamera(igdeCamera *camera){
	if(camera == pCamera){
		return;
	}
	
	Cancel();
	pCamera = camera;
}

void igdeCameraInteractionListener::SetEnableRotate(bool enable){
	pEnableRotate = enable;
}

void igdeCameraInteractionListener::SetEnablePan(bool enable){
	pEnablePan = enable;
}

void igdeCameraInteractionListener::SetEnableMove(bool enable){
	pEnableMove = enable;
}

void igdeCameraInteractionListener::SetEnableZoom(bool enable){
	pEnableZoom = enable;
}

void igdeCameraInteractionListener::SetEnabledAll(bool enable){
	pEnableRotate = enable;
	pEnablePan = enable;
	pEnableMove = enable;
	pEnableZoom = enable;
}


void igdeCameraInteractionListener::SetSpeedRotate(float degreesPerPixel){
	pSpeedRotate = decMath::max(degreesPerPixel, 0.0f);
}

void igdeCameraInteractionListener::SetSpeedPan(float metersPerPixel){
	pSpeedPan = decMath::max(metersPerPixel, 0.0f);
}

void igdeCameraInteractionListener::SetSpeedMove(float metersPerPixel){
	pSpeedMove = decMath::max(metersPerPixel, 0.0f);
}

void igdeCameraInteractionListener::SetSpeedZoom(float metersPerPixel){
	pSpeedZoom = decMath::max(metersPerPixel, 0.0f);
}

void igdeCameraInteractionListener::SetSpeedFly(float metersPerSecond){
	pSpeedFly = decMath::max(metersPerSecond, 0.0f);
	pFlyVelocity.SetAdjustRange(GetSpeedFlyModified());
}

void igdeCameraInteractionListener::SetSpeedFlyModifier(int multiplier){
	multiplier = decMath::clamp(multiplier, -20, 25);
	if(multiplier == pFlySpeedModifier){
		return;
	}
	
	pFlySpeedModifier = multiplier;
	pFlyVelocity.SetAdjustRange(GetSpeedFlyModified());
	
	pViewRenderWindow->GetToastBubble()->ShowToast(decString::Formatted(
		pEnvironment.GetTranslationManager().Translate("Igde.ViewRenderWindow.Toast.FlyModeSpeed").ToUTF8(),
		pFlySpeedModifier));
}

float igdeCameraInteractionListener::GetSpeedFlyModified() const{
	return pSpeedFly * powf(1.25f, (float)pFlySpeedModifier);
}


void igdeCameraInteractionListener::SetInteraction(eInteraction interaction){
	pInteraction = interaction;
}


void igdeCameraInteractionListener::SetMoveOrigin(const decDVector &position){
	pMoveOrigin = position;
}

void igdeCameraInteractionListener::SetMoveCurrent(const decDVector &position){
	pMoveCurrent = position;
}

void igdeCameraInteractionListener::SetRotateOrigin(const decVector &rotation){
	pRotateOrigin = rotation;
}

void igdeCameraInteractionListener::SetRotationCurrent(const decVector &rotation){
	pRotateCurrent = rotation;
}

void igdeCameraInteractionListener::SetZoomOrigin(float zoom){
	pZoomOrigin = decMath::max(zoom, 0.0f);
}

void igdeCameraInteractionListener::SetZoomCurrent(float zoom){
	pZoomCurrent = decMath::max(zoom, 0.0f);
}

void igdeCameraInteractionListener::SetMoveMatrix(const decDMatrix &matrix){
	pMoveMatrix = matrix;
}


void igdeCameraInteractionListener::AddListeners(igdeViewRenderWindow &widget){
	DEASSERT_NULL(pViewRenderWindow);
	
	pViewRenderWindow = &widget;
	
	widget.AddListener(pMouseDragListener);
	pEnvironment.AddListener(pEnvironmentListener);
}

void igdeCameraInteractionListener::RemoveListeners(igdeViewRenderWindow &widget){
	DEASSERT_NOTNULL(pViewRenderWindow);
	
	pEnvironment.RemoveListener(pEnvironmentListener);
	widget.RemoveListener(pMouseDragListener);
	
	pViewRenderWindow.Clear();
}


// Interaction events
///////////////////////

igdeCameraInteractionListener::eInteraction igdeCameraInteractionListener::ChooseInteraction(){
	if(pEnvironment.Get3DViewFlyMode()){
		if(pRightMouseButtonPressed){
			return eiFly;
		}
		
	}else{
		if(GetDragState() != edsRight){
			return eiNone;
		}
		
		if(GetShiftOrigin() && GetControlOrigin()){
			return eiZoom;
			
		}else if(GetShiftOrigin()){
			return eiPan;
			
		}else if(GetControlOrigin()){
			return eiMove;
			
		}else{
			return eiRotate;
		}
	}
	
	return eiNone;
}


bool igdeCameraInteractionListener::OnRotateBegin(){
	if(!pCamera){
		return false;
	}
	
	pRotateOrigin = pCamera->GetOrientation();
	pRotateCurrent = pRotateOrigin;
	OnBeginInteraction();
	return true;
}

void igdeCameraInteractionListener::OnRotateUpdate(){
	if(!pCamera){
		return;
	}
	
	const decPoint distance(GetDragDistance());
	
	pRotateCurrent.y = decMath::normalize(
		pRotateOrigin.y - pSpeedRotate * (float)distance.x, 360.0f);
	pRotateCurrent.x = decMath::normalize(
		pRotateOrigin.x + pSpeedRotate * (float)distance.y, 360.0f);
	
	pCamera->SetOrientation(pRotateCurrent);
	
	OnCameraChanged();
}

void igdeCameraInteractionListener::OnRotateFinish(bool cancelled){
	OnEndInteraction(cancelled);
}


bool igdeCameraInteractionListener::OnPanBegin(){
	if(!pCamera){
		return false;
	}
	
	pMoveOrigin = pCamera->GetPosition();
	pMoveCurrent = pMoveOrigin;
	pMoveMatrix = pCamera->GetViewMatrix();
	OnBeginInteraction();
	return true;
}

void igdeCameraInteractionListener::OnPanUpdate(){
	if(!pCamera){
		return;
	}
	
	const decPoint distance(GetDragDistance());
	
	pMoveCurrent = pMoveOrigin
		- pMoveMatrix.TransformRight() * (double)(pSpeedPan * (float)distance.x)
		+ pMoveMatrix.TransformUp() * (double)(pSpeedPan * (float)distance.y);
	pCamera->SetPosition(pMoveCurrent);
	
	OnCameraChanged();
}

void igdeCameraInteractionListener::OnPanFinish(bool cancelled){
	OnEndInteraction(cancelled);
}


bool igdeCameraInteractionListener::OnMoveBegin(){
	if(!pCamera){
		return false;
	}
	
	pMoveOrigin = pCamera->GetPosition();
	pMoveCurrent = pMoveOrigin;
	pMoveMatrix = pCamera->GetViewMatrix();
	OnBeginInteraction();
	return true;
}

void igdeCameraInteractionListener::OnMoveUpdate(){
	if(!pCamera){
		return;
	}
	
	const decPoint distance(GetDragDistance());
	
	pMoveCurrent = pMoveOrigin
		- pMoveMatrix.TransformView() * (double)(pSpeedMove * (float)distance.y);
	
	pCamera->SetPosition(pMoveCurrent);
	
	OnCameraChanged();
}

void igdeCameraInteractionListener::OnMoveFinish(bool cancelled){
	OnEndInteraction(cancelled);
}


bool igdeCameraInteractionListener::OnZoomBegin(){
	if(!pCamera){
		return false;
	}
	
	pZoomOrigin = pCamera->GetDistance();
	pZoomCurrent = pZoomOrigin;
	OnBeginInteraction();
	return true;
}

void igdeCameraInteractionListener::OnZoomUpdate(){
	if(!pCamera){
		return;
	}
	
	const decPoint distance(GetDragDistance());
	
	pZoomCurrent = decMath::max(pZoomOrigin + pSpeedZoom * (float)distance.y, 0.0f);
	
	pCamera->SetDistance(pZoomCurrent);
	
	OnCameraChanged();
}

void igdeCameraInteractionListener::OnZoomFinish(bool cancelled){
	OnEndInteraction(cancelled);
}


void igdeCameraInteractionListener::OnFlyBegin(){
	pFlyForward = pFlyBackward = pFlyLeft = pFlyRight = pFlyUp = pFlyDown = false;
	pFlyVelocity.SetGoal({});
	pFlyVelocity.SetValue({});
	pFlyMouseLastPosition.SetZero();
	pFlyMouseCurrentPosition.SetZero();
	
	pViewRenderWindow->GetToastBubble()->ShowToast("@Igde.ViewRenderWindow.Toast.FlyModeEnabled");
	
	OnBeginInteraction();
}

void igdeCameraInteractionListener::OnFlyUpdate(float elapsed){
	if(pInteraction != eiFly || !pCamera){
		return;
	}
	
	const float speed = GetSpeedFlyModified();
	decDVector velocity;
	if(pFlyForward){
		velocity += decDVector(0.0, 0.0, speed);
	}
	if(pFlyBackward){
		velocity += decDVector(0.0, 0.0, -speed);
	}
	if(pFlyLeft){
		velocity += decDVector(-speed, 0.0, 0.0);
	}
	if(pFlyRight){
		velocity += decDVector(speed, 0.0, 0.0);
	}
	if(pFlyUp){
		velocity += decDVector(0.0, speed, 0.0);
	}
	if(pFlyDown){
		velocity += decDVector(0.0, -speed, 0.0);
	}
	
	pFlyVelocity.SetGoal(velocity);
	pFlyVelocity.Update(elapsed);
	const decDVector movement(pFlyVelocity.GetValue() * (double)elapsed);
	
	const decPoint mouseDiffPos(pFlyMouseCurrentPosition - pFlyMouseLastPosition);
	pFlyMouseLastPosition = pFlyMouseCurrentPosition;
	
	auto matrix = pCamera->GetViewMatrix();
	
	auto rotation = matrix.GetEulerAngles() * RAD2DEG;
	rotation.x = decMath::clamp(rotation.x + (float)mouseDiffPos.y * pSpeedRotate, -89.9, 89.9);
	rotation.y = decMath::normalize(rotation.y - (float)mouseDiffPos.x * pSpeedRotate, 0.0, 360.0);
	rotation.z = 0.0;
	
	matrix = decDMatrix::CreateTranslation(0.0, 0.0, (double)pCamera->GetMPDistance()).
		QuickMultiply(decDMatrix::CreateTranslation(movement)).
		QuickMultiply(decDMatrix::CreateRT(rotation * DEG2RAD, matrix.GetPosition()));
	
	pCamera->GetMPPosition() = matrix.GetPosition();
	pCamera->GetMPRotation() = matrix.ToQuaternion();
	
	OnCameraChanged();
}

void igdeCameraInteractionListener::OnFlyEnd(){
	pFlyForward = pFlyBackward = pFlyLeft = pFlyRight = pFlyUp = pFlyDown = false;
	pFlyVelocity.SetGoal({});
	pFlyVelocity.SetValue({});
	pFlyMouseLastPosition.SetZero();
	pFlyMouseCurrentPosition.SetZero();
	
	pViewRenderWindow->GetToastBubble()->ShowToast("@Igde.ViewRenderWindow.Toast.FlyModeDisabled");
	
	OnEndInteraction(false);
}


void igdeCameraInteractionListener::OnBeginInteraction(){
}

void igdeCameraInteractionListener::OnCameraChanged(){
}

void igdeCameraInteractionListener::OnEndInteraction(bool cancelled){
}



// Dragging events
////////////////////

bool igdeCameraInteractionListener::OnDragBegin(){
	if(pEnvironment.Get3DViewFlyMode()){
		return false;
	}
	
	pInteraction = ChooseInteraction();
	
	switch(pInteraction){
	case eiRotate:
		return OnRotateBegin();
		
	case eiPan:
		return OnPanBegin();
		
	case eiMove:
		return OnMoveBegin();
		
	case eiZoom:
		return OnZoomBegin();
		
	default:
		return false;
	}
}

void igdeCameraInteractionListener::OnDragUpdate(){
	switch(pInteraction){
	case eiRotate:
		OnRotateUpdate();
		break;
		
	case eiPan:
		OnPanUpdate();
		break;
		
	case eiMove:
		OnMoveUpdate();
		break;
		
	case eiZoom:
		OnZoomUpdate();
		break;
		
	default:
		break;
	}
}

void igdeCameraInteractionListener::OnDragFinish(bool cancelled){
	switch(pInteraction){
	case eiRotate:
		OnRotateFinish(cancelled);
		pInteraction = eiNone;
		break;
		
	case eiPan:
		OnPanFinish(cancelled);
		pInteraction = eiNone;
		break;
		
	case eiMove:
		OnMoveFinish(cancelled);
		pInteraction = eiNone;
		break;
		
	case eiZoom:
		OnZoomFinish(cancelled);
		pInteraction = eiNone;
		break;
		
	default:
		break;
	}
}

void igdeCameraInteractionListener::OnButtonPress(igdeWidget *widget, int button,
const decPoint &position, int modifiers){
	if(button == deInputEvent::embcRight){
		pRightMouseButtonPressed = true;
	}
	
	igdeMouseDragListener::OnButtonPress(widget, button, position, modifiers);
}

void igdeCameraInteractionListener::OnMouseMoved(igdeWidget *widget, const decPoint &position,
int modifiers){
	if(pInteraction == eiFly){
		pFlyMouseCurrentPosition = position;
		
		auto const vrw = pViewRenderWindow.Pointer();
		if(vrw){
			const auto size(vrw->GetRenderAreaSize());
			const decPoint newPosition(
				((position.x % size.x) + size.x) % size.x,
				((position.y % size.y) + size.y) % size.y);
			
			if(newPosition != position && vrw->SetMousePointerPosition(newPosition)){
				pFlyMouseLastPosition += newPosition - position;
				pFlyMouseCurrentPosition = newPosition;
			}
		}
	}
	
	igdeMouseDragListener::OnMouseMoved(widget, position, modifiers);
}

void igdeCameraInteractionListener::OnButtonRelease(igdeWidget *widget,
int button, const decPoint &position, int modifiers){
	if(pEnvironment.Get3DViewFlyMode()){
		switch(pInteraction){
		case eiFly:
			if(button == deInputEvent::embcRight){
				OnFlyEnd();
				pInteraction = eiNone;
				auto const vrw = pViewRenderWindow.Pointer();
				if(vrw){
					vrw->ReleaseInput();
				}
			}
			break;
			
		case eiNone:
			pInteraction = ChooseInteraction();
			if(pInteraction == eiFly){
				auto const vrw = pViewRenderWindow.Pointer();
				if(vrw){
					vrw->GrabInput();
				}
				OnFlyBegin();
				pFlyMouseCurrentPosition = pFlyMouseLastPosition = position;
			}
			break;
			
		default:
			break;
		}
	}
	
	if(button == deInputEvent::embcRight){
		pRightMouseButtonPressed = false;
	}
	
	igdeMouseDragListener::OnButtonRelease(widget, button, position, modifiers);
}

void igdeCameraInteractionListener::OnMouseWheeled(igdeWidget *widget,
const decPoint &position, const decPoint &change, int modifiers){
	if(pInteraction != eiFly){
		return;
	}
	
	if(change.y > 0){
		SetSpeedFlyModifier(pFlySpeedModifier + 1);
		
	}else if(change.y < 0){
		SetSpeedFlyModifier(pFlySpeedModifier - 1);
	}
}

void igdeCameraInteractionListener::OnKeyPress(igdeWidget *widget,
deInputEvent::eKeyCodes keyCode, int key){
	if(pInteraction != eiFly){
		return;
	}
	
	switch(keyCode){
	case deInputEvent::ekcW:
		pFlyForward = true;
		break;
		
	case deInputEvent::ekcS:
		pFlyBackward = true;
		break;
		
	case deInputEvent::ekcA:
		pFlyLeft = true;
		break;
		
	case deInputEvent::ekcD:
		pFlyRight = true;
		break;
		
	case deInputEvent::ekcSpace:
		pFlyUp = true;
		break;
		
	case deInputEvent::ekcShift:
		pFlyDown = true;
		break;
		
	default:
		break;
	}
}

void igdeCameraInteractionListener::OnKeyRelease(igdeWidget *widget,
deInputEvent::eKeyCodes keyCode, int key){
	if(pInteraction != eiFly){
		return;
	}
	
	switch(keyCode){
	case deInputEvent::ekcW:
		pFlyForward = false;
		break;
		
	case deInputEvent::ekcS:
		pFlyBackward = false;
		break;
		
	case deInputEvent::ekcA:
		pFlyLeft = false;
		break;
		
	case deInputEvent::ekcD:
		pFlyRight = false;
		break;
		
	case deInputEvent::ekcSpace:
		pFlyUp = false;
		break;
		
	case deInputEvent::ekcShift:
		pFlyDown = false;
		break;
		
	case deInputEvent::ekcEscape:
		{
		OnFlyEnd();
		pInteraction = eiNone;
		auto const vrw = pViewRenderWindow.Pointer();
		if(vrw){
			vrw->ReleaseInput();
		}
		}break;
		
	default:
		break;
	}
}

void igdeCameraInteractionListener::OnFrameUpdate(float elapsed){
	if(pInteraction == eiFly){
		OnFlyUpdate(elapsed);
	}
}
