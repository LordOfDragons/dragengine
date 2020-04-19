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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deCollider.h"
#include "deColliderVolume.h"
#include "deColliderRig.h"
#include "deColliderComponent.h"
#include "deColliderManager.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deColliderManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deColliderManager::deColliderManager( deEngine *engine ) : deResourceManager( engine, ertCollider ){
	SetLoggingName( "collider" );
}

deColliderManager::~deColliderManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deColliderManager::GetColliderCount() const{
	return pColliders.GetCount();
}

deCollider *deColliderManager::GetRootCollider() const{
	return ( deCollider* )pColliders.GetRoot();
}

deColliderVolume *deColliderManager::CreateColliderVolume(){
	deColliderVolume *collider = NULL;
	// create and add collider
	try{
		collider = new deColliderVolume( this );
		if( ! collider ) DETHROW( deeOutOfMemory );
		GetPhysicsSystem()->LoadCollider( collider );
		GetScriptingSystem()->LoadCollider( collider );
		pColliders.Add( collider );
	}catch( const deException & ){
		if( collider ){
			collider->FreeReference();
		}
		throw;
	}
	// finished
	return collider;
}

deColliderRig *deColliderManager::CreateColliderRig(){
	deColliderRig *collider = NULL;
	// create and add collider
	try{
		collider = new deColliderRig( this );
		if( ! collider ) DETHROW( deeOutOfMemory );
		GetPhysicsSystem()->LoadCollider( collider );
		GetScriptingSystem()->LoadCollider( collider );
		pColliders.Add( collider );
	}catch( const deException & ){
		if( collider ){
			collider->FreeReference();
		}
		throw;
	}
	// finished
	return collider;
}

deColliderComponent *deColliderManager::CreateColliderComponent(){
	deColliderComponent *collider = NULL;
	// create and add collider
	try{
		collider = new deColliderComponent( this );
		if( ! collider ) DETHROW( deeOutOfMemory );
		GetPhysicsSystem()->LoadCollider( collider );
		GetScriptingSystem()->LoadCollider( collider );
		pColliders.Add( collider );
	}catch( const deException & ){
		if( collider ){
			collider->FreeReference();
		}
		throw;
	}
	// finished
	return collider;
}



void deColliderManager::ReleaseLeakingResources(){
	if( GetColliderCount() > 0 ){
		LogWarnFormat( "%i leaking colliders", GetColliderCount() );
		pColliders.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deColliderManager::SystemPhysicsLoad(){
	deCollider *collider = ( deCollider* )pColliders.GetRoot();
	
	while( collider ){
		if( ! collider->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadCollider( collider );
		}
		
		collider = ( deCollider* )collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemPhysicsUnload(){
	deCollider *collider = ( deCollider* )pColliders.GetRoot();
	
	while( collider ){
		collider->SetPeerPhysics( NULL );
		collider = ( deCollider* )collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemScriptingLoad(){
	deCollider *collider = ( deCollider* )pColliders.GetRoot();
	
	while( collider ){
		if( ! collider->GetPeerScripting() ){
			GetScriptingSystem()->LoadCollider( collider );
		}
		
		collider = ( deCollider* )collider->GetLLManagerNext();
	}
}

void deColliderManager::SystemScriptingUnload(){
	deCollider *collider = ( deCollider* )pColliders.GetRoot();
	
	while( collider ){
		collider->SetPeerScripting( NULL );
		collider = ( deCollider* )collider->GetLLManagerNext();
	}
}

void deColliderManager::RemoveResource( deResource *resource ){
	pColliders.RemoveIfPresent( resource );
}
