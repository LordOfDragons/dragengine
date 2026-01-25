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

#include "igdeLoggerHistory.h"
#include "igdeLoggerHistoryEntry.h"
#include "igdeLoggerHistoryListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class igdeLoggerHistory
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoggerHistory::igdeLoggerHistory(){
	pEntryPointer = 0;
	pEntryCount = 0;
	
	pLogInfo = true;
	pLogWarn = true;
	pLogError = true;
}

igdeLoggerHistory::~igdeLoggerHistory() = default;



// Management
///////////////

void igdeLoggerHistory::SetHistorySize(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	
	const deMutexGuard guard(pMutex);
	
	pEntries.SetCountDiscard(size);
	pEntryPointer = 0;
	pEntryCount = 0;
}

igdeLoggerHistoryEntry &igdeLoggerHistory::GetEntryAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	
	return pEntries[(pEntryPointer + index) % pEntries.GetCount()];
}

const igdeLoggerHistoryEntry &igdeLoggerHistory::GetEntryAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	
	return pEntries[(pEntryPointer + index) % pEntries.GetCount()];
}

igdeLoggerHistoryEntry &igdeLoggerHistory::AddEntry(){
	DEASSERT_TRUE(pEntries.IsNotEmpty())
	
	const int position = (pEntryPointer + pEntryCount) % pEntries.GetCount();
	
	if(pEntryCount == pEntries.GetCount()){
		pEntryPointer = (pEntryPointer + 1) % pEntries.GetCount();
		
	}else{
		pEntryCount++;
	}
	
	return pEntries[position];
}

void igdeLoggerHistory::Clear(){
	const deMutexGuard guard(pMutex);
	
	pEntryPointer = 0;
	pEntryCount = 0;
	
	pListeners.Visit([&](igdeLoggerHistoryListener &l){
		l.HistoryCleared(this);
	});
}



bool igdeLoggerHistory::CanAddMessage(int type, const char *source){
	if(pEntries.IsEmpty()){
		return false;
	}
	
	if(type == igdeLoggerHistoryEntry::emtInfo && !pLogInfo){
		return false;
	}
	if(type == igdeLoggerHistoryEntry::emtWarn && !pLogWarn){
		return false;
	}
	if(type == igdeLoggerHistoryEntry::emtError && !pLogError){
		return false;
	}
	
	return true;
}



void igdeLoggerHistory::AddListener(igdeLoggerHistoryListener *listener){
	DEASSERT_NOTNULL(listener)
	
	pListeners.Add(listener);
}

void igdeLoggerHistory::RemoveListener(igdeLoggerHistoryListener *listener){
	pListeners.Remove(listener);
}

void igdeLoggerHistory::NotifyMessageAdded(igdeLoggerHistoryEntry &entry){
	pListeners.Visit([&](igdeLoggerHistoryListener &l){
		l.MessageAdded(this, entry);
	});
}



void igdeLoggerHistory::LogInfo(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(CanAddMessage(igdeLoggerHistoryEntry::emtInfo, source)){
		igdeLoggerHistoryEntry &entry = AddEntry();
		entry.SetType(igdeLoggerHistoryEntry::emtInfo);
		entry.SetSource(source);
		entry.SetMessage(message);
		entry.CleanUpMessage();
		
		NotifyMessageAdded(entry);
	}
}

void igdeLoggerHistory::LogWarn(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(CanAddMessage(igdeLoggerHistoryEntry::emtWarn, source)){
		igdeLoggerHistoryEntry &entry = AddEntry();
		entry.SetType(igdeLoggerHistoryEntry::emtWarn);
		entry.SetSource(source);
		entry.SetMessage(message);
		entry.CleanUpMessage();
		
		NotifyMessageAdded(entry);
	}
}

void igdeLoggerHistory::LogError(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(CanAddMessage(igdeLoggerHistoryEntry::emtError, source)){
		igdeLoggerHistoryEntry &entry = AddEntry();
		entry.SetType(igdeLoggerHistoryEntry::emtError);
		entry.SetSource(source);
		entry.SetMessage(message);
		entry.CleanUpMessage();
		
		NotifyMessageAdded(entry);
	}
}
