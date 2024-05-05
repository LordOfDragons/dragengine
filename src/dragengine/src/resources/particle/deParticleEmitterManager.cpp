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
