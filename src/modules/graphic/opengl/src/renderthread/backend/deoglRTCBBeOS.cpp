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

#include <dragengine/dragengine_configuration.h>

#ifdef OS_BEOS

#include "deoglRTCBBeOS.h"

#include <GLView.h>
#include <dragengine/app/deOSBeOS.h>

#include "../deoglRTContext.h"
#include "../deoglRenderThread.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/resources/rendering/deRenderWindow.h>


// Class deoglRTCBBeOS
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBBeOS::deoglRTCBBeOS(deoglRTContext &context) :
deoglRTCBackend(context),
pOSBeOS(context.GetRenderThread().GetOgl().GetOS()->CastToOSBeOS()){
}

deoglRTCBBeOS::~deoglRTCBBeOS() = default;


// Management
///////////////

void deoglRTCBBeOS::AssignOSWindow(deRenderWindow *renderWindow){
	pOSBeOS->SetWindow(renderWindow->GetWindow());
}

void deoglRTCBBeOS::CleanUp(){
	pOSBeOS->SetWindow(nullptr);
	// context is managed by deoglRRenderWindow. BGLView is the context
}

void deoglRTCBBeOS::BeforeDeactivateContext(deoglRRenderWindow &active){
	active.GetGLView()->UnlockGL();
}

void deoglRTCBBeOS::ActivateContext(deoglRRenderWindow &window){
	window.GetGLView()->LockGL();
}

void deoglRTCBBeOS::SwapBuffers(deoglRRenderWindow &window){
	auto view = window.GetGLView();
	DEASSERT_NOTNULL(view)
	
	view->SwapBuffers(false); // true = sync
	
	// ensure window resizing is properly applied to the opengl context
	view->UnlockGL();
	view->LockGL();
}

#endif // OS_BEOS
