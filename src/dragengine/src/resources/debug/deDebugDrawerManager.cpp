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
#include "deDebugDrawer.h"
#include "deDebugDrawerManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deDebugDrawerManager
///////////////////////////////

// constructor, destructor
////////////////////////////

deDebugDrawerManager::deDebugDrawerManager( deEngine *engine ) : deResourceManager( engine, ertDebugDrawer ){
	SetLoggingName( "debug drawer" );
}

deDebugDrawerManager::~deDebugDrawerManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deDebugDrawerManager::GetDebugDrawerCount() const{
	return pDebugDrawers.GetCount();
}

deDebugDrawer *deDebugDrawerManager::GetRootDebugDrawer() const{
	return ( deDebugDrawer* )pDebugDrawers.GetRoot();
}

deDebugDrawer *deDebugDrawerManager::CreateDebugDrawer(){
	deDebugDrawer *debugDrawer = NULL;
	// create and add debug drawer
	try{
		debugDrawer = new deDebugDrawer( this );
		if( ! debugDrawer ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadDebugDrawer( debugDrawer );
		pDebugDrawers.Add( debugDrawer );
	}catch( const deException & ){
		if( debugDrawer ){
			debugDrawer->FreeReference();
		}
		throw;
	}
	// finished
	return debugDrawer;
}



void deDebugDrawerManager::ReleaseLeakingResources(){
	if( GetDebugDrawerCount() > 0 ){
		LogWarnFormat( "%i leaking debug drawers", GetDebugDrawerCount() );
		pDebugDrawers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deDebugDrawerManager::SystemGraphicLoad(){
	deDebugDrawer *debugDrawer = ( deDebugDrawer* )pDebugDrawers.GetRoot();
	
	while( debugDrawer ){
		if( ! debugDrawer->GetPeerGraphic() ){
			GetGraphicSystem()->LoadDebugDrawer( debugDrawer );
		}
		
		debugDrawer = ( deDebugDrawer* )debugDrawer->GetLLManagerNext();
	}
}

void deDebugDrawerManager::SystemGraphicUnload(){
	deDebugDrawer *debugDrawer = ( deDebugDrawer* )pDebugDrawers.GetRoot();
	
	while( debugDrawer ){
		debugDrawer->SetPeerGraphic( NULL );
		debugDrawer = ( deDebugDrawer* )debugDrawer->GetLLManagerNext();
	}
}

void deDebugDrawerManager::RemoveResource( deResource *resource ){
	pDebugDrawers.RemoveIfPresent( resource );
}
