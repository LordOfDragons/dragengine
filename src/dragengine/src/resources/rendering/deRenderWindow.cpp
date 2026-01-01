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

#ifdef OS_W32
#include <stdint.h>
#endif

#include "deRenderWindow.h"
#include "deRenderWindowManager.h"
#include "../deResourceManager.h"
#include "../canvas/deCanvasManager.h"
#include "../canvas/deCanvasView.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/graphic/deBaseGraphicRenderWindow.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deRenderWindow
/////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindow::deRenderWindow(deRenderWindowManager *manager) :
deResource(manager),

#ifdef OS_ANDROID
pHostWindow(nullptr),
pWindow(nullptr),

#elif defined OS_WEBWASM
pHostWindow(nullptr),
pWindow(nullptr),

#elif defined OS_BEOS
pHostWindow(nullptr),
pWindow(nullptr),

#elif defined OS_W32
pHostWindow(NULL),
pWindow(NULL),

#elif defined OS_MACOS
pHostWindow(nullptr),
pWindow(nullptr),

#elif defined OS_UNIX_X11
pHostWindow(0),
pWindow(0),
#endif

pTitle("Dragengine"),
pX(0),
pY(0),
pWidth(100),
pHeight(100),
pFullScreen(false),
pScaleFactor(100),
pNotifyScriptOnResize(false),
pPaint(true),

pPeerGraphic(NULL)
{
	pCanvasView = GetEngine()->GetCanvasManager()->CreateCanvasView();
	pCanvasView->SetSize(decPoint(pWidth, pHeight));
}

deRenderWindow::~deRenderWindow(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
}



// Management
///////////////

#ifdef OS_ANDROID
void deRenderWindow::SetHostWindow(void *window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(void *window){
	pWindow = window;
}

#elif defined OS_WEBWASM
void deRenderWindow::SetHostWindow(void *window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(void *window){
	pWindow = window;
}

#elif defined OS_BEOS
void deRenderWindow::SetHostWindow(BWindow *window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(BWindow *window){
	pWindow = window;
}

#elif defined OS_W32
void deRenderWindow::SetHostWindow(HWND window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(HWND window){
	pWindow = window;
}

#elif defined OS_MACOS
void deRenderWindow::SetHostWindow(NSWindow *window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(NSWindow *window){
	pWindow = window;
}

#elif defined OS_UNIX_X11
void deRenderWindow::SetHostWindow(Window window){
	if(window == pHostWindow){
		return;
	}
	
	pHostWindow = window;
	
	if(pPeerGraphic){
		pPeerGraphic->HostWindowChanged();
	}
}

void deRenderWindow::SetWindow(Window window){
	pWindow = window;
}
#endif

void deRenderWindow::SetPosition(int x, int y){
	if(pX == x && pY == y){
		return;
	}
	
	pX = x;
	pY = y;
	
	if(pNotifyScriptOnResize){
		GetResourceManager()->GetScriptingSystem()->OnResizeRenderWindow();
	}
	
	if(pPeerGraphic){
		pPeerGraphic->PositionChanged();
	}
}

void deRenderWindow::SetSize(int width, int height){
	if(width < 0 || height < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pWidth == width && pHeight == height){
		return;
	}
	
	pWidth = width;
	pHeight = height;
	
	pCanvasView->SetSize(decPoint(width, height));
	
	if(pNotifyScriptOnResize){
		GetResourceManager()->GetScriptingSystem()->OnResizeRenderWindow();
	}
	
	if(pPeerGraphic){
		pPeerGraphic->SizeChanged();
	}
}

void deRenderWindow::SetFullScreen(bool fullScreen){
	if(pFullScreen == fullScreen){
		return;
	}
	
	pFullScreen = fullScreen;
	
	if(pPeerGraphic){
		pPeerGraphic->FullScreenChanged();
	}
}

void deRenderWindow::SetScaleFactor(int scaleFactor){
	if(scaleFactor == pScaleFactor){
		return;
	}
	
	pScaleFactor = scaleFactor;
	
	if(pNotifyScriptOnResize){
		GetResourceManager()->GetScriptingSystem()->OnResizeRenderWindow();
	}
}

void deRenderWindow::SetNotifyScriptOnResize(bool notify){
	if(pNotifyScriptOnResize == notify){
		return;
	}
	
	pNotifyScriptOnResize = notify;
	
	if(pPeerGraphic){
		pPeerGraphic->ScriptNotifyChanged();
	}
}

void deRenderWindow::SetPaint(bool paint){
	if(paint == pPaint){
		return;
	}
	
	pPaint = paint;
	
	if(pPeerGraphic){
		pPeerGraphic->PaintEnabledChanged();
	}
}

void deRenderWindow::SetTitle(const char *title){
	if(pTitle == title){
		return;
	}
	
	pTitle = title;
	
	if(pPeerGraphic){
		pPeerGraphic->TitleChanged();
	}
}

void deRenderWindow::SetIcon(deImage *icon){
	if(pIcon == icon){
		return;
	}
	
	pIcon = icon;
	
	if(pPeerGraphic){
		pPeerGraphic->IconChanged();
	}
}



// Graphic system management
//////////////////////////////

void deRenderWindow::SetPeerGraphic(deBaseGraphicRenderWindow *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
