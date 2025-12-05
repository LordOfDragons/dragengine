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

dedaiWorld::dedaiWorld( deDEAIModule &deai, deWorld &world ) :
pDEAI( deai ),
pWorld( world ),
pHeightTerrain( NULL ),
pDevModeUpdateTracker( 0 )
{
	try{
		HeightTerrainChanged();
		
		deNavigationSpace *navspace = world.GetRootNavigationSpace();
		while( navspace ){
			NavigationSpaceAdded( navspace );
			navspace = navspace->GetLLWorldNext();
		}
		
		deNavigationBlocker *blocker = world.GetRootNavigationBlocker();
		while( blocker ){
			NavigationBlockerAdded( blocker );
			blocker = blocker->GetLLWorldNext();
		}
		
		deNavigator *navigator = world.GetRootNavigator();
		while( navigator ){
			NavigatorAdded( navigator );
			navigator = navigator->GetLLWorldNext();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedaiWorld::~dedaiWorld(){
	pCleanUp();
}



// Management
///////////////

dedaiLayer *dedaiWorld::GetLayer( int layer ){
	const int count = pLayers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dedaiLayer * const objLayer = ( dedaiLayer* )pLayers.GetAt( i );
		if( objLayer->GetLayer() == layer ){
			return objLayer;
		}
	}
	
	const dedaiLayer::Ref newLayer(dedaiLayer::Ref::NewWith(*this, layer));
	pLayers.Add( newLayer );
	return newLayer; // pLayers keeps reference
}



void dedaiWorld::CheckDeveloperMode(){
	const dedaiDeveloperMode &devmode = pDEAI.GetDeveloperMode();
	
	if( ! devmode.GetEnabled() ){
		return;
	}
	
	// check if an update is forced
	const bool trackerChanged = ( pDEAI.GetDeveloperMode().GetUpdateTracker() != pDevModeUpdateTracker );
	pDevModeUpdateTracker = pDEAI.GetDeveloperMode().GetUpdateTracker();
	
	// invalidate blocking
	if( trackerChanged && pDEAI.GetDeveloperMode().GetQuickDebug() >= 8 && pDEAI.GetDeveloperMode().GetQuickDebug() <= 10 ){
		const int count = pLayers.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			( ( dedaiLayer* )pLayers.GetAt( i ) )->InvalidateBlocking();
		}
	}
	
	// height terrain navspace debug drawing
	if( pHeightTerrain ){
		const int sectorCount = pHeightTerrain->GetSectorCount();
		int i, j;
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *pHeightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				sector.GetNavSpaceAt( j )->GetSpace()->UpdateDDSSpace();
			}
		}
	}
	
	// navigation spaces debug drawing
	if( devmode.GetShowSpaces() ){
		if( trackerChanged ){
			deNavigationSpace *engNavSpace = pWorld.GetRootNavigationSpace();
			while( engNavSpace ){
				( ( dedaiNavSpace* )engNavSpace->GetPeerAI() )->GetSpace()->UpdateDDSSpace();
				engNavSpace = engNavSpace->GetLLWorldNext();
			}
		}
		
	}else{
		if( trackerChanged ){
			deNavigationSpace *engNavSpace = pWorld.GetRootNavigationSpace();
			while( engNavSpace ){
				( ( dedaiNavSpace* )engNavSpace->GetPeerAI() )->GetSpace()->UpdateDDSSpace();
				engNavSpace = engNavSpace->GetLLWorldNext();
			}
		}
	}
	
	// navigation blockers debug drawing
	if( devmode.GetShowBlockers() ){
		if( trackerChanged ){
			deNavigationBlocker *engBlocker = pWorld.GetRootNavigationBlocker();
			while( engBlocker ){
				( ( dedaiNavBlocker* )engBlocker->GetPeerAI() )->UpdateDDSBlocker();
				engBlocker = engBlocker->GetLLWorldNext();
			}
		}
		
	}else{
		if( trackerChanged ){
			deNavigationBlocker *engBlocker = pWorld.GetRootNavigationBlocker();
			while( engBlocker ){
				( ( dedaiNavBlocker* )engBlocker->GetPeerAI() )->UpdateDDSBlocker();
				engBlocker = engBlocker->GetLLWorldNext();
			}
		}
	}
	
	// navigator path debug drawing
	if( devmode.GetShowPath() || devmode.GetShowPathFaces() ){
		if( trackerChanged ){
			deNavigator *engNav = pWorld.GetRootNavigator();
			while( engNav ){
				( ( dedaiNavigator* )engNav->GetPeerAI() )->UpdateDDSPath();
				engNav = engNav->GetLLWorldNext();
			}
		}
		
	}else{
		if( trackerChanged ){
			deNavigator *engNav = pWorld.GetRootNavigator();
			while( engNav ){
				( ( dedaiNavigator* )engNav->GetPeerAI() )->UpdateDDSPath();
				engNav = engNav->GetLLWorldNext();
			}
		}
	}
}



// Notifications
//////////////////

void dedaiWorld::Update( float elapsed ){
	CheckDeveloperMode();
	
	const int count = pLayers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( dedaiLayer* )pLayers.GetAt( i ) )->Update( elapsed );
	}
}



void dedaiWorld::NavigationSpaceAdded( deNavigationSpace *navspace ){
	dedaiNavSpace * const peer = ( dedaiNavSpace* )navspace->GetPeerAI();
	if( peer ){
		peer->SetParentWorld( this );
	}
}

void dedaiWorld::NavigationSpaceRemoved( deNavigationSpace *navspace ){
	dedaiNavSpace * const peer = ( dedaiNavSpace* )navspace->GetPeerAI();
	if( peer ){
		peer->SetParentWorld( NULL );
	}
}

void dedaiWorld::AllNavigationSpacesRemoved(){
	deNavigationSpace *navspace = pWorld.GetRootNavigationSpace();
	
	while( navspace ){
		dedaiNavSpace * const peer = ( dedaiNavSpace* )navspace->GetPeerAI();
		navspace = navspace->GetLLWorldNext();
		if( peer ){
			peer->SetParentWorld( NULL );
		}
	}
}



void dedaiWorld::NavigationBlockerAdded( deNavigationBlocker *blocker ){
	( ( dedaiNavBlocker* )blocker->GetPeerAI() )->SetParentWorld( this );
}

void dedaiWorld::NavigationBlockerRemoved( deNavigationBlocker *blocker ){
	dedaiNavBlocker * const peer = ( dedaiNavBlocker* )blocker->GetPeerAI();
	
	if( peer ){
		peer->SetParentWorld( NULL );
	}
}

void dedaiWorld::AllNavigationBlockersRemoved(){
	deNavigationBlocker *blocker = pWorld.GetRootNavigationBlocker();
	dedaiNavBlocker *peer;
	
	while( blocker ){
		peer = ( dedaiNavBlocker* )blocker->GetPeerAI();
		blocker = blocker->GetLLWorldNext();
		
		if( peer ){
			peer->SetParentWorld( NULL );
		}
	}
}



void dedaiWorld::NavigatorAdded( deNavigator *navigator ){
	( ( dedaiNavigator* )navigator->GetPeerAI() )->SetParentWorld( this );
}

void dedaiWorld::NavigatorRemoved( deNavigator *navigator ){
	dedaiNavigator * const peer = ( dedaiNavigator* )navigator->GetPeerAI();
	
	if( peer ){
		peer->SetParentWorld( NULL );
	}
}

void dedaiWorld::AllNavigatorsRemoved(){
	deNavigator *navigator = pWorld.GetRootNavigator();
	dedaiNavigator *peer;
	
	while( navigator ){
		peer = ( dedaiNavigator* )navigator->GetPeerAI();
		navigator = navigator->GetLLWorldNext();
		
		if( peer ){
			peer->SetParentWorld( NULL );
		}
	}
}



void dedaiWorld::HeightTerrainChanged(){
	if( pHeightTerrain ){
		pHeightTerrain->SetParentWorld( NULL );
		pHeightTerrain = NULL;
	}
	
	if( pWorld.GetHeightTerrain() ){
		pHeightTerrain = ( ( dedaiHeightTerrain* )pWorld.GetHeightTerrain()->GetPeerAI() );
		if( pHeightTerrain ){
			pHeightTerrain->SetParentWorld( this );
		}
	}
}



// Private Functions
//////////////////////

void dedaiWorld::pCleanUp(){
	AllNavigatorsRemoved();
	AllNavigationBlockersRemoved();
	AllNavigationSpacesRemoved();
	
	if( pHeightTerrain ){
		pHeightTerrain->SetParentWorld( NULL );
	}
}
