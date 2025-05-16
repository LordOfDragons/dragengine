/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "../dragengine_configuration.h"

#ifdef OS_WEBWASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include "deOSWebWasm.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"


// Class deOSWebWasm
//////////////////////

// Constructor, Destructor
////////////////////////////

deOSWebWasm::deOSWebWasm() :
pScreenWidth(800),
pScreenHeight(600),
pScreenRefreshRate(30),
pScaleFactor(100),
pCurWindow(nullptr),
pHostingMainWindow(nullptr),
pHostingRenderWindow(nullptr),
pAppFrozen(false)
{
	try{
		pGetOSParameters();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deOSWebWasm::~deOSWebWasm(){
	pCleanUp();
}



// Management
///////////////

decString deOSWebWasm::GetPathEngine(){
	return "/dragengine/lib";
}

decString deOSWebWasm::GetPathShare(){
	return "/dragengine/share";
}

decString deOSWebWasm::GetPathSystemConfig(){
	return "/dragengine/etc";
}

decString deOSWebWasm::GetPathUserConfig(){
	return "/dragengine/userConfig";
}

decString deOSWebWasm::GetPathUserCache(){
	return "/dragengine/userCache";
}

decString deOSWebWasm::GetPathUserCapture(){
	return "/dragengine/userCapture";
}

void deOSWebWasm::ProcessEventLoop(bool sendToInputModule){
	// not supported yet
}

decString deOSWebWasm::GetUserLocaleLanguage(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			return ls.GetLeft(deli).GetLower();
			
		}else{
			return ls.GetLower();
		}
	}
	return "en";
}

decString deOSWebWasm::GetUserLocaleTerritory(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			const int deli2 = ls.Find('.', deli + 1);
			if(deli2 != -1){
				return ls.GetMiddle(deli + 1, deli2).GetLower();
				
			}else{
				return ls.GetMiddle(deli + 1).GetLower();
			}
			
		}else{
			return ls.GetLower();
		}
	}
	return "";
}



// Display information
////////////////////////

int deOSWebWasm::GetDisplayCount(){
	return 1;
}

decPoint deOSWebWasm::GetDisplayCurrentResolution(int display){
	DEASSERT_TRUE(display == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSWebWasm::GetDisplayCurrentRefreshRate(int display){
	DEASSERT_TRUE(display == 0)
	
	return pScreenRefreshRate;
}

int deOSWebWasm::GetDisplayResolutionCount(int display){
	DEASSERT_TRUE(display == 0)
	
	return 1;
}

decPoint deOSWebWasm::GetDisplayResolution(int display, int resolution){
	DEASSERT_TRUE(display == 0)
	DEASSERT_TRUE(resolution == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSWebWasm::GetDisplayCurrentScaleFactor(int display){
	DEASSERT_TRUE(display == 0)
	
	return pScaleFactor;
}



// Casting
////////////

deOSWebWasm *deOSWebWasm::CastToOSWebWasm(){
	return this;
}

// Web WASM related
/////////////////////

void deOSWebWasm::SetWindow(void *wnd){
	pCurWindow = wnd;
}

void deOSWebWasm::SetHostingMainWindow(void *window){
	pHostingMainWindow = window;
}

void deOSWebWasm::SetHostingRenderWindow(void *window){
	pHostingRenderWindow = window;
}

bool deOSWebWasm::HasHostingMainWindow() const{
	return pHostingMainWindow != nullptr;
}

bool deOSWebWasm::HasHostingRenderWindow() const{
	return pHostingRenderWindow != nullptr;
}



void deOSWebWasm::SetAppFrozen(bool frozen){
	pAppFrozen = frozen;
}

void deOSWebWasm::SetContentRect(const decBoundary &rect){
	pContentRect = rect;
}



// Private Functions
//////////////////////

void deOSWebWasm::pCleanUp(){
}

void deOSWebWasm::pGetOSParameters(){
	setlocale(LC_ALL, "");
}

#endif
