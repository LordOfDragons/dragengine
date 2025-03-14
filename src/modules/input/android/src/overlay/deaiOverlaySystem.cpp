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

#include "deaiInputBinding.h"
#include "deaiOverlaySystem.h"
#include "deaiOverlayCirclePad.h"
#include "deaiOverlayActionButton.h"
#include "../deAndroidInput.h"
#include "../deaiDeviceKeyboard.h"
#include "../deaiDeviceAxis.h"
#include "../deaiDeviceButton.h"
#include "../deaiDeviceMouse.h"
#include "../deaiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>



// Class deaiOverlaySystem
////////////////////////////

// Constructor, destructor
////////////////////////////

deaiOverlaySystem::deaiOverlaySystem( deAndroidInput &androidInput ) :
pAndroidInput( androidInput ),

pCanvas( NULL ),
pInHorizontalMode( false )
{
	decPoint screenSize( androidInput.GetScreenSize() );
	pInHorizontalMode = screenSize.x > screenSize.y;
	if( pInHorizontalMode ){
		const int temporary = screenSize.x;
		screenSize.x = screenSize.y;
		screenSize.y = temporary;
	}
	
	try{
		pCanvas = androidInput.GetGameEngine()->GetCanvasManager()->CreateCanvasView();
		pCanvas->SetSize( screenSize );
		
#if 0
		const deaiDeviceMouse &mouse = *androidInput.GetDevices().GetMouse();
		const deaiDeviceKeyboard &keyboard = *androidInput.GetDevices().GetKeyboard();
		
		deaiOverlayActionButton::Ref actionButton;
		deaiOverlayCirclePad::Ref circlePad;
		
		circlePad.TakeOver(new deaiOverlayCirclePad(androidInput));
		circlePad->SetKeyCodeLeft(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcY)).GetID()));
		circlePad->SetKeyCodeUp(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcD)).GetID()));
		circlePad->SetKeyCodeRight(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcV)).GetID()));
		circlePad->SetKeyCodeDown(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcC)).GetID()));
		circlePad->SetLayoutHorizontal(deaiLayout(decPoint(20,
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		circlePad->SetLayoutVertical(deaiLayout(decPoint(20,
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(circlePad->GetCanvas());
		pOverlays.Add(circlePad);
		
		circlePad.TakeOver(new deaiOverlayCirclePad(androidInput));
		circlePad->SetAxisLeftRight(deaiInputBinding(mouse.GetID(),
			mouse.GetAxisAt(0).GetID(), false)); // mouse left-right
		circlePad->SetAxisUpDown(deaiInputBinding(mouse.GetID(),
			mouse.GetAxisAt(1).GetID(), true)); // mouse up-down
		circlePad->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		circlePad->SetLayoutVertical( deaiLayout(decPoint(
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y),
			circlePad->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(circlePad->GetCanvas());
		pOverlays.Add(circlePad);
		
		actionButton.TakeOver(new deaiOverlayActionButton(androidInput));
		actionButton->SetBinding(deaiInputBinding(mouse.GetID(),
			mouse.GetButtonAt(0).GetID())); // left mouse button
		actionButton->SetText("LMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 416, screenSize.x - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deaiLayout(decPoint(
				screenSize.x - 416, screenSize.y - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deaiOverlayActionButton(androidInput));
		actionButton->SetBinding(deaiInputBinding(mouse.GetID(),
			mouse.GetButtonAt(1).GetID())); // right mouse button
		actionButton->SetText("RMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 142, screenSize.x - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deaiLayout(decPoint(
				screenSize.x - 142, screenSize.y - 512),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deaiOverlayActionButton(androidInput));
		actionButton->SetBinding(deaiInputBinding(mouse.GetID(),
			mouse.GetButtonAt(2).GetID())); // middle mouse button
		actionButton->SetText("MMB");
		actionButton->SetColor(decColor(1.0f, 0.0f, 0.0f));
		actionButton->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 278, screenSize.x - 543),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deaiLayout(decPoint(
				screenSize.x - 278, screenSize.y - 543),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deaiOverlayActionButton(androidInput));
		actionButton->SetBinding(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcQ)).GetID()));
		actionButton->SetText("Quit");
		actionButton->SetColor(decColor(0.0f, 0.5f, 1.0f));
		actionButton->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 517, screenSize.x - 390),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deaiLayout(decPoint(
				screenSize.x - 517, screenSize.y - 390),
			actionButton->GetLayoutHorizontal().GetSize()));
		pCanvas->AddCanvas(actionButton->GetCanvas());
		pOverlays.Add(actionButton);
		
		actionButton.TakeOver(new deaiOverlayActionButton(androidInput));
		actionButton->SetBinding(deaiInputBinding(keyboard.GetID(), keyboard.GetButtonAt(
			keyboard.IndexOfButtonWithKeyCode(deInputEvent::ekcR)).GetID()));
		actionButton->SetText("Debug");
		actionButton->SetColor(decColor(0.0f, 0.5f, 1.0f));
		actionButton->SetLayoutHorizontal(deaiLayout(decPoint(
				screenSize.y - 544, screenSize.x - 239),
			actionButton->GetLayoutHorizontal().GetSize()));
		actionButton->SetLayoutVertical(deaiLayout(decPoint(
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

deaiOverlaySystem::~deaiOverlaySystem(){
	pCleanUp();
}



// Management
///////////////

void deaiOverlaySystem::UpdateContent(){
	const decPoint screenSize( pAndroidInput.GetScreenSize() );
	SetInHorizontalMode( screenSize.x > screenSize.y );
	
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pOverlays.GetAt( i )->UpdateContent();
	}
}

void deaiOverlaySystem::ScreenSizeChanged(){
}

void deaiOverlaySystem::SetInHorizontalMode( bool inHorizontalMode ){
	if( inHorizontalMode == pInHorizontalMode ){
		return;
	}
	
	pInHorizontalMode = inHorizontalMode;
	UpdateFromLayout();
}

void deaiOverlaySystem::UpdateBindingIndices(){
	const int count = pOverlays.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pOverlays.GetAt( i )->UpdateBindingIndices();
	}
}

void deaiOverlaySystem::UpdateFromLayout(){
	pCanvas->SetSize( pAndroidInput.GetScreenSize() );
	
	const int count = pOverlays.GetCount();
	int i;
	
	if( pInHorizontalMode ){
		for( i=0; i<count; i++ ){
			pOverlays.GetAt( i )->UpdateFromHorizontalLayout();
		}
		
	}else{
		for( i=0; i<count; i++ ){
			pOverlays.GetAt( i )->UpdateFromVerticalLayout();
		}
	}
}



bool deaiOverlaySystem::OnTouch( int pointerId, const decPoint &position ){
	const int count = pOverlays.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.OnTouch( pointerId, position - overlay.GetCanvas()->GetPosition() ) ){
			return true;
		}
	}
	
	return false;
}

bool deaiOverlaySystem::OnMove( int pointerId, const decPoint &position ){
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.GetPointer() == pointerId ){
			overlay.OnMove( position - overlay.GetCanvas()->GetPosition() );
			return true;
		}
	}
	
	return false;
}

bool deaiOverlaySystem::OnRelease( int pointerId ){
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.GetPointer() == pointerId ){
			overlay.OnRelease();
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void deaiOverlaySystem::pCleanUp(){
	pOverlays.RemoveAll();
	
	if( pCanvas ){
		pCanvas->RemoveAllCanvas();
		pCanvas->FreeReference();
	}
}
