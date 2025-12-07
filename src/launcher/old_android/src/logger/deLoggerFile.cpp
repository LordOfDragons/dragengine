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

#include "deLoggerFile.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/exceptions.h"
#include "../common/string/decString.h"



// Class deLoggerFile
///////////////////////

// Constructor, destructor
////////////////////////////

deLoggerFile::deLoggerFile(decBaseFileWriter *writer) :
pWriter(NULL)
{
	if(! writer){
		DETHROW(deeInvalidParam);
	}
	
	pWriter = writer;
	writer->AddReference();
}

deLoggerFile::~deLoggerFile(){
	if(pWriter){
		pWriter->FreeReference();
	}
}



// Management
///////////////

void deLoggerFile::LogInfo(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	const int len = strlen(message);
	decString string;
	
	if(len == 0 || message[len - 1] != '\n'){
		string.Format("II [%s] %s\n", source, message);
		
	}else{
		string.Format("II [%s] %s", source, message);
	}
	
	pMutex.Lock();
	
	try{
		pWriter->Write(string.GetString(), string.GetLength());
		fflush(NULL);
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerFile::LogWarn(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	const int len = strlen(message);
	decString string;
	
	if(len == 0 || message[len - 1] != '\n'){
		string.Format("WW [%s] %s\n", source, message);
		
	}else{
		string.Format("WW [%s] %s", source, message);
	}
	
	pMutex.Lock();
	
	try{
		pWriter->Write(string.GetString(), string.GetLength());
		fflush(NULL);
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerFile::LogError(const char *source, const char *message){
	if(! source || ! message){
		DETHROW(deeInvalidParam);
	}
	
	const int len = strlen(message);
	decString string;
	
	if(len == 0 || message[len - 1] != '\n'){
		string.Format("EE [%s] %s\n", source, message);
		
	}else{
		string.Format("EE [%s] %s", source, message);
	}
	
	pMutex.Lock();
	
	try{
		pWriter->Write(string.GetString(), string.GetLength());
		fflush(NULL);
		
		pMutex.Unlock();
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
}
