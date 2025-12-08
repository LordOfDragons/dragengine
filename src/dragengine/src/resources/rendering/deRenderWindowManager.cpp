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

#include "deRenderWindow.h"
#include "deRenderWindowManager.h"
#include "../../deEngine.h"
#include "../../systems/deGraphicSystem.h"
#include "../../common/exceptions.h"


// Class deRenderWindowManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindowManager::deRenderWindowManager(deEngine *engine) :
deResourceManager(engine,ertRenderWindow){
	SetLoggingName("render window");
}

deRenderWindowManager::~deRenderWindowManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deRenderWindowManager::GetRenderWindowCount() const{
	return pRenderWindows.GetCount();
}

deRenderWindow *deRenderWindowManager::GetRootRenderWindow() const{
	return (deRenderWindow*)pRenderWindows.GetRoot();
}

deRenderWindow *deRenderWindowManager::CreateRenderWindow(){
	deRenderWindow *renderWindow = NULL;
	
	try{
		renderWindow = new deRenderWindow(this);
		GetGraphicSystem()->LoadRenderWindow(renderWindow);
		pRenderWindows.Add(renderWindow);
		
	}catch(const deException &){
		throw;
	}
	
	return renderWindow;
}

#ifdef OS_ANDROID
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside(void *window)
#define IMPLEMENT_CREATERENDERWINDOWINSIDE

#elif defined OS_WEBWASM
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside(void *window)
#define IMPLEMENT_CREATERENDERWINDOWINSIDE

#elif defined OS_BEOS
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside(BWindow *window)
#define IMPLEMENT_CREATERENDERWINDOWINSIDE

#elif defined OS_UNIX_X11
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside(Window window)
#define IMPLEMENT_CREATERENDERWINDOWINSIDE

#elif defined OS_W32
deRenderWindow *deRenderWindowManager::CreateRenderWindowInside(HWND window)
#define IMPLEMENT_CREATERENDERWINDOWINSIDE
#endif

#ifdef IMPLEMENT_CREATERENDERWINDOWINSIDE
{
	deRenderWindow *renderWindow = nullptr;
	
	try{
		renderWindow = new deRenderWindow(this);
		renderWindow->SetHostWindow(window); // tells graphic module to created hosted window
		GetGraphicSystem()->LoadRenderWindow(renderWindow);
		pRenderWindows.Add(renderWindow);
		
	}catch(const deException &){
		throw;
	}
	
	return renderWindow;
}
#endif



void deRenderWindowManager::ReleaseLeakingResources(){
	if(GetRenderWindowCount() > 0){
		LogWarnFormat("%i leaking render windows", GetRenderWindowCount());
		pRenderWindows.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deRenderWindowManager::SystemGraphicLoad(){
	deRenderWindow *renderWindow = (deRenderWindow*)pRenderWindows.GetRoot();
	
	while(renderWindow){
		if(!renderWindow->GetPeerGraphic()){
			GetGraphicSystem()->LoadRenderWindow(renderWindow);
		}
		
		renderWindow = (deRenderWindow*)renderWindow->GetLLManagerNext();
	}
}

void deRenderWindowManager::SystemGraphicUnload(){
	deRenderWindow *renderWindow = (deRenderWindow*)pRenderWindows.GetRoot();
	
	while(renderWindow){
		renderWindow->SetPeerGraphic(NULL);
		renderWindow = (deRenderWindow*)renderWindow->GetLLManagerNext();
	}
}

void deRenderWindowManager::RemoveResource(deResource *resource){
	pRenderWindows.RemoveIfPresent(resource);
}
