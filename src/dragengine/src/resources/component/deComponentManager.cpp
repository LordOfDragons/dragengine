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

#include "deComponent.h"
#include "deComponentManager.h"
#include "../model/deModel.h"
#include "../animation/deAnimation.h"
#include "../skin/deSkin.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deComponentManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deComponentManager::deComponentManager( deEngine *engine ) : deResourceManager( engine, ertComponent ){
	SetLoggingName( "component" );
}

deComponentManager::~deComponentManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deComponentManager::GetComponentCount() const{
	return pComponents.GetCount();
}

deComponent *deComponentManager::GetRootComponent() const{
	return ( deComponent* )pComponents.GetRoot();
}

deComponent *deComponentManager::CreateComponent( deModel *model, deSkin *skin ){
	deComponent *component = NULL;
	
	try{
		component = new deComponent(this, model, skin);
		
		GetGraphicSystem()->LoadComponent( component );
		GetPhysicsSystem()->LoadComponent( component );
		GetAudioSystem()->LoadComponent( component );
		GetAnimatorSystem()->LoadComponent( component );
		
		pComponents.Add( component );
		
	}catch( const deException & ){
		if( component ){
			component->FreeReference();
		}
		throw;
	}
	
	return component;
}



void deComponentManager::ReleaseLeakingResources(){
	if( GetComponentCount() > 0 ){
		LogWarnFormat( "%i leaking components", GetComponentCount() );
		pComponents.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deComponentManager::SystemGraphicLoad(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		if( ! component->GetPeerGraphic() ){
			GetGraphicSystem()->LoadComponent( component );
		}
		
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemGraphicUnload(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		component->SetPeerGraphic( NULL );
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemPhysicsLoad(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		if( ! component->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadComponent( component );
		}
		
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemPhysicsUnload(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		component->SetPeerPhysics( NULL );
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAudioLoad(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		if( ! component->GetPeerAudio() ){
			GetAudioSystem()->LoadComponent( component );
		}
		
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAudioUnload(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		component->SetPeerAudio( NULL );
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAnimatorLoad(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	deAnimatorSystem &manager = *GetAnimatorSystem();
	
	while( component ){
		if( ! component->GetPeerAnimator() ){
			manager.LoadComponent( component );
		}
		
		component = ( deComponent* )component->GetLLManagerNext();
	}
}

void deComponentManager::SystemAnimatorUnload(){
	deComponent *component = ( deComponent* )pComponents.GetRoot();
	
	while( component ){
		component->SetPeerAnimator( NULL );
		component = ( deComponent* )component->GetLLManagerNext();
	}
}



void deComponentManager::RemoveResource( deResource *resource ){
	pComponents.RemoveIfPresent( resource );
}
