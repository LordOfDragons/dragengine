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
