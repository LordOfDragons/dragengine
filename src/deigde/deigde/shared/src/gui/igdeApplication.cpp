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

#include "igdeApplication.h"
#include "igdeMainWindow.h"
#include "native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Static
///////////

igdeApplication *igdeApplication::pApp = NULL;



// Class igdeApplication
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeApplication::igdeApplication() :
pNativeApplication(NULL)
{
	DEASSERT_NULL(pApp)
	
	pApp = this;
	pNativeApplication = igdeNativeApplication::CreateNativeApplication(*this);
}

igdeApplication::~igdeApplication(){
	if(pNativeApplication){
		((igdeNativeApplication*)pNativeApplication)->DestroyNativeApplication();
	}
	
	if(pApp == this){
		pApp = NULL;
	}
}



// Management
///////////////

igdeMainWindow *igdeApplication::GetMainWindow() const{
	return (igdeMainWindow*)(igdeWidget*)pMainWindow;
}

#ifdef OS_UNIX
void igdeApplication::Run(int argCount, char **args){
	decUnicodeStringList arguments;
	igdeNativeApplication::GetOSStartUpArguments(arguments, argCount, args);
	
	pSharedRun(arguments);
}

#elif defined OS_W32
void igdeApplication::Run(){
	// quick note on why we use GetCommandLineW() here and not the value from wWinMain.
	// MinGW does not support wWinMain so it has to be faked by implementing WinMain which
	// calls GetCommandLineW(). This can be unfortunatly not fed directly to wWinMain
	// because GetCommandLineW() includes the executable name as first argument whereas
	// CRT calls wWinMain without the executable name as first argument. This incompatibility
	// makes it hard to do this properly. Instead this class simply ignores the wWinMain
	// provided command line value and fetches GetCommandLineW. ugly but it works
	decUnicodeArgumentList windowsArguments;
	windowsArguments.ParseCommand(deOSWindows::WideToUnicode(GetCommandLineW()));
	
	decUnicodeStringList arguments;
	igdeNativeApplication::GetOSStartUpArguments(arguments, windowsArguments);
	
	pSharedRun(arguments);
}

#else
#error "Unsupported OS"
#endif

decColor igdeApplication::GetSystemColor(igdeEnvironment::eSystemColors color) const{
	DEASSERT_NOTNULL(pNativeApplication)
	return ((igdeNativeApplication*)pNativeApplication)->GetSystemColor(color);
}

void igdeApplication::GetAppFontConfig(igdeFont::sConfiguration &config){
	DEASSERT_NOTNULL(pNativeApplication)
	((igdeNativeApplication*)pNativeApplication)->GetAppFontConfig(config);
}

igdeApplication &igdeApplication::app(){
	DEASSERT_NOTNULL(pApp)
	return *pApp;
}

void igdeApplication::RunModalWhileShown(igdeWindow &window){
	DEASSERT_NOTNULL(pNativeApplication)
	((igdeNativeApplication*)pNativeApplication)->RunModalWhileShown(window);
}


int igdeApplication::GetDisplayScaleFactor(){
	DEASSERT_NOTNULL(pNativeApplication)
	return ((igdeNativeApplication*)pNativeApplication)->GetDisplayScaleFactor();
}

float igdeApplication::GetDisplayScaleFactorFloat(){
	return (float)GetDisplayScaleFactor() / 100.0f;
}

float igdeApplication::DisplayScaled(float scalar){
	return scalar * GetDisplayScaleFactorFloat();
}

int igdeApplication::DisplayScaled(int value){
	return (int)(DisplayScaled((float)value) + 0.5f);
}

decPoint igdeApplication::DisplayScaled(const decPoint &point){
	return DisplayScaled(decVector2(point)).Round();
}

decVector2 igdeApplication::DisplayScaled(const decVector2 &point){
	return point * GetDisplayScaleFactorFloat();
}


// Protected Functions
////////////////////////

void igdeApplication::SetMainWindow(igdeMainWindow *mainWindow, bool takeOver){
	if(takeOver){
		pMainWindow.TakeOver(mainWindow);
		
	}else{
		pMainWindow = mainWindow;
	}
}

void igdeApplication::CleanUp(){
}



// Private Functions
//////////////////////

void igdeApplication::pSharedRun(decUnicodeStringList &arguments){
	try{
		((igdeNativeApplication*)pNativeApplication)->Initialize(arguments);
		
		if(Initialize(arguments)){
			((igdeNativeApplication*)pNativeApplication)->Run();
		}
		
	}catch(const deException &e){
		((igdeNativeApplication*)pNativeApplication)->ShowError(e);
		
		try{
			CleanUp();
			pMainWindow = NULL;
			
		}catch(const deException &e2){
			e2.PrintError();
			((igdeNativeApplication*)pNativeApplication)->ShowError(e2);
		}
		
		((igdeNativeApplication*)pNativeApplication)->Quit();
		throw;
	}
	
	CleanUp();
	pMainWindow = NULL;
	
	((igdeNativeApplication*)pNativeApplication)->Quit();
}
