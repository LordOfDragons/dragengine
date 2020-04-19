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
#include "deSky.h"
#include "deSkyManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deSkyManager
/////////////////////////////

// constructor, destructor
deSkyManager::deSkyManager( deEngine *engine ) : deResourceManager( engine, ertSky ){
	SetLoggingName( "sky" );
}
deSkyManager::~deSkyManager(){
	ReleaseLeakingResources();
}

// management
int deSkyManager::GetSkyCount() const{
	return pSkies.GetCount();
}

deSky *deSkyManager::GetRootSky() const{
	return ( deSky* )pSkies.GetRoot();
}

deSky *deSkyManager::CreateSky(){
	deSky *sky = NULL;
	// create and add sky
	try{
		sky = new deSky( this );
		if( ! sky ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadSky( sky );
		pSkies.Add( sky );
	}catch( const deException & ){
		if( sky ){
			sky->FreeReference();
		}
		throw;
	}
	// finished
	return sky;
}



void deSkyManager::ReleaseLeakingResources(){
	if( GetSkyCount() > 0 ){
		LogWarnFormat( "%i leaking skies", GetSkyCount() );
		pSkies.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSkyManager::SystemGraphicLoad(){
	deSky *sky = ( deSky* )pSkies.GetRoot();
	
	while( sky ){
		if( ! sky->GetPeerGraphic() ){
			GetGraphicSystem()->LoadSky( sky );
		}
		sky = ( deSky* )sky->GetLLManagerNext();
	}
}

void deSkyManager::SystemGraphicUnload(){
	deSky *sky = ( deSky* )pSkies.GetRoot();
	
	while( sky ){
		sky->SetPeerGraphic( NULL );
		sky = ( deSky* )sky->GetLLManagerNext();
	}
}

void deSkyManager::RemoveResource( deResource *resource ){
	pSkies.RemoveIfPresent( resource );
}
