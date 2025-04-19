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

#include "deoglRTContext.h"
#include "deoglRenderThread.h"
#include "deoglLoaderThread.h"
#include "deoglRTLogger.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"
#include "../canvas/deoglCanvasView.h"
#ifdef BACKEND_OPENGL
	#include "../extensions/deoglExtensions.h"
#endif // BACKEND_OPENGL
#include "../window/deoglRenderWindow.h"
#include "../window/deoglRRenderWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#ifdef OS_ANDROID
#include <dragengine/app/deOSAndroid.h>
#include <android/native_window.h>

#elif defined OS_WEBWASM
#include <dragengine/app/deOSWebWasm.h>

#elif defined OS_BEOS
#include <GLView.h>
#include <dragengine/app/deOSBeOS.h>

#elif defined OS_MACOS
#include "../extensions/macosfix.h"
#include <dragengine/app/deOSMacOS.h>

#elif defined OS_W32
#include <dragengine/app/deOSWindows.h>
#include "../extensions/wglext.h"

#elif defined OS_UNIX_X11
#include <dragengine/app/deOSUnix.h>
#ifdef BACKEND_OPENGL
	#include "../extensions/deoglXExtResult.h"
#endif // BACKEND_OPENGL
#endif // OS_*



// event function (local)
#ifdef OS_UNIX_X11
/*
static Bool lfWaitForNotify( Display *d, XEvent *e, char *arg ){
	return ( e->type == MapNotify ) && ( e->xmap.window == (Window)arg );
}
*/

// dummy window function
#elif defined OS_W32
static deoglRTContext *oglW32WndFuncContext = NULL;

static LRESULT CALLBACK oglW32WndFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
	if( oglW32WndFuncContext ){
		return oglW32WndFuncContext->ProcessWindowMessage( hwnd, message, wParam, lParam );
		
	}else{
		return DefWindowProc( hwnd, message, wParam, lParam );
	}
}
#endif // OS_*

#ifndef WITH_OPENGLES
struct sOpenGlVersion{
	int major;
	int minor;
};
static const int vOpenGLVersionCount = 9;
static const sOpenGlVersion vOpenGLVersions[ vOpenGLVersionCount ] = {
	{ 4, 6 }, { 4, 5 }, { 4, 4 }, { 4, 3 }, { 4, 2 }, { 4, 1 }, { 4, 0 }, { 3, 3 }, { 3, 2 } };
#endif // WITH_OPENGLES



// Class deoglRTContext
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTContext::deoglRTContext(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pCompileContextCount(0),

#ifdef OS_UNIX_X11
pOSUnix( renderThread.GetOgl().GetOS()->CastToOSUnix() ),

pDisplay( NULL ),
pScreen( 0 ),

#ifdef BACKEND_OPENGL
pContext( nullptr ),
pLoaderContext( nullptr ),
#endif // BACKEND_OPENGL

pColMap( 0 ),
pVisInfo( NULL ),

#elif defined OS_ANDROID
pOSAndroid( renderThread.GetOgl().GetOS()->CastToOSAndroid() ),

pDisplay( EGL_NO_DISPLAY ),
pSurface( EGL_NO_SURFACE ),
pContext( EGL_NO_CONTEXT ),
pLoaderSurface( EGL_NO_SURFACE ),
pLoaderContext( EGL_NO_CONTEXT ),

pScreenWidth( 0 ),
pScreenHeight( 0 ),

#elif defined OS_WEBWASM
pOSWebWasm(renderThread.GetOgl().GetOS()->CastToOSWebWasm()),
pContext(0),
pLoaderContext(0),
pScreenWidth(0),
pScreenHeight(0),

#elif defined OS_BEOS
pOSBeOS( renderThread.GetOgl().GetOS()->CastToOSBeOS() ),
//pContext( NULL ),

#elif defined OS_MACOS
pOSMacOS( renderThread.GetOgl().GetOS()->CastToOSMacOS() ),
pPixelFormat( NULL ),
pContext( nullptr ),
pLoaderContext( nullptr ),

#elif defined OS_W32
pWindowClassname( "DEOpenGLWindow" ),
pOSWindows( renderThread.GetOgl().GetOS()->CastToOSWindows() ),
pContext( NULL ),
pLoaderContext( NULL ),
#endif // OS_*

pActiveRRenderWindow( NULL ),
pUserRequestedQuit( false ),
pAppActivated( true )
{
#ifdef OS_UNIX_X11
	#ifdef BACKEND_OPENGL
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = nullptr;
	}
	#endif // BACKEND_OPENGL
	
#elif defined OS_ANDROID
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileSurface[i] = EGL_NO_SURFACE;
		pCompileContext[i] = EGL_NO_CONTEXT;
	}
	
#elif defined OS_WEBWASM
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = 0;
	}
	
#elif defined OS_MACOS
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = nullptr;
	}
	
#elif defined OS_W32
	int i;
	for(i=0; i<MaxCompileContextCount; i++){
		pCompileContext[i] = NULL;
	}
#endif // OS_*
}

deoglRTContext::~deoglRTContext(){
}



// Management
///////////////

void deoglRTContext::InitPhase1( deRenderWindow* ){
	// NOTE called from the main thread after creating the context. do here everything required to be
	//      linked to the main thread and required by render thread initialization.
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 1 Entering" );
	
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 1 Exiting" );
}

void deoglRTContext::InitPhase2( deRenderWindow* ){
	// NOTE called from the render thread after starting initialization. do here everything required
	//      to be linked to the render thread after prerequisited have been done in the main thread
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 2 Entering" );
	
#ifdef OS_UNIX_X11
	pOpenDisplay();
	#ifdef BACKEND_OPENGL
		pChooseVisual();
		pChooseFBConfig();
	#endif // BACKEND_OPENGL
	pCreateColorMap();
	pCreateAtoms();
	pCreateContext();
	
#elif defined OS_ANDROID
	pInitDisplay();
	
#elif defined OS_BEOS
	pCreateContext();
	
#elif defined OS_MACOS
	pCreateContext();
#endif // OS_*
	
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 2 Exiting" );
}

void deoglRTContext::InitPhase3( deRenderWindow *renderWindow ){
	// NOTE called from the main thread after first thread initialization. do here everything required to be
	//      linked to the main thread that requires some render thread objects to be present
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 3 Entering" );
	
	// register window class in main thread so the window function is linked to the main thread
#ifdef OS_W32
	pRegisterWindowClass();
#endif
	
	// set the render window. it should not have a peer yet so we create one.
	// this includes also the attached primary render target. this has to be done in the main
	// thread since all peer creation happens in the main thread. especially the window creation
	// triggered below has to be in the main thread to keep all window handling there
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	
	if( ! renderWindow->GetCanvasView()->GetPeerGraphic() ){
		renderWindow->GetCanvasView()->SetPeerGraphic( new deoglCanvasView( ogl, *renderWindow->GetCanvasView() ) );
	}
	
	if( ! renderWindow->GetPeerGraphic() ){
		// deoglRenderWindow is calling CreateWindow. this one is important to be in the main thread
		renderWindow->SetPeerGraphic( new deoglRenderWindow( ogl, *renderWindow ) );
		if( ! renderWindow->GetPeerGraphic() ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	if( ! renderWindow->GetHostWindow() ){
		( ( deoglRenderWindow* )renderWindow->GetPeerGraphic() )->GetRRenderWindow()->CenterOnScreen();
	}
	
	// HACK assign window. the constructor above ensures the window is created
#ifdef OS_UNIX_X11
	pOSUnix->SetWindow( renderWindow->GetWindow() );
	
#elif defined OS_ANDROID
	//pOSAndroid->SetWindow( renderWindow->GetWindow() );
	
#elif defined OS_BEOS
	pOSBeOS->SetWindow( renderWindow->GetWindow() );
	
#elif defined OS_MACOS
	pOSMacOS->SetWindow( renderWindow->GetWindow() );
	
#elif defined OS_W32
	pOSWindows->SetWindow( renderWindow->GetWindow() );
#endif // OS_*
	
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 3 Exiting" );
}

void deoglRTContext::InitPhase4( deRenderWindow *renderWindow ){
	// NOTE called from the render thread to finish thread initialization. do here everything required to be
	//      linked to the render thread that requires some in between main thread objects to be present
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 4 Entering" );
	
	// activate render window
	ActivateRRenderWindow( ( ( deoglRenderWindow* )renderWindow->GetPeerGraphic() )->GetRRenderWindow() );
	
#ifdef OS_W32
	pCreateContext();
#endif
	
	pRenderThread.GetLogger().LogInfo( "RTContext Init Phase 4 Exiting" );
}

void deoglRTContext::CleanUp(){
	// this is called in the render thread but during cleanup where the main thread is waiting for
	// the render thread. thus certain main thread access is allowed here
	
	// ensure the active deoglRRenderWindow is dropped. if there are still deoglRRenderWindow
	// around this will not work but at this point in time no deoglRRenderWindow should exist 
	// anymore in the render thread otherwise something is wrong
	if( pRenderThread.GetRRenderWindowList().GetCount() > 0 ){
		pRenderThread.GetLogger().LogWarnFormat(
			"deoglRTContext.CleanUp: Render thread still has %d RRenderWindows!",
			pRenderThread.GetRRenderWindowList().GetCount() );
	}
	
	// we need to forcefully drop the active window in case the list above is not empty otherwise
	// we end up with another window being active
	ActivateRRenderWindow( NULL, true );
	
	// HACK unassign window
#ifdef OS_UNIX_X11
	pOSUnix->SetWindow( 0 );
	
#elif defined OS_BEOS
	pOSBeOS->SetWindow( NULL );
	
#elif defined OS_MACOS
	pOSMacOS->SetWindow( NULL );
	
#elif defined OS_W32
	pOSWindows->SetWindow( NULL );
#endif // OS_*
	
	// free operating system specific objects
#ifdef OS_UNIX_X11
	pFreeContext();
	pFreeVisualInfo();
	pCloseDisplay();
	
#elif defined OS_ANDROID
	pCloseDisplay();
	
#elif defined OS_BEOS
	pFreeContext();
	
#elif defined OS_MACOS
	pFreeContext();
	
#elif defined OS_W32
	pFreeContext();
	pUnregisterWindowClass();
#endif // OS_*
}

#ifdef OS_ANDROID
void deoglRTContext::InitAppWindow(){
	pInitDisplay();
}

void deoglRTContext::TerminateAppWindow(){
	if( pDisplay == EGL_NO_DISPLAY ){
		return;
	}
	if( pSurface == EGL_NO_SURFACE ){
		return;
	}
	
	pRenderThread.GetLogger().LogInfo( "Destroy surface (close window)" );
	eglMakeCurrent( pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	
	eglDestroySurface( pDisplay, pSurface );
	pSurface = EGL_NO_SURFACE;
}

#elif defined OS_WEBWASM
void deoglRTContext::InitAppWindow(){
}

void deoglRTContext::TerminateAppWindow(){
}
#endif // OS_*



void deoglRTContext::ActivateRRenderWindow( deoglRRenderWindow *rrenderWindow, bool forceIfNull ){
	// NULL should be prevented for the time being. pick the first available window if possible
	if( ! rrenderWindow && pRenderThread.GetRRenderWindowList().GetCount() > 0 && ! forceIfNull ){
		rrenderWindow = ( deoglRRenderWindow* )pRenderThread.GetRRenderWindowList().GetAt( 0 );
	}
	
	if( pActiveRRenderWindow == rrenderWindow ){
		return;
	}
	
#ifdef OS_BEOS
	if( pActiveRRenderWindow ){
		pActiveRRenderWindow->GetGLView()->UnlockGL();
	}
#endif // OS_BEOS
	
	pActiveRRenderWindow = rrenderWindow;
	
	if( rrenderWindow ){
#ifdef OS_UNIX_X11
		#ifdef BACKEND_OPENGL
			// printf( "glXMakeCurrent(%lu,%p) previous(%lu,%p)\n", rrenderWindow->GetWindow(), pContext, glXGetCurrentDrawable(), glXGetCurrentContext() );
			OGLX_CHECK( pRenderThread, glXMakeCurrent( pDisplay, rrenderWindow->GetWindow(), pContext ) );
		#endif // BACKEND_OPENGL
		
#elif defined OS_ANDROID
		if( eglMakeCurrent( pDisplay, pSurface, pSurface, pContext ) == EGL_FALSE ){
			DETHROW( deeInvalidParam );
		}
		
#elif defined OS_WEBWASM
		DEASSERT_TRUE(emscripten_webgl_make_context_current(pContext) == EMSCRIPTEN_RESULT_SUCCESS)
		
#elif defined OS_BEOS
		rrenderWindow->GetGLView()->LockGL();
		
#elif defined OS_MACOS
		pContextMakeCurrent( rrenderWindow->GetView() );
		
#elif defined OS_W32
		if( ! wglMakeCurrent( rrenderWindow->GetWindowDC(), pContext ) ){
			pRenderThread.GetLogger().LogErrorFormat( "wglMakeCurrent failed (%s:%i): error=0x%lx\n",
				__FILE__, __LINE__, GetLastError() );
		}
#endif // OS_*
		
#ifndef OS_BEOS
		#ifdef BACKEND_OPENGL
		if( pLoaderContext ){
			// this check is required for windows to work correctly because on windows the window
			// is activated before the loader context is created which would cause problems
			pRenderThread.GetLoaderThread().EnableContext( true );
		}
		#endif // BACKEND_OPENGL
#endif // OS_BEOS
		
	}else{
		pRenderThread.GetLoaderThread().EnableContext( false );
		
#ifdef OS_UNIX_X11
		#ifdef BACKEND_OPENGL
// 		printf( "glXMakeCurrent(clear) previous(%lu,%p)\n", glXGetCurrentDrawable(), glXGetCurrentContext() );
		OGLX_CHECK( pRenderThread, glXMakeCurrent( pDisplay, None, NULL ) );
		#endif // BACKEND_OPENGL
		
#elif defined OS_ANDROID
		if( eglMakeCurrent( pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT ) == EGL_FALSE ){
			DETHROW( deeInvalidParam );
		}
		
#elif defined OS_WEBWASM
		DEASSERT_TRUE(emscripten_webgl_make_context_current(0) == EMSCRIPTEN_RESULT_SUCCESS)
		
#elif defined OS_MACOS
		pGLContextMakeCurrent( NULL );
		
#elif defined OS_W32
		if( ! wglMakeCurrent( NULL, NULL ) ){
			pRenderThread.GetLogger().LogErrorFormat( "wglMakeCurrent failed (%s:%i): error=0x%lx\n",
				__FILE__, __LINE__, GetLastError() );
		}
#endif // OS_*
	}
}

bool deoglRTContext::GetUserRequestedQuit(){
	const bool requested = pUserRequestedQuit;
	pUserRequestedQuit = false;
	return requested;
}



#ifdef BACKEND_OPENGL
void *deoglRTContext::GetFunctionPointer( const char *funcName ){
	// linux
#ifdef OS_UNIX_X11
	#ifndef HAS_OGL_GETPROC
		return ( void* )glXGetProcAddressARB( ( const GLubyte * )funcName );
	#else
		return ( void* )glXGetProcAddress( ( const GLubyte * )funcName );
	#endif
	
#elif defined WITH_OPENGLES
	return ( void* )androidGetProcAddress(pRenderThread, funcName);
	
#elif defined OS_BEOS
	DEASSERT_NOTNULL( pActiveRRenderWindow )
	return ( void* )pActiveRRenderWindow->GetGLView()->GetGLProcAddress( funcName );
	
#elif defined OS_MACOS
	return ( void* )macosGetProcAddress( funcName );
	
#elif defined OS_W32
	return ( void* )wglGetProcAddress( funcName );
#endif // OS_*
}
#endif // BACKEND_OPENGL


#ifdef OS_ANDROID
void deoglRTContext::CheckConfigurationChanged(){
	const decBoundary &contentRect = pOSAndroid->GetContentRect();
	
	const int width = contentRect.x2 - contentRect.x1;
	const int height = contentRect.y2 - contentRect.y1;
	
	if(width > 0 || height > 0){
		pScreenWidth = width;
		pScreenHeight = height;
		
	}else{
		// initial situation where content area is not known yet. use screen instead
		eglQuerySurface(pDisplay, pSurface, EGL_WIDTH, &pScreenWidth);
		eglQuerySurface(pDisplay, pSurface, EGL_HEIGHT, &pScreenHeight);
		
		//pScreenWidth = (int)ANativeWindow_getWidth(pOSAndroid->GetNativeWindow());
		//pScreenHeight = (int)ANativeWindow_getHeight(pOSAndroid->GetNativeWindow());
	}
}


#elif defined OS_WEBWASM
void deoglRTContext::CheckConfigurationChanged(){
	const decBoundary &contentRect = pOSWebWasm->GetContentRect();
	pScreenWidth = contentRect.x2 - contentRect.x1;
	pScreenHeight = contentRect.y2 - contentRect.y1;
	
	DEASSERT_TRUE(emscripten_webgl_get_drawing_buffer_size(
		pContext, &pScreenWidth, &pScreenHeight) == EMSCRIPTEN_RESULT_SUCCESS)
}


#elif defined OS_W32
LRESULT deoglRTContext::ProcessWindowMessage( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
	// WARNING send by main thread while processing events
	
	switch( message ){
	case WM_SETCURSOR:{
		const deoglRenderWindowList &windows = pRenderThread.GetOgl().GetRenderWindowList();
		const int count = windows.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			deoglRenderWindow &window = *windows.GetAt( i );
			if( window.GetRRenderWindow()->GetWindow() == hwnd ){
				if( window.GetRRenderWindow()->GetHostWindow() ){
					return DefWindowProc( hwnd, message, wParam, lParam );
				}
				break;
			}
		}
		if( LOWORD( lParam ) != HTCLIENT ){
			return DefWindowProc( hwnd, message, wParam, lParam );
		}

		SetCursor( NULL );
		}return TRUE;
		
	case WM_CLOSE:
		pRenderThread.GetOgl().GetGameEngine()->GetScriptingSystem()->UserRequestQuit();
		return 0; // do not close
		
	case WM_ACTIVATEAPP:
		// changing this value in the main thread is acceptable
		pAppActivated = wParam == TRUE;
		
		// this situation is now annoying. the deOSWindow does not receive WM_ACTIVATEAPP
		// only we do. this in turn means the windows input module will not receive this
		// message either but it is required to get it. so we forward the message to
		// the input module
		{
		MSG msg;
		memset( &msg, 0, sizeof( msg ) );
		msg.hwnd = hwnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		pRenderThread.GetOgl().GetGameEngine()->GetInputSystem()->GetActiveModule()->EventLoop( msg );
		}
		return 0;
		
	case WM_ERASEBKGND:
		// according to opengl wiki required to prevent flickering
		return 1;
		
	case WM_SIZE:{
		const deoglRenderWindowList &windows = pRenderThread.GetOgl().GetRenderWindowList();
		const int count = windows.GetCount();
		//const int height = HIWORD( lParam ); // outer width
		//const int width = LOWORD( lParam ); // outer height
		int i;
		
		for( i=0; i<count; i++ ){
			deoglRenderWindow &window = *windows.GetAt( i );
			if( window.GetRRenderWindow()->GetWindow() == hwnd ){
				// for fullscreen windows the size is incorrect while the window style is changed.
				// to avoid problems ignore size change if the window is full screen
				if( window.GetRRenderWindow()->GetFullScreen() ){
					break;
				}

				// the size provided in the message are the outer size of the window not the inner size.
				// for non-fullscreen windows we have to calculate the correct inner size
				const decPoint innerSize( window.GetRRenderWindow()->GetInnerSize() );
				window.GetRenderWindow().SetSize( innerSize.x, innerSize.y );
				break;
			}
		}
		}return 0;
		
	case WM_DPICHANGED:{
		const deoglRenderWindowList &windows = pRenderThread.GetOgl().GetRenderWindowList();
		const int scaleFactor = 100 * HIWORD(wParam) / USER_DEFAULT_SCREEN_DPI;
		const RECT &rect = *((RECT*)lParam);
		const int count = windows.GetCount();
		int i;

		for(i=0; i<count; i++){
			deoglRenderWindow &window = *windows.GetAt(i);
			deoglRRenderWindow &rwindow = *window.GetRRenderWindow();

			if(rwindow.GetWindow() == hwnd){
				deRenderWindow &engWindow = window.GetRenderWindow();

				engWindow.SetScaleFactor(scaleFactor);

				if(rwindow.GetFullScreen()){
					engWindow.SetSize(rect.right - rect.left, rect.bottom - rect.top);

				}else{
					engWindow.SetPosition(rect.left, rect.top);
					engWindow.SetSize(rect.right - rect.left, rect.bottom - rect.top);
				}
				break;
			}
		}
		}return 0;

	/*
	case WM_SIZING:{
		RECT rect( *( ( RECT* )lParam ) );
		RECT inverseRect;
		SetRectEmpty( &inverseRect );
		if( AdjustWindowRectEx( &inverseRect, GetWindowLong( hwnd, GWL_STYLE ),
		FALSE, GetWindowLong( hwnd, GWL_EXSTYLE ) ) ){
			rect.left -= inverseRect.left;
			rect.top -= inverseRect.top;
			rect.right -= inverseRect.right;
			rect.bottom -= inverseRect.bottom;
		}
		
		const decObjectOrderedSet &windows = pRenderThread.GetRRenderWindowList();
		const int count = windows.GetCount();
		const int height = rect.right - rect.left;
		const int width = rect.bottom - rect.top;
		int i;
		for( i=0; i<count; i++ ){
			deoglRRenderWindow &window = *( ( deoglRRenderWindow* )windows.GetAt( i ) );
			if( window.GetWindow() == hwnd ){
				window.OnResize( width, height );
				break;
			}
		}
		}return 0;
	*/
	}
	
	return DefWindowProc( hwnd, message, wParam, lParam );
}
#endif // OS_*



// Private Functions
//////////////////////

#ifdef OS_UNIX_X11

Display *deoglRTContext::GetMainThreadDisplay() const{
	return pOSUnix->GetDisplay();
}

void deoglRTContext::pOpenDisplay(){
	const char *dispName = getenv( "DISPLAY" );
	
	// open display
	if( ! dispName ){
		dispName = "";
	}
	pDisplay = XOpenDisplay( dispName );
	if( ! pDisplay ){
		pRenderThread.GetLogger().LogErrorFormat( "Cannot not open display %s\n", XDisplayName( dispName ) );
		DETHROW( deeInvalidAction );
	}
	
	pScreen = XDefaultScreen( pDisplay );
}

void deoglRTContext::ProcessEventLoop(){
	XEvent event;
	while( XPending( pDisplay ) ){
		XNextEvent( pDisplay, &event );
		
		switch( event.type ){
		case ClientMessage:
			if( event.xclient.message_type == pAtomProtocols
			&& event.xclient.data.l[0] == ( int )pAtomDeleteWindow ){
				pUserRequestedQuit = true;
			}
			break;
			
		case FocusIn:
			pAppActivated = true;
			break;
			
		case FocusOut:
			pAppActivated = false;
			break;
			
		case ConfigureNotify:{
			const decObjectOrderedSet &windows = pRenderThread.GetRRenderWindowList();
			const int count = windows.GetCount();
			int i;
			for( i=0; i<count; i++ ){
				deoglRRenderWindow &window = *( ( deoglRRenderWindow* )windows.GetAt( i ) );
				if( window.GetWindow() == event.xconfigure.window ){
					window.OnResize( event.xconfigure.width, event.xconfigure.height );
					break;
				}
			}
			}break;
		}
	}
}

#ifdef BACKEND_OPENGL

void deoglRTContext::pPrintVisualInfo(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	int value;
	
	logger.LogInfo( "Visual Infos:" );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_RED_SIZE, &value );
	logger.LogInfoFormat( "  Red Bit Count:      %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_GREEN_SIZE, &value );
	logger.LogInfoFormat( "  Green Bit Count:    %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_BLUE_SIZE, &value );
	logger.LogInfoFormat( "  Blue Bit Count:     %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_ALPHA_SIZE, &value );
	logger.LogInfoFormat( "  Alpha Bit Count:    %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_DEPTH_SIZE, &value );
	logger.LogInfoFormat( "  Depth Bit Count:    %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_STENCIL_SIZE, &value );
	logger.LogInfoFormat( "  Stencil Bit Count:  %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_ACCUM_RED_SIZE, &value );
	logger.LogInfoFormat( "  Accumulation Buffer Red Bit Count:    %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_ACCUM_GREEN_SIZE, &value );
	logger.LogInfoFormat( "  Accumulation Buffer Green Bit Count:  %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_ACCUM_BLUE_SIZE, &value );
	logger.LogInfoFormat( "  Accumulation Buffer Blue Bit Count:   %i", value );
	
	glXGetConfig( pDisplay, pVisInfo, GLX_ACCUM_ALPHA_SIZE, &value );
	logger.LogInfoFormat( "  Accumulation Buffer Alpha Bit Count:  %i", value );
}

void deoglRTContext::pChooseFBConfig(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	GLXFBConfig *configs = NULL;
	int configCount = 0;
	int configValue;
	int bestConfig;
	
	int attribs[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_CONFIG_CAVEAT, GLX_NONE,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		None };
	
	try{
		// try to find a matching fbconfig
		configs = glXChooseFBConfig( pDisplay, pScreen, attribs, &configCount );
		if( configCount == 0 || ! configs ){
			DETHROW_INFO( deeInvalidAction, "glXChooseFBConfig failed" );
		}
		
		// find the best config that is not over-sized
		bestConfig = configCount - 1;
//		for(bestConfig=0; bestConfig<configCount; bestConfig++){
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_RED_SIZE, &configValue);
//			if(configValue > maxBitsRed) continue;
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_GREEN_SIZE, &configValue);
//			if(configValue > maxBitsGreen) continue;
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_BLUE_SIZE, &configValue);
//			if(configValue > maxBitsBlue) continue;
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_ALPHA_SIZE, &configValue);
//			if(configValue > maxBitsAlpha) continue;
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_DEPTH_SIZE, &configValue);
//			if(configValue > maxBitsDepth) continue;
//			glXGetFBConfigAttrib(pDisplay, configs[0], GLX_STENCIL_SIZE, &configValue);
//			if(configValue > maxBitsStencil) continue;
//			// good
//			break;
//		}
//		if(bestConfig == configCount) bestConfig = configCount - 1;
		
		// store the best config away and free memory
		pBestFBConfig = configs[ bestConfig ];
		XFree( configs );
		configs = NULL;
		
		// print out the stats of the best config we have chose
		if( pRenderThread.GetConfiguration().GetDoLogDebug() ){
			logger.LogInfo( "glXChooseFBConfig:" );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_RED_SIZE, &configValue );
			logger.LogInfoFormat( "  Red Bit Count:      %i", configValue );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_GREEN_SIZE, &configValue );
			logger.LogInfoFormat( "  Green Bit Count:    %i", configValue );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_BLUE_SIZE, &configValue );
			logger.LogInfoFormat( "  Blue Bit Count:     %i", configValue );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_ALPHA_SIZE, &configValue );
			logger.LogInfoFormat( "  Alpha Bit Count:    %i", configValue );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_DEPTH_SIZE, &configValue );
			logger.LogInfoFormat( "  Depth Bit Count:    %i", configValue );
			
			glXGetFBConfigAttrib( pDisplay, pBestFBConfig, GLX_STENCIL_SIZE, &configValue );
			logger.LogInfoFormat( "  Stencil Bit Count:  %i", configValue );
		}
		
	}catch( const deException & ){
		if( configs ){
			XFree( configs );
		}
		throw;
	}
}

void deoglRTContext::pChooseVisual(){
	int attrList[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER,
		None };
	
	pVisInfo = glXChooseVisual( pDisplay, pScreen, attrList );
	
	if( ! pVisInfo ){
		DETHROW( deeInvalidAction );
	}
	
	if( pRenderThread.GetConfiguration().GetDoLogDebug() ){
		pPrintVisualInfo();
	}
}

#endif // BACKEND_OPENGL

void deoglRTContext::pCreateColorMap(){
	Window rootWindow = RootWindow( pDisplay, pVisInfo->screen );
	
	pColMap = XCreateColormap( pDisplay, rootWindow, pVisInfo->visual, AllocNone );
}

void deoglRTContext::pCreateAtoms(){
	pAtomProtocols = XInternAtom( pDisplay, "WM_PROTOCOLS", False );
	pAtomDeleteWindow = XInternAtom( pDisplay, "WM_DELETE_WINDOW", False );
}

void deoglRTContext::pCreateContext(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
#ifdef BACKEND_OPENGL
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRenderThread.GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	// create render context. try new version first then the old one if not possible
	// NOTE it is important to try to find first the "ARB" version of the function.
	//      some driver implementations have bugged "No-ARB" version of the function
	//      returning NULL. the "ARB" version seems less error prone
	PFNGLXCREATECONTEXTATTRIBSARBPROC pglXCreateContextAttribs = NULL;
	pglXCreateContextAttribs = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
		GetFunctionPointer( "glXCreateContextAttribsARB" );
	if( ! pglXCreateContextAttribs ){
		pglXCreateContextAttribs = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
			GetFunctionPointer( "glXCreateContextAttribs" );
	}
	
	if( pglXCreateContextAttribs ){
		logger.LogInfo( "Creating OpenGL Context using new method" );
		
		int contextFlags = 0;
		
		// according to OpenGL wiki this has no effect on 3.2+ context but MacOS requies
		// this to be set otherwise context creation fails
		#ifdef OS_MACOS
		contextFlags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
		#endif
		
		if( pRenderThread.GetConfiguration().GetDebugContext() ){
			logger.LogInfo( "Enable debug context" );
			contextFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		}
		
		// this one here is really the biggest mess of all times. AMD does it right in that
		// requesting a 3.3 context (as minimum) gives you up to 4.6 context if supported.
		// this is how it is correctly done. and then we have nVidia which (again) fails to
		// to it correctly. it gives you exactly a 3.3 context instead of the highest supported
		// context. this causes problems since nVidia (again) fails compiling shaders if the
		// shader version is not set high enough. this is again wrong in many ways since using
		// the #extension directive in the shader overrules the #version directive but nVidia
		// doesn't give a damn about OpenGL specs. Intel is in the same boat but nobody expects
		// any sane 3D drivers from them anyway. so how to solve this mess? we do it ugly by
		// simply trying all possible OpenGL context versions from the highest down to the
		// lowest until we find the first one that works. and yes, this is really, really,
		// REALLY ugly. thanks nVidia for nothing
		int contextAttribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
			GLX_RENDER_TYPE, GLX_RGBA_TYPE,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			GLX_CONTEXT_FLAGS_ARB, contextFlags,
			None, None,
			None };
// 		int contextAttribCount = 10;
		
		int i;
		for( i=0; i<vOpenGLVersionCount; i++ ){
			contextAttribs[ 1 ] = vOpenGLVersions[ i ].major;
			contextAttribs[ 3 ] = vOpenGLVersions[ i ].minor;
			
			pContext = pglXCreateContextAttribs( pDisplay, pBestFBConfig, NULL, True, contextAttribs );
			if( pContext ){
				logger.LogInfoFormat( "- Trying %d.%d Core... Success",
					vOpenGLVersions[ i ].major, vOpenGLVersions[ i ].minor );
				pLoaderContext = pglXCreateContextAttribs( pDisplay, pBestFBConfig, pContext, True, contextAttribs );
				int j;
				for(j=0; j<compileContextCount; j++){
					pCompileContext[j] = pglXCreateContextAttribs(pDisplay, pBestFBConfig, pContext, True, contextAttribs);
					if(!pCompileContext[j]){
						break;
					}
				}
				pCompileContextCount = j;
				logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
				break;
			}
			
			logger.LogInfoFormat( "- Trying %d.%d Core... Failed",
				vOpenGLVersions[ i ].major, vOpenGLVersions[ i ].minor );
		}
		
		if( ! pContext ){
			logger.LogWarn( "No supported OpenGL Context could be created with new method. "
				"Creating OpenGL Context using old method" );
			pContext = glXCreateNewContext( pDisplay, pBestFBConfig, GLX_RGBA_TYPE, NULL, True );
			pLoaderContext = glXCreateNewContext( pDisplay, pBestFBConfig, GLX_RGBA_TYPE, pContext, True );
			for(i=0; i<compileContextCount; i++){
				pCompileContext[i] = glXCreateNewContext(pDisplay, pBestFBConfig, GLX_RGBA_TYPE, pContext, True);
				if(!pCompileContext[i]){
					break;
				}
			}
			pCompileContextCount = i;
			logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
		}
		
	}else{
		logger.LogInfo( "Creating OpenGL Context using old method" );
		pContext = glXCreateNewContext( pDisplay, pBestFBConfig, GLX_RGBA_TYPE, NULL, True );
		pLoaderContext = glXCreateNewContext( pDisplay, pBestFBConfig, GLX_RGBA_TYPE, pContext, True );
		int i;
		for(i=0; i<compileContextCount; i++){
			pCompileContext[i] = glXCreateNewContext(pDisplay, pBestFBConfig, GLX_RGBA_TYPE, pContext, True);
			if(!pCompileContext[i]){
				break;
			}
		}
		pCompileContextCount = i;
		logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
	}
	
	DEASSERT_NOTNULL( pContext )
	DEASSERT_NOTNULL( pLoaderContext )
	
	if( ! glXIsDirect( pDisplay, pContext ) ){
		logger.LogError( "No matching direct rendering context found!" );
		DETHROW( deeInvalidAction );
	}
	
#elif defined BACKEND_VULKAN
	logger.LogInfo("Creating Vulkan device");
	pVulkan.TakeOver(new deSharedVulkan(pRenderThread.GetOgl()));
	pDevice = pVulkan->GetInstance().CreateDeviceHeadlessGraphic(0);
	
	pQueueGraphic = &pDevice->GetGraphicQueue();
	pQueueCompute = &pDevice->GetComputeQueue();
	pQueueTransfer = &pDevice->GetTransferQueue();
	
	pCommandPoolGraphic = pQueueGraphic->CreateCommandPool();
	pCommandPoolCompute = pQueueCompute->CreateCommandPool();
	pCommandPoolTransfer = pQueueTransfer->CreateCommandPool();
#endif // BACKEND_OPENGL
}

void deoglRTContext::pFreeContext(){
	if( ! pDisplay ){
		return;
	}
	
#ifdef BACKEND_OPENGL
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i]){
			glXDestroyContext(pDisplay, pCompileContext[i]);
			pCompileContext[i] = nullptr;
		}
	}
	if( pLoaderContext ){
		glXDestroyContext( pDisplay, pLoaderContext );
		pLoaderContext = nullptr;
	}
	if( pContext ){
		pRenderThread.GetLogger().LogInfo( "Free Context" );
		glXDestroyContext( pDisplay, pContext );
		pContext = nullptr;
	}
	
#elif defined BACKEND_VULKAN
	pDevice = nullptr;
	pVulkan = nullptr;
#endif // BACKEND_OPENGL
}

void deoglRTContext::pFreeVisualInfo(){
	if( pVisInfo ){
		XFree( pVisInfo );
		pVisInfo = NULL;
	}
}

void deoglRTContext::pCloseDisplay(){
	if( pDisplay ){
		XCloseDisplay( pDisplay );
		pDisplay = NULL;
	}
}


#elif defined OS_ANDROID

void deoglRTContext::pInitDisplay(){
	if( pSurface != EGL_NO_SURFACE ){
		return;
	}
	
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	DEASSERT_NOTNULL(pOSAndroid->GetNativeWindow())
	
	// initialize display
	if( pDisplay == EGL_NO_DISPLAY ){
		pRenderThread.GetLogger().LogInfo( "Init display" );
		pDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
		if( pDisplay == EGL_NO_DISPLAY ){
			DETHROW( deeInvalidParam );
		}
		
		eglInitialize( pDisplay, 0, 0 );
		
		// choose configuration
		const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_NONE
		};
		EGLint configCount;
		eglChooseConfig( pDisplay, attribs, &pConfig, 1, &configCount );
		if( configCount == 0 ){
			DETHROW( deeInvalidParam );
		}
	}
	
	// set buffer geometry
	/*
	EGLint format;
	eglGetConfigAttrib( pDisplay, pConfig, EGL_NATIVE_VISUAL_ID, &format );
	ANativeWindow_setBuffersGeometry( pOSAndroid->GetNativeWindow(), 0, 0, format );
	*/
	
	// create surface
	pSurface = eglCreateWindowSurface( pDisplay, pConfig, pOSAndroid->GetNativeWindow(), NULL );
	if( pSurface == EGL_NO_SURFACE ){
#ifdef OS_ANDROID_QUEST
		// happens if VR-Mode is enabled. in this case we need a small off-screen surface
		logger.LogInfo("Window surface already connected (VR-Mode). Creating tiny PBuffer surface");
		const EGLint eglBufferAttribList[]{
			EGL_WIDTH, 16,
			EGL_HEIGHT, 16,
			EGL_NONE
		};
		
		pSurface = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
		DEASSERT_FALSE(pSurface == EGL_NO_SURFACE)
#else
		DETHROW( deeInvalidParam );
#endif
	}
	
	// create context if not existing. it can be kept around while frozen
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRenderThread.GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	if( pContext == EGL_NO_CONTEXT ){
		//EGLint contextFlags = 0;
		EGLint valUseDebugContext = EGL_FALSE;
		if( pRenderThread.GetConfiguration().GetDebugContext() ){
			logger.LogInfo( "Enable debug context" );
			//contextFlags |= EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;
			valUseDebugContext = EGL_TRUE;
		}
		
		EGLint eglAttribList[] = {
			EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 0,
			//EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
			// ^== fails on meta quest
			EGL_CONTEXT_OPENGL_DEBUG, valUseDebugContext,
			EGL_NONE, EGL_NONE,
			EGL_NONE };
		
		pContext = eglCreateContext(pDisplay, pConfig, nullptr, eglAttribList);
		DEASSERT_FALSE(pContext == EGL_NO_CONTEXT)
		
		// loader context needs also an own surface
		const EGLint eglBufferAttribList[] = {
			EGL_WIDTH, 8,
			EGL_HEIGHT, 8,
			EGL_LARGEST_PBUFFER, EGL_TRUE,
			EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
			EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
			EGL_NONE
		};
		
		pLoaderSurface = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
		DEASSERT_FALSE(pLoaderSurface == EGL_NO_SURFACE)
		
		pLoaderContext = eglCreateContext(pDisplay, pConfig, pContext, eglAttribList);
		DEASSERT_FALSE(pLoaderContext == EGL_NO_CONTEXT)
		
		int i;
		for(i=0; i<compileContextCount; i++){
			pCompileContext[i] = eglCreateContext(pDisplay, pConfig, pContext, eglAttribList);
			if(pCompileContext[i] == EGL_NO_CONTEXT){
				break;
			}
			
			pCompileSurface[i] = eglCreatePbufferSurface(pDisplay, pConfig, eglBufferAttribList);
			if(pCompileSurface[i] == EGL_NO_SURFACE){
				eglDestroyContext(pDisplay, pCompileContext[i]);
				pCompileContext[i] = EGL_NO_CONTEXT;
				break;
			}
		}
		pCompileContextCount = i;
		pRenderThread.GetLogger().LogInfoFormat("Created %d compile contexts", pCompileContextCount);
	}
	
	// make surface current. we have to make it current each render loop
	if( eglMakeCurrent( pDisplay, pSurface, pSurface, pContext ) == EGL_FALSE ){
		DETHROW( deeInvalidParam );
	}
	
	// query display parameters
	eglQuerySurface( pDisplay, pSurface, EGL_WIDTH, &pScreenWidth );
	eglQuerySurface( pDisplay, pSurface, EGL_HEIGHT, &pScreenHeight );
	pRenderThread.GetLogger().LogInfoFormat( "Display size %ix%i", pScreenWidth, pScreenHeight );
}

void deoglRTContext::pCloseDisplay(){
	if( pDisplay == EGL_NO_DISPLAY ){
		return;
	}
	
	TerminateAppWindow();
	
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i] != EGL_NO_CONTEXT){
			eglDestroyContext(pDisplay, pCompileContext[i]);
			pCompileContext[i] = EGL_NO_CONTEXT;
		}
		if(pCompileSurface[i] != EGL_NO_SURFACE){
			eglDestroySurface(pDisplay, pCompileSurface[i]);
			pCompileSurface[i] = EGL_NO_SURFACE;
		}
	}
	
	if(pLoaderContext != EGL_NO_CONTEXT){
		eglDestroyContext(pDisplay, pLoaderContext);
		pLoaderContext = EGL_NO_CONTEXT;
	}
	if(pLoaderSurface != EGL_NO_SURFACE){
		eglDestroySurface(pDisplay, pLoaderSurface);
		pLoaderSurface = EGL_NO_SURFACE;
	}
	
	if( pContext != EGL_NO_CONTEXT ){
		eglDestroyContext( pDisplay, pContext );
		pContext = EGL_NO_CONTEXT;
	}
	
	eglTerminate( pDisplay );
	pDisplay = EGL_NO_DISPLAY;
}


#elif defined OS_WEBWASM

void deoglRTContext::pCreateContext(){
	if(pContext){
		return;
	}
	
	pCompileContextCount = decMath::min(MaxCompileContextCount,
		pRenderThread.GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	if(!pContext){
		EmscriptenWebGLContextAttributes attrs{};
		emscripten_webgl_init_context_attributes(&attrs);
		attrs.alpha = false;
		attrs.depth = true;
		attrs.stencil = true;
		attrs.antialias = false;
		attrs.premultipliedAlpha = false;
		attrs.preserveDrawingBuffer = false;
		attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
		attrs.failIfMajorPerformanceCaveat = false;
		attrs.majorVersion = 2;
		attrs.minorVersion = 0;
		attrs.enableExtensionsByDefault = false;
		attrs.explicitSwapControl = true;
		attrs.renderViaOffscreenBackBuffer = true;
		//attrs.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_FALLBACK;
		
		pContext = emscripten_webgl_create_context("#dragengine", &attrs);
		DEASSERT_NOTNULL(pContext)
		
		// loader and compiler contexts have to be created by the thread using them
	}
	
	DEASSERT_TRUE(emscripten_webgl_make_context_current(pContext) == EMSCRIPTEN_RESULT_SUCCESS)
	
	DEASSERT_TRUE(emscripten_webgl_get_drawing_buffer_size(
		pContext, &pScreenWidth, &pScreenHeight) == EMSCRIPTEN_RESULT_SUCCESS)
	
	pRenderThread.GetLogger().LogInfoFormat("Display size %ix%i", pScreenWidth, pScreenHeight);
}

void deoglRTContext::pFreeContext(){
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i]){
			emscripten_webgl_destroy_context(pCompileContext[i]);
			pCompileContext[i] = 0;
		}
	}
	
	if(pLoaderContext){
		emscripten_webgl_destroy_context(pLoaderContext);
		pLoaderContext = 0;
	}
	
	if(pContext){
		emscripten_webgl_destroy_context(pContext);
		pContext = 0;
	}
}

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE deoglRTContext::GetLoaderContext(){
	if(!pLoaderContext){
		EmscriptenWebGLContextAttributes attrs{};
		emscripten_webgl_init_context_attributes(&attrs);
		attrs.alpha = false;
		attrs.depth = false;
		attrs.stencil = false;
		attrs.antialias = false;
		attrs.premultipliedAlpha = false;
		attrs.preserveDrawingBuffer = false;
		attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
		attrs.failIfMajorPerformanceCaveat = false;
		attrs.majorVersion = 2;
		attrs.minorVersion = 0;
		attrs.enableExtensionsByDefault = false;
		attrs.explicitSwapControl = true;
		attrs.renderViaOffscreenBackBuffer = false;
		
		pLoaderContext = emscripten_webgl_create_context("#dragengine", &attrs);
		DEASSERT_NOTNULL(pLoaderContext)
	}
	return pLoaderContext;
}

/** Compile context. */
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE deoglRTContext::GetCompileContextAt(int index){
	if(!pCompileContext[index]){
		EmscriptenWebGLContextAttributes attrs{};
		emscripten_webgl_init_context_attributes(&attrs);
		attrs.alpha = false;
		attrs.depth = false;
		attrs.stencil = false;
		attrs.antialias = false;
		attrs.premultipliedAlpha = false;
		attrs.preserveDrawingBuffer = false;
		attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_LOW_POWER;
		attrs.failIfMajorPerformanceCaveat = false;
		attrs.majorVersion = 2;
		attrs.minorVersion = 0;
		attrs.enableExtensionsByDefault = false;
		attrs.explicitSwapControl = true;
		attrs.renderViaOffscreenBackBuffer = false;
		
		pCompileContext[index] = emscripten_webgl_create_context("#dragengine", &attrs);
		DEASSERT_NOTNULL(pCompileContext[index])
	}
	return pCompileContext[index];
}


#elif defined OS_BEOS

void deoglRTContext::pCreateContext(){
	// done by deoglRRenderWindow. BGLView is the context
}

void deoglRTContext::pFreeContext(){
	// done by deoglRRenderWindow. BGLView is the context
}


#elif defined OS_W32

void deoglRTContext::pRegisterWindowClass(){
	oglW32WndFuncContext = this;
	
	WNDCLASSEX wce;
	memset( &wce, 0, sizeof( wce ) );
	wce.hInstance = pOSWindows->GetInstApp();
	wchar_t wideClassName[ 200 ];
	deOSWindows::Utf8ToWide( pWindowClassname, wideClassName, MAX_PATH );
	wce.lpszClassName = wideClassName;
	wce.lpfnWndProc = oglW32WndFunc;
	wce.cbSize = sizeof( wce );
	wce.hIcon = NULL; //LoadIcon( NULL, IDI_APPLICATION );
	wce.hIconSm = NULL; //LoadIcon( NULL, IDI_APPLICATION );
	wce.hCursor = NULL; //LoadCursor( NULL, IDC_ARROW );
	wce.lpszMenuName = NULL;
	wce.hbrBackground = NULL;
	wce.style = CS_OWNDC;
	
	if( ! RegisterClassEx( &wce ) ){
		DETHROW( deeOutOfMemory );
	}
}

void deoglRTContext::pCreateContext(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	const int compileContextCount = decMath::min(MaxCompileContextCount,
		pRenderThread.GetOgl().GetGameEngine()->GetParallelProcessing().GetCoreCount());
	
	// create render context. on windows obtaining function pointers only works if a context has
	// been created. unfortunately the wglCreateContextAttribs is such a function pointer so we
	// need a fake context first before we can obtain that method. for this reason we first
	// create a context using the old method
	logger.LogInfo( "Creating OpenGL Context using old method" );
	pContext = wglCreateContext( pActiveRRenderWindow->GetWindowDC() );
	if( ! pContext ){
		logger.LogErrorFormat( "wglCreateContext failed with code %d", ( int )GetLastError() );
		DETHROW( deeOutOfMemory );
	}
	
	wglMakeCurrent( pActiveRRenderWindow->GetWindowDC(), pContext );
	
	// now we check if we can find the function pointer to create a new version context. if this
	// is the case we destroy the old context and create a new one using the function pointer
	// we just obtained. should this fail we create again an old context
	PFNWGLCREATECONTEXTATTRIBSARBPROC pwglCreateContextAttribs = NULL;
	pwglCreateContextAttribs = ( PFNWGLCREATECONTEXTATTRIBSARBPROC )
		GetFunctionPointer( "wglCreateContextAttribs" );
	if( ! pwglCreateContextAttribs ){
		pwglCreateContextAttribs = ( PFNWGLCREATECONTEXTATTRIBSARBPROC )
			GetFunctionPointer( "wglCreateContextAttribsARB" );
	}
	
	if( pwglCreateContextAttribs ){
		logger.LogInfoFormat( "Re-creating OpenGL Context using new method" );
		
		wglDeleteContext( pContext );
		pContext = NULL;
		
		int contextFlags = 0;
		
		if( pRenderThread.GetConfiguration().GetDebugContext() ){
			logger.LogInfo( "Enable debug context" );
			contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		}
		
		// this one here is really the biggest mess of all times. AMD does it right in that
		// requesting a 3.3 context (as minimum) gives you up to 4.6 context if supported.
		// this is how it is correctly done. and then we have nVidia which (again) fails to
		// do it correctly. it gives you exactly a 3.3 context instead of the highest supported
		// context. this causes problems since nVidia (again) fails compiling shaders if the
		// shader version is not set high enough. this is again wrong in many ways since using
		// the #extension directive in the shader overrules the #version directive but nVidia
		// doesn't give a damn about OpenGL specs. Intel is in the same boat but nobody expects
		// any sane 3D drivers from them anyway. so how to solve this mess? we do it ugly by
		// simply trying all possible OpenGL context versions from the highest down to the
		// lowest until we find the first one that works. and yes, this is really, really,
		// REALLY ugly. thanks nVidia for nothing
		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_FLAGS_ARB, contextFlags,
			0, 0,
			0 };
		
		int i;
		for( i=0; i<vOpenGLVersionCount; i++ ){
			contextAttribs[ 1 ] = vOpenGLVersions[ i ].major;
			contextAttribs[ 3 ] = vOpenGLVersions[ i ].minor;
			
			pContext = pwglCreateContextAttribs( pActiveRRenderWindow->GetWindowDC(), NULL, contextAttribs );
			if( pContext ){
				logger.LogInfoFormat( "- Trying %d.%d Core... Success",
					vOpenGLVersions[ i ].major, vOpenGLVersions[ i ].minor );
				pLoaderContext = pwglCreateContextAttribs( pActiveRRenderWindow->GetWindowDC(), pContext, contextAttribs );
				int j;
				for(j=0; j<compileContextCount; j++){
					pCompileContext[j] = pwglCreateContextAttribs(pActiveRRenderWindow->GetWindowDC(), pContext, contextAttribs);
					if(!pCompileContext[j]){
						break;
					}
				}
				pCompileContextCount = j;
				logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
				break;
			}
			
			logger.LogInfoFormat( "- Trying %d.%d Core... Failed",
				vOpenGLVersions[ i ].major, vOpenGLVersions[ i ].minor );
		}
		
		if( ! pContext ){
			logger.LogWarn( "No supported OpenGL Context could be created with new method. "
				"Creating OpenGL Context using old method" );
			pContext = wglCreateContext( pActiveRRenderWindow->GetWindowDC() );
			pLoaderContext = wglCreateContext( pActiveRRenderWindow->GetWindowDC() );
			DEASSERT_TRUE( wglShareLists( pLoaderContext, pContext ) )
			int i;
			for(i=0; i<compileContextCount; i++){
				pCompileContext[i] = wglCreateContext(pActiveRRenderWindow->GetWindowDC());
				if(!pCompileContext[i]){
					break;
				}
				if(!wglShareLists(pCompileContext[i], pContext)){
					wglDeleteContext(pCompileContext[i]);
					pCompileContext[i] = NULL;
					break;
				}
			}
			pCompileContextCount = i;
			logger.LogInfoFormat("Created %d compile contexts", pCompileContextCount);
		}
		
	}else{
		logger.LogInfoFormat( "Functions required for new OpenGL Context creation not found" );
	}
	
	if( ! pContext ){
		logger.LogErrorFormat( "wglCreateContext with code %i", ( int )GetLastError() );
		DETHROW( deeOutOfMemory );
	}
	DEASSERT_NOTNULL( pLoaderContext )
	
	// show the new render window
	//pOSWindows->SetWindow( pActiveRRenderWindow->GetWindow() ); // problem, hangs if called from render-thread
	
	// attach to context
	wglMakeCurrent( pActiveRRenderWindow->GetWindowDC(), pContext );

	// for windows we have to delay starting the make sure the loader thread is enabled after the loader
	// context is ready. before this point AcivateRRenderWindow() is called and then the loader context
	// is nullptr which causes problems. ActivateRRenderWindow() thus avoids enabling the loader if
	// no loader context exists. we thus have to do it here to make sure enabling works
	pRenderThread.GetLoaderThread().EnableContext( true );
}

void deoglRTContext::pUnregisterWindowClass(){
	wchar_t wideClassName[ 200 ];
	deOSWindows::Utf8ToWide( pWindowClassname, wideClassName, MAX_PATH );
	UnregisterClass( wideClassName, pOSWindows->GetInstApp() );
	oglW32WndFuncContext = NULL;
}

void deoglRTContext::pFreeContext(){
	// destroy render window
	//pDestroyRenderWindow();
	
	int i;
	for(i=0; i<pCompileContextCount; i++){
		if(pCompileContext[i]){
			wglDeleteContext(pCompileContext[i]);
			pCompileContext[i] = NULL;
		}
	}
	if( pLoaderContext ){
		wglDeleteContext( pLoaderContext );
		pLoaderContext = NULL;
	}
	if( pContext ){
		pRenderThread.GetLogger().LogInfo( "Free Context" );
		wglDeleteContext( pContext );
		pContext = NULL;
	}
}

#endif // OS_*
