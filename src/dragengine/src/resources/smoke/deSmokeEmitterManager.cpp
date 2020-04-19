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

#include "deSmokeEmitter.h"
#include "deSmokeEmitterManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicSmokeEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsSmokeEmitter.h"

#include "../../common/exceptions.h"



// Class deSmokeEmitterManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSmokeEmitterManager::deSmokeEmitterManager( deEngine *engine ) : deResourceManager( engine, ertSmokeEmitter ){
	SetLoggingName( "smoke emitter" );
}

deSmokeEmitterManager::~deSmokeEmitterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSmokeEmitterManager::GetSmokeEmitterCount() const{
	return pSmokeEmitters.GetCount();
}

deSmokeEmitter *deSmokeEmitterManager::GetRootSmokeEmitter() const{
	return ( deSmokeEmitter* )pSmokeEmitters.GetRoot();
}

deSmokeEmitter *deSmokeEmitterManager::CreateSmokeEmitter(){
	deSmokeEmitter *smokeEmitter = NULL;
	
	try{
		smokeEmitter = new deSmokeEmitter( this );
		if( ! smokeEmitter ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadSmokeEmitter( smokeEmitter );
		GetPhysicsSystem()->LoadSmokeEmitter( smokeEmitter );
		
		pSmokeEmitters.Add( smokeEmitter );
		
	}catch( const deException & ){
		if( smokeEmitter ){
			smokeEmitter->FreeReference();
		}
		throw;
	}
	
	return smokeEmitter;
}



void deSmokeEmitterManager::ReleaseLeakingResources(){
	if( GetSmokeEmitterCount() > 0 ){
		LogWarnFormat( "%i leaking smoke emitters", GetSmokeEmitterCount() );
		pSmokeEmitters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSmokeEmitterManager::SystemGraphicLoad(){
	deSmokeEmitter *smokeEmitter = ( deSmokeEmitter* )pSmokeEmitters.GetRoot();
	
	while( smokeEmitter ){
		if( ! smokeEmitter->GetPeerGraphic() ){
			GetGraphicSystem()->LoadSmokeEmitter( smokeEmitter );
		}
		
		smokeEmitter = ( deSmokeEmitter* )smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemGraphicUnload(){
	deSmokeEmitter *smokeEmitter = ( deSmokeEmitter* )pSmokeEmitters.GetRoot();
	
	while( smokeEmitter ){
		smokeEmitter->SetPeerGraphic( NULL );
		smokeEmitter = ( deSmokeEmitter* )smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemPhysicsLoad(){
	deSmokeEmitter *smokeEmitter = ( deSmokeEmitter* )pSmokeEmitters.GetRoot();
	
	while( smokeEmitter ){
		if( ! smokeEmitter->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadSmokeEmitter( smokeEmitter );
		}
		
		smokeEmitter = ( deSmokeEmitter* )smokeEmitter->GetLLManagerNext();
	}
}

void deSmokeEmitterManager::SystemPhysicsUnload(){
	deSmokeEmitter *smokeEmitter = ( deSmokeEmitter* )pSmokeEmitters.GetRoot();
	
	while( smokeEmitter ){
		smokeEmitter->SetPeerPhysics( NULL );
		smokeEmitter = ( deSmokeEmitter* )smokeEmitter->GetLLManagerNext();
	}
}



void deSmokeEmitterManager::RemoveResource( deResource *resource ){
	pSmokeEmitters.RemoveIfPresent( resource );
}
