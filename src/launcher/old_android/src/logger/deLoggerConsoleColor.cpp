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

#include "deLoggerConsoleColor.h"
#include "../common/exceptions.h"

#ifdef OS_ANDROID
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif



// Defines
////////////

/*
#define ATTR_RESET		0
#define ATTR_BRIGHT		1
#define ATTR_DIM		2
#define ATTR_UNDERLINE 	3
#define ATTR_BLINK		4
#define ATTR_REVERSE	7
#define ATTR_HIDDEN		8

#define FG_BLACK 		30
#define FG_RED			31
#define FG_GREEN		32
#define FG_YELLOW		33
#define FG_BLUE			34
#define FG_MAGENTA		35
#define FG_CYAN			36
#define	FG_WHITE		37

#define BG_BLACK 		40
#define BG_RED			41
#define BG_GREEN		42
#define BG_YELLOW		43
#define BG_BLUE			44
#define BG_MAGENTA		45
#define BG_CYAN			46
#define	BG_WHITE		47
*/

static const int vColorTableForeground[] = {
	0,  // etcDefault
	30, // etcBlack
	31, // etcRed
	32, // etcGreen
	33, // etcYellow
	34, // etcBlue
	35, // etcMagenta
	36, // etcCyan
	37  // etcWhite
};

static const int vColorTableBackground[] = {
	0,  // etcDefault
	40, // etcBlack
	41, // etcRed
	42, // etcGreen
	43, // etcYellow
	44, // etcBlue
	45, // etcMagenta
	46, // etcCyan
	47  // etcWhite
};



// Class deLoggerConsoleColor
///////////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerConsoleColor::deLoggerConsoleColor() :
pInfoAttribute(etaDefault),
pInfoColorForeground(etcDefault), //(etcWhite),
pInfoColorBackground(etcDefault), //(etcBlack),

pWarnAttribute(etaBright),
pWarnColorForeground(etcYellow),
pWarnColorBackground(etcBlack),

pErrorAttribute(etaBright),
pErrorColorForeground(etcRed),
pErrorColorBackground(etcBlack){
}

deLoggerConsoleColor::~deLoggerConsoleColor(){
}



// Management
///////////////

deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetInfoAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pInfoAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetInfoAttribute(eTextAttributes attribute){
	if(attribute < etaDefault || attribute > etaBlink){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pInfoAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pInfoColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetInfoColorForeground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pInfoColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetInfoColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pInfoColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetInfoColorBackground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pInfoColorBackground = color;
	pMutex.Unlock();
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetWarnAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pWarnAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetWarnAttribute(eTextAttributes attribute){
	if(attribute < etaDefault || attribute > etaBlink){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pWarnAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pWarnColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetWarnColorForeground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pWarnColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetWarnColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pWarnColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetWarnColorBackground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pWarnColorBackground = color;
	pMutex.Unlock();
}



deLoggerConsoleColor::eTextAttributes deLoggerConsoleColor::GetErrorAttribute(){
	eTextAttributes attribute;
	pMutex.Lock();
	attribute = pErrorAttribute;
	pMutex.Unlock();
	return attribute;
}

void deLoggerConsoleColor::SetErrorAttribute(eTextAttributes attribute){
	if(attribute < etaDefault || attribute > etaBlink){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pErrorAttribute = attribute;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorForeground(){
	eTextColors color;
	pMutex.Lock();
	color = pErrorColorForeground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetErrorColorForeground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pErrorColorForeground = color;
	pMutex.Unlock();
}

deLoggerConsoleColor::eTextColors deLoggerConsoleColor::GetErrorColorBackground(){
	eTextColors color;
	pMutex.Lock();
	color = pErrorColorBackground;
	pMutex.Unlock();
	return color;
}

void deLoggerConsoleColor::SetErrorColorBackground(eTextColors color){
	if(color < etcDefault || color > etcWhite){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	pErrorColorBackground = color;
	pMutex.Unlock();
}



void deLoggerConsoleColor::LogInfo(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			pPrintMessage(source, message, pInfoAttribute, pInfoColorForeground, pInfoColorBackground, 0);
			
		}else{
			pPrintMessage(source, message, pInfoAttribute, pInfoColorForeground, pInfoColorBackground, '\n');
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsoleColor::LogWarn(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			pPrintMessage(source, message, pWarnAttribute, pWarnColorForeground, pWarnColorBackground, 0);
			
		}else{
			pPrintMessage(source, message, pWarnAttribute, pWarnColorForeground, pWarnColorBackground, '\n');
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerConsoleColor::LogError(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int len = strlen(message);
		
		if(len > 0 && message[len - 1] == '\n'){
			pPrintMessage(source, message, pErrorAttribute, pErrorColorForeground, pErrorColorBackground, 0);
			
		}else{
			pPrintMessage(source, message, pErrorAttribute, pErrorColorForeground, pErrorColorBackground, '\n');
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}



// Protected Functions
////////////////////////

void deLoggerConsoleColor::pPrintMessage(const char *source, const char *message,
eTextAttributes attribute, eTextColors colorForeground, eTextColors colorBackground,
char lineEnd) const {
	//if( attribute == etaDefault && colorForeground == etcWhite && colorBackground == etcBlack ){
	if(attribute == etaDefault && colorForeground == etcDefault && colorBackground == etcDefault){
		printf("[%s] %s%c", source, message, lineEnd);
		
	}else{
		printf("\033[%d;%d;%dm[%s] %s\33[0;0;0m%c", attribute,
			vColorTableForeground[colorForeground],
			vColorTableBackground[colorBackground],
			source, message, lineEnd);
	}
}
