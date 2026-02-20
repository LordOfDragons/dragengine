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

#include "deoglEnvironmentMap.h"
#include "deoglEnvMapSlotManager.h"
#include "deoglEnvMapSlot.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglEnvMapSlotManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglEnvMapSlotManager::deoglEnvMapSlotManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
	pArrayTexture = nullptr;
	
	pWidth = 512;
	pHeight = 256;
	
	// memory consumption with 512x256:
	// ((512*256*3*2)*4/3) = 1048576 bytes ~= 1MB
	// 
	// memory consumption with 256x128:
	// ((245*128*3*2)*4/3) = 250880 bytes ~= 0.25MB
	
	try{
		pSlots.SetAll(5, {});
		
		pArrayTexture = new deoglArrayTexture(renderThread);
		pArrayTexture->SetSize(pWidth, pHeight, pSlots.GetCount());
		pArrayTexture->SetFBOFormat(3, true);
		pArrayTexture->SetMipMapped(true);
		pArrayTexture->CreateTexture();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglEnvMapSlotManager::~deoglEnvMapSlotManager(){
	pCleanUp();
}



// Management
///////////////

deoglEnvMapSlot &deoglEnvMapSlotManager::GetSlotAt(int index){
	return pSlots[index];
}

const deoglEnvMapSlot &deoglEnvMapSlotManager::GetSlotAt(int index) const{
	return pSlots[index];
}

int deoglEnvMapSlotManager::IndexOfSlotWith(deoglEnvironmentMap *envmap) const{
	DEASSERT_NOTNULL(envmap)
	
	return pSlots.IndexOfMatching([&](const deoglEnvMapSlot &slot){
		return slot.GetEnvMap() == envmap;
	});
}

int deoglEnvMapSlotManager::IndexOfOldestUnusedSlot() const{
	int oldestLastUsed = 0;
	int slotIndex = -1;
	
	pSlots.VisitIndexed([&](int i, const deoglEnvMapSlot &slot){
		// also protect all slots used in this and the last frame?
		// if(!slot.GetInUse() && slot.GetLastUsed() > 1 && (slotIndex == -1 || slot.GetLastUsed() > oldestLastUsed)){
		// or used in this frame?
		// if(!slot.GetInUse() && slot.GetLastUsed() > 0 && (slotIndex == -1 || slot.GetLastUsed() > oldestLastUsed)){
		if(!slot.GetInUse() && (slotIndex == -1 || slot.GetLastUsed() > oldestLastUsed)){
			slotIndex = i;
			oldestLastUsed = slot.GetLastUsed();
		}
	});
	
	return slotIndex;
}



void deoglEnvMapSlotManager::MarkSlotsUnused(){
	pSlots.Visit([](deoglEnvMapSlot &slot){
		slot.SetInUse(false);
	});
}

void deoglEnvMapSlotManager::AddEnvironmentMap(deoglEnvironmentMap *envmap){
	DEASSERT_NOTNULL(envmap)
	
	int slotIndex = envmap->GetSlotIndex();
	
	if(slotIndex == -1){
		// if the environment map is not hosted by any slots find the oldest unused slot
		slotIndex = IndexOfOldestUnusedSlot();
		
		if(slotIndex == -1){
			// if there are no empty slots available we have to increase the layer count to make room for more
			const int newLayerCount = pSlots.GetCount() + 5;
			
			pRenderThread.GetLogger().LogInfoFormat(
				"EnvMapSlotManager.AddEnvironmentMap: Increase layer count from %d to %d",
				pSlots.GetCount(), newLayerCount);
			
			slotIndex = pSlots.GetCount(); // this is going to be the new slot to assign the environment map to
			
			// enlarge also the array texture. this unfortunately erases all the content so we have to copy
			// the textures of all environment maps to the corresponding layer
			pArrayTexture->SetSize(pWidth, pHeight, newLayerCount);
			pArrayTexture->CreateTexture();
			pArrayTexture->CreateMipMaps(); // would be better to have a quicker init here to make sure the mip map levels exist
			
			pSlots.VisitIndexed([&](int i, const deoglEnvMapSlot &slot){
				if(slot.GetEnvMap() && slot.GetEnvMap()->GetEquiEnvMap()
				&& slot.GetEnvMap()->GetEquiEnvMap()->GetTexture()){
					pRenderThread.GetLogger().LogInfoFormat(
						"EnvMapSlotManager.AddEnvironmentMap(IncreaseLayer): Copy EnvMap %p to layer %d",
						slot.GetEnvMap(), i);
					pArrayTexture->CopyFrom(*slot.GetEnvMap()->GetEquiEnvMap(), true, i);
				}
			});
			
			// add more slots
			pSlots.SetCount(newLayerCount, {});
		}
		
		// assign the environment map to the found slot and copy the texture to the corresponding level in the array texture
		if(pSlots[slotIndex].GetEnvMap()){
			pSlots[slotIndex].GetEnvMap()->SetSlotIndex(-1);
		}
		
		pSlots[slotIndex].SetEnvMap(envmap);
		envmap->SetSlotIndex(slotIndex);
		
		pRenderThread.GetLogger().LogInfoFormat(
			"EnvMapSlotManager.AddEnvironmentMap: Copy EnvMap %p to layer %d", envmap, slotIndex);
		pArrayTexture->CopyFrom(*pSlots[slotIndex].GetEnvMap()->GetEquiEnvMap(), true, slotIndex);
	}
	
	// in all cases mark the slot in use and reset the last used counter to 0
	pSlots[slotIndex].SetInUse(true);
	pSlots[slotIndex].ResetLastUsed();
}

void deoglEnvMapSlotManager::IncreaseSlotLastUsedCounters(){
	pSlots.Visit([](deoglEnvMapSlot &slot){
		slot.IncrementLastUsed();
	});
}

void deoglEnvMapSlotManager::NotifyEnvMapChanged(int slotIndex){
	DEASSERT_NOTNULL(pSlots[slotIndex].GetEnvMap())
	
	pRenderThread.GetLogger().LogInfoFormat(
		"EnvMapSlotManager.NotifyEnvMapChanged: Copy EnvMap %p to layer %d",
		pSlots[slotIndex].GetEnvMap(), slotIndex);
	pArrayTexture->CopyFrom(*pSlots[slotIndex].GetEnvMap()->GetEquiEnvMap(), true, slotIndex);
}



int deoglEnvMapSlotManager::GetUsedSlotIndexAt(int index) const{
	return pUsedSlots[index];
}

void deoglEnvMapSlotManager::UpdateUsedSlots(){
	pUsedSlots.RemoveAll();
	
	pSlots.VisitIndexed([&](int i, const deoglEnvMapSlot &slot){
		if(slot.GetInUse()){
			pUsedSlots.Add(i);
		}
	});
}



// Private Functions
//////////////////////

void deoglEnvMapSlotManager::pCleanUp(){
	if(pArrayTexture){
		delete pArrayTexture;
	}
}
