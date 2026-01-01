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

#include "deoalEffectSlot.h"
#include "deoalEffectSlotManager.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../capabilities/deoalCapabilities.h"

#include <dragengine/common/exceptions.h>



// #define DO_DEBUG 1

#ifdef DO_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif



// Class deoalEffectSlotManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalEffectSlotManager::deoalEffectSlotManager(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pMaxCount(0){
}

deoalEffectSlotManager::~deoalEffectSlotManager(){
}



// Management
///////////////

deoalEffectSlot *deoalEffectSlotManager::Bind(void *owner, float importance){
	DEASSERT_NOTNULL(owner)
	
	deoalEffectSlot *slot = pBestUnbound();
	if(slot){
		slot->AssignOwner(owner, importance);
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"EffectSlotManager: Bind previously unbound effectslot %d to %p[%g] (%d)",
			slot->GetSlot(), owner, importance, pSlots.GetCount()));
		return slot;
	}
	
	slot = pCreateNew();
	if(slot){
		slot->AssignOwner(owner, importance);
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"EffectSlotManager: Create new effectslot %d to %p[%g] (%d)",
			slot->GetSlot(), owner, importance, pSlots.GetCount()));
		return slot;
	}
	
	slot = pBestKeptAlive();
	if(slot){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"EffectSlotManager: Rebind kept alive effectslot %d from %p[%g] to %p[%g] (%d)",
			slot->GetSlot(), slot->GetOwner(), slot->GetImportance(),
			owner, importance, pSlots.GetCount()));
		slot->AssignOwner(owner, importance);
		return slot;
	}
	
	slot = pBestRebindable();
	if(slot && importance > slot->GetImportance()){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"EffectSlotManager: Rebind bound effectslot %d from %p[%g] to %p[%g] (%d)",
			slot->GetSlot(), slot->GetOwner(), slot->GetImportance(),
			owner, importance, pSlots.GetCount()));
		slot->AssignOwner(owner, importance);
		return slot;
	}
	
	return nullptr;
}

void deoalEffectSlotManager::Unbind(deoalEffectSlot *slot){
	DEASSERT_NOTNULL(slot)
	DEASSERT_NOTNULL(slot->GetOwner())
	
	DEBUG(pAudioThread.GetLogger().LogInfoFormat(
		"EffectSlotManager: Unbind effectslot %d with %p[%g] (%d)",
		slot->GetSlot(), slot->GetOwner(), slot->GetImportance(), pSlots.GetCount()));
	slot->ClearOwner();
}

void deoalEffectSlotManager::Update(float elapsed){
	const int count = pSlots.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoalEffectSlot*)pSlots.GetAt(i))->Update(elapsed);
	}
}



// Private Functions
//////////////////////

deoalEffectSlot *deoalEffectSlotManager::pBestUnbound() const{
	const int count = pSlots.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalEffectSlot * const slot = (deoalEffectSlot*)pSlots.GetAt(i);
		if(slot->IsUnbound() && !slot->IsKeptAlive()){
			return slot;
		}
	}
	
	return nullptr;
}

deoalEffectSlot *deoalEffectSlotManager::pBestKeptAlive() const{
	const int count = pSlots.GetCount();
	deoalEffectSlot *bestSlot = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		deoalEffectSlot * const slot = (deoalEffectSlot*)pSlots.GetAt(i);
		if(slot->IsUnbound() && slot->IsKeptAlive() && (!bestSlot
		|| slot->GetElapsedKeepAliveTime() > bestSlot->GetElapsedKeepAliveTime())){
			bestSlot = slot;
		}
	}
	return bestSlot;
}

deoalEffectSlot *deoalEffectSlotManager::pBestRebindable() const{
	const int count = pSlots.GetCount();
	deoalEffectSlot *bestSlot = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		deoalEffectSlot * const slot = (deoalEffectSlot*)pSlots.GetAt(i);
		if(slot->IsBound() && (!bestSlot || slot->GetImportance() < bestSlot->GetImportance())){
			bestSlot = slot;
		}
	}
	return bestSlot;
}

deoalEffectSlot *deoalEffectSlotManager::pCreateNew(){
	if(pMaxCount != 0 && pSlots.GetCount() == pMaxCount){
		return nullptr;
	}
	
	try{
		const deoalEffectSlot::Ref slot(deoalEffectSlot::Ref::New(pAudioThread));
		pSlots.Add(slot);
		return slot;
		
	}catch(const deeOutOfMemory &){
		// assume we hit the maximum count of effect slots. according to openal code
		// this should be 64 but since we can not query this limit we use this version
		pMaxCount = pSlots.GetCount();
		pAudioThread.GetLogger().LogInfoFormat("EffectSlotManager: OutOfMemoy while creating"
			" effect. Assuming maximum effect slot count %d", pMaxCount);
		return nullptr;
		
	}catch(const deException &e){
		pAudioThread.GetLogger().LogException(e);
		pMaxCount = pSlots.GetCount();
		pAudioThread.GetLogger().LogInfoFormat("EffectSlotManager: Exception while creating"
			" effect. Assuming maximum effect slot count %d", pMaxCount);
		return nullptr;
	}
}
