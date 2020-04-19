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
#include "deLight.h"
#include "deLightManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deLightManager
/////////////////////////////

// constructor, destructor
deLightManager::deLightManager( deEngine *engine ) : deResourceManager( engine, ertLight ){
	SetLoggingName( "light" );
}
deLightManager::~deLightManager(){
	ReleaseLeakingResources();
}

// management
int deLightManager::GetLightCount() const{
	return pLights.GetCount();
}

deLight *deLightManager::GetRootLight() const{
	return ( deLight* )pLights.GetRoot();
}

deLight *deLightManager::CreateLight(){
	deLight *light = NULL;
	// create and add light
	try{
		light = new deLight( this );
		if( ! light ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadLight( light );
		pLights.Add( light );
	}catch( const deException & ){
		if( light ){
			light->FreeReference();
		}
		throw;
	}
	// finished
	return light;
}



void deLightManager::ReleaseLeakingResources(){
	if( GetLightCount() > 0 ){
		LogWarnFormat( "%i leaking lights", GetLightCount() );
		pLights.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deLightManager::SystemGraphicLoad(){
	deLight *light = ( deLight* )pLights.GetRoot();
	
	while( light ){
		if( ! light->GetPeerGraphic() ){
			GetGraphicSystem()->LoadLight( light );
		}
		
		light = ( deLight* )light->GetLLManagerNext();
	}
}

void deLightManager::SystemGraphicUnload(){
	deLight *light = ( deLight* )pLights.GetRoot();
	
	while( light ){
		light->SetPeerGraphic( NULL );
		light = ( deLight* )light->GetLLManagerNext();
	}
}

void deLightManager::RemoveResource( deResource *resource ){
	pLights.RemoveIfPresent( resource );
}
