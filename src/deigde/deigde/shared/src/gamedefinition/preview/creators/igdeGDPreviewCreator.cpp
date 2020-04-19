/* 
 * Drag[en]gine IGDE
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

#include "igdeGDPreviewCreator.h"
#include "../igdeGDPreviewListener.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvasManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class igdeGDPreviewCreator
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPreviewCreator::igdeGDPreviewCreator( igdeEnvironment &environment, const decPoint &size ) :
pEnvironment( environment ),
pState( esInitial ),
pEnableDebug( false )
{
	if( ! ( size > decPoint( 1, 1 ) ) ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	pCanvas.TakeOver( engine.GetCanvasManager()->CreateCanvasView() );
	pCanvas->SetSize( size );
	
	pImage.TakeOver( engine.GetImageManager()->CreateImage( size.x, size.y, 1, 3, 8 ) );
	
	pCaptureCanvas.TakeOver( engine.GetCaptureCanvasManager()->CreateCaptureCanvas() );
	pCaptureCanvas->SetCanvasView( pCanvas );
	pCaptureCanvas->SetImage( pImage );
}

igdeGDPreviewCreator::~igdeGDPreviewCreator(){
}



// Management
///////////////

void igdeGDPreviewCreator::SetImage( deImage *image ){
	pImage = image;
	pCaptureCanvas->SetImage( image );
}

void igdeGDPreviewCreator::AddListener( igdeGDPreviewListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}



void igdeGDPreviewCreator::Update( float elapsed ){
	pUpdate();
	
	if( pState == esFrameUpdate ){
		DebugLog( "update canvas for render" );
		UpdateCanvasForRender( elapsed );
		pState = esBeginCapture;
		
		pUpdate();
	}
}

void igdeGDPreviewCreator::BeginCreation(){
	if( pState == esInitial ){
		DebugLog( "prepare canvas" );
		pState = esPrepareCanvas;
		pUpdate();
	}
}

bool igdeGDPreviewCreator::FinishCreation(){
	pUpdate();
	
	if( pState != esCaptureFinished ){
		return false;
	}
	
	DebugLog( "finish creation" );
	NotifyImageCreated( pImage );
	pListeners.RemoveAll();
	return true;
}

void igdeGDPreviewCreator::AbortCreation(){
	DebugLog( "abort creation" );
	pState = esCaptureFinished;
	pCaptureCanvas->SetCapture( false );
	NotifyImageCreated( NULL );
}



void igdeGDPreviewCreator::NotifyImageCreated( deImage *image ){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeGDPreviewListener* )pListeners.GetAt( i ) )->ImageCreated( image );
	}
}

void igdeGDPreviewCreator::DebugLog( const char *message ){
	if( ! pEnableDebug ){
		return;
	}
	
	/*
	static const char * const stateNames[] = {
		"initial", "prepareCanvas", "waitCanvasReady", "frameUpdate",
		"beginCapture", "waitCaptureFinished", "captureFinished"
	};
	*/
	
	const decString prefix = DebugPrefix();
	//pEnvironment.GetLogger()->LogInfoFormat( "%s [%s] %s", prefix.GetString(), stateNames[ pState ], message );
	pEnvironment.GetLogger()->LogInfoFormat( "IGDE", "%s %s", prefix.GetString(), message );
}



// Private Functions
//////////////////////

void igdeGDPreviewCreator::pUpdate(){
	if( pState == esInitial ){
		return;
	}
	
	if( pState == esPrepareCanvas ){
		DebugLog( "prepare canvas for render" );
		PrepareCanvasForRender();
		pState = esWaitCanvasReady;
	}
	
	if( pState == esWaitCanvasReady ){
		if( ! IsCanvasReadyForRender() ){
			return;
		}
		DebugLog( "canvas ready for render" );
		pState = esBeginCapture;
	}
	
	if( pState == esBeginCapture ){
		DebugLog( "begin capture" );
		pCaptureCanvas->SetCapture( true );
		pState = esWaitCaptureFinished;
	}
	
	if( pState == esWaitCaptureFinished ){
		if( pCaptureCanvas->GetCapture() ){
			return;
		}
		DebugLog( "capture finished" );
		pState = esCaptureFinished;
	}
}
