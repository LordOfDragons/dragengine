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
#include <string.h>

#include "deDecal.h"
#include "deDecalManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deDecalManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deDecalManager::deDecalManager( deEngine *engine ) : deResourceManager( engine, ertDecal ){
	SetLoggingName( "decal" );
}

deDecalManager::~deDecalManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deDecalManager::GetDecalCount() const{
	return pDecals.GetCount();
}

deDecal *deDecalManager::GetRootDecal() const{
	return ( deDecal* )pDecals.GetRoot();
}

deDecal *deDecalManager::CreateDecal(){
	deDecal *decal = NULL;
	
	try{
		decal = new deDecal( this );
		
		GetGraphicSystem()->LoadDecal( decal );
		GetPhysicsSystem()->LoadDecal( decal );
		GetAudioSystem()->LoadDecal( decal );
		
		pDecals.Add( decal );
		
	}catch( const deException & ){
		if( decal ){
			decal->FreeReference();
		}
		throw;
	}
	
	return decal;
}



void deDecalManager::ReleaseLeakingResources(){
	const int count = GetDecalCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking decals", count );
		pDecals.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deDecalManager::SystemGraphicLoad(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while( decal ){
		if( ! decal->GetPeerGraphic() ){
			graSys.LoadDecal( decal );
		}
		
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}

void deDecalManager::SystemGraphicUnload(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	
	while( decal ){
		decal->SetPeerGraphic( NULL );
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}

void deDecalManager::SystemPhysicsLoad(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	
	while( decal ){
		if( ! decal->GetPeerPhysics() ){
			phySys.LoadDecal( decal );
		}
		
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}

void deDecalManager::SystemPhysicsUnload(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	
	while( decal ){
		decal->SetPeerPhysics( NULL );
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}

void deDecalManager::SystemAudioLoad(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while( decal ){
		if( ! decal->GetPeerAudio() ){
			audSys.LoadDecal( decal );
		}
		
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}

void deDecalManager::SystemAudioUnload(){
	deDecal *decal = ( deDecal* )pDecals.GetRoot();
	
	while( decal ){
		decal->SetPeerAudio( NULL );
		decal = ( deDecal* )decal->GetLLManagerNext();
	}
}



void deDecalManager::RemoveResource( deResource *resource ){
	pDecals.RemoveIfPresent( resource );
}
