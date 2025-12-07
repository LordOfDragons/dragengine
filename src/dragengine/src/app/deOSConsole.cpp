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
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include "deOSConsole.h"
#include "../common/exceptions.h"



// Class deOSConsole
//////////////////////

// Constructor, destructor
////////////////////////////

deOSConsole::deOSConsole(){
	pScreenWidth = 80; // ncurses?
	pScreenHeight = 60; // ncurses?
	
	// init locale
	setlocale(LC_ALL, "");
}

deOSConsole::~deOSConsole(){
}



// Management
///////////////

decString deOSConsole::GetPathEngine(){
	const char * const envPath = getenv("DE_ENGINE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_ENGINE_PATH);
}

decString deOSConsole::GetPathShare(){
	const char * const envPath = getenv("DE_SHARE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_SHARE_PATH);
}

decString deOSConsole::GetPathSystemConfig(){
	const char * const envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(DE_CONFIG_PATH);
}

decString deOSConsole::GetPathUserConfig(){
	const char *envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("XDG_CONFIG_HOME");
	if(envPath){
		return decString(envPath) + "/dragengine";
	}
	
	return decString(pGetHomeDirectory() + "/.config/dragengine");
}

decString deOSConsole::GetPathUserCache(){
	const char *envPath = getenv("DE_CACHE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("XDG_CACHE_HOME");
	if(envPath){
		return decString(envPath) + "/dragengine";
	}
	
	return decString(pGetHomeDirectory() + "/.cache/dragengine");
}

decString deOSConsole::GetPathUserCapture(){
	const char * const envPath = getenv("DE_CAPTURE_PATH");
	if(envPath){
		return decString(envPath);
	}
	
	return decString(GetPathUserCache() + "/capture");
}



void deOSConsole::ProcessEventLoop(bool sendToInputModule){
	// do nothing for the moment
	/*
	see console input module
	*/
}

decString deOSConsole::GetUserLocaleLanguage(){
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

decString deOSConsole::GetUserLocaleTerritory(){
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

int deOSConsole::GetDisplayCount(){
	return 1;
}

decPoint deOSConsole::GetDisplayCurrentResolution(int display){
	DEASSERT_TRUE(display == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSConsole::GetDisplayCurrentRefreshRate(int display){
	DEASSERT_TRUE(display == 0)
	
	return 30;
}

int deOSConsole::GetDisplayResolutionCount(int display){
	DEASSERT_TRUE(display == 0)
	
	return 1;
}

decPoint deOSConsole::GetDisplayResolution(int display, int resolution){
	DEASSERT_TRUE(display == 0)
	DEASSERT_TRUE(resolution == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSConsole::GetDisplayCurrentScaleFactor(int display){
	DEASSERT_TRUE(display == 0)
	
	return 100;
}



// Casting
////////////

deOSConsole *deOSConsole::CastToOSConsole(){
	return this;
}



// Private Functions
//////////////////////

decString deOSConsole::pGetHomeDirectory(){
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	const char *envPath = getenv("HOME");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("USER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	envPath = getenv("LOGUSER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	DETHROW(deeInvalidParam);
}
