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

#include "deoglRTCBUnixX11.h"

#ifdef OS_UNIX_X11

#include <dragengine/app/deOSUnix.h>

#include "../deoglRTContext.h"
#include "../deoglRTLogger.h"
#include "../deoglRenderThread.h"
#include "../../window/deoglRRenderWindow.h"
#include "../../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rendering/deRenderWindow.h>


// Class deoglRTCBUnixX11
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBUnixX11::deoglRTCBUnixX11(deoglRTContext &context) :
deoglRTCBackend(context),
pOSUnix(context.GetRenderThread().GetOgl().GetOS()->CastToOSUnix()),
pDisplay(nullptr),
pScreen(0),
pColMap(0),
pVisInfo(nullptr),
pAtomProtocols(0),
pAtomDeleteWindow(0){
}

deoglRTCBUnixX11::~deoglRTCBUnixX11() = default;


// Management
///////////////

void deoglRTCBUnixX11::InitPhase2(){
	pOpenDisplay();
	pCreateAtoms();
	pChooseVisual();
	pCreateColorMap();
}

void deoglRTCBUnixX11::CleanUp(){
	AssignOSWindow(nullptr);
	pFreeVisualInfo();
	pCloseDisplay();
}

void deoglRTCBUnixX11::AssignOSWindow(deRenderWindow *renderWindow){
	pOSUnix->SetWindow(renderWindow ? renderWindow->GetWindow() : 0);
}


void deoglRTCBUnixX11::ProcessEventLoop(){
	XEvent event;
	while(XPending(pDisplay)){
		XNextEvent(pDisplay, &event);
		
		switch(event.type){
		case ClientMessage:
			if(event.xclient.message_type == pAtomProtocols
			&& event.xclient.data.l[0] == (int)pAtomDeleteWindow){
				pRTContext.SetUserRequestedQuit(true);
			}
			break;
			
		case FocusIn:
			pRTContext.SetAppActivated(true);
			break;
			
		case FocusOut:
			pRTContext.SetAppActivated(false);
			break;
			
		case ConfigureNotify:{
			deoglRRenderWindow * const window = pRTContext.GetRenderThread().
				GetRRenderWindowList().FindOrDefault([&](const deoglRRenderWindow &w){
					return w.GetWindow() == event.xconfigure.window;
				});
			if(window){
				window->OnResize(event.xconfigure.width, event.xconfigure.height);
			}
			}break;
		}
	}
}

Display *deoglRTCBUnixX11::GetMainThreadDisplay() const{
	return pOSUnix->GetDisplay();
}


// Protected Functions
///////////////////////

void deoglRTCBUnixX11::pFreeVisualInfo(){
	if(pVisInfo){
		XFree(pVisInfo);
		pVisInfo = nullptr;
	}
}

void deoglRTCBUnixX11::pOpenDisplay(){
	const char *dispName = getenv("DISPLAY");
	
	if(!dispName){
		dispName = "";
	}
	pDisplay = XOpenDisplay(dispName);
	if(!pDisplay){
		pRTContext.GetRenderThread().GetLogger().LogErrorFormat(
			"Cannot not open display %s\n", XDisplayName(dispName));
		DETHROW(deeInvalidAction);
	}
	
	pScreen = XDefaultScreen(pDisplay);
}

void deoglRTCBUnixX11::pCreateColorMap(){
	Window rootWindow = RootWindow(pDisplay, pVisInfo->screen);
	pColMap = XCreateColormap(pDisplay, rootWindow, pVisInfo->visual, AllocNone);
}

void deoglRTCBUnixX11::pCreateAtoms(){
	pAtomProtocols = XInternAtom(pDisplay, "WM_PROTOCOLS", False);
	pAtomDeleteWindow = XInternAtom(pDisplay, "WM_DELETE_WINDOW", False);
}

void deoglRTCBUnixX11::pCloseDisplay(){
	if(pDisplay){
		XCloseDisplay(pDisplay);
		pDisplay = nullptr;
	}
}

#endif // OS_UNIX_X11
