/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

deHeightTerrainManager::deHeightTerrainManager( deEngine *engine ) :
deResourceManager( engine, ertHeightTerrain ){
	SetLoggingName( "Height Terrain Manager" );
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
	return ( deHeightTerrain* )pTerrains.GetRoot();
}

deHeightTerrain *deHeightTerrainManager::CreateHeightTerrain( float sectorSize, int heightImageSize ){
	if( sectorSize < 0.001f || heightImageSize < 2 ){
		DETHROW( deeInvalidParam );
	}
	
	deHeightTerrain *heightTerrain = NULL;
	
	try{
		heightTerrain = new deHeightTerrain( this, sectorSize, heightImageSize );
		
		GetGraphicSystem()->LoadHeightTerrain( heightTerrain );
		GetPhysicsSystem()->LoadHeightTerrain( heightTerrain );
		GetAISystem()->LoadHeightTerrain( *heightTerrain );
		
		pTerrains.Add( heightTerrain );
		
	}catch( const deException & ){
		if( heightTerrain ){
			heightTerrain->FreeReference();
		}
		throw;
	}
	
	return heightTerrain;
}



void deHeightTerrainManager::ReleaseLeakingResources(){
	if( GetHeightTerrainCount() == 0 ){
		return;
	}
	
	LogWarnFormat( "%i leaking height terrains.", GetHeightTerrainCount() );
	pTerrains.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
}



// Systems Support
////////////////////

void deHeightTerrainManager::SystemGraphicLoad(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( heightTerrain ){
		if( ! heightTerrain->GetPeerGraphic() ){
			grasys.LoadHeightTerrain( heightTerrain );
		}
		
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemGraphicUnload(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	
	while( heightTerrain ){
		heightTerrain->SetPeerGraphic( NULL );
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemPhysicsLoad(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while( heightTerrain ){
		if( ! heightTerrain->GetPeerPhysics() ){
			physys.LoadHeightTerrain( heightTerrain );
		}
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemPhysicsUnload(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	
	while( heightTerrain ){
		heightTerrain->SetPeerPhysics( NULL );
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemAILoad(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while( heightTerrain ){
		if( ! heightTerrain->GetPeerAI() ){
			aisys.LoadHeightTerrain( *heightTerrain );
		}
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemAIUnload(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	
	while( heightTerrain ){
		heightTerrain->SetPeerAI( NULL );
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemAudioLoad(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	deAudioSystem &audioSystem = *GetAudioSystem();
	
	while( heightTerrain ){
		if( ! heightTerrain->GetPeerAudio() ){
			audioSystem.LoadHeightTerrain( *heightTerrain );
		}
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}

void deHeightTerrainManager::SystemAudioUnload(){
	deHeightTerrain *heightTerrain = ( deHeightTerrain* )pTerrains.GetRoot();
	
	while( heightTerrain ){
		heightTerrain->SetPeerAudio( NULL );
		heightTerrain = ( deHeightTerrain* )heightTerrain->GetLLManagerNext();
	}
}



void deHeightTerrainManager::RemoveResource( deResource *resource ){
	pTerrains.RemoveIfPresent( resource );
}
