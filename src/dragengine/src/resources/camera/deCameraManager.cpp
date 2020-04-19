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
#include "deCamera.h"
#include "deCameraManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deGraphicSystem.h"



// Class deCameraManager
//////////////////////////

// Constructor, destructor
////////////////////////////

deCameraManager::deCameraManager( deEngine *engine ) : deResourceManager( engine, ertCamera ){
	SetLoggingName( "camera" );
}

deCameraManager::~deCameraManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deCameraManager::GetCameraCount() const{
	return pCameras.GetCount();
}

deCamera *deCameraManager::GetRootCamera() const{
	return ( deCamera* )pCameras.GetRoot();
}

deCamera *deCameraManager::CreateCamera(){
	deCamera *camera = NULL;
	
	// create and add camera
	try{
		camera = new deCamera( this );
		if( ! camera ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadCamera( camera );
		pCameras.Add( camera );
		
	}catch( const deException & ){
		if( camera ){
			camera->FreeReference();
		}
		throw;
	}
	
	// finished
	return camera;
}



void deCameraManager::ReleaseLeakingResources(){
	if( GetCameraCount() > 0 ){
		LogWarnFormat( "%i leaking cameras", GetCameraCount() );
		pCameras.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deCameraManager::SystemGraphicLoad(){
	deCamera *camera = ( deCamera* )pCameras.GetRoot();
	
	while( camera ){
		if( ! camera->GetPeerGraphic() ){
			GetGraphicSystem()->LoadCamera( camera );
		}
		
		camera = ( deCamera* )camera->GetLLManagerNext();
	}
}

void deCameraManager::SystemGraphicUnload(){
	deCamera *camera = ( deCamera* )pCameras.GetRoot();
	
	while( camera ){
		camera->SetPeerGraphic( NULL );
		camera = ( deCamera* )camera->GetLLManagerNext();
	}
}

void deCameraManager::RemoveResource( deResource *resource ){
	pCameras.RemoveIfPresent( resource );
}
