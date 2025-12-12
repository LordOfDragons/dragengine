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

#include "igdeRealApplication.h"
#include "igdeWindowMain.h"

#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeWidget.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>



// Class igdeRealApplication
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeRealApplication::igdeRealApplication(){
}

igdeRealApplication::~igdeRealApplication(){
}



// Management
///////////////

igdeWindowMain &igdeRealApplication::GetWindowMain() const{
	igdeMainWindow * const mainWindow = GetMainWindow();
	DEASSERT_NOTNULL(mainWindow)
	return *((igdeWindowMain*)mainWindow);
}

bool igdeRealApplication::Initialize(const decUnicodeStringList &arguments){
	try{
		SetMainWindow(igdeWindowMain::Ref::New(pEnvironment));
		GetWindowMain().CreateNativeWidget();
		
	// 	if( updateWithTimer ){
	// 		//GetWindowMain().SetMinUpdateTime( 0.0f ); // timer granularity limits already
	// 		GetWindowMain().SetMinUpdateTime( 1.0f / 100.0f - 0.001f ); // 100Hz max update speed, 1ms timer granularity
	// 		
	// 	}else{
			GetWindowMain().SetMinUpdateTime(1.0f / 100.0f); // 100Hz max update speed
	// 	}
		
		// process command line. run application only if it succeeded
		if(!GetWindowMain().ProcessCommandLine(arguments)){
			GetWindowMain().Close();
			return false;
		}
		
	}catch(const deException &e){
		// it is possible creating the window already failed so we have to check
		if(GetMainWindow()){
			GetWindowMain().DisplayException(e);
			
		}else{
			// if is even possible the logger could not be created so check this too
			if(pEnvironment.GetLogger()){
				pEnvironment.GetLogger()->LogException("IGDE", e);
				
			}else{
				e.PrintError();
			}
			
			igdeCommonDialogs::ErrorFormat(nullptr, "Failed starting IGDE",
				"Please see logs for details:\n%s", e.FormatOutput().Join("\n").GetString());
		}
		return false;
	}
	
	return true;
}

void igdeRealApplication::CleanUp(){
}
