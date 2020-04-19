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
#include <string.h>

#include "deWorld.h"
#include "deWorldManager.h"
#include "../../systems/deAISystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deWorldManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deWorldManager::deWorldManager( deEngine *engine ) : deResourceManager( engine, ertWorld ){
	SetLoggingName( "world" );
}

deWorldManager::~deWorldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deWorldManager::GetWorldCount() const{
	return pWorlds.GetCount();
}

deWorld *deWorldManager::GetRootWorld() const{
	return ( deWorld* )pWorlds.GetRoot();
}

deWorld *deWorldManager::CreateWorld(){
	deWorld *world = NULL;
	
	try{
		world = new deWorld( this );
		if( ! world ){
			DETHROW( deeOutOfMemory );
		}
		
		GetGraphicSystem()->LoadWorld( world );
		GetPhysicsSystem()->LoadWorld( world );
		GetAudioSystem()->LoadWorld( world );
		GetNetworkSystem()->LoadWorld( world );
		GetAISystem()->LoadWorld( world );
		
		pWorlds.Add( world );
		
	}catch( const deException & ){
		if( world ){
			world->FreeReference();
		}
		throw;
	}
	
	return world;
}



void deWorldManager::ReleaseLeakingResources(){
	const int count = GetWorldCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking worlds", count );
		pWorlds.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deWorldManager::SystemGraphicLoad(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( world ){
		if( ! world->GetPeerGraphic() ){
			grasys.LoadWorld( world );
		}
		
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemGraphicUnload(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	
	while( world ){
		world->SetPeerGraphic( NULL );
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemPhysicsLoad(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while( world ){
		if( ! world->GetPeerPhysics() ){
			physys.LoadWorld( world );
		}
		
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemPhysicsUnload(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	
	while( world ){
		world->SetPeerPhysics( NULL );
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAudioLoad(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	deAudioSystem &audsys = *GetAudioSystem();
	
	while( world ){
		if( ! world->GetPeerAudio() ){
			audsys.LoadWorld( world );
		}
		
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAudioUnload(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	
	while( world ){
		world->SetPeerAudio( NULL );
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemNetworkLoad(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	deNetworkSystem &netsys = *GetNetworkSystem();
	
	while( world ){
		if( ! world->GetPeerNetwork() ){
			netsys.LoadWorld( world );
		}
		
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemNetworkUnload(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	
	while( world ){
		world->SetPeerNetwork( NULL );
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAILoad(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while( world ){
		if( ! world->GetPeerAI() ){
			aisys.LoadWorld( world );
		}
		
		world = ( deWorld* )world->GetLLManagerNext();
	}
}

void deWorldManager::SystemAIUnload(){
	deWorld *world = ( deWorld* )pWorlds.GetRoot();
	
	while( world ){
		world->SetPeerAI( NULL );
		world = ( deWorld* )world->GetLLManagerNext();
	}
}



void deWorldManager::RemoveResource( deResource *resource ){
	pWorlds.RemoveIfPresent( resource );
}
