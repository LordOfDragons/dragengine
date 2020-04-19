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

#include "deParticleEmitter.h"
#include "deParticleEmitterManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitter.h"

#include "../../common/exceptions.h"



// Class deParticleEmitterManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterManager::deParticleEmitterManager( deEngine *engine ) : deResourceManager( engine, ertParticleEmitter ){
	SetLoggingName( "particle emitter" );
}

deParticleEmitterManager::~deParticleEmitterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deParticleEmitterManager::GetParticleEmitterCount() const{
	return pParticleEmitters.GetCount();
}

deParticleEmitter *deParticleEmitterManager::GetRootParticleEmitter() const{
	return ( deParticleEmitter* )pParticleEmitters.GetRoot();
}

deParticleEmitter *deParticleEmitterManager::CreateParticleEmitter(){
	deParticleEmitter *psys = NULL;
	
	try{
		psys = new deParticleEmitter( this );
		if( ! psys ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadParticleEmitter( psys );
		GetPhysicsSystem()->LoadParticleEmitter( psys );
		
		pParticleEmitters.Add( psys );
		
	}catch( const deException & ){
		if( psys ){
			psys->FreeReference();
		}
		throw;
	}
	
	return psys;
}



void deParticleEmitterManager::ReleaseLeakingResources(){
	if( GetParticleEmitterCount() > 0 ){
		LogWarnFormat( "%i leaking particle emitters", GetParticleEmitterCount() );
		pParticleEmitters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deParticleEmitterManager::SystemGraphicLoad(){
	deParticleEmitter *psys = ( deParticleEmitter* )pParticleEmitters.GetRoot();
	
	while( psys ){
		if( ! psys->GetPeerGraphic() ){
			GetGraphicSystem()->LoadParticleEmitter( psys );
		}
		
		psys = ( deParticleEmitter* )psys->GetLLManagerNext();
	}
}

void deParticleEmitterManager::SystemGraphicUnload(){
	deParticleEmitter *psys = ( deParticleEmitter* )pParticleEmitters.GetRoot();
	
	while( psys ){
		psys->SetPeerGraphic( NULL );
		psys = ( deParticleEmitter* )psys->GetLLManagerNext();
	}
}

void deParticleEmitterManager::SystemPhysicsLoad(){
	deParticleEmitter *psys = ( deParticleEmitter* )pParticleEmitters.GetRoot();
	
	while( psys ){
		if( ! psys->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadParticleEmitter( psys );
		}
		
		psys = ( deParticleEmitter* )psys->GetLLManagerNext();
	}
}

void deParticleEmitterManager::SystemPhysicsUnload(){
	deParticleEmitter *psys = ( deParticleEmitter* )pParticleEmitters.GetRoot();
	
	while( psys ){
		psys->SetPeerPhysics( NULL );
		psys = ( deParticleEmitter* )psys->GetLLManagerNext();
	}
}



void deParticleEmitterManager::RemoveResource( deResource *resource ){
	pParticleEmitters.RemoveIfPresent( resource );
}
