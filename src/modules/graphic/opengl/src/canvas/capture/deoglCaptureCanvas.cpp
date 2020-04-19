/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglCaptureCanvas.h"
#include "deoglRCaptureCanvas.h"
#include "../render/deoglRCanvasView.h"
#include "../deoglCanvasView.h"
#include "../../deGraphicOpenGl.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglCaptureCanvas
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCaptureCanvas::deoglCaptureCanvas( deGraphicOpenGl &ogl, deCaptureCanvas &captureCanvas ) :
pOgl( ogl ),
pCaptureCanvas( captureCanvas ),

pRCaptureCanvas( NULL ),

pDirtyCanvasView( true ),
pCapturePending( false )
{
	ogl.GetCaptureCanvasList().Add( this );
}

deoglCaptureCanvas::~deoglCaptureCanvas(){
	pOgl.GetCaptureCanvasList().Remove( this );
	if( pRCaptureCanvas ){
		pRCaptureCanvas->FreeReference();
	}
}



// Management
///////////////

void deoglCaptureCanvas::SyncToRender(){
	if( ! pRCaptureCanvas ){
		pRCaptureCanvas = new deoglRCaptureCanvas( GetOgl().GetRenderThread() );
	}
	
	if( pDirtyCanvasView ){
		deCanvasView * const canvasView = pCaptureCanvas.GetCanvasView();
		
		pRCaptureCanvas->DropPixelBuffer();
		
		if( canvasView ){
			deoglCanvasView * const oglCanvasView = ( deoglCanvasView* )canvasView->GetPeerGraphic();
			oglCanvasView->SyncToRender(); // required otherwise it does not work... strange
			pRCaptureCanvas->SetCanvasView( oglCanvasView->GetRCanvasView() );
			
		}else{
			pRCaptureCanvas->SetCanvasView( NULL );
		}
		
		pDirtyCanvasView = false;
	}
	
	// handle capturing if enabled
	if( ! pCaptureCanvas.GetCapture() ){
		return;
	}
	
	deImage * const image = pCaptureCanvas.GetImage();
	if( ! image ){
		return;
	}
	
	if( pCapturePending ){
		if( ! pRCaptureCanvas->GetCapturePending() ){
			if( image->GetData() ){ // in case the user forgot to retain the image data
				const deoglPixelBuffer &pixelBuffer = *pRCaptureCanvas->GetPixelBuffer();
				memcpy( image->GetData(), pixelBuffer.GetPointer(), pixelBuffer.GetImageSize() );
				image->NotifyImageDataChanged();
			}
			
			pRCaptureCanvas->DropPixelBuffer(); // save memory
			
			pCaptureCanvas.SetCapture( false );
			pCapturePending = false;
		}
		
	}else{
		// ensure canvas view is synchronized. required for rendering dynamic canvas view not
		// part of a render window
		deCanvasView * const canvasView = pCaptureCanvas.GetCanvasView();
		if( canvasView ){
			( ( deoglCanvasView* )canvasView->GetPeerGraphic() )->SyncToRender();
		}
		
		// now start capturing
		pRCaptureCanvas->StartCapture( image->GetWidth(), image->GetHeight(),
			image->GetComponentCount(), image->GetBitCount() );
		pCapturePending = true;
	}
}



// Notifications
//////////////////

void deoglCaptureCanvas::CanvasViewChanged(){
	pDirtyCanvasView = true;
}

void deoglCaptureCanvas::ImageChanged(){
}

void deoglCaptureCanvas::CaptureChanged(){
}
