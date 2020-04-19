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

#include "deParticleEmitterInstance.h"
#include "deParticleEmitterInstanceManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitterInstance.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitterInstance.h"
#include "../../systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h"

#include "../../common/exceptions.h"



// Class deParticleEmitterInstanceManager
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterInstanceManager::deParticleEmitterInstanceManager( deEngine *engine ) : deResourceManager( engine, ertParticleEmitterInstance ){
	SetLoggingName( "particle emitter instance" );
}

deParticleEmitterInstanceManager::~deParticleEmitterInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deParticleEmitterInstanceManager::GetInstanceCount() const{
	return pInstances.GetCount();
}

deParticleEmitterInstance *deParticleEmitterInstanceManager::GetRootInstance() const{
	return ( deParticleEmitterInstance* )pInstances.GetRoot();
}

deParticleEmitterInstance *deParticleEmitterInstanceManager::CreateInstance(){
	deParticleEmitterInstance *instance = NULL;
	
	try{
		instance = new deParticleEmitterInstance( this );
		
		GetGraphicSystem()->LoadParticleEmitterInstance( instance );
		GetPhysicsSystem()->LoadParticleEmitterInstance( instance );
		GetScriptingSystem()->LoadParticleEmitterInstance( instance );
		
		pInstances.Add( instance );
		
	}catch( const deException & ){
		if( instance ){
			instance->FreeReference();
		}
		throw;
	}
	
	return instance;
}



void deParticleEmitterInstanceManager::ReleaseLeakingResources(){
	if( GetInstanceCount() > 0 ){
		LogWarnFormat( "%i leaking particle emitter instances", GetInstanceCount() );
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deParticleEmitterInstanceManager::SystemGraphicLoad(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( instance ){
		if( ! instance->GetPeerGraphic() ){
			grasys.LoadParticleEmitterInstance( instance );
		}
		
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemGraphicUnload(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerGraphic( NULL );
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemPhysicsLoad(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	dePhysicsSystem &physys = *GetPhysicsSystem();
	
	while( instance ){
		if( ! instance->GetPeerPhysics() ){
			physys.LoadParticleEmitterInstance( instance );
		}
		
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemPhysicsUnload(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerPhysics( NULL );
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemScriptingLoad(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	deScriptingSystem &scrsys = *GetScriptingSystem();
	
	while( instance ){
		if( ! instance->GetPeerScripting() ){
			scrsys.LoadParticleEmitterInstance( instance );
		}
		
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}

void deParticleEmitterInstanceManager::SystemScriptingUnload(){
	deParticleEmitterInstance *instance = ( deParticleEmitterInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerScripting( NULL );
		instance = ( deParticleEmitterInstance* )instance->GetLLManagerNext();
	}
}



void deParticleEmitterInstanceManager::RemoveResource( deResource *resource ){
	pInstances.RemoveIfPresent( resource );
}
