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
#include <string.h>

#include "dedaiWorld.h"
#include "../deDEAIModule.h"
#include "../navigation/dedaiNavigator.h"
#include "../navigation/dedaiNavSpace.h"
#include "../navigation/spaces/dedaiSpace.h"
#include "../navigation/blocker/dedaiNavBlocker.h"
#include "../navigation/heightterrain/dedaiHeightTerrain.h"
#include "../navigation/heightterrain/dedaiHeightTerrainSector.h"
#include "../navigation/heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../navigation/layer/dedaiLayer.h"
#include "../devmode/dedaiDeveloperMode.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/world/deWorld.h>



// Class dedaiWorld
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiWorld::dedaiWorld(deDEAIModule &deai, deWorld &world) :
pDEAI(deai),
pWorld(world),
pHeightTerrain(nullptr),
pDevModeUpdateTracker(0)
{
	try{
		HeightTerrainChanged();
		
		world.GetNavigationSpaces().Visit([&](deNavigationSpace *navspace){
			NavigationSpaceAdded(navspace);
		});
		
		world.GetNavigationBlockers().Visit([&](deNavigationBlocker *blocker){
			NavigationBlockerAdded(blocker);
		});
		
		world.GetNavigators().Visit([&](deNavigator *navigator){
			NavigatorAdded(navigator);
		});
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dedaiWorld::~dedaiWorld(){
	pCleanUp();
}



// Management
///////////////

dedaiLayer *dedaiWorld::GetLayer(int layer){
	const int count = pLayers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		dedaiLayer * const objLayer = pLayers.GetAt(i);
		if(objLayer->GetLayer() == layer){
			return objLayer;
		}
	}
	
	const dedaiLayer::Ref newLayer(dedaiLayer::Ref::New(*this, layer));
	pLayers.Add(newLayer);
	return newLayer; // pLayers keeps reference
}



void dedaiWorld::CheckDeveloperMode(){
	const dedaiDeveloperMode &devmode = pDEAI.GetDeveloperMode();
	
	if(!devmode.GetEnabled()){
		return;
	}
	
	// check if an update is forced
	const bool trackerChanged = (pDEAI.GetDeveloperMode().GetUpdateTracker() != pDevModeUpdateTracker);
	pDevModeUpdateTracker = pDEAI.GetDeveloperMode().GetUpdateTracker();
	
	// invalidate blocking
	if(trackerChanged && pDEAI.GetDeveloperMode().GetQuickDebug() >= 8 && pDEAI.GetDeveloperMode().GetQuickDebug() <= 10){
		pLayers.Visit([](dedaiLayer &layer){
			layer.InvalidateBlocking();
		});
	}
	
	// height terrain navspace debug drawing
	if(pHeightTerrain){
		const int sectorCount = pHeightTerrain->GetSectors().GetCount();
		int i, j;
		for(i=0; i<sectorCount; i++){
			dedaiHeightTerrainSector &sector = *pHeightTerrain->GetSectors().GetAt(i);
			const int navSpaceCount = sector.GetNavSpaces().GetCount();
			for(j=0; j<navSpaceCount; j++){
				sector.GetNavSpaces().GetAt(j)->GetSpace()->UpdateDDSSpace();
			}
		}
	}
	
	// navigation spaces debug drawing
	if(devmode.GetShowSpaces()){
		if(trackerChanged){
			pWorld.GetNavigationSpaces().Visit([&](deNavigationSpace *engNavSpace){
				((dedaiNavSpace*)engNavSpace->GetPeerAI())->GetSpace()->UpdateDDSSpace();
			});
		}
		
	}else{
		if(trackerChanged){
			pWorld.GetNavigationSpaces().Visit([&](deNavigationSpace *engNavSpace){
				((dedaiNavSpace*)engNavSpace->GetPeerAI())->GetSpace()->UpdateDDSSpace();
			});
		}
	}
	
	// navigation blockers debug drawing
	if(devmode.GetShowBlockers()){
		if(trackerChanged){
			pWorld.GetNavigationBlockers().Visit([&](deNavigationBlocker *engBlocker){
				((dedaiNavBlocker*)engBlocker->GetPeerAI())->UpdateDDSBlocker();
			});
		}
		
	}else{
		if(trackerChanged){
			pWorld.GetNavigationBlockers().Visit([&](deNavigationBlocker *engBlocker){
				((dedaiNavBlocker*)engBlocker->GetPeerAI())->UpdateDDSBlocker();
			});
		}
	}
	
	// navigator path debug drawing
	if(devmode.GetShowPath() || devmode.GetShowPathFaces()){
		if(trackerChanged){
			pWorld.GetNavigators().Visit([&](deNavigator *engNav){
				((dedaiNavigator*)engNav->GetPeerAI())->UpdateDDSPath();
			});
		}
		
	}else{
		if(trackerChanged){
			pWorld.GetNavigators().Visit([&](deNavigator *engNav){
				((dedaiNavigator*)engNav->GetPeerAI())->UpdateDDSPath();
			});
		}
	}
}



// Notifications
//////////////////

void dedaiWorld::Update(float elapsed){
	CheckDeveloperMode();
	
	const int count = pLayers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pLayers.GetAt(i)->Update(elapsed);
	}
}



void dedaiWorld::NavigationSpaceAdded(deNavigationSpace *navspace){
	dedaiNavSpace * const peer = (dedaiNavSpace*)navspace->GetPeerAI();
	if(peer){
		peer->SetParentWorld(this);
	}
}

void dedaiWorld::NavigationSpaceRemoved(deNavigationSpace *navspace){
	dedaiNavSpace * const peer = (dedaiNavSpace*)navspace->GetPeerAI();
	if(peer){
		peer->SetParentWorld(nullptr);
	}
}

void dedaiWorld::AllNavigationSpacesRemoved(){
	pWorld.GetNavigationSpaces().Visit([&](deNavigationSpace *navspace){
		dedaiNavSpace * const peer = (dedaiNavSpace*)navspace->GetPeerAI();
		if(peer){
			peer->SetParentWorld(nullptr);
		}
	});
}



void dedaiWorld::NavigationBlockerAdded(deNavigationBlocker *blocker){
	((dedaiNavBlocker*)blocker->GetPeerAI())->SetParentWorld(this);
}

void dedaiWorld::NavigationBlockerRemoved(deNavigationBlocker *blocker){
	dedaiNavBlocker * const peer = (dedaiNavBlocker*)blocker->GetPeerAI();
	
	if(peer){
		peer->SetParentWorld(nullptr);
	}
}

void dedaiWorld::AllNavigationBlockersRemoved(){
	pWorld.GetNavigationBlockers().Visit([&](deNavigationBlocker *blocker){
		dedaiNavBlocker * const peer = (dedaiNavBlocker*)blocker->GetPeerAI();
		if(peer){
			peer->SetParentWorld(nullptr);
		}
	});
}



void dedaiWorld::NavigatorAdded(deNavigator *navigator){
	((dedaiNavigator*)navigator->GetPeerAI())->SetParentWorld(this);
}

void dedaiWorld::NavigatorRemoved(deNavigator *navigator){
	dedaiNavigator * const peer = (dedaiNavigator*)navigator->GetPeerAI();
	
	if(peer){
		peer->SetParentWorld(nullptr);
	}
}

void dedaiWorld::AllNavigatorsRemoved(){
	pWorld.GetNavigators().Visit([&](deNavigator *navigator){
		dedaiNavigator * const peer = (dedaiNavigator*)navigator->GetPeerAI();
		if(peer){
			peer->SetParentWorld(nullptr);
		}
	});
}



void dedaiWorld::HeightTerrainChanged(){
	if(pHeightTerrain){
		pHeightTerrain->SetParentWorld(nullptr);
		pHeightTerrain = nullptr;
	}
	
	if(pWorld.GetHeightTerrain()){
		pHeightTerrain = ((dedaiHeightTerrain*)pWorld.GetHeightTerrain()->GetPeerAI());
		if(pHeightTerrain){
			pHeightTerrain->SetParentWorld(this);
		}
	}
}



// Private Functions
//////////////////////

void dedaiWorld::pCleanUp(){
	AllNavigatorsRemoved();
	AllNavigationBlockersRemoved();
	AllNavigationSpacesRemoved();
	
	if(pHeightTerrain){
		pHeightTerrain->SetParentWorld(nullptr);
	}
}
