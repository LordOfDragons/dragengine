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
#include "deCaptureCanvas.h"
#include "deCaptureCanvasManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../systems/deGraphicSystem.h"


// Class deCaptureCanvasManager
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deCaptureCanvasManager::deCaptureCanvasManager( deEngine *engine ) : deResourceManager( engine, ertCaptureCanvas ){
	SetLoggingName( "capture canvas" );
}

deCaptureCanvasManager::~deCaptureCanvasManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deCaptureCanvasManager::GetCaptureCanvasCount() const{
	return pCaptureCanvas.GetCount();
}

deCaptureCanvas *deCaptureCanvasManager::GetRootCaptureCanvas() const{
	return ( deCaptureCanvas* )pCaptureCanvas.GetRoot();
}

deCaptureCanvas *deCaptureCanvasManager::CreateCaptureCanvas(){
	deCaptureCanvas *captureCanvas = NULL;
	
	try{
		captureCanvas = new deCaptureCanvas( this );
		GetGraphicSystem()->LoadCaptureCanvas( captureCanvas );
		pCaptureCanvas.Add( captureCanvas );
		
	}catch( const deException & ){
		if( captureCanvas ){
			captureCanvas->FreeReference();
		}
		throw;
	}
	
	return captureCanvas;
}



void deCaptureCanvasManager::ReleaseLeakingResources(){
	if( GetCaptureCanvasCount() > 0 ){
		LogWarnFormat( "%i leaking capture canvas", GetCaptureCanvasCount() );
		pCaptureCanvas.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deCaptureCanvasManager::SystemGraphicLoad(){
	deCaptureCanvas *captureCanvas = ( deCaptureCanvas* )pCaptureCanvas.GetRoot();
	
	while( captureCanvas ){
		if( ! captureCanvas->GetPeerGraphic() ){
			GetGraphicSystem()->LoadCaptureCanvas( captureCanvas );
		}
		
		captureCanvas = ( deCaptureCanvas* )captureCanvas->GetLLManagerNext();
	}
}

void deCaptureCanvasManager::SystemGraphicUnload(){
	deCaptureCanvas *captureCanvas = ( deCaptureCanvas* )pCaptureCanvas.GetRoot();
	
	while( captureCanvas ){
		captureCanvas->SetPeerGraphic( NULL );
		captureCanvas = ( deCaptureCanvas* )captureCanvas->GetLLManagerNext();
	}
}

void deCaptureCanvasManager::RemoveResource( deResource *resource ){
	pCaptureCanvas.RemoveIfPresent( resource );
}
