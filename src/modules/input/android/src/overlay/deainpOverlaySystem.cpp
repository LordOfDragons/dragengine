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

#include <stdlib.h>

#include "deainpInputBinding.h"
#include "deainpOverlaySystem.h"
#include "deainpOverlayCirclePad.h"
#include "deainpOverlayActionButton.h"
#include "../deAndroidInput.h"
#include "../deainpDeviceKeyboard.h"
#include "../deainpDeviceAxis.h"
#include "../deainpDeviceButton.h"
#include "../deainpDeviceMouse.h"
#include "../deainpDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>



// Class deainpOverlaySystem
////////////////////////////

// Constructor, destructor
////////////////////////////

deainpOverlaySystem::deainpOverlaySystem(deAndroidInput &androidInput) :
pAndroidInput(androidInput),
pInHorizontalMode(false)
{
	decPoint screenSize(androidInput.GetScreenSize());
	pInHorizontalMode = screenSize.x > screenSize.y;
	if(pInHorizontalMode){
		const int temporary = screenSize.x;
		screenSize.x = screenSize.y;
		screenSize.y = temporary;
	}
	
	try{
		pCanvas = androidInput.GetGameEngine()->GetCanvasManager()->CreateCanvasView();
		pCanvas->SetSize(screenSize);
		
#if 0
		const deainpDeviceMouse &mouse = *androidInput.GetDevices().GetMouse();
		const deainpDeviceKeyboard &keyboard = *androidInput.GetDevices().GetKeyboard();
		
		deainpOverlayActionButton::Ref actionButton;
		deainpOverlayCirclePad::Ref circlePad;
		
		circlePad.TakeOver(new deainpOverlayCirclePad(androidInput));
		circlePad->SetKeyCodeLeft(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcY)).GetID()));
		circlePad->SetKeyCodeUp(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcD)).GetID()));
		circlePad->SetKeyCodeRight(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcV)).GetID()));
		circlePad->SetKeyCodeDown(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcC)).GetID()));
		circlePad->SetLayoutHorizontal(deainpLayout(decPoint(20,
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		circlePad->SetLayoutVertical(deainpLayout(decPoint(20,
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(circlePad->GetCanvas());
		pOverlays.Add(circlePad);
		
		circlePad.TakeOver(new deainpOverlayCirclePad(androidInput));
		circlePad->SetAxisLeftRight(deainpInputBinding(mouse.GetID(),
			mouse.GetAxisAt(0).GetID(), false)); // mouse left-right
		circlePad->SetAxisUpDown(deainpInputBinding(mouse.GetID(),
			mouse.GetAxisAt(1).GetID(), true)); // mouse up-down
		circlePad->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		circlePad->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(circlePad->GetCanvas());
		pOverlays.Add(circlePad);
		
		actionButton.TakeOver(new deainpOverlayActionButton(androidInput));
		actionButton->SetBinding(deainpInputBinding(mouse.GetID(),
			mouse.GetButtonAt(0).GetID())); // left mouse button
		actionButton->SetText("LMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 416, screenSize.x - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 416, screenSize.y - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deainpOverlayActionButton(androidInput));
		actionButton->SetBinding(deainpInputBinding(mouse.GetID(),
			mouse.GetButtonAt(1).GetID())); // right mouse button
		actionButton->SetText("RMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 142, screenSize.x - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 142, screenSize.y - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deainpOverlayActionButton(androidInput));
		actionButton->SetBinding(deainpInputBinding(mouse.GetID(),
			mouse.GetButtonAt(2).GetID())); // middle mouse button
		actionButton->SetText("MMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 278, screenSize.x - 543),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 278, screenSize.y - 543),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deainpOverlayActionButton(androidInput));
		actionButton->SetBinding(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcQ)).GetID()));
		actionButton->SetText("Quit");
		actionButton->SetColor(decColor(0.0f, 0.5f, 1.0f));
		actionButton->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 517, screenSize.x - 390),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 517, screenSize.y - 390),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deainpOverlayActionButton(androidInput));
		actionButton->SetBinding(deainpInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcR)).GetID()));
		actionButton->SetText("Debug");
		actionButton->SetColor(decColor(0.0f, 0.5f, 1.0f));
		actionButton->SetLayoutHorizontal(deainpLayout(decPoint(
				screenSize.y - 544, screenSize.x - 239),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deainpLayout(decPoint(
				screenSize.x - 544, screenSize.y - 239),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
#endif
		
		UpdateBindingIndices();
		UpdateFromLayout();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deainpOverlaySystem::~deainpOverlaySystem(){
	pCleanUp();
}



// Management
///////////////

void deainpOverlaySystem::UpdateContent(){
	const decPoint screenSize(pAndroidInput.GetScreenSize());
	SetInHorizontalMode(screenSize.x > screenSize.y);
	
	const int count = pOverlays.GetCount();
	int i;
	for(i=0; i<count; i++){
		pOverlays.GetAt(i)->UpdateContent();
	}
}

void deainpOverlaySystem::ScreenSizeChanged(){
}

void deainpOverlaySystem::SetInHorizontalMode(bool inHorizontalMode){
	if(inHorizontalMode == pInHorizontalMode){
		return;
	}
	
	pInHorizontalMode = inHorizontalMode;
	UpdateFromLayout();
}

void deainpOverlaySystem::UpdateBindingIndices(){
	const int count = pOverlays.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pOverlays.GetAt(i)->UpdateBindingIndices();
	}
}

void deainpOverlaySystem::UpdateFromLayout(){
	pCanvas->SetSize(pAndroidInput.GetScreenSize());
	
	const int count = pOverlays.GetCount();
	int i;
	
	if(pInHorizontalMode){
		for(i=0; i<count; i++){
			pOverlays.GetAt(i)->UpdateFromHorizontalLayout();
		}
		
	}else{
		for(i=0; i<count; i++){
			pOverlays.GetAt(i)->UpdateFromVerticalLayout();
		}
	}
}



bool deainpOverlaySystem::OnTouch(int pointerId, const decPoint &position){
	const int count = pOverlays.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deainpOverlay &overlay = *pOverlays.GetAt(i);
		if(overlay.OnTouch(pointerId, position - overlay.GetCanvas()->GetPosition())){
			return true;
		}
	}
	
	return false;
}

bool deainpOverlaySystem::OnMove(int pointerId, const decPoint &position){
	const int count = pOverlays.GetCount();
	int i;
	for(i=0; i<count; i++){
		deainpOverlay &overlay = *pOverlays.GetAt(i);
		if(overlay.GetPointer() == pointerId){
			overlay.OnMove(position - overlay.GetCanvas()->GetPosition());
			return true;
		}
	}
	
	return false;
}

bool deainpOverlaySystem::OnRelease(int pointerId){
	const int count = pOverlays.GetCount();
	int i;
	for(i=0; i<count; i++){
		deainpOverlay &overlay = *pOverlays.GetAt(i);
		if(overlay.GetPointer() == pointerId){
			overlay.OnRelease();
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void deainpOverlaySystem::pCleanUp(){
	pOverlays.RemoveAll();
	
	if(pCanvas){
		pCanvas->RemoveAllCanvas();
	}
}
