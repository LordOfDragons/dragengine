/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include "deoglRenderWindow.h"
#include "deoglRRenderWindow.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasView.h"
#include "../texture/deoglImage.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deScriptingSystem.h>

#ifdef OS_BEOS
#include <DirectWindow.h>
#endif



// Class deoglRenderWindow
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderWindow::deoglRenderWindow( deGraphicOpenGl &ogl, deRenderWindow &renderWindow ) :
pOgl( ogl ),
pRenderWindow( renderWindow ),

pCanvasView( NULL ),
pDirtyParams( false ),
pDirtyPosition(true),
pDirtySize( true ),
pDirtyWindowTitle( true ),
pDirtyFullScreen( true ),
pDirtyIcon( true ),

pRRenderWindow( NULL )
{
	try{
		pCanvasView = ( deoglCanvasView* )renderWindow.GetCanvasView()->GetPeerGraphic();
		
		pRRenderWindow = new deoglRRenderWindow( ogl.GetRenderThread() );
		
		pRRenderWindow->SetHostWindow( renderWindow.GetHostWindow() );
		pRRenderWindow->SetPosition(renderWindow.GetX(), renderWindow.GetY());
		pRRenderWindow->SetSize( renderWindow.GetWidth(), renderWindow.GetHeight() );
		pRRenderWindow->SetTitle( renderWindow.GetTitle() );
		pRRenderWindow->SetFullScreen( renderWindow.GetFullScreen() );
		
		ogl.GetRenderThread().CreateRenderWindow( pRRenderWindow );
		
		renderWindow.SetWindow( pRRenderWindow->GetWindow() );
		renderWindow.SetScaleFactor(pRRenderWindow->GetAfterCreateScaleFactor());
		
		// set application as active. x-system does not tell the new window it obtained
		// the focus so we do this instead. (is this a bug or feature of x-system?)
		pOgl.GetOS()->SetAppActive( true );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	ogl.GetRenderWindowList().Add( this );
}

deoglRenderWindow::~deoglRenderWindow(){
	pOgl.GetRenderWindowList().Remove( this );
	pCleanUp();
}



// Management
///////////////



// Notifications
//////////////////

void deoglRenderWindow::HostWindowChanged(){
	pRRenderWindow->SetHostWindow( pRenderWindow.GetHostWindow() );
}

void deoglRenderWindow::PositionChanged(){
	pDirtyPosition = true;
}

void deoglRenderWindow::SizeChanged(){
	pDirtySize = true;
}

void deoglRenderWindow::TitleChanged(){
	pDirtyWindowTitle = true;
}

void deoglRenderWindow::FullScreenChanged(){
	pDirtyFullScreen = true;
}

void deoglRenderWindow::ScriptNotifyChanged(){
}

void deoglRenderWindow::PaintEnabledChanged(){
	pDirtyParams = true;
}

void deoglRenderWindow::IconChanged(){
	pDirtyIcon = true;
}



void deoglRenderWindow::SyncToRender(){
// 		decTimer timer;
	if( pDirtyFullScreen ){
		pRRenderWindow->SetFullScreen( pRenderWindow.GetFullScreen() );
		pDirtyFullScreen = false;
// 			pOgl.LogInfoFormat( "RWindow.Sync full screen: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyPosition){
		pRRenderWindow->SetPosition(pRenderWindow.GetX(), pRenderWindow.GetY());
		pDirtyPosition = false;
	}
	
	if( pDirtySize ){
		pRRenderWindow->SetSize( pRenderWindow.GetWidth(), pRenderWindow.GetHeight() );
		pDirtySize = false;
// 			pOgl.LogInfoFormat( "RWindow.Sync size: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if( pDirtyWindowTitle ){
		pRRenderWindow->SetTitle( pRenderWindow.GetTitle() );
		pDirtyWindowTitle = false;
// 			pOgl.LogInfoFormat( "RWindow.Sync title: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if( pDirtyIcon ){
		if( pRenderWindow.GetIcon() ){
			deoglImage &image = *( ( deoglImage* )pRenderWindow.GetIcon()->GetPeerGraphic() );
			
			image.CreatePixelBuffer();
			
			try{
				pRRenderWindow->SetIcon( deoglPixelBuffer::Ref::New( new deoglPixelBuffer( image.GetPixelBuffer() ) ) );
				
			}catch( const deException & ){
				image.ReleasePixelBuffer();
				throw;
			}
			image.ReleasePixelBuffer();
			
		}else{
			pRRenderWindow->SetIcon( NULL );
		}
		pDirtyIcon = false;
// 			pOgl.LogInfoFormat( "RWindow.Sync icon: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	// if parameters changed set them to the window. this has to be done always
	if( pDirtyParams ){
		//pRRenderWindow->SetHostWindow( pRenderWindow.GetHostWindow() );
		pRRenderWindow->SetPaint( pRenderWindow.GetPaint() );
		pDirtyParams = false;
// 			pOgl.LogInfoFormat( "RWindow.Sync set paint: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	// sync canvas view only if painting is enabled. saves performance on invisible windows
	if( pRenderWindow.GetPaint() ){
		pCanvasView->SyncToRender();
		pRRenderWindow->SetRCanvasView( pCanvasView->GetRCanvasView() );
// 			pOgl.LogInfoFormat( "RWindow.Sync set rcanvasview: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	// window size changed due to OS events
	if( pRRenderWindow->GetNotifySizeChanged() ){
		pRenderWindow.SetSize( pRRenderWindow->GetWidth(), pRRenderWindow->GetHeight() );
// 			pOgl.LogInfoFormat( "RWindow.Sync notify size changed: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
}



// Private Functions
//////////////////////

void deoglRenderWindow::pCleanUp(){
	if( pRRenderWindow ){
		pRRenderWindow->SetFullScreen( false ); // make sure full screen is disabled if enabled
		pRRenderWindow->FreeReference();
	}
	
	#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	pRenderWindow.SetWindow( 0 );
	#endif
	
	#ifdef OS_ANDROID
	pRenderWindow.SetWindow( NULL );
	#endif
	
	#ifdef OS_BEOS
	pRenderWindow.SetWindow( NULL );
	#endif
	
	#ifdef OS_W32
	pRenderWindow.SetWindow( NULL );
	#endif
	
	#ifdef OS_MACOS
	pRenderWindow.SetWindow( NULL );
	#endif
}
