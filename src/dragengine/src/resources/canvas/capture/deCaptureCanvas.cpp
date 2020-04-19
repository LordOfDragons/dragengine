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
#include "../deCanvasView.h"
#include "../../image/deImage.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/graphic/deBaseGraphicCaptureCanvas.h"



// Class deCaptureCanvas
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCaptureCanvas::deCaptureCanvas( deCaptureCanvasManager *manager ) :
deResource( manager ),

pCapture( false ),

pPeerGraphic( NULL ){
}

deCaptureCanvas::~deCaptureCanvas(){
	SetPeerGraphic( NULL );
}



// Management
///////////////

void deCaptureCanvas::SetCanvasView( deCanvasView *canvasView ){
	if( canvasView == pCanvasView ){
		return;
	}
	
	pCanvasView = canvasView;
	
	if( pPeerGraphic ){
		pPeerGraphic->CanvasViewChanged();
	}
}

void deCaptureCanvas::SetImage( deImage *image ){
	if( image == pImage ){
		return;
	}
	
	pImage = image;
	
	if( pPeerGraphic ){
		pPeerGraphic->ImageChanged();
	}
}

void deCaptureCanvas::SetCapture( bool capture ){
	if( capture == pCapture ){
		return;
	}
	
	pCapture = capture;
	
	if( pPeerGraphic ){
		pPeerGraphic->CaptureChanged();
	}
}



// System Peers
/////////////////

void deCaptureCanvas::SetPeerGraphic( deBaseGraphicCaptureCanvas *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
