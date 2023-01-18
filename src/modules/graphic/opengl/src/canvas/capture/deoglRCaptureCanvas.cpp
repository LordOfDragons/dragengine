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

#include "deoglRCaptureCanvas.h"
#include "../render/deoglRCanvasView.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../target/deoglRenderTarget.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCaptureCanvas
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCaptureCanvas::deoglRCaptureCanvas( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pCanvasView( NULL ),
pCapturePending( false ){
	LEAK_CHECK_CREATE( renderThread, CaptureCanvas );
}

deoglRCaptureCanvas::~deoglRCaptureCanvas(){
	LEAK_CHECK_FREE( pRenderThread, CaptureCanvas );
	SetCanvasView( NULL );
}



// Management
///////////////

void deoglRCaptureCanvas::SetCanvasView( deoglRCanvasView *canvasView ){
	if( canvasView == pCanvasView ){
		return;
	}
	
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
	
	pCanvasView = canvasView;
	
	if( canvasView ){
		canvasView->AddReference();
	}
}

void deoglRCaptureCanvas::SetCapturePending( bool capturePending ){
	pCapturePending = capturePending;
}

void deoglRCaptureCanvas::StartCapture( int width, int height, int componentCount, int bitCount ){
	DropPixelBuffer();
	
	deoglPixelBuffer::ePixelFormats pbformat;
	
	if( bitCount == 8 ){
		if( componentCount == 1 ){
			pbformat = deoglPixelBuffer::epfByte1;
			
		}else if( componentCount == 2 ){
			pbformat = deoglPixelBuffer::epfByte2;
			
		}else if( componentCount == 3 ){
			pbformat = deoglPixelBuffer::epfByte3;
			
		}else{
			pbformat = deoglPixelBuffer::epfByte4;
		}
		
	}else{
		if( componentCount == 1 ){
			pbformat = deoglPixelBuffer::epfFloat1;
			
		}else if( componentCount == 2 ){
			pbformat = deoglPixelBuffer::epfFloat2;
			
		}else if( componentCount == 3 ){
			pbformat = deoglPixelBuffer::epfFloat3;
			
		}else{
			pbformat = deoglPixelBuffer::epfFloat4;
		}
	}
	
	pPixelBuffer.TakeOver( new deoglPixelBuffer( pbformat, width, height, 1 ) );
	
	pCapturePending = true;
	pComponentCount = componentCount;
	pBitCount = bitCount;
}

void deoglRCaptureCanvas::DropPixelBuffer(){
	pPixelBuffer = nullptr;
	pCapturePending = false;
}



void deoglRCaptureCanvas::CaptureRenderWindow( deoglRRenderWindow &renderWindow ){
	if( ! pCapturePending ){
		return;
	}
	
	deoglRCanvasView * const rcanvasView = renderWindow.GetRCanvasView();
	if( rcanvasView != pCanvasView ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( GetRenderThread(), "CaptureCanvas.CaptureRenderWindow" );
	const int width = pPixelBuffer->GetWidth();
	const int height = pPixelBuffer->GetHeight();
	
	if( width == renderWindow.GetWidth() && height == renderWindow.GetHeight() ){
		// extract image into memory
		const deoglPixelBuffer::Ref tempData( deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer( pPixelBuffer->GetFormat(), width, height, 1 ) ) );
		
		OGL_CHECK( pRenderThread, glReadPixels( 0, 0, width, height, pPixelBuffer->GetGLPixelFormat(),
			pPixelBuffer->GetGLPixelType(), tempData->GetPointer() ) );
		
		// copy data over in in the correct order (hence flipped upside down)
		const char * const ptrTempData = ( char* )tempData->GetPointer();
		char * const ptrPixBuf = ( char* )pPixelBuffer->GetPointer();
		const int stride = pPixelBuffer->GetLineStride();
		int y;
		
		for( y=0; y<height; y++ ){
			memcpy( ptrPixBuf + stride * y, ptrTempData + stride * ( height - 1 - y ), stride );
		}
		
	}else{
		// not matching. set black for the time being. requires fbo blitting with stretching to get working
		CaptureBlack();
	}
	
	pCapturePending = false;
}

void deoglRCaptureCanvas::CapturePending(){
	if( ! pCapturePending ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( GetRenderThread(), "CaptureCanvas.CapturePending" );
	deoglTexture *texture = NULL;
	
	if( pCanvasView ){
		pCanvasView->PrepareRenderTarget( nullptr, pComponentCount, pBitCount );
		pCanvasView->RenderRenderTarget( nullptr );
			// TODO capturing can be a problem. if a render world view is contained and it is
			//      not rendered yet it will be rendered. doing so using NULL as mask causes
			//      a full blown rendering as if the camera is used in a render window.
			//      this can cause problems. the main problem here is GI. we need a way to
			//      make the GI use the closest nearby camera plan GI state as render only
			//      GI state preventing any updating.
		
		deoglRenderTarget * const renderTarget = pCanvasView->GetRenderTarget();
		if( renderTarget ){
			texture = renderTarget->GetTexture();
		}
	}
	
	if( texture ){
		if( pPixelBuffer->GetWidth() == texture->GetWidth()
		&& pPixelBuffer->GetHeight() == texture->GetHeight() ){
			texture->GetPixels( *pPixelBuffer );
			
		}else{
			// size not matching. set black for the time being. requires fbo
			// blitting with stretching to get working
			CaptureBlack();
		}
		
	}else{
		// no texture present. use black
		CaptureBlack();
	}
	
	pCapturePending = false;
}

void deoglRCaptureCanvas::CaptureBlack(){
	switch( pPixelBuffer->GetFormat() ){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:
	case deoglPixelBuffer::epfByte4:
		pPixelBuffer->SetToIntColor( 0, 0, 0, 255 );
		break;
		
	default:
		pPixelBuffer->SetToFloatColor( 0.0f, 0.0f, 0.0f, 1.0f );
	}
}
