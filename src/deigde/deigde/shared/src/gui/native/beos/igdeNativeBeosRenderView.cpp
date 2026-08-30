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

#include "igdeNativeBeosRenderView.h"
#include "../../igdeRenderView.h"
#include "../../igdeViewRenderWindow.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosRenderView
///////////////////////////////////

igdeNativeBeosRenderView::igdeNativeBeosRenderView(igdeViewRenderWindow &owner, BView *parent) :
BView(BRect(0, 0, 400, 300), "renderview", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS),
pOwner(nullptr),
pRenderWindowOwner(&owner),
pRenderContext(nullptr),
pViewportWidth(400),
pViewportHeight(300),
pCanAttachRenderWindow(true),
pRenderWindowAttached(false),
pCanRender(true),
pErrorRenderWindow(false),
pMouseButtons(0),
pMouseInside(false)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Initialize rendering context
	// For BeOS/Haiku, we need to set up a rendering surface
	// This is typically done through BGLView or equivalent GL context
	InitializeRenderContext();
}

igdeNativeBeosRenderView::~igdeNativeBeosRenderView(){
	if(pRenderContext){
		// Cleanup rendering context
		pRenderContext = nullptr;
	}
}

void igdeNativeBeosRenderView::InitializeRenderContext(){
	// Initialize OpenGL rendering context for the viewport
	// In a real implementation, this would:
	// 1. Create an OpenGL context associated with this BView
	// 2. Set up rendering state (projection, modelview matrices, etc.)
	// 3. Allocate resources for rendering
	// 
	// For now, create a placeholder context that indicates readiness
	pRenderContext = (void*)1; // Non-null to indicate context exists
	
	// Set up basic OpenGL state if context is available
	if(pRenderContext){
		// In a full implementation:
		// glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		// glClearDepth(1.0f);
		// glEnable(GL_DEPTH_TEST);
		// glDepthFunc(GL_LEQUAL);
		// glEnable(GL_LIGHTING);
		// etc.
	}
}

igdeNativeBeosRenderView* igdeNativeBeosRenderView::CreateNativeWidget(igdeViewRenderWindow &owner){
	// RenderView is a 3D rendering surface within the UI
	// Implementation provides platform-specific rendering context
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosRenderView(owner, parent);
	parent->AddChild(widget);
	
	return widget;
}

void igdeNativeBeosRenderView::PostCreateNativeWidget(igdeViewRenderWindow &owner, void *native){
	// RenderView post-creation setup
	auto widget = (igdeNativeBeosRenderView*)native;
	if(widget){
		widget->pRenderWindowOwner = &owner;
		// Initialize rendering context if needed
		pProcessRenderWindowAttachment();
	}
}

void igdeNativeBeosRenderView::DestroyNativeWidget(igdeViewRenderWindow &owner, void *native){
	auto widget = (igdeNativeBeosRenderView*)native;
	if(widget){
		widget->RemoveSelf();
		delete widget;
	}
}


// Management
///////////////

bool igdeNativeBeosRenderView::IsFocusable() const{
	return true;
}

void igdeNativeBeosRenderView::DropNativeWindow(){
	// Drop/detach the native window
	DetachRenderWindow();
}

bool igdeNativeBeosRenderView::IsReallyVisible() const{
	if(!IsVisible()){
		return false;
	}
	
	// Check if parent is visible
	auto parent = Parent();
	while(parent){
		if(!parent->IsVisible()){
			return false;
		}
		parent = parent->Parent();
	}
	
	return true;
}

bool igdeNativeBeosRenderView::IsShown() const{
	return IsVisible();
}

decPoint igdeNativeBeosRenderView::GetSize() const{
	return decPoint((int)Bounds().Width(), (int)Bounds().Height());
}

void igdeNativeBeosRenderView::OnFrameUpdate(){
	// Frame update callback from owner
	if(pRenderWindowAttached && pCanRender){
		Invalidate();
	}
}

void igdeNativeBeosRenderView::AttachRenderWindow(){
	// Attach a render window if available
	// This method allows the render window from the graphics module to be
	// placed into this widget instead of the default BeOS rendering
	
	try{
		if(pRenderWindowOwner && pCanAttachRenderWindow && !pRenderWindowAttached){
			pRenderWindowAttached = true;
			pCanRender = true;
			Invalidate();
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView", e);
		}
	}
}

void igdeNativeBeosRenderView::DetachRenderWindow(){
	// Detach the render window
	try{
		if(pRenderWindowAttached){
			pRenderWindowAttached = false;
			Invalidate();
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView", e);
		}
	}
}

void igdeNativeBeosRenderView::DrawEngineUnavailable(BView *dc){
	// Draw message when render engine is unavailable
	if(!dc) return;
	
	dc->SetHighColor(100, 100, 100);
	dc->DrawString("Render Engine Unavailable", BPoint(10, 30));
}

void igdeNativeBeosRenderView::DrawErrorRenderWindow(BView *dc){
	// Draw error message for render window issues
	if(!dc) return;
	
	dc->SetHighColor(200, 0, 0);
	dc->DrawString("Failed to Create Render Window", BPoint(10, 30));
	dc->DrawString("Check graphics module configuration", BPoint(10, 50));
}

void igdeNativeBeosRenderView::DrawErrorRendering(BView *dc){
	// Draw error message for rendering failures
	if(!dc) return;
	
	dc->SetHighColor(200, 100, 0);
	dc->DrawString("Rendering Error Occurred", BPoint(10, 30));
	dc->DrawString("Check application logs", BPoint(10, 50));
}

void igdeNativeBeosRenderView::GrabInput(){
	// Grab input focus for this render view
	try{
		if(pRenderWindowOwner && Window()){
			MakeFocus(true);
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView", e);
		}
	}
}

void igdeNativeBeosRenderView::ReleaseInput(){
	// Release input focus
	try{
		if(Window()){
			MakeFocus(false);
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView", e);
		}
	}
}

void *igdeNativeBeosRenderView::GetRenderContext(){
	// Get the rendering context for 3D rendering
	return pRenderContext;
}

void igdeNativeBeosRenderView::SetViewSize(int width, int height){
	// Update viewport dimensions
	pViewportWidth = width;
	pViewportHeight = height;
	Invalidate();
}

void igdeNativeBeosRenderView::HandleRedraw(){
	// Force redraw of the rendering surface
	Invalidate();
}


// Event Handlers
//////////////////

void igdeNativeBeosRenderView::Draw(BRect updateRect){
	// Draw rendering surface
	
	// Fill background with dark color for rendering surface (typical 3D viewport background)
	SetHighColor(40, 40, 40);
	FillRect(updateRect);
	
	// If rendering context is available, perform actual 3D rendering
	if(pRenderContext && pCanRender){
		// In a full implementation:
		// 1. Bind the OpenGL context
		// 2. Clear color and depth buffers
		// 3. Set up projection and view matrices
		// 4. Render scene objects
		// 5. Swap buffers
		// 6. Unbind the context
		
		// For now, just draw a grid to indicate rendering is active
		SetHighColor(60, 60, 60);
		
		int gridSize = 20;
		for(int x = 0; x < (int)Bounds().Width(); x += gridSize){
			StrokeLine(BPoint(x, Bounds().top), BPoint(x, Bounds().bottom));
		}
		for(int y = 0; y < (int)Bounds().Height(); y += gridSize){
			StrokeLine(BPoint(Bounds().left, y), BPoint(Bounds().right, y));
		}
		
		// Draw viewport info
		SetHighColor(150, 150, 150);
		char info[256];
		snprintf(info, sizeof(info), "3D Viewport: %dx%d", pViewportWidth, pViewportHeight);
		DrawString(info, BPoint(10, 20));
	}
	else if(pErrorRenderWindow){
		DrawErrorRenderWindow(this);
	}
}

void igdeNativeBeosRenderView::MouseDown(BPoint where){
	BView::MouseDown(where);
	
	try{
		if(pRenderWindowOwner){
			// Track which mouse button was pressed
			auto msg = Window()->CurrentMessage();
			if(msg){
				int32 buttons = 0;
				msg->FindInt32("buttons", &buttons);
				pMouseButtons = buttons;
				
				// Fire mouse down event
				// Implementation would call into owner's input handling
			}
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView MouseDown", e);
		}
	}
}

void igdeNativeBeosRenderView::MouseUp(BPoint where){
	BView::MouseUp(where);
	
	try{
		if(pRenderWindowOwner){
			auto msg = Window()->CurrentMessage();
			if(msg){
				int32 buttons = 0;
				msg->FindInt32("buttons", &buttons);
				pMouseButtons = buttons;
				
				// Fire mouse up event
			}
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView MouseUp", e);
		}
	}
}

void igdeNativeBeosRenderView::MouseMoved(BPoint where, uint32 code, const BMessage *message){
	BView::MouseMoved(where, code, message);
	
	try{
		if(pRenderWindowOwner){
			// Track mouse position and button state
			// Fire mouse move event with position info
			
			// Check for enter/leave
			if(code == B_ENTERED_VIEW && !pMouseInside){
				pMouseInside = true;
			}
			else if(code == B_EXITED_VIEW && pMouseInside){
				pMouseInside = false;
			}
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView MouseMoved", e);
		}
	}
}

void igdeNativeBeosRenderView::KeyDown(const char *bytes, int32 numBytes){
	BView::KeyDown(bytes, numBytes);
	
	try{
		if(pRenderWindowOwner){
			// Fire keyboard action callbacks
			// Camera movement, viewport controls, etc.
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView KeyDown", e);
		}
	}
}

void igdeNativeBeosRenderView::KeyUp(const char *bytes, int32 numBytes){
	BView::KeyUp(bytes, numBytes);
	
	try{
		if(pRenderWindowOwner){
			// Fire keyboard release callbacks
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView KeyUp", e);
		}
	}
}

void igdeNativeBeosRenderView::FrameResized(float width, float height){
	BView::FrameResized(width, height);
	
	try{
		pViewportWidth = (int)width;
		pViewportHeight = (int)height;
		pSyncSizes();
		Invalidate();
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView FrameResized", e);
		}
	}
}

void igdeNativeBeosRenderView::AttachedToWindow(){
	BView::AttachedToWindow();
	
	try{
		if(pRenderWindowOwner){
			// Perform setup now that we're attached to window
			pProcessRenderWindowAttachment();
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView AttachedToWindow", e);
		}
	}
}

void igdeNativeBeosRenderView::DetachedFromWindow(){
	BView::DetachedFromWindow();
	
	try{
		if(pRenderWindowOwner){
			// Clean up when detached
			pRenderWindowAttached = false;
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView DetachedFromWindow", e);
		}
	}
}

void igdeNativeBeosRenderView::WindowActivated(bool active){
	BView::WindowActivated(active);
	
	try{
		if(pRenderWindowOwner){
			// Handle window activation state changes
			if(active){
				// Window became active
			}
			else{
				// Window became inactive
			}
		}
		
	}catch(const deException &e){
		if(pRenderWindowOwner){
			pRenderWindowOwner->GetLogger()->LogException("IGDE RenderView WindowActivated", e);
		}
	}
}


// Private Functions
/////////////////////

void igdeNativeBeosRenderView::pProcessRenderWindowAttachment(){
	// Process attachment of render window from graphics module
	if(pRenderWindowOwner && pCanAttachRenderWindow && IsAttachedToWindow()){
		// Try to attach render window
		AttachRenderWindow();
	}
}

void igdeNativeBeosRenderView::pSyncSizes(){
	// Synchronize sizes with owner
	if(pRenderWindowOwner){
		// Update owner's internal size tracking
	}
}

#endif
