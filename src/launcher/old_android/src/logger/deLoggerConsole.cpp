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

#include "../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "deLoggerConsole.h"
#include "../common/exceptions.h"

#ifdef OS_ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif



// Class deLoggerConsole
//////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerConsole::deLoggerConsole(){
}

deLoggerConsole::~deLoggerConsole(){
}



// Management
///////////////

void deLoggerConsole::LogInfo(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			printf("II [%s] %s", source, message);
			
		}else{
			printf("II [%s] %s\n", source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsole::LogWarn(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			printf("WW [%s] %s", source, message);
			
		}else{
			printf("WW [%s] %s\n", source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsole::LogError(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			printf("EE [%s] %s", source, message);
			
		}else{
			printf("EE [%s] %s\n", source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}
