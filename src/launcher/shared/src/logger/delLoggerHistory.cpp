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

#include "delLoggerHistory.h"
#include "delLoggerHistoryEntry.h"
#include "delLoggerHistoryListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class delLoggerHistory
///////////////////////////

// Constructor, destructor
////////////////////////////

delLoggerHistory::delLoggerHistory(int size) :
pEntryCount(0),
pEntryOffset(0),
pLogInfo(true),
pLogWarn(true),
pLogError(true)
{
	DEASSERT_TRUE(size >= 0)
	
	pEntries.AddRange(size, {});
}

delLoggerHistory::~delLoggerHistory() = default;



// Management
///////////////

void delLoggerHistory::SetHistorySize(int size){
	DEASSERT_TRUE(size >= 0)
	
	Clear();
	
	const deMutexGuard guard(pMutex);
	
	pEntries.SetCountDiscard(size);
	
	pEntryCount = 0;
	pEntryOffset = 0;
}

delLoggerHistoryEntry &delLoggerHistory::GetEntryAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	
	return pEntries[(pEntryOffset + index) % pEntries.GetCount()];
}

const delLoggerHistoryEntry &delLoggerHistory::GetEntryAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	
	return pEntries[(pEntryOffset + index) % pEntries.GetCount()];
}

delLoggerHistoryEntry &delLoggerHistory::AddEntry(){
	DEASSERT_TRUE(pEntries.IsNotEmpty())
	
	const int position = (pEntryOffset + pEntryCount) % pEntries.GetCount();
	
	if(pEntryCount == pEntries.GetCount()){
		pEntryOffset = (pEntryOffset + 1) % pEntries.GetCount();
		
	}else{
		pEntryCount++;
	}
	
	pEntries[position].Clear();
	return pEntries[position];
}

void delLoggerHistory::Clear(){
	const deMutexGuard guard(pMutex);
	
	pEntryOffset = 0;
	pEntryCount = 0;
	
	pListeners.Visit([&](delLoggerHistoryListener &l){
		l.HistoryCleared(*this);
	});
}



bool delLoggerHistory::CanAddMessage(int type, const char *source){
	if(pEntries.IsEmpty()){
		return false;
	}
	if(type == delLoggerHistoryEntry::emtInfo && !pLogInfo){
		return false;
	}
	if(type == delLoggerHistoryEntry::emtWarn && !pLogWarn){
		return false;
	}
	if(type == delLoggerHistoryEntry::emtError && !pLogError){
		return false;
	}
	return true;
}



void delLoggerHistory::AddListener(delLoggerHistoryListener *listener){
	DEASSERT_NOTNULL(listener)
	
	const deMutexGuard guard(pMutex);
	pListeners.Add(listener);
}

void delLoggerHistory::RemoveListener(delLoggerHistoryListener *listener){
	DEASSERT_NOTNULL(listener)
	
	const deMutexGuard guard(pMutex);
	pListeners.Remove(listener);
}

void delLoggerHistory::NotifyMessageAdded(const delLoggerHistoryEntry &entry){
	pListeners.Visit([&](delLoggerHistoryListener &l){
		l.MessageAdded(*this, entry);
	});
}



void delLoggerHistory::LogInfo(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(!CanAddMessage(delLoggerHistoryEntry::emtInfo, source)){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType(delLoggerHistoryEntry::emtInfo);
	entry.SetSource(source);
	entry.SetMessage(message);
	entry.CleanUpMessage();
	
	NotifyMessageAdded(entry);
}

void delLoggerHistory::LogWarn(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(!CanAddMessage(delLoggerHistoryEntry::emtWarn, source)){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType(delLoggerHistoryEntry::emtWarn);
	entry.SetSource(source);
	entry.SetMessage(message);
	entry.CleanUpMessage();
	
	NotifyMessageAdded(entry);
}

void delLoggerHistory::LogError(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	const deMutexGuard guard(pMutex);
	
	if(!CanAddMessage(delLoggerHistoryEntry::emtError, source)){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType(delLoggerHistoryEntry::emtError);
	entry.SetSource(source);
	entry.SetMessage(message);
	entry.CleanUpMessage();
	
	NotifyMessageAdded(entry);
}
