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

#ifdef OS_BEOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include <Application.h>
#include <MessageFilter.h>
#include <MessageQueue.h>
#include <Roster.h>
#include <Screen.h>
#include <Window.h>

#include "deOSBeOS.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// Class deOSBeOS
///////////////////

// Constructor, Destructor
////////////////////////////

deOSBeOS::deOSBeOS() :
pScreenWidth(0),
pScreenHeight(0),
pCurWindow(NULL),
pHostingMainWindow(NULL),
pHostingRenderWindow(NULL)
{
	const BRect screenSize(BScreen().Frame());
	pScreenWidth = screenSize.IntegerWidth();
	pScreenHeight = screenSize.IntegerHeight();
	
	// init locale
	setlocale(LC_ALL, "");
}

deOSBeOS::~deOSBeOS(){
	pCleanUp();
}



// Management
///////////////

decString deOSBeOS::GetPathEngine(){
	const char * const envPath = getenv("DE_ENGINE_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(DE_ENGINE_PATH);
}

decString deOSBeOS::GetPathShare(){
	const char * const envPath = getenv("DE_SHARE_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(DE_SHARE_PATH);
}

decString deOSBeOS::GetPathSystemConfig(){
	const char * const envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(DE_CONFIG_PATH);
}

decString deOSBeOS::GetPathUserConfig(){
	const char * const envPath = getenv("DE_CONFIG_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(pGetHomeDirectory() + "/config/settings/dragengine");
}

decString deOSBeOS::GetPathUserCache(){
	const char * const envPath = getenv("DE_CACHE_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(pGetHomeDirectory() + "/config/cache/dragengine");
}

decString deOSBeOS::GetPathUserCapture(){
	const char * const envPath = getenv("DE_CAPTURE_PATH");
	if(envPath){
		return decString(envPath);
	}
	return decString(GetPathUserCache() + "/config/capture");
}



// Display information
////////////////////////

int deOSBeOS::GetDisplayCount(){
	return 1;
}

decPoint deOSBeOS::GetDisplayCurrentResolution(int display){
	DEASSERT_TRUE(display == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSBeOS::GetDisplayCurrentRefreshRate(int display){
	DEASSERT_TRUE(display == 0)
	
	return 30;
}

int deOSBeOS::GetDisplayResolutionCount(int display){
	DEASSERT_TRUE(display == 0)
	
	return 1;
}

decPoint deOSBeOS::GetDisplayResolution(int display, int resolution){
	DEASSERT_TRUE(display == 0)
	DEASSERT_TRUE(resolution == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSBeOS::GetDisplayCurrentScaleFactor(int display){
	DEASSERT_TRUE(display == 0)
	
	return 100;
}



// Casting
////////////

deOSBeOS *deOSBeOS::CastToOSBeOS(){
	return this;
}



// Unix related
/////////////////

void deOSBeOS::SetWindow(BWindow *window){
	// check if there is a window at the moment
	if(pCurWindow){
		// ungrab mouse pointer
	}
	
	// set new window
	pCurWindow = window;
	
	// show new window if not null
	if(pCurWindow){
		pCurWindow->Show();
	}
}

void deOSBeOS::SetHostingMainWindow(BWindow *window){
	pHostingMainWindow = window;
}

void deOSBeOS::SetHostingRenderWindow(BWindow *window){
	pHostingRenderWindow = window;
}

bool deOSBeOS::HasHostingMainWindow() const{
	return pHostingMainWindow != NULL;
}

bool deOSBeOS::HasHostingRenderWindow() const{
	return pHostingRenderWindow != NULL;
}



void deOSBeOS::ProcessEventLoop(bool sendToInputModule){
	BMessage *message = NULL;
	while(true){
		// get next message
		pMessageQueue.Lock();
		message = pMessageQueue.NextMessage();
		pMessageQueue.Unlock();
		if(! message){
			break; // finished
		}
		
		/*const char whatStr[4] = {(char)((message->what>>24)&0xff), (char)((message->what>>16)&0xff),
			(char)((message->what>>8)&0xff), (char)(message->what&0xff)};
		printf("OS.ProcessEventLoop: procees %.4s\n", (char*)whatStr);*/
		
		// process message
		try{
			switch(message->what){
			case B_APP_ACTIVATED:
				SetAppActive(message->GetBool("active", true));
				break;
				
			case B_QUIT_REQUESTED:
				GetEngine()->Quit();
				break;
				
			default:
				break;
			}
			
			if(sendToInputModule){
				GetEngine()->GetInputSystem()->GetActiveModule()->EventLoop(*message);
			}
			
		}catch(const deException &e){
			if(message){
				delete message;
			}
			throw;
		}
		
		delete message;
		message = NULL;
	}
}

void deOSBeOS::MessageReceived(BMessage *message){
	pMessageQueue.Lock();
	pMessageQueue.AddMessage(new BMessage(*message));
	pMessageQueue.Unlock();
}

decString deOSBeOS::GetUserLocaleLanguage(){
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

decString deOSBeOS::GetUserLocaleTerritory(){
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



// Private Functions
//////////////////////

void deOSBeOS::pCleanUp(){
}

decString deOSBeOS::pGetHomeDirectory(){
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

#endif
