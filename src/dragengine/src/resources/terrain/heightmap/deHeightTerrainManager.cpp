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

#include "deHeightTerrain.h"
#include "deHeightTerrainManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/dePhysicsSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/audio/deBaseAudioHeightTerrain.h"
#include "../../../systems/modules/graphic/deBaseGraphicHeightTerrain.h"
#include "../../../systems/modules/physics/deBasePhysicsHeightTerrain.h"

#include "../../../common/exceptions.h"



// Class deHeightTerrainManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrainManager::deHeightTerrainManager(deEngine *engine) :
deResourceManager(engine, ertHeightTerrain){
	SetLoggingName("Height Terrain Manager");
}

deHeightTerrainManager::~deHeightTerrainManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deHeightTerrainManager::GetHeightTerrainCount() const{
	return pTerrains.GetCount();
}

deHeightTerrain *deHeightTerrainManager::GetRootHeightTerrain() const{
	return (deHeightTerrain*)pTerrains.GetRoot();
}

deHeightTerrain::Ref deHeightTerrainManager::CreateHeightTerrain(float sectorSize, int heightImageSize){
	if(sectorSize < 0.001f || heightImageSize < 2){
		DETHROW(deeInvalidParam);
	}
	
	const deHeightTerrain::Ref heightTerrain(deHeightTerrain::Ref::New(
		this, sectorSize, heightImageSize));
	GetGraphicSystem()->LoadHeightTerrain(heightTerrain);
	GetPhysicsSystem()->LoadHeightTerrain(heightTerrain);
	GetAISystem()->LoadHeightTerrain(*heightTerrain);
	pTerrains.Add(heightTerrain);
	return heightTerrain;
}



void deHeightTerrainManager::ReleaseLeakingResources(){
	if(GetHeightTerrainCount() == 0){
		return;
	}
	
	LogWarnFormat("%i leaking height terrains.", GetHeightTerrainCount());
	pTerrains.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
}



// Systems Support
////////////////////
void deHeightTerrainManager::SystemGraphicLoad(){
	deGraphicSystem &graSys = *GetGraphicSystem();
	pTerrains.GetResources().Visit([&](deResource *res){
		deHeightTerrain *heightTerrain = static_cast<deHeightTerrain*>(res);
		if(!heightTerrain->GetPeerGraphic()){
			graSys.LoadHeightTerrain(heightTerrain);
		}
	});
}

void deHeightTerrainManager::SystemGraphicUnload(){
	pTerrains.GetResources().Visit([](deResource *res){
		static_cast<deHeightTerrain*>(res)->SetPeerGraphic(nullptr);
	});
}

void deHeightTerrainManager::SystemPhysicsLoad(){
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	pTerrains.GetResources().Visit([&phySys](deResource *res){
		deHeightTerrain *heightTerrain = static_cast<deHeightTerrain*>(res);
		if(!heightTerrain->GetPeerPhysics()){
			phySys.LoadHeightTerrain(heightTerrain);
		}
	});
}

void deHeightTerrainManager::SystemPhysicsUnload(){
	pTerrains.GetResources().Visit([](deResource *res){
		static_cast<deHeightTerrain*>(res)->SetPeerPhysics(nullptr);
	});
}

void deHeightTerrainManager::SystemAILoad(){
	deAISystem &aiSys = *GetAISystem();
	pTerrains.GetResources().Visit([&aiSys](deResource *res){
		deHeightTerrain *heightTerrain = static_cast<deHeightTerrain*>(res);
		if(!heightTerrain->GetPeerAI()){
			aiSys.LoadHeightTerrain(*heightTerrain);
		}
	});
}

void deHeightTerrainManager::SystemAIUnload(){
	pTerrains.GetResources().Visit([](deResource *res){
		static_cast<deHeightTerrain*>(res)->SetPeerAI(nullptr);
	});
}

void deHeightTerrainManager::SystemAudioLoad(){
	deAudioSystem &audSys = *GetAudioSystem();
	pTerrains.GetResources().Visit([&](deResource *res){
		deHeightTerrain *heightTerrain = static_cast<deHeightTerrain*>(res);
		if(!heightTerrain->GetPeerAudio()){
			audSys.LoadHeightTerrain(*heightTerrain);
		}
	});
}

void deHeightTerrainManager::SystemAudioUnload(){
	pTerrains.GetResources().Visit([](deResource *res){
		static_cast<deHeightTerrain*>(res)->SetPeerAudio(nullptr);
	});
}



void deHeightTerrainManager::RemoveResource(deResource *resource){
	pTerrains.RemoveIfPresent(resource);
}
