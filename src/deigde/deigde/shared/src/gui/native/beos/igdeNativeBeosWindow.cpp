/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosWindow.h"
#include "../../igdeWindow.h"
#include "../../igdeContainer.h"
#include "../../igdeWidget.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosWindow::igdeNativeBeosWindow(igdeWindow &owner) :
BWindow(BRect(100, 100, 700, 500), owner.GetTitle().GetString(),
	owner.GetCanResize() ? B_TITLED_WINDOW : B_TITLED_WINDOW_WITHOUT_RESIZE,
	B_ASYNCHRONOUS_CONTROLS),
pOwner(&owner),
pContentView(nullptr)
{
	pContentView = new BView(Bounds(), "content", B_FOLLOW_ALL, B_WILL_DRAW);
	pContentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(pContentView);
}

igdeNativeBeosWindow::~igdeNativeBeosWindow() = default;


igdeNativeBeosWindow* igdeNativeBeosWindow::CreateNativeWidget(igdeWindow &owner){
	return new igdeNativeBeosWindow(owner);
}

void igdeNativeBeosWindow::PostCreateNativeWidget(){
	Show();
}

void igdeNativeBeosWindow::DestroyNativeWidget(){
	if(LockLooper()){
		Quit();
	}
}

void igdeNativeBeosWindow::UpdateEnabled(){
	if(pOwner){
		bool enabled = pOwner->GetEnabled();
		// BeOS doesn't have window-level enable/disable directly
		// Would need to recursively enable/disable all children
	}
}

void igdeNativeBeosWindow::UpdateStyle(){
	if(pOwner && LockLooper()){
		// Apply theme colors to content view
		pContentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		pContentView->Invalidate();
		UnlockLooper();
	}
}

void igdeNativeBeosWindow::UpdatePosition(){
	if(pOwner && LockLooper()){
		const auto &pos = pOwner->GetPosition();
		MoveTo(pos.x, pos.y);
		UnlockLooper();
	}
}

void igdeNativeBeosWindow::UpdateIcon(){
	// BeOS window icon support would go here
}

void igdeNativeBeosWindow::UpdateTitle(){
	if(pOwner && LockLooper()){
		SetTitle(pOwner->GetTitle().GetString());
		UnlockLooper();
	}
}

void igdeNativeBeosWindow::UpdateSize(){
	if(pOwner && LockLooper()){
		const auto &size = pOwner->GetSize();
		ResizeTo(size.x - 1, size.y - 1);
		UnlockLooper();
	}
}

void igdeNativeBeosWindow::RaiseAndActivate(){
	if(LockLooper()){
		Activate();
		UnlockLooper();
	}
}

void igdeNativeBeosWindow::OnFrameUpdate(){
}

void *igdeNativeBeosWindow::GetNativeContainer() const{
	return pContentView;
}

void igdeNativeBeosWindow::MessageReceived(BMessage *message){
	BWindow::MessageReceived(message);
	
	try{
		// Handle any window-specific messages
		// Most window messages are handled by BWindow base class
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE Window", e);
		}
	}
}

bool igdeNativeBeosWindow::QuitRequested(){
	if(pOwner && pOwner->CloseWindow()){
		return true;
	}
	return false;
}

#endif
