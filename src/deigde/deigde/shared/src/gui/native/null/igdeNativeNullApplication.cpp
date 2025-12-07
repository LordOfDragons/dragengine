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

#ifdef IGDE_TOOLKIT_NULL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeNullApplication.h"
#include "igdeNativeNullWindow.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../igdeWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/logger/deLogger.h>

#ifdef OS_W32
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#endif



// Class igdeNativeNullApplication
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullApplication::igdeNativeNullApplication(igdeApplication &owner) :
pOwner(owner),
pQuitRequested(false){
}

igdeNativeNullApplication::~igdeNativeNullApplication(){
}

igdeNativeNullApplication *igdeNativeNullApplication::CreateNativeApplication(igdeApplication &application){
	return new igdeNativeNullApplication(application);
}

void igdeNativeNullApplication::DestroyNativeApplication(){
	delete this;
}



// Management
///////////////

#ifdef OS_UNIX
void igdeNativeNullApplication::GetOSStartUpArguments(decUnicodeStringList &arguments, int argCount, char **args){
	int i;
	for(i=1; i<argCount; i++){
		arguments.Add(decUnicodeString::NewFromUTF8(args[i]));
	}
}

#elif defined OS_W32
void igdeNativeNullApplication::GetOSStartUpArguments(decUnicodeStringList &arguments,
const decUnicodeArgumentList &windowsArguments){
	const int count = windowsArguments.GetArgumentCount();
	int i;
	for(i=1; i<count; i++){
		arguments.Add(*windowsArguments.GetArgumentAt(i));
	}
}
#endif

void igdeNativeNullApplication::Initialize(decUnicodeStringList&){
}

void igdeNativeNullApplication::Run(){
	while(!pQuitRequested){
		igdeMainWindow * const mainWindow = pOwner.GetMainWindow();
		if(mainWindow){
			mainWindow->OnFrameUpdate();
		}
	}
}

void igdeNativeNullApplication::Quit(){
	pQuitRequested = true;
}

decColor igdeNativeNullApplication::GetSystemColor(igdeEnvironment::eSystemColors color) const{
	switch(color){
	case igdeEnvironment::escWindowForeground:
	case igdeEnvironment::escWidgetForeground:
	case igdeEnvironment::escWidgetSelectedBackground:
		return decColor(0.0f, 0.0f, 0.0f);
		
	case igdeEnvironment::escWidgetBackground:
		return decColor(0.65f, 0.65f, 0.65f);
		
	case igdeEnvironment::escWidgetHighlight:
		return decColor(0.9f, 0.9f, 0.9f);
		
	case igdeEnvironment::escWidgetShadow:
		return decColor(0.2f, 0.2f, 0.2f);
		
	case igdeEnvironment::escWindowBackground:
	case igdeEnvironment::escWidgetSelectedForeground:
	default:
		return decColor(1.0f, 1.0f, 1.0f);
	}
}

void igdeNativeNullApplication::GetAppFontConfig(igdeFont::sConfiguration &config) const{
	config.name = "Courier";
	config.size = 12.0;
	config.bold = false;
	config.italic = false;
	config.underline = false;
	config.strikeThrough = false;
}

void igdeNativeNullApplication::ShowError(const deException &exception) const{
	pOwner.GetMainWindow()->GetLogger()->LogError("IGDE", exception.FormatOutput().Join("\n"));
}

void igdeNativeNullApplication::RunModalWhileShown(igdeWindow &window){
	igdeNativeNullWindow * const native = (igdeNativeNullWindow*)window.GetNativeWidget();
	if(!native){
		DETHROW(deeInvalidParam);
	}
	
	while(!pQuitRequested && !native->GetIsClosed()){
		igdeMainWindow * const mainWindow = pOwner.GetMainWindow();
		if(mainWindow){
			mainWindow->OnFrameUpdate();
		}
		
		native->OnFrameUpdate();
	}
}

int igdeNativeNullApplication::GetDisplayScaleFactor(){
	return 100;
}

#endif
