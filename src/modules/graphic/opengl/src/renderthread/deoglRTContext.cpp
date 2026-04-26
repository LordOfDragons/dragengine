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

#include "deoglRTContext.h"
#include "deoglRenderThread.h"
#include "deoglLoaderThread.h"
#include "deoglRTLogger.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"
#include "../canvas/deoglCanvasView.h"
#ifdef BACKEND_OPENGL
	#include "../extensions/deoglExtensions.h"
#	ifdef OS_UNIX_X11
#		include "backend/deoglRTCBUnixX11GLX.h"
#		include "backend/deoglRTCBUnixX11EGL.h"
#	endif
#endif // BACKEND_OPENGL
#include "../window/deoglRenderWindow.h"
#include "../window/deoglRRenderWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>


// Class deoglRTContext
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTContext::deoglRTContext(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pActiveRRenderWindow(nullptr),
pUserRequestedQuit(false),
pAppActivated(true)
{
#ifdef OS_UNIX_X11
#	ifdef BACKEND_OPENGL
	{
	renderThread.GetLogger().LogInfo("deoglRTContext: Try creating EGL backend");
	auto backend = deTUniqueReference<deoglRTCBUnixX11EGL>::New(*this);
	if(backend->TryInit()){
		pBackend = std::move(backend);
	}
	}
	
	if(!pBackend){
		renderThread.GetLogger().LogInfo("deoglRTContext: Try creating GLX backend");
		auto backend = deTUniqueReference<deoglRTCBUnixX11GLX>::New(*this);
		if(backend->TryInit()){
			pBackend = std::move(backend);
		}
	}
	
	if(!pBackend){
		DETHROW_INFO(deeInvalidAction, "No supported OpenGL backend found");
	}
	
#	elif defined(BACKEND_VULKAN)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating Vulkan backend");
	pBackend = deTUniqueReference<deoglRTCBUnixX11Vulkan>::New(*this);
#	endif
	
#elif defined(OS_ANDROID)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating Android backend");
	pBackend = deTUniqueReference<deoglRTCBAndroid>::New(*this);
	
#elif defined(OS_WEBWASM)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating WebAssembly backend");
	pBackend = deTUniqueReference<deoglRTCBWasm>::New(*this);
	
#elif defined(OS_BEOS)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating BeOS backend");
	pBackend = deTUniqueReference<deoglRTCBBeOS>::New(*this);
	
#elif defined(OS_MACOS)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating MacOS backend");
	pBackend = deTUniqueReference<deoglRTCBMacOS>::New(*this);
	
#elif defined(OS_W32)
	renderThread.GetLogger().LogInfo("deoglRTContext: Creating Windows backend");
	pBackend = deTUniqueReference<deoglRTCBWindows>::New(*this);
#endif
}

deoglRTContext::~deoglRTContext() = default;


// Management
///////////////

void deoglRTContext::InitPhase1(deRenderWindow*){
	// NOTE called from the main thread after creating the context. do here everything required to be
	//      linked to the main thread and required by render thread initialization.
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 1 Entering");
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 1 Exiting");
}

void deoglRTContext::InitPhase2(deRenderWindow*){
	// NOTE called from the render thread after starting initialization. do here everything required
	//      to be linked to the render thread after prerequisited have been done in the main thread
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 2 Entering");
	pBackend->InitPhase2();
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 2 Exiting");
}

void deoglRTContext::InitPhase3(deRenderWindow *renderWindow){
	DEASSERT_NOTNULL(renderWindow)
	
	// NOTE called from the main thread after first thread initialization. do here everything required to be
	//      linked to the main thread that requires some render thread objects to be present
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 3 Entering");
	
	pBackend->InitPhase3();
	
	// set the render window. it should not have a peer yet so we create one.
	// this includes also the attached primary render target. this has to be done in the main
	// thread since all peer creation happens in the main thread. especially the window creation
	// triggered below has to be in the main thread to keep all window handling there
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	
	if(!renderWindow->GetCanvasView()->GetPeerGraphic()){
		renderWindow->GetCanvasView()->SetPeerGraphic(new deoglCanvasView(ogl, *renderWindow->GetCanvasView()));
	}
	
	if(!renderWindow->GetPeerGraphic()){
		// deoglRenderWindow is calling CreateWindow. this one is important to be in the main thread
		renderWindow->SetPeerGraphic(new deoglRenderWindow(ogl, *renderWindow));
	}
	
	if(!renderWindow->GetHostWindow()){
		((deoglRenderWindow*)renderWindow->GetPeerGraphic())->GetRRenderWindow()->CenterOnScreen();
	}
	
	// assign OS window
	pBackend->AssignOSWindow(renderWindow);
	
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 3 Exiting");
}

void deoglRTContext::InitPhase4(deRenderWindow *renderWindow){
	// NOTE called from the render thread to finish thread initialization. do here everything required to be
	//      linked to the render thread that requires some in between main thread objects to be present
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 4 Entering");
	
	// activate render window
	ActivateRRenderWindow(((deoglRenderWindow*)renderWindow->GetPeerGraphic())->GetRRenderWindow());
	
	pBackend->InitPhase4();
	
	pRenderThread.GetLogger().LogInfo("RTContext Init Phase 4 Exiting");
}

void deoglRTContext::CleanUp(){
	// this is called in the render thread but during cleanup where the main thread is waiting for
	// the render thread. thus certain main thread access is allowed here
	
	// ensure the active deoglRRenderWindow is dropped. if there are still deoglRRenderWindow
	// around this will not work but at this point in time no deoglRRenderWindow should exist 
	// anymore in the render thread otherwise something is wrong
	if(pRenderThread.GetRRenderWindowList().IsNotEmpty()){
		pRenderThread.GetLogger().LogWarnFormat(
			"deoglRTContext.CleanUp: Render thread still has %d RRenderWindows!",
			pRenderThread.GetRRenderWindowList().GetCount());
	}
	
	// we need to forcefully drop the active window in case the list above is not empty otherwise
	// we end up with another window being active
	ActivateRRenderWindow(nullptr, true);
	
	pBackend->CleanUp();
	pBackend.Clear();
}

void deoglRTContext::ActivateRRenderWindow(deoglRRenderWindow *rrenderWindow, bool forceIfNull){
	// NULL should be prevented for the time being. pick the first available window if possible
	if(!rrenderWindow && pRenderThread.GetRRenderWindowList().GetCount() > 0 && !forceIfNull){
		rrenderWindow = (deoglRRenderWindow*)pRenderThread.GetRRenderWindowList().GetAt(0);
	}
	
	if(pActiveRRenderWindow == rrenderWindow){
		return;
	}
	
	if(pActiveRRenderWindow){
		pBackend->BeforeDeactivateContext(*pActiveRRenderWindow);
	}
	
	pActiveRRenderWindow = rrenderWindow;
	
	if(rrenderWindow){
		pBackend->ActivateContext(*rrenderWindow);
		
	}else{
		pRenderThread.GetLoaderThread().EnableContext(false);
		pBackend->DeactivateContext();
	}
}

bool deoglRTContext::GetUserRequestedQuit(){
	const bool requested = pUserRequestedQuit;
	pUserRequestedQuit = false;
	return requested;
}

void deoglRTContext::SetUserRequestedQuit(bool userRequestedQuit){
	pUserRequestedQuit = userRequestedQuit;
}
