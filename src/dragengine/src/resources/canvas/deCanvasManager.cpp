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
#include "deCanvas.h"
#include "deCanvasImage.h"
#include "deCanvasPaint.h"
#include "deCanvasRenderWorld.h"
#include "deCanvasText.h"
#include "deCanvasVideoPlayer.h"
#include "deCanvasView.h"
#include "deCanvasManager.h"
#include "deCanvasCanvasView.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deCanvasManager
//////////////////////////

// Constructor, Destructor
////////////////////////////

deCanvasManager::deCanvasManager( deEngine *engine ) : deResourceManager( engine, ertCanvas ){
	SetLoggingName( "canvas" );
}

deCanvasManager::~deCanvasManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deCanvasManager::GetCanvasCount() const{
	return pCanvas.GetCount();
}

deCanvas *deCanvasManager::GetRootCanvas() const{
	return ( deCanvas* )pCanvas.GetRoot();
}

deCanvasImage *deCanvasManager::CreateCanvasImage(){
	deCanvasImage *canvas = NULL;
	
	try{
		canvas = new deCanvasImage( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasPaint *deCanvasManager::CreateCanvasPaint(){
	deCanvasPaint *canvas = NULL;
	
	try{
		canvas = new deCanvasPaint( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasCanvasView *deCanvasManager::CreateCanvasCanvasView(){
	deCanvasCanvasView *canvas = NULL;
	
	try{
		canvas = new deCanvasCanvasView( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasRenderWorld *deCanvasManager::CreateCanvasRenderWorld(){
	deCanvasRenderWorld *canvas = NULL;
	
	try{
		canvas = new deCanvasRenderWorld( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasText *deCanvasManager::CreateCanvasText(){
	deCanvasText *canvas = NULL;
	
	try{
		canvas = new deCanvasText( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasVideoPlayer *deCanvasManager::CreateCanvasVideoPlayer(){
	deCanvasVideoPlayer *canvas = NULL;
	
	try{
		canvas = new deCanvasVideoPlayer( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}

deCanvasView *deCanvasManager::CreateCanvasView(){
	deCanvasView *canvas = NULL;
	
	try{
		canvas = new deCanvasView( this );
		GetGraphicSystem()->LoadCanvas( canvas );
		pCanvas.Add( canvas );
		
	}catch( const deException & ){
		if( canvas ){
			canvas->FreeReference();
		}
		throw;
	}
	
	return canvas;
}



void deCanvasManager::ReleaseLeakingResources(){
	if( GetCanvasCount() > 0 ){
		LogWarnFormat( "%i leaking canvas", GetCanvasCount() );
		pCanvas.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deCanvasManager::SystemGraphicLoad(){
	deCanvas *canvas = ( deCanvas* )pCanvas.GetRoot();
	
	while( canvas ){
		if( ! canvas->GetPeerGraphic() ){
			GetGraphicSystem()->LoadCanvas( canvas );
		}
		
		canvas = ( deCanvas* )canvas->GetLLManagerNext();
	}
}

void deCanvasManager::SystemGraphicUnload(){
	deCanvas *canvas = ( deCanvas* )pCanvas.GetRoot();
	
	while( canvas ){
		canvas->SetPeerGraphic( NULL );
		canvas = ( deCanvas* )canvas->GetLLManagerNext();
	}
}

void deCanvasManager::RemoveResource( deResource *resource ){
	pCanvas.RemoveIfPresent( resource );
}
