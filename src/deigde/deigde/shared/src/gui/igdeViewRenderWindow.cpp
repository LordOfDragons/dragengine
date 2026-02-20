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

#include "igdeContainer.h"
#include "igdeViewRenderWindow.h"
#include "event/igdeMouseKeyListener.h"
#include "native/toolkit.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>



// Class igdeViewRenderWindow
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewRenderWindow::igdeViewRenderWindow(igdeEnvironment &environment) :
igdeWidget(environment),
pEnableRendering(false),
pEngineRunning(false),
pNativeViewRenderWindow(nullptr){
}

igdeViewRenderWindow::~igdeViewRenderWindow(){
}



// Management
///////////////

void igdeViewRenderWindow::SetEnableRendering(bool enable){
	pEnableRendering = enable;
	
	if(pRenderWindow){
		pRenderWindow->SetPaint(enable && pNativeViewRenderWindow && pNativeViewRenderWindow->IsShown());
	}
}

bool igdeViewRenderWindow::GetCanRender() const{
	return pNativeViewRenderWindow && pNativeViewRenderWindow->GetCanRender();
}

decPoint igdeViewRenderWindow::GetRenderAreaSize() const{
	return pNativeViewRenderWindow ? pNativeViewRenderWindow->GetSize() : decPoint();
}



void igdeViewRenderWindow::ClearErrorRenderWindow(){
	// TODO
}



void igdeViewRenderWindow::OnAfterEngineStart(){
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->AttachRenderWindow();
	}
	pEngineRunning = true;
}

void igdeViewRenderWindow::OnBeforeEngineStop(){
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->DetachRenderWindow();
	}
	pEngineRunning = false;
}

void igdeViewRenderWindow::OnFrameUpdate(float){
	if(!pRenderWindow){
		return;
	}
	
	pRenderWindow->SetPaint(pEnableRendering && pNativeViewRenderWindow && pNativeViewRenderWindow->IsReallyVisible());
	
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->OnFrameUpdate();
	}
}



void igdeViewRenderWindow::SetRenderWorld(deCamera *camera){
	deCanvasRenderWorld * const canvasRenderWorld = GetCanvasRenderWorld();
	if(canvasRenderWorld){
		canvasRenderWorld->SetCamera(camera);
	}
}

void igdeViewRenderWindow::AddCanvas(deCanvas *canvas){
	if(!canvas || !pRenderWindow){
		DETHROW(deeInvalidParam);
	}
	
	deCanvasView * canvasView = pRenderWindow->GetCanvasView();
	if(canvasView){
		canvasView->AddCanvas(canvas);
	}
}

void igdeViewRenderWindow::RemoveCanvas(deCanvas *canvas){
	if(!canvas || !pRenderWindow){
		DETHROW(deeInvalidParam);
	}
	
	deCanvasView * canvasView = pRenderWindow->GetCanvasView();
	if(canvasView){
		canvasView->RemoveCanvas(canvas);
	}
}

deCanvasView *igdeViewRenderWindow::GetRenderWindowCanvas() const{
	return pRenderWindow ? pRenderWindow->GetCanvasView().Pointer() : nullptr;
}

deCanvasRenderWorld *igdeViewRenderWindow::GetCanvasRenderWorld() const{
	return static_cast<deCanvasRenderWorld*>(pCanvasRenderWorld.Pointer());
}

deCanvasPaint *igdeViewRenderWindow::GetCanvasBackground() const{
	return static_cast<deCanvasPaint*>(pCanvasBackground.Pointer());
}

void igdeViewRenderWindow::CreateCanvas(){
	if(!pCanvasBackground){
		pCanvasBackground = GetEngineController().GetEngine()->GetCanvasManager()->CreateCanvasPaint();
		deCanvasPaint &background = static_cast<deCanvasPaint&>(pCanvasBackground.Reference());
		background.SetOrder(0.0f);
		background.SetShapeType(deCanvasPaint::estRectangle);
		background.SetThickness(0.0f);
		background.SetFillColor(GetEnvironment().GetSystemColor(igdeEnvironment::escWidgetShadow));
		AddCanvas(pCanvasBackground);
	}
	if(!pCanvasRenderWorld){
		pCanvasRenderWorld = GetEngineController().GetEngine()->GetCanvasManager()->CreateCanvasRenderWorld();
		pCanvasRenderWorld->SetOrder(1.0f);
		AddCanvas(pCanvasRenderWorld);
	}
}

void igdeViewRenderWindow::GrabInput(){
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->GrabInput();
	}
}

void igdeViewRenderWindow::ReleaseInput(){
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->ReleaseInput();
	}
}



void igdeViewRenderWindow::AddListener(igdeMouseKeyListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeViewRenderWindow::RemoveListener(igdeMouseKeyListener *listener){
	pListeners.Remove(listener);
}
void igdeViewRenderWindow::NotifyKeyPress(deInputEvent::eKeyCodes keyCode, int key){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnKeyPress(this, keyCode, key);
	});
}

void igdeViewRenderWindow::NotifyKeyRelease(deInputEvent::eKeyCodes keyCode, int key){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnKeyRelease(this, keyCode, key);
	});
}

void igdeViewRenderWindow::NotifyButtonPress(int button, const decPoint &position, int modifiers){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnButtonPress(this, button, position, modifiers);
	});
}

void igdeViewRenderWindow::NotifyButtonRelease(int button, const decPoint &position, int modifiers){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnButtonRelease(this, button, position, modifiers);
	});
}

void igdeViewRenderWindow::NotifyDoubleClicked(int button, const decPoint &position, int modifiers){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnDoubleClicked(this, button, position, modifiers);
	});
}

void igdeViewRenderWindow::NotifyMouseMoved(const decPoint &position, int modifiers){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnMouseMoved(this, position, modifiers);
	});
}

void igdeViewRenderWindow::NotifyMouseWheeled(const decPoint &position, const decPoint &change, int modifiers){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnMouseWheeled(this, position, change, modifiers);
	});
}

void igdeViewRenderWindow::NotifyMouseEnter(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnMouseEnter(this);
	});
}

void igdeViewRenderWindow::NotifyMouseLeave(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeMouseKeyListener &l){
		l.OnMouseLeave(this);
	});
}



void igdeViewRenderWindow::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeRenderView * const native = igdeNativeRenderView::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeViewRenderWindow = native;
	native->PostCreateNativeWidget();
}

void igdeViewRenderWindow::DropNativeWidget(){
	if(pNativeViewRenderWindow){
		pNativeViewRenderWindow->DropNativeWindow();
	}
	
	pNativeViewRenderWindow = nullptr;
	igdeWidget::DropNativeWidget();
}

void igdeViewRenderWindow::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeRenderView*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeViewRenderWindow::CreateAndAttachRenderWindow(){
	if(!GetNativeWidget()){
		DETHROW(deeNullPointer);
	}
	
	if(!pRenderWindow){
		pRenderWindow = GetEngineController().CreateRenderWindow(*this);
		pRenderWindow->SetPaint(pEnableRendering);
		CreateCanvas();
	}
	
	if(pEngineRunning && pNativeViewRenderWindow){
		pNativeViewRenderWindow->AttachRenderWindow();
	}
}
