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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
pEngineRunning(false){
}

igdeViewRenderWindow::~igdeViewRenderWindow(){
}



// Management
///////////////

void igdeViewRenderWindow::SetEnableRendering(bool enable){
	pEnableRendering = enable;
	
	if(pRenderWindow){
		igdeNativeRenderView * const native = (igdeNativeRenderView*)GetNativeWidget();
		pRenderWindow->SetPaint(enable && native && native->IsShown());
	}
}

bool igdeViewRenderWindow::GetCanRender() const{
	const igdeNativeRenderView * const native = (const igdeNativeRenderView*)GetNativeWidget();
	return native && native->GetCanRender();
}

decPoint igdeViewRenderWindow::GetRenderAreaSize() const{
	const igdeNativeRenderView * const native = (igdeNativeRenderView*)GetNativeWidget();
	return native ? native->GetSize() : decPoint();
}



void igdeViewRenderWindow::ClearErrorRenderWindow(){
	// TODO
}



void igdeViewRenderWindow::OnAfterEngineStart(){
	igdeNativeRenderView * const native = (igdeNativeRenderView*)GetNativeWidget();
	if(native){
		native->AttachRenderWindow();
	}
	pEngineRunning = true;
}

void igdeViewRenderWindow::OnBeforeEngineStop(){
	igdeNativeRenderView * const native = (igdeNativeRenderView*)GetNativeWidget();
	if(native){
		native->DetachRenderWindow();
	}
	pEngineRunning = false;
}

void igdeViewRenderWindow::OnFrameUpdate(float){
	if(!pRenderWindow){
		return;
	}
	
	igdeNativeRenderView * const native = (igdeNativeRenderView*)GetNativeWidget();
	pRenderWindow->SetPaint(pEnableRendering && native && native->IsReallyVisible());
	
	if(native){
		native->OnFrameUpdate();
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
	return pRenderWindow ? pRenderWindow->GetCanvasView() : nullptr;
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
	igdeNativeRenderView * const native = static_cast<igdeNativeRenderView*>(GetNativeWidget());
	if(native){
		native->GrabInput();
	}
}

void igdeViewRenderWindow::ReleaseInput(){
	igdeNativeRenderView * const native = static_cast<igdeNativeRenderView*>(GetNativeWidget());
	if(native){
		native->ReleaseInput();
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
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnKeyPress(this, keyCode, key);
	}
}

void igdeViewRenderWindow::NotifyKeyRelease(deInputEvent::eKeyCodes keyCode, int key){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnKeyRelease(this, keyCode, key);
	}
}

void igdeViewRenderWindow::NotifyButtonPress(int button, const decPoint &position, int modifiers){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnButtonPress(this,
			button, position, modifiers);
	}
}

void igdeViewRenderWindow::NotifyButtonRelease(int button, const decPoint &position, int modifiers){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnButtonRelease(this,
			button, position, modifiers);
	}
}

void igdeViewRenderWindow::NotifyDoubleClicked(int button, const decPoint &position, int modifiers){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnDoubleClicked(this,
			button, position, modifiers);
	}
}

void igdeViewRenderWindow::NotifyMouseMoved(const decPoint &position, int modifiers){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnMouseMoved(this, position, modifiers);
	}
}

void igdeViewRenderWindow::NotifyMouseWheeled(const decPoint &position, const decPoint &change, int modifiers){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnMouseWheeled(this, position, change, modifiers);
	}
}

void igdeViewRenderWindow::NotifyMouseEnter(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnMouseEnter(this);
	}
}

void igdeViewRenderWindow::NotifyMouseLeave(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeMouseKeyListener*)listeners.GetAt(i))->OnMouseLeave(this);
	}
}



void igdeViewRenderWindow::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeRenderView * const native = igdeNativeRenderView::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeViewRenderWindow::DropNativeWidget(){
	if(GetNativeWidget()){
		((igdeNativeRenderView*)GetNativeWidget())->DropNativeWindow();
	}
	
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
	
	if(pEngineRunning){
		((igdeNativeRenderView*)GetNativeWidget())->AttachRenderWindow();
	}
}
