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

#include <math.h>
#include <stdio.h>
#include <string.h>

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
	pArrayTexture = NULL;
	
	pWidth = 512;
	pHeight = 256;
	pLayerCount = 5;
	
	pSlots = NULL;
	pUsedSlots = NULL;
	pUsedSlotsSize = 0;
	pUsedSlotsCount = 0;
	
	// memory consumption with 512x256:
	// ((512*256*3*2)*4/3) = 1048576 bytes ~= 1MB
	// 
	// memory consumption with 256x128:
	// ((245*128*3*2)*4/3) = 250880 bytes ~= 0.25MB
	
	try{
		pSlots = new deoglEnvMapSlot[pLayerCount];
		
		pArrayTexture = new deoglArrayTexture(renderThread);
		pArrayTexture->SetSize(pWidth, pHeight, pLayerCount);
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

deoglEnvMapSlot &deoglEnvMapSlotManager::GetSlotAt(int index) const{
	if(index < 0 || index >= pLayerCount){
		DETHROW(deeInvalidParam);
	}
	
	return pSlots[index];
}

int deoglEnvMapSlotManager::IndexOfSlotWith(deoglEnvironmentMap *envmap) const{
	if(! envmap){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pLayerCount; i++){
		if(envmap == pSlots[i].GetEnvMap()){
			return i;
		}
	}
	
	return -1;
}

int deoglEnvMapSlotManager::IndexOfOldestUnusedSlot() const{
	int oldestLastUsed = 0;
	int slotIndex = -1;
	int i;
	
	for(i=0; i<pLayerCount; i++){
		// also protect all slots used in this and the last frame?
		// if( ! pSlots[ i ].GetInUse() && pSlots[ i ].GetLastUsed() > 1 && ( slotIndex == -1 || pSlots[ i ].GetLastUsed() > oldestLastUsed ) ){
		// or used in this frame?
		// if( ! pSlots[ i ].GetInUse() && pSlots[ i ].GetLastUsed() > 0 && ( slotIndex == -1 || pSlots[ i ].GetLastUsed() > oldestLastUsed ) ){
		if(! pSlots[i].GetInUse() && (slotIndex == -1 || pSlots[i].GetLastUsed() > oldestLastUsed)){
			slotIndex = i;
			oldestLastUsed = pSlots[i].GetLastUsed();
		}
	}
	
	return slotIndex;
}



void deoglEnvMapSlotManager::MarkSlotsUnused(){
	int i;
	
	for(i=0; i<pLayerCount; i++){
		pSlots[i].SetInUse(false);
	}
}

void deoglEnvMapSlotManager::AddEnvironmentMap(deoglEnvironmentMap *envmap){
	if(! envmap){
		DETHROW(deeInvalidParam);
	}
	
	int slotIndex = envmap->GetSlotIndex();
	
	if(slotIndex == -1){
		// if the environment map is not hosted by any slots find the oldest unused slot
		slotIndex = IndexOfOldestUnusedSlot();
		
		if(slotIndex == -1){
			// if there are no empty slots available we have to increase the layer count to make room for more
			const int newLayerCount = pLayerCount + 5;
			int i;
			
			pRenderThread.GetLogger().LogInfoFormat("EnvMapSlotManager.AddEnvironmentMap: Increase layer count from %i to %i", pLayerCount, newLayerCount);
			//printf( "EnvMapSlotManager.AddEnvironmentMap: Increase layer count from %i to %i\n", pLayerCount, newLayerCount );
			
			slotIndex = pLayerCount; // this is going to be the new slot to assign the environment map to
			
			// enlarge also the array texture. this unfortunately erases all the content so we have to copy
			// the textures of all environment maps to the corresponding layer
			pArrayTexture->SetSize(pWidth, pHeight, newLayerCount);
			pArrayTexture->CreateTexture();
			pArrayTexture->CreateMipMaps(); // would be better to have a quicker init here to make sure the mip map levels exist
			
			for(i=0; i<pLayerCount; i++){
				if(pSlots[i].GetEnvMap() && pSlots[i].GetEnvMap()->GetEquiEnvMap() && pSlots[i].GetEnvMap()->GetEquiEnvMap()->GetTexture()){
					pRenderThread.GetLogger().LogInfoFormat("EnvMapSlotManager.AddEnvironmentMap(IncreaseLayer): Copy EnvMap %p to layer %i", pSlots[i].GetEnvMap(), i);
					//printf( "EnvMapSlotManager.AddEnvironmentMap(IncreaseLayer): Copy EnvMap %p to layer %i\n", pSlots[ i ].GetEnvMap(), i );
					pArrayTexture->CopyFrom(*pSlots[i].GetEnvMap()->GetEquiEnvMap(), true, i);
				}
			}
			
			// add more slots
			deoglEnvMapSlot * const newArray = new deoglEnvMapSlot[newLayerCount];
			
			memcpy(newArray, pSlots, sizeof(deoglEnvMapSlot) * pLayerCount);
			delete [] pSlots;
			
			pSlots = newArray;
			pLayerCount = newLayerCount;
		}
		
		// assign the environment map to the found slot and copy the texture to the corresponding level in the array texture
		if(pSlots[slotIndex].GetEnvMap()){
			pSlots[slotIndex].GetEnvMap()->SetSlotIndex(-1);
		}
		
		pSlots[slotIndex].SetEnvMap(envmap);
		envmap->SetSlotIndex(slotIndex);
		
		pRenderThread.GetLogger().LogInfoFormat("EnvMapSlotManager.AddEnvironmentMap: Copy EnvMap %p to layer %i", envmap, slotIndex);
		//printf( "EnvMapSlotManager.AddEnvironmentMap: Copy EnvMap %p to layer %i\n", envmap, slotIndex );
		pArrayTexture->CopyFrom(*pSlots[slotIndex].GetEnvMap()->GetEquiEnvMap(), true, slotIndex);
	}
	
	// in all cases mark the slot in use and reset the last used counter to 0
	pSlots[slotIndex].SetInUse(true);
	pSlots[slotIndex].ResetLastUsed();
}

void deoglEnvMapSlotManager::IncreaseSlotLastUsedCounters(){
	int i;
	
	for(i=0; i<pLayerCount; i++){
		pSlots[i].IncrementLastUsed();
	}
}

void deoglEnvMapSlotManager::NotifyEnvMapChanged(int slotIndex){
	if(slotIndex < 0 || slotIndex >= pLayerCount || ! pSlots[slotIndex].GetEnvMap()){
		DETHROW(deeInvalidParam);
	}
	
	pRenderThread.GetLogger().LogInfoFormat("EnvMapSlotManager.NotifyEnvMapChanged: Copy EnvMap %p to layer %i", pSlots[slotIndex].GetEnvMap(), slotIndex);
	//printf( "EnvMapSlotManager.NotifyEnvMapChanged: Copy EnvMap %p to layer %i\n", pSlots[ slotIndex ].GetEnvMap(), slotIndex );
	pArrayTexture->CopyFrom(*pSlots[slotIndex].GetEnvMap()->GetEquiEnvMap(), true, slotIndex);
}



int deoglEnvMapSlotManager::GetUsedSlotIndexAt(int index) const{
	if(index < 0 || index >= pUsedSlotsCount){
		DETHROW(deeInvalidParam);
	}
	
	return pUsedSlots[index];
}

void deoglEnvMapSlotManager::UpdateUsedSlots(){
	int i;
	
	if(pUsedSlotsSize < pLayerCount){
		int * const newArray = new int[pLayerCount];
		
		if(pUsedSlots){
			memcpy(newArray, pUsedSlots, sizeof(int) * pUsedSlotsSize);
			delete [] pUsedSlots;
		}
		
		pUsedSlots = newArray;
		pUsedSlotsSize = pLayerCount;
	}
	
	pUsedSlotsCount = 0;
	
	for(i=0; i<pLayerCount; i++){
		if(pSlots[i].GetInUse()){
			pUsedSlots[pUsedSlotsCount++] = i;
		}
	}
}



// Private Functions
//////////////////////

void deoglEnvMapSlotManager::pCleanUp(){
	if(pArrayTexture){
		delete pArrayTexture;
	}
	if(pUsedSlots){
		delete [] pUsedSlots;
	}
	if(pSlots){
		delete [] pSlots;
	}
}
