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

#include <dragengine/dragengine_configuration.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef OS_UNIX
#include <unistd.h>
#endif

#include "delLoggerWritePipe.h"
#include "../engine/delEngineProcess.h"

#include <dragengine/common/exceptions.h>



// Class delLoggerWritePipe
/////////////////////////////

// Constructor, destructor
////////////////////////////

#ifdef OS_W32
delLoggerWritePipe::delLoggerWritePipe(HANDLE pipe) :
pPipe(pipe)
#else
delLoggerWritePipe::delLoggerWritePipe(int pipe) :
pPipe(pipe)
#endif
{
}

delLoggerWritePipe::~delLoggerWritePipe(){
}



// Management
///////////////

void delLoggerWritePipe::LogInfo(const char *source, const char *message){
	if(!source){
		DETHROW_INFO(deeNullPointer, "source");
	}
	if(!message){
		DETHROW_INFO(deeNullPointer, "message");
	}
	
	LogToPipe(source, message, delEngineProcess::eltInfo);
}

void delLoggerWritePipe::LogWarn(const char *source, const char *message){
	if(!source){
		DETHROW_INFO(deeNullPointer, "source");
	}
	if(!message){
		DETHROW_INFO(deeNullPointer, "message");
	}
	
	LogToPipe(source, message, delEngineProcess::eltWarn);
}

void delLoggerWritePipe::LogError(const char *source, const char *message){
	if(!source){
		DETHROW_INFO(deeNullPointer, "source");
	}
	if(!message){
		DETHROW_INFO(deeNullPointer, "message");
	}
	
	LogToPipe(source, message, delEngineProcess::eltError);
}



// Protected Functions
////////////////////////

void delLoggerWritePipe::LogToPipe(const char *source, const char *message, int type){
	unsigned short lenMessage = (unsigned short)strlen(message);
	unsigned short lenSource = (unsigned short)strlen(source);
	unsigned char wtype = (unsigned char)type;
	char *data = NULL;
	int dataLen = 0;
	char *dataPtr;
	
	// calculate the length of the required data buffer
	dataLen += sizeof(wtype);
	dataLen += sizeof(lenSource);
	dataLen += lenSource;
	dataLen += sizeof(lenMessage);
	dataLen += lenMessage;
	
	try{
		// create data buffer with the appropriate data
		data = new char[dataLen];
		dataPtr = data;
		
		memcpy(dataPtr, &wtype, sizeof(wtype));
		dataPtr += sizeof(wtype);
		
		memcpy(dataPtr, &lenSource, sizeof(lenSource));
		dataPtr += sizeof(lenSource);
		memcpy(dataPtr, source, lenSource);
		dataPtr += lenSource;
		
		memcpy(dataPtr, &lenMessage, sizeof(lenMessage));
		dataPtr += sizeof(lenMessage);
		memcpy(dataPtr, message, lenMessage);
		dataPtr += lenMessage;
		
		// send data to pipe
		WriteToPipe(data, dataLen);
		
		// clean up
		delete [] data;
		
	}catch(const deException &){
		if(data){
			delete [] data;
		}
		throw;
	}
}

void delLoggerWritePipe::WriteToPipe(const void *data, int length){
#ifdef OS_W32
	DWORD bytesWritten = 0;
	
	if(!WriteFile(pPipe, data, length, &bytesWritten, NULL)){
		DETHROW(deeInvalidAction);
	}
	if((int)bytesWritten < length){
		DETHROW(deeInvalidAction);
	}
	
#else
	if(write(pPipe, data, length) < length){
		DETHROW(deeInvalidAction);
	}
#endif
}
