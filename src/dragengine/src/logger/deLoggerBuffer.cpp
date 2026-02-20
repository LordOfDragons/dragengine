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

#include "deLoggerBuffer.h"
#include "../common/exceptions.h"
#include "../common/collection/decTList.h"
#include "../threading/deMutexGuard.h"



// Class deLoggerBuffer::sEntry
/////////////////////////////////

deLoggerBuffer::sEntry::sEntry(){
}

deLoggerBuffer::sEntry::sEntry(const deLoggerBuffer::sEntry &entry) :
type(entry.type),
source(entry.source),
message(entry.message){
}

deLoggerBuffer::sEntry::sEntry(eMessageTypes atype, const decString &asource, const decString &amessage) :
type(atype),
source(asource),
message(amessage){
}

deLoggerBuffer::sEntry &deLoggerBuffer::sEntry::operator=(const deLoggerBuffer::sEntry &entry){
	type = entry.type;
	source = entry.source;
	message = entry.message;
	return *this;
}



// Class deLoggerBuffer
/////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerBuffer::deLoggerBuffer(){
}

deLoggerBuffer::~deLoggerBuffer(){
}



// Management
///////////////

int deLoggerBuffer::GetEntryCount(){
	const deMutexGuard guard(pMutex);
	return pEntries.GetCount();
}

deLoggerBuffer::sEntry deLoggerBuffer::GetEntryAt(int index){
	const deMutexGuard guard(pMutex);
	return pEntries[index];
}

void deLoggerBuffer::AddEntry(eMessageTypes type, const decString &source, const decString &message){
	const deMutexGuard guard(pMutex);
	pEntries.Add({type, source, message});
}

void deLoggerBuffer::Clear(){
	const deMutexGuard guard(pMutex);
	pEntries.RemoveAll();
}



void deLoggerBuffer::SendToLogger(deLogger &logger){
	const deMutexGuard guard(pMutex);
	pEntries.Visit([&](const sEntry &entry){
		switch(entry.type){
		case emtInfo:
			logger.LogInfo(entry.source, entry.message);
			break;
			
		case emtWarn:
			logger.LogWarn(entry.source, entry.message);
			break;
			
		case emtError:
			logger.LogError(entry.source, entry.message);
			break;
		}
	});
	pEntries.RemoveAll();
}



void deLoggerBuffer::LogInfo(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	pEntries.Add({emtInfo, source, message});
}

void deLoggerBuffer::LogWarn(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	pEntries.Add({emtWarn, source, message});
}

void deLoggerBuffer::LogError(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	pEntries.Add({emtError, source, message});
}
