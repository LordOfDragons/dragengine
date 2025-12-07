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
#include <stdarg.h>

#include "deLoggerChain.h"
#include "../common/exceptions.h"



// Class deLoggerChain
////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerChain::deLoggerChain(){
}

deLoggerChain::~deLoggerChain(){
	pLoggerList.RemoveAll();
}



// Management
///////////////

int deLoggerChain::GetLoggerCount(){
	int count;
	pMutex.Lock();
	count = pLoggerList.GetCount();
	pMutex.Unlock();
	return count;
}

deLogger *deLoggerChain::GetLoggerAt(int index){
	deLogger *logger;
	
	pMutex.Lock();
	
	try{
		logger = (deLogger*)pLoggerList.GetAt(index);
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
	
	return logger;
}

void deLoggerChain::AddLogger(deLogger *logger){
	if(!logger){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		pLoggerList.Add(logger);
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::RemoveLogger(deLogger *logger){
	pMutex.Lock();
	
	try{
		pLoggerList.Remove(logger);
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::RemoveAllLoggers(){
	pMutex.Lock();
	
	try{
		pLoggerList.RemoveAll();
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}



void deLoggerChain::LogInfo(const char *source, const char *message){
	if(!source || !message){
		DETHROW(deeInvalidParam);
	}
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		for(i=0; i<count; i++){
			((deLogger*)pLoggerList.GetAt(i))->LogInfo(source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::LogWarn(const char *source, const char *message){
	if(!source || !message) DETHROW(deeInvalidParam);
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			((deLogger*)pLoggerList.GetAt(i))->LogWarn(source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::LogError(const char *source, const char *message){
	if(!source || !message) DETHROW(deeInvalidParam);
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			((deLogger*)pLoggerList.GetAt(i))->LogError(source, message);
		}
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}
