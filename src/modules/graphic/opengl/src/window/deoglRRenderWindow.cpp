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
#include <stdlib.h>
#include <string.h>

#include "deoglRRenderWindow.h"
#include "../canvas/capture/deoglRCaptureCanvas.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../extensions/deoglExtensions.h"
#include "../rendering/deoglRenderCanvasContext.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTContext.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! OS_MACOS
#include <dragengine/app/deOSUnix.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include "../extensions/deoglXExtResult.h"
#endif

#ifdef OS_ANDROID
#include <dragengine/app/deOSAndroid.h>
#endif

#ifdef OS_BEOS
#include <Window.h>
#include <DirectWindow.h>
#include <Cursor.h>
#include <GLView.h>
#include <dragengine/deEngine.h>
#include <dragengine/app/deOSBeOS.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include "../deGraphicOpenGl.h"
#endif

#ifdef  OS_W32
#include <stdint.h>
#include <dragengine/app/deOSWindows.h>
#include "../extensions/deoglWExtResult.h"
#endif

#ifdef  OS_MACOS
#include <dragengine/app/deOSMacOS.h>
#endif

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

//#define REUSE_WINDOW 1

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
#define WSA_MASK             ( CWColormap | CWEventMask | CWDontPropagate | CWCursor \
                             | CWOverrideRedirect | CWSaveUnder | CWWinGravity \
                             | CWBitGravity | CWBorderPixel | CWBackingStore )
#define BASIC_EVENT_MASK     ( StructureNotifyMask | ExposureMask | PropertyChangeMask \
                             | EnterWindowMask | LeaveWindowMask | KeyPressMask \
                             | KeyReleaseMask | KeymapStateMask | FocusChangeMask )
#define ENABLED_EVENT_MASK   ( ButtonPressMask | ButtonReleaseMask | PointerMotionMask )
#define NOT_PROPAGATE_MASK   ( KeyPressMask | KeyReleaseMask | ButtonPressMask \
                             | ButtonReleaseMask | PointerMotionMask | ButtonMotionMask )
#endif



#ifdef OS_BEOS
deoglRRenderWindow::cGLView::cGLView( deoglRRenderWindow::cGLWindow &window, const BRect &frame ) :
BGLView( frame, "glview", B_FOLLOW_ALL, B_WILL_DRAW, BGL_RGB | BGL_DOUBLE | BGL_DEPTH | BGL_STENCIL ),
pWindow( window )
{
	//SetViewColor( 0, 0, 0 );
	
	// prevent app server from drawing to this window locking things up
	SetViewColor( B_TRANSPARENT_32_BIT );
	
	// get events from all places even outside window
	//SetEventMask( B_POINTER_EVENTS | B_KEYBOARD_EVENTS, B_NO_POINTER_HISTORY );
}

void deoglRRenderWindow::cGLView::KeyDown( const char *bytes, int32 numBytes ){
	pWindow.SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLView::KeyUp( const char *bytes, int32 numBytes ){
	pWindow.SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLView::MouseDown( BPoint point ){
	pWindow.SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLView::MouseMoved( BPoint point, uint32 transit, const BMessage *message ){
	pWindow.SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLView::MouseUp( BPoint point ){
	pWindow.SendCurMessageToEngine();
}

deoglRRenderWindow::cGLWindow::cGLWindow( deoglRRenderWindow &window ) :
BDirectWindow(
	BRect( 0.0f, 0.0f, ( float )window.GetWidth(), ( float )window.GetHeight() ),
	"Drag[en]gine", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0, B_CURRENT_WORKSPACE ),
pWindow( window ),
pGLView( NULL ),
pCursor( NULL ),
pBlockQuitRequested( true )
{
	pGLView = new cGLView( *this, BRect( 0.0f, 0.0f, ( float )window.GetWidth(), ( float )window.GetHeight() ) );
	AddChild( pGLView );
	pGLView->MakeFocus();
	
	// create empty cursor and assign it
	pCursor = new BCursor( B_CURSOR_ID_NO_CURSOR );
	pGLView->SetViewCursor( pCursor );
}

deoglRRenderWindow::cGLWindow::~cGLWindow(){
	if( pCursor ){
		pGLView->SetViewCursor( B_CURSOR_SYSTEM_DEFAULT );
		delete pCursor;
	}
}

void deoglRRenderWindow::cGLWindow::SendCurMessageToEngine(){
	pWindow.GetRenderThread().GetContext().GetOSBeOS()->MessageReceived( CurrentMessage() );
}

void deoglRRenderWindow::cGLWindow::DirectConnected( direct_buffer_info *info ){
	BDirectWindow::DirectConnected( info );
	pGLView->DirectConnected( info );
}

void deoglRRenderWindow::cGLWindow::WindowActivated( bool active ){
	SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLWindow::MessageReceived( BMessage *message ){
	BDirectWindow::MessageReceived( message );
	SendCurMessageToEngine();
}

void deoglRRenderWindow::cGLWindow::FrameResized( float newWidth, float newHeight ){
	BDirectWindow::FrameResized( newWidth, newHeight );
	pWindow.OnResize( ( int )( newWidth + 0.5f ), ( int )( newHeight + 0.5f ) );
}

bool deoglRRenderWindow::cGLWindow::QuitRequested(){
	if( pBlockQuitRequested ){
		// do not close the window but post a quit request
		be_app->PostMessage( B_QUIT_REQUESTED );
		return false;
	}
	
	return BDirectWindow::QuitRequested();
}

void deoglRRenderWindow::cGLWindow::SetBlockQuitRequested( bool blockQuitRequested ){
	pBlockQuitRequested = blockQuitRequested;
}
#endif



// Class deoglRRenderWindow
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRRenderWindow::deoglRRenderWindow( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
pHostWindow( 0 ),
pWindow( 0 ),
pNullCursor( 0 ),
#endif

#ifdef OS_BEOS
pHostWindow( NULL ),
pWindow( NULL ),
#endif

#ifdef OS_W32
pHostWindow( NULL ),
pWindow( NULL ),
pWindowDC( NULL ),
pWindowIcon( NULL ),
#endif

#ifdef OS_MACOS
pHostWindow( NULL ),
pWindow( NULL ),
#endif

pWidth( 100 ),
pHeight( 100 ),
pFullScreen( false ),
pPaint( true ),

pRCanvasView( NULL ),

pSwapBuffers( false ),
pNotifyPositionChanged(false),
pNotifySizeChanged( false ),

pVSyncMode( deoglConfiguration::evsmAdaptive ),
pInitSwapInterval( true ),

pAfterCreateScaleFactor(100)
{
	LEAK_CHECK_CREATE( renderThread, RenderWindow );
}

deoglRRenderWindow::~deoglRRenderWindow(){
	LEAK_CHECK_FREE( pRenderThread, RenderWindow );
	
	DropRCanvasView();
	pDestroyWindow();
}



// Management
///////////////

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
void deoglRRenderWindow::SetHostWindow( Window window ){
	pHostWindow = window;
};
#endif

#ifdef OS_ANDROID
void deoglRRenderWindow::SetHostWindow( void *window ){
};
#endif

#ifdef OS_BEOS
void deoglRRenderWindow::SetHostWindow( BWindow *window ){
	pHostWindow = window;
};

BGLView *deoglRRenderWindow::GetGLView() const{
	return pWindow->GetGLView();
}
#endif

#ifdef OS_MACOS
void deoglRRenderWindow::SetHostWindow( NSWindow *window ){
	pHostWindow = window;
};
#endif

#ifdef OS_W32
void deoglRRenderWindow::SetHostWindow( HWND window ){
	pHostWindow = window;
};

decPoint deoglRRenderWindow::GetInnerSize() const{
	DEASSERT_NOTNULL( pWindow )
	
	RECT rect;
	DEASSERT_TRUE( GetClientRect( pWindow, &rect ) )
	return decPoint( ( int )( rect.right - rect.left ), ( int )( rect.bottom - rect.top ) );
}
#endif

void deoglRRenderWindow::SetPosition(int x, int y){
	if(x < 0 || y < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(x == pX && y == pY){
		return;
	}
	
	if(pNotifyPositionChanged){
		// user is most probably still resizing the window. do not change the size and
		// wait for the next sync call to avoid problems
		return;
	}
	
	pX = x;
	pY = y;
	pNotifyPositionChanged = false;
	
	pResizeWindow();
}

void deoglRRenderWindow::SetSize( int width, int height ){
	if( width < 0 || height < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( width == pWidth && height == pHeight ){
		return;
	}
	
	if( pNotifySizeChanged ){
		// user is most probably still resizing the window. do not change the size and
		// wait for the next sync call to avoid problems
		return;
	}
	
	pWidth = width;
	pHeight = height;
	pNotifySizeChanged = false;
	
	pResizeWindow();
}

void deoglRRenderWindow::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	pSetWindowTitle();
}

void deoglRRenderWindow::SetFullScreen( bool fullScreen ){
	if( fullScreen == pFullScreen ){
		return;
	}
	
	pFullScreen = fullScreen;
	pUpdateFullScreen();
}

void deoglRRenderWindow::SetPaint( bool paint ){
	pPaint = paint;
}

void deoglRRenderWindow::SetIcon( deoglPixelBuffer *icon ){
	if( icon == pIcon ){
		return;
	}
	
	pIcon = icon;
	pSetIcon();
}



void deoglRRenderWindow::SetRCanvasView( deoglRCanvasView *rcanvasView ){
	if( rcanvasView == pRCanvasView ){
		return;
	}
	
	if( pRCanvasView ){
		pRCanvasView->FreeReference();
	}
	
	pRCanvasView = rcanvasView;
	
	if( rcanvasView ){
		rcanvasView->AddReference();
	}
}

void deoglRRenderWindow::DropRCanvasView(){
	if( pRCanvasView ){
		pRCanvasView->FreeReference();
		pRCanvasView = NULL;
	}
}



void deoglRRenderWindow::CreateWindow(){
	// WARNING! CreateWindows is always called from the main thread. this is required since all
	//          UI related operations (especially window event handling) is done in the main
	//          thread. No OpenGL related objects are allowed to be touched in here.
	// 
	// NOTE It might be possible to hold an own display connection in the main thread. It should
	//      not be required since CreateWindow runs only while the render thread is waiting
	
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	// if the window exists we do nothing
	if( pWindow > 255 ){
		return;
	}
	
	Display * const display = pRenderThread.GetContext().GetDisplay();
	XVisualInfo * const visInfo = pRenderThread.GetContext().GetVisualInfo();
	XSetWindowAttributes swa;
	Colormap colMap = 0;
	//Cursor cursor = 0;
	Window rootWindow;
	//XEvent event;
	
	pWindow = 0; //pHostWindow;
	#ifdef REUSE_WINDOW
	//if( pHostWindow <= 255 ){
	#endif
		try{
			// determine the root window
			#ifdef REUSE_WINDOW
			rootWindow = XRootWindow( display, visInfo->screen );
			#else
			if( pHostWindow > 255 ){
				rootWindow = pHostWindow;
				
			}else{
				rootWindow = XRootWindow( display, visInfo->screen );
			}
			#endif
			
			// create color map
			colMap = XCreateColormap( display, rootWindow, visInfo->visual, AllocNone );
			
			// create window
			swa.colormap = colMap;
			swa.backing_store = NotUseful;
			swa.border_pixel = 0;
			swa.event_mask = BASIC_EVENT_MASK;
			swa.do_not_propagate_mask = NOT_PROPAGATE_MASK;
			swa.override_redirect = False;
			swa.save_under = False;
			swa.cursor = 0;
			swa.bit_gravity = ForgetGravity;
			swa.win_gravity = NorthWestGravity;
			
			pWindow = XCreateWindow( display, rootWindow, 0, 0,
				decMath::max( pWidth, 1 ), decMath::max( pHeight, 1 ), 0,
				visInfo->depth, InputOutput, visInfo->visual, WSA_MASK, &swa );
			
			if( pWindow < 256 ){
				DETHROW( deeInvalidAction );
			}
			XSync( display, False ); // required or strange problems can happen
			
			pCreateNullCursor();
			#ifndef REUSE_WINDOW
			//if( pHostWindow <= 255 )
			#endif
			XDefineCursor( display, pWindow, pNullCursor );
			
			// prevent user from killing us with the close button
			Atom atom = pRenderThread.GetContext().GetAtomProtocols();
			XSetWMProtocols( display, pWindow, &atom, 1 );
			
			atom = pRenderThread.GetContext().GetAtomDeleteWindow();
			XSetWMProtocols( display, pWindow, &atom, 1 );
			
			// show window
			if( pWidth > 0 && pHeight > 0 ){
				XMapWindow( display, pWindow );
			}
			XSync( display, False ); // required or strange problems can happen
			
			pUpdateFullScreen();
			
		}catch( const deException &e ){
			if( pWindow > 255 ){
				XUndefineCursor( display, pWindow );
				XDestroyWindow( display, pWindow );
				XFlush( display );
				pWindow = 0;
			}
			throw;
		}
	#ifdef REUSE_WINDOW
	//}
	#endif
	
	// set window title
	pSetWindowTitle();
#endif

#ifdef OS_BEOS
	// if the window exists we do nothing
	if( pWindow ){
		return;
	}
	
	pWindow = new cGLWindow( *this );
	
	// show window
	pWindow->Show();
#endif

#ifdef OS_MACOS
	pMacOSCreateWindow();
#endif

#ifdef OS_W32
	// if the window exists we do nothing
	if( pWindow ){
		return;
	}
	
	HWND parentWindow = HWND_DESKTOP;
	if( pHostWindow ){
		parentWindow = pHostWindow;
	}
	
	// create window
	DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
	DWORD exStyle = 0;
	
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = pWidth;
	windowRect.bottom = pHeight;
	
	if( pHostWindow ){
		style = WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		exStyle = WS_EX_NOPARENTNOTIFY;
		
	}else if( pFullScreen ){
		style = WS_VISIBLE | WS_POPUP | WS_MAXIMIZE;
		
	}else{
		AdjustWindowRectEx( &windowRect, style, FALSE, 0 );
	}
	
	wchar_t wideName[ 200 ];
	deOSWindows::Utf8ToWide( pRenderThread.GetContext().GetWindowClassname(), wideName, 200 );
	pWindow = CreateWindowEx( exStyle, wideName, L"Drag[en]gine OpenGL", style, 0, 0,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, parentWindow,
		NULL, pRenderThread.GetContext().GetOSWindow()->GetInstApp(), NULL );
	if( ! pWindow ){
		pRenderThread.GetLogger().LogErrorFormat( "CreateWindowEx failed with error %lu", GetLastError() );
		DETHROW( deeOutOfMemory );
	}
	
	// set window title
	pSetWindowTitle();
	
	// create the context
	pWindowDC = GetDC( pWindow );
	if( ! pWindowDC ){
		pRenderThread.GetLogger().LogErrorFormat( "GetDC failed with error %lu", GetLastError() );
		DETHROW( deeOutOfMemory );
	}
	
	// set pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset( &pfd, 0, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	
	SetPixelFormat( pWindowDC, ChoosePixelFormat( pWindowDC, &pfd ), &pfd );
	
	// activate window just in case windows messes up again
	if( ! pHostWindow ){
		SetActiveWindow( pWindow );
	}
#endif

	pAfterCreateScaleFactor = pGetDisplayScaleFactor();
}

void deoglRRenderWindow::SwapBuffers(){
	if( ! pSwapBuffers ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( pRenderThread, "Window.SwapBuffers" );
	
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	pUpdateVSync();
	
	// [XERR] BadMatch (invalid parameter attributes): request_code(155) minor_code(11)
	// 155=GLX, 11=glXSwapBuffers
	// 
	// but why BadMatch? from the doc:
	// 
	// BadMatch wird generiert wenn drawable nicht mit dem selben X-Screen und Visual erstellt
	//    wurde wie ctx. Dieser Fehler wird auch generiert wenn drawable NIL und ctx nicht NIL war.
	// second situation can not happen. pWindow never is NULL
	// first should also not happen
	// 
	// happens only with IGDE hence it has to do something with changing windows or with
	// hosted windows in general
		//XSync( pRenderThread.GetContext().GetDisplay(), False );
	
	if( pRenderThread.GetConfiguration().GetRenderDocMode() ){
		// NOTE if RenderDoc is running glXSwapBuffers can throw invalid enum error while capturing
		try{
			OGL_CHECK( pRenderThread, glXSwapBuffers( pRenderThread.GetContext().GetDisplay(), pWindow ) );
		}catch(...){
			return;
		}
		
	}else{
		OGL_CHECK( pRenderThread, glXSwapBuffers( pRenderThread.GetContext().GetDisplay(), pWindow ) );
	}
		//XSync( pRenderThread.GetContext().GetDisplay(), False );
#endif
	
#ifdef OS_ANDROID
	if( eglSwapBuffers( pRenderThread.GetContext().GetDisplay(), pRenderThread.GetContext().GetSurface() ) == EGL_FALSE ){
		// log failure but do not thow exception
	}
#endif
	
#ifdef OS_BEOS
	pWindow->GetGLView()->SwapBuffers( false ); // true = sync
	
	// ensure window resizing is properly applied to the opengl context
	pWindow->GetGLView()->UnlockGL();
	pWindow->GetGLView()->LockGL();
#endif
	
#ifdef OS_MACOS
	pMacOSSwapBuffers();
#endif
	
#ifdef OS_W32
	pUpdateVSync();
	
	if( ! ::SwapBuffers( pWindowDC ) ){
		pRenderThread.GetLogger().LogErrorFormat( "SwapBuffers failed (%s:%i): error=0x%lx\n",
			__FILE__, __LINE__, GetLastError() );
	}
#endif
	
	pSwapBuffers = false;
}

void deoglRRenderWindow::Render(){
	if( ! pPaint || ! pRCanvasView || pWidth < 1 || pHeight < 1 ){
		return;
	}
	#ifdef OS_MACOS
	if( ! pWindow ){
		return; // window got closed under our nose
	}
	#endif
	
	deoglDebugTraceGroup debugTrace( GetRenderThread(), "Window.Render" );
	
	// make window current in the render context
	pRenderThread.GetContext().ActivateRRenderWindow( this );
	
	// silence driver warnings for deprecated stuff
	glDisable( GL_DITHER );
	
	// prepare canvas
	deoglRCanvas * const debugOverlayCanvas = ! pRenderThread.GetVRCamera()
		? pRenderThread.GetCanvasDebugOverlay() : nullptr;
	
	deoglRCanvas * const inputOverlayCanvas = pRenderThread.GetCanvasInputOverlay();
	deoglRCanvas * const overlayCanvas = pRenderThread.GetCanvasOverlay();
	bool isMainWindow = true; // a problem only if more than one render window exists
	
	pRCanvasView->PrepareForRender( nullptr );
	pRCanvasView->PrepareForRenderRender( nullptr );
	if( isMainWindow ){
		if( inputOverlayCanvas ){
			inputOverlayCanvas->PrepareForRender( nullptr );
			inputOverlayCanvas->PrepareForRenderRender( nullptr );
		}
		if( debugOverlayCanvas ){
			debugOverlayCanvas->PrepareForRender( nullptr );
			debugOverlayCanvas->PrepareForRenderRender( nullptr );
		}
		if( overlayCanvas ){
			overlayCanvas->PrepareForRender( nullptr );
			overlayCanvas->PrepareForRenderRender( nullptr );
		}
	}
	
	pRenderThread.SampleDebugTimerRenderThreadRenderWindowsPrepare();
	
	// create render taget if required
	const decPoint size( pWidth, pHeight );
	
	// render canvas
	pRenderThread.GetFramebuffer().Activate( nullptr /*pRenderTarget->GetFBO()*/ );
	
	deoglRenderCanvasContext context( *pRCanvasView,
		nullptr /*pRenderTarget->GetFBO()*/, decPoint(), size, true, NULL );
	pRenderThread.GetRenderers().GetCanvas().Prepare( context );

	const decPoint canvasSize(pRCanvasView->GetSize().Round());
	if(canvasSize == size){
		pRCanvasView->Render(context);

	}else{
		// this happens if global display scaling is not 100%. in this case the
		// canvas view is smaller than the window and has to be upscaled
		const decTexMatrix2 transform(context.GetTransform());
		context.SetTransformScaled(canvasSize, true);
		pRCanvasView->Render(context);
		context.SetTransform(transform);
	}
	
	if( isMainWindow ){
		if( inputOverlayCanvas ){
			inputOverlayCanvas->Render( context );
		}
		if( debugOverlayCanvas ){
			debugOverlayCanvas->Render( context );
		}
		if( overlayCanvas ){
			overlayCanvas->Render( context );
		}
	}
	
	pRenderThread.SampleDebugTimerRenderThreadRenderWindowsRender();
	
	debugTrace.Close();
	
	// capture if any capture canvas are pending
	Capture();
	
	// request swap buffers
	pSwapBuffers = true;
	
#ifdef OS_W32
	// make not current anymore. required for windows using multiple windows
// 	pRenderThread.GetContext().ActivateRRenderWindow( NULL );
#endif
}

void deoglRRenderWindow::Capture(){
	decObjectOrderedSet &list = pRenderThread.GetRCaptureCanvasList();
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRCaptureCanvas* )list.GetAt( i ) )->CaptureRenderWindow( *this );
	}
}

void deoglRRenderWindow::CenterOnScreen(){
	#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow < 255 ){
		return;
	}
	const decPoint screenSize( pRenderThread.GetContext().GetOSUnix()->GetDisplayCurrentResolution( 0 ) );
	XMoveWindow( pRenderThread.GetContext().GetDisplay(), pWindow,
		( screenSize.x - pWidth ) / 2, ( screenSize.y - pHeight ) / 2 );
	#endif
	
	#ifdef OS_BEOS
	if( ! pWindow ){
		return;
	}
	const decPoint screenSize( pRenderThread.GetContext().GetOSBeOS()->GetDisplayCurrentResolution( 0 ) );
	pWindow->MoveTo( ( float )( screenSize.x - pWidth ) * 0.5f, ( float )( screenSize.y - pHeight ) * 0.5f );
	#endif
	
	#ifdef OS_MACOS
	pMacOSCenterOnScreen();
	#endif
	
	#ifdef OS_W32
	if( ! pWindow ){
		return;
	}
	const decPoint screenSize( pRenderThread.GetContext().GetOSWindow()->GetDisplayCurrentResolution( 0 ) );
	SetWindowPos( pWindow, NULL, ( screenSize.x - pWidth ) / 2, ( screenSize.y - pHeight ) / 2,
		0, 0, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOREDRAW
		| SWP_NOZORDER | SWP_SHOWWINDOW );
	#endif
}

void deoglRRenderWindow::OnReposition(int x, int y){
	if(x == pX && y == pY){
		return;
	}
	
	pX = x;
	pY = y;
	
	#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	#endif
	
	#ifdef OS_BEOS
	#endif
	
	#ifdef OS_MACOS
	#endif
	
	#ifdef OS_W32
	#endif
	
	pNotifyPositionChanged = true;
}

void deoglRRenderWindow::OnResize( int width, int height ){
	if( pWidth == 0 || pHeight == 0 ){
		// due to X restrictions zero sized windows can not be defined so avoid
		// triggering wrong resize events in this case
		return;
	}
	
	if( width == pWidth && height == pHeight ){
		return;
	}
	
	pWidth = width;
	pHeight = height;
	
	#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	#endif
	
	#ifdef OS_BEOS
	#endif
	
	#ifdef OS_MACOS
	#endif
	
	#ifdef OS_W32
	#endif
	
	pNotifySizeChanged = true;
}



bool deoglRRenderWindow::GetNotifySizeChanged(){
	const bool notify = pNotifySizeChanged || pNotifyPositionChanged;
	pNotifyPositionChanged = false;
	pNotifySizeChanged = false;
	return notify;
}



// Private Functions
//////////////////////

void deoglRRenderWindow::pDestroyWindow(){
	// if context is missing this can be due to early start-up failure where setting up the
	// proper context had not been yet possible
	
	// the context is not allowed to keep a reference to the active render window. this would run
	// the risk of two threads manipulating the reference count at the same time. only the main
	// thread is allowed to manipulate references. at this point in time the render thread is
	// waiting so only the main thread is running. if we are the active window we have to first
	// clear the active window from the render context otherwise freed memory is accessed.
	if( pRenderThread.HasContext() ){
		if( pRenderThread.GetContext().GetActiveRRenderWindow() == this ){
			// the context takes care of setting a new active render window if possible and required
			pRenderThread.GetContext().ActivateRRenderWindow( NULL );
		}
	}
	
	// Linux
	#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow > 255 ){
		if( pRenderThread.HasContext() ){
			Display * const display = pRenderThread.GetContext().GetDisplay();
			XSync( display, False ); // required or strange problems can happen
			
			XUndefineCursor( display, pWindow );
			XDestroyWindow( display, pWindow );
			XSync( display, False ); // required or strange problems can happen
		}
	}
	pWindow = 0;
	
	if( pNullCursor > 255 ){
		if( pRenderThread.HasContext() ){
			Display * const display = pRenderThread.GetContext().GetDisplay();
			XFreeCursor( display, pNullCursor );
		}
	}
	pNullCursor = 0;
	#endif
	
	// BeOS
	#ifdef OS_BEOS
	if( pWindow ){
		pWindow->SetBlockQuitRequested( false );
		if( ! pWindow->IsLocked() ){
			pWindow->Lock();
		}
		pWindow->Quit(); // does unlock itself
		pWindow = NULL;
	}
	#endif
	
	// MacOS
	#ifdef OS_MAC_OS
	pMacOSDestroyWindow();
	#endif
	
	// Windows
	#ifdef OS_W32
	if( pWindow ){
		if( pWindowDC ){
			// CS_OWNDC are not required to be released according to MSDN
			//ReleaseDC( pWindow, pWindowDC );
			pWindowDC = NULL;
		}
		if( pWindowIcon ){
			DestroyIcon( pWindowIcon );
			pWindowIcon = NULL;
		}
		::DestroyWindow( pWindow );
	}
	pWindow = NULL;
	#endif
}

void deoglRRenderWindow::pRepositionWindow(){
	// TODO
}

void deoglRRenderWindow::pResizeWindow(){
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow > 255 ){
		Display * const display = pRenderThread.GetContext().GetDisplay();
		
		if( pWidth > 0 && pHeight > 0 ){
			XResizeWindow( display, pWindow, pWidth, pHeight );
			XMapWindow( display, pWindow );
			
		}else{
			XResizeWindow( display, pWindow, 1, 1 );
			XUnmapWindow( display, pWindow );
		}
		XSync( display, False ); // required or strange problems can happen
	}
#endif
	
#ifdef OS_BEOS
	if( pWindow ){
		pWindow->ResizeTo( ( float )pWidth, ( float )pHeight );
	}
#endif
	
#ifdef OS_MACOS
	pMacOSResizeWindow();
#endif

#ifdef OS_W32
	if( pWindow ){
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = pWidth;
		rect.bottom = pHeight;
		
		AdjustWindowRectEx( &rect, GetWindowLong( pWindow, GWL_STYLE ),
			FALSE, GetWindowLong( pWindow, GWL_EXSTYLE ) );
		
		SetWindowPos( pWindow, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER
			| SWP_NOREDRAW | SWP_NOZORDER | SWP_SHOWWINDOW );
	}
#endif
}

void deoglRRenderWindow::pSetWindowTitle(){
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow > 255 ){
		Display * const display = pRenderThread.GetContext().GetDisplay();
		XTextProperty textProp;
		const char * const title = pTitle.GetString();
		
		if( ! XStringListToTextProperty( ( char** )&title, 1, &textProp ) ){
			DETHROW( deeOutOfMemory );
		}
		XSetWMName( display, pWindow, &textProp );
		XFree( textProp.value );
	}
#endif
	
#ifdef OS_BEOS
	if( pWindow ){
		pWindow->SetTitle( pTitle );
	}
#endif
	
#ifdef OS_MACOS
	pMacOSSetWindowTitle();
#endif

#ifdef OS_W32
	if( pWindow ){
		wchar_t wideName[ 200 ];
		deOSWindows::Utf8ToWide( pTitle, wideName, 200 );
		SetWindowText( pWindow, wideName );
	}
#endif
}

void deoglRRenderWindow::pUpdateFullScreen(){
	if( pFullScreen ){
		// TODO find the closest display resolution from deOS and change window and screen
		//      resolution to match. window managers do not change the screen which can
		//      lead to artifacts around the window
		
	}else{
	}
	
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow > 255 ){
		Display * const display = pRenderThread.GetContext().GetDisplay();
		const XVisualInfo &visInfo = *pRenderThread.GetContext().GetVisualInfo();
		Window rootWindow = XRootWindow( display, visInfo.screen );
		
		// set window manager hints. window managers should honor those flags when a window is mapped.
		// atoms present in the list are enabled while atoms absent from the list are disabled.
		const Atom atomWMState = XInternAtom( display, "_NET_WM_STATE", False );
		const Atom atomFullScreen = XInternAtom( display, "_NET_WM_STATE_FULLSCREEN", False );
		
		XChangeProperty( display, pWindow, atomWMState, XA_ATOM, 32,
			PropModeReplace, ( const unsigned char* )&atomFullScreen, 1 );
		
		XEvent event;
		memset( &event, 0, sizeof( event ) );
		event.xclient.type = ClientMessage;
		event.xclient.send_event = True;
		event.xclient.display = display;
		event.xclient.window = pWindow;
		event.xclient.message_type = atomWMState;
		event.xclient.format = 32;
		event.xclient.data.l[ 0 ] = pFullScreen ? 1 : 0; // 0(unset property), 1(set property), 2(toggle property)
		event.xclient.data.l[ 1 ] = atomFullScreen;
		event.xclient.data.l[ 2 ] = 0; // must be set to zero if only one property is to be changed
		event.xclient.data.l[ 3 ] = 1; // source indicator: 0(legacy application), 1(application), 2(direct user input)
		
		int rc = XSendEvent( display, rootWindow, False,
			SubstructureRedirectMask | SubstructureNotifyMask, &event );
		if( rc != Success ){
			pRenderThread.GetLogger().LogErrorFormat("Switch fullscreen failed: %d", rc);
		}
		XSync( display, False ); // make sure the request is processed before going on
	}
#endif
	
#ifdef OS_BEOS
	/*
	if( pWindow ){
		pWindow->ResizeTo( ( float )pWidth, ( float )pHeight );
	}
	*/
	pWindow->SetFullScreen( pFullScreen );
#endif
	
#ifdef OS_MACOS
	pMacOSUpdateFullscreen();
#endif

#ifdef OS_W32
	if( pFullScreen ){
		/*
		LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		SetWindowLong(hwnd, GWL_STYLE, lStyle);
		LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		*/
		SetWindowLong( pWindow, GWL_STYLE, WS_VISIBLE | WS_POPUP | WS_MAXIMIZE );
		
	}else{
		SetWindowLong( pWindow, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW );
		// we need to adjust the position again... see CreateWindow
	}
	
	SetWindowPos(pWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE
		| SWP_NOZORDER | SWP_NOOWNERZORDER);
	
	if( pFullScreen ){
		// if fullscreen force the window position to fill the entire work area. relying on WS_MAXIMIZE
		// is unfortunately not working reliably so we resize and move the window on our own to be sure
		RECT rect{};
		//DEASSERT_TRUE(SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0))
		rect.right = GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = GetSystemMetrics(SM_CYSCREEN);
		
		pX = rect.left;
		pY = rect.top;
		pWidth = rect.right - rect.left;
		pHeight = rect.bottom - rect.top;
		pNotifyPositionChanged = true;
		pNotifySizeChanged = true;
		pRenderThread.GetLogger().LogInfoFormat("Window.FullScreen: %d %d %d %d",
			rect.left, rect.top, pWidth, pHeight);
		
		SetWindowPos(pWindow, NULL, pX, pY, pWidth, pHeight,
			SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	/*
	if( pWindow ){
		SetWindowPos( pWindow, NULL, 0, 0, pWidth, pHeight,
			SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER
			| SWP_NOREDRAW | SWP_NOZORDER | SWP_SHOWWINDOW );
	}
	*/
#endif
}

void deoglRRenderWindow::pSetIcon(){
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	if( pWindow <= 255 ){
		return;
	}
	
	Display * const display = pRenderThread.GetContext().GetDisplay();
	
	// image data has to be in ARGB format and using long as mentioned here:
	// https://stackoverflow.com/a/15595582
	unsigned long *iconBuffer = NULL;
	int iconBufferLen = 0;
	
	if( pIcon ){
		const int width = pIcon->GetWidth();
		const int height = pIcon->GetHeight();
		const int pixelCount = width * height;
		
		iconBuffer = new unsigned long[ 2 + pixelCount ];
		iconBuffer[ 0 ] = width;
		iconBuffer[ 1 ] = height;
		iconBufferLen = 2 + pixelCount;
		
		unsigned long *dest = iconBuffer + 2;
		int i;
		
		switch( pIcon->GetFormat() ){
		case deoglPixelBuffer::epfByte1:{
			const deoglPixelBuffer::sByte1 *src = pIcon->GetPointerByte1();
			for( i=0; i<pixelCount; i++ ){
				dest[ i ] = 0xff000000 | ( src[ i ].r << 16 ) | ( src[ i ].r << 8 ) | src[ i ].r;
			}
			}break;
			
		case deoglPixelBuffer::epfByte2:{
			const deoglPixelBuffer::sByte2 *src = pIcon->GetPointerByte2();
			for( i=0; i<pixelCount; i++ ){
				dest[ i ] = ( src[ i ].g << 24 ) | ( src[ i ].r << 16 ) | ( src[ i ].r << 8 ) | src[ i ].r;
			}
			}break;
			
		case deoglPixelBuffer::epfByte3:{
			const deoglPixelBuffer::sByte3 *src = pIcon->GetPointerByte3();
			for( i=0; i<pixelCount; i++ ){
				dest[ i ] = 0xff000000 | ( src[ i ].r << 16 ) | ( src[ i ].g << 8 ) | src[ i ].b;
			}
			}break;
			
		case deoglPixelBuffer::epfByte4:{
			const deoglPixelBuffer::sByte4 *src = pIcon->GetPointerByte4();
			for( i=0; i<pixelCount; i++ ){
				dest[ i ] = ( src[ i ].a << 24 ) | ( src[ i ].r << 16 ) | ( src[ i ].g << 8 ) | src[ i ].b;
			}
			}break;
			
		case deoglPixelBuffer::epfFloat1:{
			const deoglPixelBuffer::sFloat1 *src = pIcon->GetPointerFloat1();
			for( i=0; i<pixelCount; i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				dest[ i ] = 0xff000000 | ( r << 16 ) | ( r << 8 ) | r;
			}
			}break;
			
		case deoglPixelBuffer::epfFloat2:{
			const deoglPixelBuffer::sFloat2 *src = pIcon->GetPointerFloat2();
			for( i=0; i<pixelCount; i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				dest[ i ] = ( g << 24 ) | ( r << 16 ) | ( r << 8 ) | r;
			}
			}break;
			
		case deoglPixelBuffer::epfFloat3:{
			const deoglPixelBuffer::sFloat3 *src = pIcon->GetPointerFloat3();
			for( i=0; i<pixelCount; i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				const unsigned char b = ( unsigned char )( decMath::clamp( src[ i ].b * 255.0f, 0.0f, 255.0f ) );
				dest[ i ] = 0xff000000 | ( r << 16 ) | ( g << 8 ) | b;
			}
			}break;
			
		case deoglPixelBuffer::epfFloat4:{
			const deoglPixelBuffer::sFloat4 *src = pIcon->GetPointerFloat4();
			for( i=0; i<pixelCount; i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				const unsigned char b = ( unsigned char )( decMath::clamp( src[ i ].b * 255.0f, 0.0f, 255.0f ) );
				const unsigned char a = ( unsigned char )( decMath::clamp( src[ i ].a * 255.0f, 0.0f, 255.0f ) );
				dest[ i ] = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
			}
			}break;
			
		default:
			memset( iconBuffer, 0, sizeof( unsigned long ) * pixelCount );
			break;
		}
	}
	
	// assign icon
	Atom atomNetWmIcon = XInternAtom( display, "_NET_WM_ICON", False );
	Atom atomCardinal = XInternAtom( display, "CARDINAL", False );
	
	XChangeProperty( display, pWindow, atomNetWmIcon, atomCardinal, 32,
		PropModeReplace, ( const unsigned char* )iconBuffer, iconBufferLen );
	
	if( iconBuffer ){
		delete [] iconBuffer;
	}
#endif
	
#ifdef OS_BEOS
// 	if( pWindow ){
// 		pWindow->SetTitle( pTitle );
// 	}
#endif
	
#ifdef OS_MACOS
// 	pMacOSSetWindowTitle();
#endif

#ifdef OS_W32
	// NOTE under windows alpha=0 means solid and alpha=255 transparent
	if( ! pWindow || ! pIcon ){
		return;
	}
	
	const HICON oldWindowIcon = pWindowIcon;
	pWindowIcon = NULL;
	
	const int width = pIcon->GetWidth();
	const int height = pIcon->GetHeight();
	int x, y, i;
	
	const HDC hdcScreen = GetDC(NULL);
	
	const HDC hdcColor = CreateCompatibleDC( hdcScreen );
	const HDC hdcMask = CreateCompatibleDC( hdcScreen );
	
	const HBITMAP hbmpColor = CreateCompatibleBitmap( hdcScreen, width, height );
	const HBITMAP hbmpMask = CreateCompatibleBitmap( hdcScreen, width, height );
	
	const HBITMAP hbmpOldColor = ( HBITMAP )SelectObject( hdcColor, hbmpColor );
	const HBITMAP hbmpOldMask = ( HBITMAP )SelectObject( hdcMask, hbmpMask );
	
	switch( pIcon->GetFormat() ){
	case deoglPixelBuffer::epfByte1:{
		const deoglPixelBuffer::sByte1 *src = pIcon->GetPointerByte1();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				SetPixel( hdcColor, x, y, RGB( src[ i ].r, src[ i ].r, src[ i ].r ) );
				SetPixel( hdcMask, x, y, RGB( 0, 0, 0 ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfByte2:{
		const deoglPixelBuffer::sByte2 *src = pIcon->GetPointerByte2();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				SetPixel( hdcColor, x, y, RGB( src[ i ].r, src[ i ].r, src[ i ].r ) );
				SetPixel( hdcMask, x, y, RGB( 255 - src[ i ].g, 255 - src[ i ].g, 255 - src[ i ].g ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfByte3:{
		const deoglPixelBuffer::sByte3 *src = pIcon->GetPointerByte3();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				SetPixel( hdcColor, x, y, RGB( src[ i ].r, src[ i ].g, src[ i ].b ) );
				SetPixel( hdcMask, x, y, RGB( 0, 0, 0 ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfByte4:{
		const deoglPixelBuffer::sByte4 *src = pIcon->GetPointerByte4();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				SetPixel( hdcColor, x, y, RGB( src[ i ].r, src[ i ].g, src[ i ].b ) );
				SetPixel( hdcMask, x, y, RGB( 255 - src[ i ].a, 255 - src[ i ].a, 255 - src[ i ].a ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfFloat1:{
		const deoglPixelBuffer::sFloat1 *src = pIcon->GetPointerFloat1();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				SetPixel( hdcColor, x, y, RGB( r, r, r ) );
				SetPixel( hdcMask, x, y, RGB( 0, 0, 0 ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfFloat2:{
		const deoglPixelBuffer::sFloat2 *src = pIcon->GetPointerFloat2();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				SetPixel( hdcColor, x, y, RGB( r, r, r ) );
				SetPixel( hdcMask, x, y, RGB( 255 - g, 255 - g, 255 - g ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfFloat3:{
		const deoglPixelBuffer::sFloat3 *src = pIcon->GetPointerFloat3();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				const unsigned char b = ( unsigned char )( decMath::clamp( src[ i ].b * 255.0f, 0.0f, 255.0f ) );
				SetPixel( hdcColor, x, y, RGB( r, g, b ) );
				SetPixel( hdcMask, x, y, RGB( 0, 0, 0 ) );
			}
		}
		}break;
		
	case deoglPixelBuffer::epfFloat4:{
		const deoglPixelBuffer::sFloat4 *src = pIcon->GetPointerFloat4();
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				const unsigned char r = ( unsigned char )( decMath::clamp( src[ i ].r * 255.0f, 0.0f, 255.0f ) );
				const unsigned char g = ( unsigned char )( decMath::clamp( src[ i ].g * 255.0f, 0.0f, 255.0f ) );
				const unsigned char b = ( unsigned char )( decMath::clamp( src[ i ].b * 255.0f, 0.0f, 255.0f ) );
				const unsigned char a = ( unsigned char )( decMath::clamp( src[ i ].a * 255.0f, 0.0f, 255.0f ) );
				SetPixel( hdcColor, x, y, RGB( r, g, b ) );
				SetPixel( hdcMask, x, y, RGB( 255 - a, 255 - a, 255 - a ) );
			}
		}
		}break;
		
	default:
		for( i=0, y=0; y<height; y++ ){
			for( x=0; x<width; x++, i++ ){
				SetPixel( hdcColor, x, y, RGB( 255, 255, 255 ) );
				SetPixel( hdcMask, x, y, RGB( 0, 0, 0 ) );
			}
		}
		break;
	}
	
	SelectObject( hdcMask, hbmpOldMask );
	DeleteDC( hdcMask );
	
	SelectObject( hdcColor, hbmpOldColor );
	DeleteDC( hdcColor );
	
	ICONINFO ii;
	ii.fIcon = TRUE;
	ii.hbmMask = hbmpMask;
	ii.hbmColor = hbmpColor;
	pWindowIcon = CreateIconIndirect( &ii );
	
	DeleteObject( hbmpMask );
	DeleteObject( hbmpColor );
	
	ReleaseDC( NULL, hdcScreen );
	
	SendMessage( pWindow, WM_SETICON, ICON_BIG, ( LPARAM )pWindowIcon );
	
	if( oldWindowIcon ){
		DestroyIcon( oldWindowIcon );
	}
#endif
}

int deoglRRenderWindow::pGetDisplayScaleFactor(){
	int scale = 100;

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	Display * const display = pRenderThread.GetContext().GetDisplay();
	const char * const resourceString = XResourceManagerString(display);
	if(!resourceString){
		return scale;
	}

	XrmInitialize(); // initialize db before calling Xrm* functions
	const XrmDatabase db = XrmGetStringDatabase(resourceString);
	
	XrmValue value;
	char *type = nullptr;
	if(XrmGetResource(db, "Xft.dpi", "String", &type, &value) != True || !value.addr){
		return scale;
	}
	
	const double scalef = 100.0 * atof(value.addr) / 96.0;
	scale = decMath::max((int)(scalef / 25.0 + 0.5) * 25, 100);
#endif

#ifdef OS_BEOS
#endif

#ifdef OS_MACOS
#endif

#ifdef OS_W32
	scale = 100 * GetDpiForWindow(pWindow) / USER_DEFAULT_SCREEN_DPI;
#endif

    return scale;
}

#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
void deoglRRenderWindow::pCreateNullCursor(){
	if( pNullCursor ){
		return;
	}
	
	Display * const display = pRenderThread.GetContext().GetDisplay();
	Pixmap cursorMask;
	XGCValues gcv;
	GC gc;
	XColor color;
	
	// create null cursor
	cursorMask = XCreatePixmap( display, pWindow, 1, 1, 1 );
	gcv.function = GXclear;
	gc =  XCreateGC( display, cursorMask, GCFunction, &gcv );
	XFillRectangle( display, cursorMask, gc, 0, 0, 1, 1 );
	color.pixel = 0;
	color.red = 0;
	color.flags = 4;
	pNullCursor = XCreatePixmapCursor( display, cursorMask, cursorMask, &color, &color, 0, 0 );
	XFreePixmap( display, cursorMask );
	XFreeGC( display, gc );
}
#endif

void deoglRRenderWindow::pUpdateVSync(){
	// check if VSync has to be enabled or disabled
	const deoglConfiguration::eVSyncMode vsyncMode = pRenderThread.GetConfiguration().GetVSyncMode();
	
	if( vsyncMode != pVSyncMode ){
		pVSyncMode = vsyncMode;
		pInitSwapInterval = true;
	}
	
	// apply changes if required
	if( ! pInitSwapInterval ){
		return;
	}
	
	pInitSwapInterval = false;
	
#if defined OS_UNIX && ! defined OS_ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	const deoglExtensions &ext = pRenderThread.GetExtensions();
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	if( ext.GetHasExtension( deoglExtensions::ext_GLX_EXT_swap_control ) ){
		switch( pVSyncMode ){
		case deoglConfiguration::evsmAdaptive:
			if( ext.GetHasExtension( deoglExtensions::ext_GLX_EXT_swap_control_tear ) ){
				logger.LogInfo( "RenderWindow: Enable Adaptive V-Sync" );
				OGL_CHECK( pRenderThread, pglXSwapInterval( pRenderThread.GetContext().GetDisplay(), pWindow, -1 ) );
				
			}else{
				logger.LogInfo( "RenderWindow: Enable V-Sync" );
				OGL_CHECK( pRenderThread, pglXSwapInterval( pRenderThread.GetContext().GetDisplay(), pWindow, 1 ) );
			}
			break;
			
		case deoglConfiguration::evsmOn:
			logger.LogInfo( "RenderWindow: Enable V-Sync" );
			OGL_CHECK( pRenderThread, pglXSwapInterval( pRenderThread.GetContext().GetDisplay(), pWindow, 1 ) );
			break;
			
		case deoglConfiguration::evsmOff:
			logger.LogInfo( "RenderWindow: Disable VSync" );
			OGL_CHECK( pRenderThread, pglXSwapInterval( pRenderThread.GetContext().GetDisplay(), pWindow, 0 ) );
		}
	}
#endif
	
#ifdef OS_W32
	const deoglExtensions &ext = pRenderThread.GetExtensions();
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	if( ext.GetHasExtension( deoglExtensions::ext_WGL_EXT_swap_control ) ){
		switch( pVSyncMode ){
		case deoglConfiguration::evsmAdaptive:
			if( ext.GetHasExtension( deoglExtensions::ext_WGL_EXT_swap_control_tear ) ){
				logger.LogInfo( "RenderWindow: Enable Adaptive V-Sync" );
				DEASSERT_TRUE( pwglSwapInterval( -1 ) )
				
			}else{
				logger.LogInfo( "RenderWindow: Enable V-Sync" );
				DEASSERT_TRUE( pwglSwapInterval( 1 ) )
			}
			break;
			
		case deoglConfiguration::evsmOn:
			logger.LogInfo( "RenderWindow: Enable V-Sync" );
			DEASSERT_TRUE( pwglSwapInterval( 1 ) )
			break;
			
		case deoglConfiguration::evsmOff:
			logger.LogInfo( "RenderWindow: Disable VSync" );
			DEASSERT_TRUE( pwglSwapInterval( 0 ) )
		}
	}
#endif
}
