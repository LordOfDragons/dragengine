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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "delEngineProcess.h"
#include "delEngineInstance.h"
#include "delEngineInstanceReadLog.h"

#include <dragengine/common/exceptions.h>



// Class delEngineInstanceReadLog
////////////////////////////////////

// Constructor, destructor
////////////////////////////

delEngineInstanceReadLog::delEngineInstanceReadLog(delEngineInstance &engineInstance, deLogger *logger) :
pEngineInstance(engineInstance),
pLogger(logger),
pBufferSource(nullptr),
pBufferSourceLen(0),
pBufferMessage(nullptr),
pBufferMessageLen(0)
{
	(void)pEngineInstance;
	if(!logger){
		DETHROW_INFO(deeNullPointer, "logger");
	}
}

delEngineInstanceReadLog::~delEngineInstanceReadLog(){
	if(pBufferMessage){
		free(pBufferMessage);
	}
	if(pBufferSource){
		free(pBufferSource);
	}
}



// Management
///////////////

void delEngineInstanceReadLog::Run(){
	unsigned short length;
	unsigned char type;
	
	while(ReadFromPipe(&type, sizeof(type))){
		ReadFromPipe(&length, sizeof(length));
		PrepareBufferSource(length);
		ReadFromPipe(pBufferSource, length);
		pBufferSource[length] = '\0';
		
		ReadFromPipe(&length, sizeof(length));
		PrepareBufferMessage(length);
		ReadFromPipe(pBufferMessage, length);
		pBufferMessage[length] = '\0';
		
		if(type == delEngineProcess::eltInfo){
			pLogger->LogInfo(pBufferSource, pBufferMessage);
			
		}else if(type == delEngineProcess::eltWarn){
			pLogger->LogWarn(pBufferSource, pBufferMessage);
			
		}else if(type == delEngineProcess::eltError){
			pLogger->LogError(pBufferSource, pBufferMessage);
		}
	}
}



bool delEngineInstanceReadLog::ReadFromPipe(void *data, int length){
	/*
#ifdef OS_W32
	DWORD bytesRead = 0;
	
	return ReadFile(pEngineInstance->GetPipeLog(), data, length, &bytesRead, NULL) && (int)bytesRead == length;
	
#else
	return read(pEngineInstance->GetPipeLog(), data, length) == length;
#endif
	*/
	return false;
}



void delEngineInstanceReadLog::PrepareBufferSource(int requiredLength){
	if(requiredLength <= pBufferSourceLen){
		return;
	}
	
	pBufferSource = reinterpret_cast<char*>(realloc(pBufferSource, requiredLength + 1));
	if(!pBufferSource){
		DETHROW(deeOutOfMemory);
	}
	
	pBufferSourceLen = requiredLength;
}

void delEngineInstanceReadLog::PrepareBufferMessage(int requiredLength){
	if(requiredLength <= pBufferMessageLen){
		return;
	}
	
	pBufferMessage = reinterpret_cast<char*>(realloc(pBufferMessage, requiredLength + 1));
	if(!pBufferMessage){
		DETHROW(deeOutOfMemory);
	}
	
	pBufferMessageLen = requiredLength;
}
