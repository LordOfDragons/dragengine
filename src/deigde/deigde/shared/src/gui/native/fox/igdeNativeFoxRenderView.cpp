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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxRenderView.h"
#include "../../igdeContainer.h"
#include "../../igdeViewRenderWindow.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/rendering/deRenderWindow.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Definitions
////////////////

#ifndef WIN32

#define BASIC_EVENT_MASK ( \
	StructureNotifyMask \
	| ExposureMask \
	| PropertyChangeMask \
	| EnterWindowMask \
	| LeaveWindowMask \
	| KeyPressMask \
	| KeyReleaseMask \
	| KeymapStateMask )

#define ENABLED_EVENT_MASK ( \
	ButtonPressMask \
	| ButtonReleaseMask \
	| PointerMotionMask )

#define NOT_PROPAGATE_MASK ( \
	KeyPressMask \
	| KeyReleaseMask \
	| ButtonPressMask \
	| ButtonReleaseMask \
	| PointerMotionMask \
	| ButtonMotionMask )

#endif

#define LOGGING_NAME "ViewRenderWindow"



// Event map
//////////////

FXDEFMAP( igdeNativeFoxRenderView ) igdeNativeFoxRenderViewMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxRenderView::onResize ),
	FXMAPFUNC( SEL_MAP, 0, igdeNativeFoxRenderView::onMap ),
	FXMAPFUNC( SEL_UNMAP, 0, igdeNativeFoxRenderView::onUnmap ),
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxRenderView::onPaint ),
	FXMAPFUNC( SEL_KEYPRESS, 0, igdeNativeFoxRenderView::onKeyPress ),
	FXMAPFUNC( SEL_KEYRELEASE, 0, igdeNativeFoxRenderView::onKeyRelease ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxRenderView::onLeftMouseDown ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, igdeNativeFoxRenderView::onLeftMouseUp ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, 0, igdeNativeFoxRenderView::onRightMouseDown ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, 0, igdeNativeFoxRenderView::onRightMouseUp ),
	FXMAPFUNC( SEL_MIDDLEBUTTONPRESS, 0, igdeNativeFoxRenderView::onMiddleMouseDown ),
	FXMAPFUNC( SEL_MIDDLEBUTTONRELEASE, 0, igdeNativeFoxRenderView::onMiddleMouseUp ),
	FXMAPFUNC( SEL_DOUBLECLICKED, 0, igdeNativeFoxRenderView::onDoubleClicked ),
	FXMAPFUNC( SEL_MOTION, 0, igdeNativeFoxRenderView::onMouseMove ),
	FXMAPFUNC( SEL_MOUSEWHEEL, 0, igdeNativeFoxRenderView::onMouseWheel ),
	FXMAPFUNC( SEL_ENTER, 0, igdeNativeFoxRenderView::onMouseEnter ),
	FXMAPFUNC( SEL_LEAVE, 0, igdeNativeFoxRenderView::onMouseLeave ),
	
	// bug fix
	FXMAPFUNC( SEL_TIMEOUT, igdeNativeFoxRenderView::ID_TIMEOUT_RETRY_MAP,
		igdeNativeFoxRenderView::onTimeoutRetryMap ),
};


FXIMPLEMENT( igdeNativeFoxRenderView, FXFrame, igdeNativeFoxRenderViewMap,
	ARRAYNUMBER( igdeNativeFoxRenderViewMap ) )



// Class igdeNativeFoxRenderView
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxRenderView::igdeNativeFoxRenderView(){
	this->flags |= FLAG_ENABLED;
}

igdeNativeFoxRenderView::igdeNativeFoxRenderView( igdeViewRenderWindow &powner,
	FXComposite *pparent, int layoutFlags ) :
FXFrame( pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pCanAttachRenderWindow( false ),
pRenderWindowAttached( false ),
pCanRender( false ),
pErrorRenderWindow( false )
#ifdef OS_W32
,pOrgWindowProc( 0 )
#endif
{
	this->flags |= FLAG_ENABLED;
	backColor = getApp()->getBackColor();
	
	if( ! pOwner->GetVisible() ){
		hide();
	}
}

igdeNativeFoxRenderView::~igdeNativeFoxRenderView(){
	getApp()->removeTimeout( this, ID_TIMEOUT_RETRY_MAP );
}

igdeNativeFoxRenderView *igdeNativeFoxRenderView::CreateNativeWidget( igdeViewRenderWindow &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxRenderView( powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxRenderView::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	
	if( ! pparent.id() ){
		DETHROW( deeInvalidParam );
	}
	
	create();
}

void igdeNativeFoxRenderView::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxRenderView::destroy(){
	DropNativeWindow();
	FXFrame::destroy();
}

FXbool igdeNativeFoxRenderView::canFocus() const{
	return true;
}

/*void igdeNativeFoxRenderView::ClearErrorRenderWindow(){
	pErrorRenderWindow = false;
}*/

bool igdeNativeFoxRenderView::IsReallyVisible() const{
	const FXWindow *window = this;
	
	while( window ){
		if( ! window->shown() ){
			return false;
		}
		window = window->getParent();
	}
	
	return true;
}

bool igdeNativeFoxRenderView::IsShown() const{
	return shown();
}

decPoint igdeNativeFoxRenderView::GetSize() const{
	return decPoint( getWidth(), getHeight() );
}

void igdeNativeFoxRenderView::OnFrameUpdate(){
	// window updates are rendered to the window by the graphic module. FOX does not notice
	// this update so we have to tell it outself. for this we need to check if the window is
	// visible on screen. this check is required since there is no way to figure out otherwise
	// if the window is actually visible on screen or hidden (either itself or a parent
	// somewhere up the chain). disabling rendering here is important since otherwise strange
	// UI artifacts can happen
	
	// update only if painting is enabled
	if( pOwner->GetRenderWindow()->GetPaint() ){
		update();
	}
}



void igdeNativeFoxRenderView::DropNativeWindow(){
	DetachRenderWindow();
	pOwner = NULL;
}



void igdeNativeFoxRenderView::AttachRenderWindow(){
	if( ! pOwner ){
		return;
	}
	
	deRenderWindow * const renderWindow = pOwner->GetRenderWindow();
	if( ! renderWindow || ! pCanAttachRenderWindow ){
		pErrorRenderWindow = true;
		return;
	}
	
	if( id() == ( FXID )renderWindow->GetWindow() ){
		return; // re-entrant protection
	}
	
	pErrorRenderWindow = true; // if something goes wrong the error will be set
	
	const int xx = getX();
	const int yy = getY();
	const int wwidth = decMath::max( getWidth(), 0 );
	const int hheight = decMath::max( getHeight(), 0 );
	
	pSyncSizes();
	
	// prepare the window to be attached to fox
#ifdef OS_W32
	const HWND hwnd = renderWindow->GetWindow();
	
	if( SetWindowLongPtr( hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ) == 0 ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: SetWindowLongPtr(GWL_STYLE) failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	if( SetWindowLongPtr( hwnd, GWL_EXSTYLE, WS_EX_NOPARENTNOTIFY ) == 0 ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: SetWindowLongPtr(GWL_EXSTYLE) failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	
	// unfortunately FXApp::wndproc is private so we need to get it the hard way
	WNDCLASSEX wndClassEx;
	memset( &wndClassEx, '\0', sizeof( wndClassEx ) );
	wndClassEx.cbSize = sizeof( wndClassEx );
	if( ! GetClassInfoEx( ( HINSTANCE )getApp()->getDisplay(), (TCHAR*)FXFrame::GetClass(), &wndClassEx ) ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: GetClassInfoEx failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	
	pOrgWindowProc = GetWindowLongPtr( hwnd, GWLP_WNDPROC );
	if( ! pOrgWindowProc ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: GetWindowLongPtr(GWLP_WNDPROC) failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	if( SetWindowLongPtr( hwnd, GWLP_WNDPROC, ( LONG_PTR )wndClassEx.lpfnWndProc ) == 0 ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: SetWindowLongPtr(GWLP_WNDPROC) failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	
	// according to MSDN if you change certain parameters using SetWindowLong this line is
	// required exactly like this below (what goes for the flags)
	SetWindowPos( hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
	
#else
	Display * const display = ( Display* )( getApp()->getDisplay() );
	Window window = renderWindow->GetWindow();
	//XUnmapWindow( ( Display* )( getApp()->getDisplay() ), ( FXID )renderWindow->GetWindow() );
	XFlush( display );
	
	XSetWindowAttributes wattr;
	unsigned long mask;
	
	mask = CWEventMask | CWDontPropagate | CWCursor | CWOverrideRedirect
		| CWSaveUnder | CWWinGravity | CWBitGravity | CWBorderPixel;
	wattr.event_mask = BASIC_EVENT_MASK;
	if( flags & FLAG_ENABLED ){
		wattr.event_mask |= ENABLED_EVENT_MASK;
	}
	wattr.do_not_propagate_mask = NOT_PROPAGATE_MASK;
	wattr.override_redirect = doesOverrideRedirect();
	wattr.save_under = doesSaveUnder(); //false;
	wattr.cursor = defaultCursor->id();
	wattr.border_pixel = 0;
	wattr.bit_gravity = ForgetGravity;
	wattr.win_gravity = NorthWestGravity;
	
	XChangeWindowAttributes( display, window, mask, &wattr );
#endif
	
	// attach the window to fox. we have to destroy the old window not just detach it
	FXFrame::destroy();  // IMPORTANT! super-call or our destroy() overwrite fires and this is bad!
	attach( ( FXID )renderWindow->GetWindow() );
	pRenderWindowAttached = true;
	
	// update window properties to match what FOX has stored for the previous window
#ifdef OS_W32
	SetWindowPos( hwnd, HWND_TOP, xx, yy, wwidth, hheight, SWP_NOACTIVATE | SWP_NOCOPYBITS
		| SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_SHOWWINDOW );
#else
	if( wwidth > 0 && hheight > 0 ){
		XMoveResizeWindow( display, window, xx, yy, wwidth, hheight );
		XMapWindow( display, window );
		
	}else{
		XMoveResizeWindow( display, window, xx, yy, 1, 1 );
		XUnmapWindow( display, window );
	}
#endif
	
	pCanRender = true;
	pErrorRenderWindow = false;
	
	pOwner->OnResize();
}

void igdeNativeFoxRenderView::DetachRenderWindow(){
	if( ! pOwner ){
		return;
	}
	
	// if the render window exists and is still attached detach the ID or the window is
	// automatically destroyed if the parent window is destroyed. This breaks the graphic
	// module and potentially causes segmentation faults in the window manager or even
	// the GPU driver
	deRenderWindow * const renderWindow = pOwner->GetRenderWindow();
	if( ! pRenderWindowAttached || ! renderWindow ){
		return;
	}
	
	if( id() != ( FXID )renderWindow->GetWindow() ){
	    pOwner->GetLogger()->LogError( LOGGING_NAME,
			"ViewRenderWindow: Marked attached but id is not render window" );
	    //DETHROW( deeInvalidAction );
		return;
	}
	if( ! renderWindow->GetWindow() ){
	    pOwner->GetLogger()->LogError( LOGGING_NAME,
			"ViewRenderWindow: Detaching window but window does not exist" );
	    //DETHROW( deeInvalidAction );
		return;
	}
	
	// detach window. FOX does not know about the window anymore now but it is still parented
	// to the parent of the FOX window
	detach();
	pRenderWindowAttached = false;
	pCanRender = false;
	pErrorRenderWindow = true;
	
	// unparent the window or it is automatically destroyed if the parent window is destroyed.
	// we have to make sure the window still exists according to the graphic module. if it did
	// already destroy it we do not attempt to modify the window. detach above works no matter
	// if the window is still existing or not but not these calls here
	#ifdef OS_UNIX
	Window window = renderWindow->GetWindow();
	Display * const display = ( Display* )( getApp()->getDisplay() );
	XUnmapWindow( display, window );
	XReparentWindow( display, window, XDefaultRootWindow( display ), 0, 0 );
	XSync( display, False ); // required or funny things happen
	#endif
	
	#ifdef OS_W32
	HWND window = renderWindow->GetWindow();
	
	// restore the old window function otherwise fox still acts on it
	if( SetWindowLongPtr( window, GWLP_WNDPROC, pOrgWindowProc ) == 0 ){
	    pOwner->GetLogger()->LogErrorFormat( LOGGING_NAME,
			"ViewRenderWindow: SetWindowLongPtr(GWLP_WNDPROC) failed with error %lx", GetLastError() );
	    DETHROW( deeInvalidAction );
	}
	pOrgWindowProc = 0;
	
	SetParent( window, NULL );
	ShowWindow( window, SW_HIDE );
	#endif
	
	// create a new FOX window and attach it. from here on the world is safe for FOX
	create();
	
	// is this really required?
	//show(); // create does a mapping so not required
	recalc(); // makes sure we get updated the next time and the window fits FOX
}



void igdeNativeFoxRenderView::DrawEngineUnavailable( FXDCWindow &dc ){
	const FXColor colorCross = FXRGB( 255, 0, 0 );
	const FXColor colorBg = FXRGB( 64, 64, 64 );
	const int hheight = getHeight();
	const int wwidth = getWidth();
	
	dc.setForeground( colorBg );
	dc.fillRectangle( 0, 0, wwidth, hheight );
	
	dc.setForeground( colorCross );
	dc.drawLine( 0, 0, wwidth, hheight );
	dc.drawLine( 0, hheight, wwidth, 0 );
}

void igdeNativeFoxRenderView::DrawErrorRenderWindow( FXDCWindow &dc ){
	const FXColor colorCross = FXRGB( 255, 128, 0 );
	const FXColor colorBg = FXRGB( 64, 64, 64 );
	const int hheight = getHeight();
	const int wwidth = getWidth();
	
	dc.setForeground( colorBg );
	dc.fillRectangle( 0, 0, wwidth, hheight );
	
	dc.setForeground( colorCross );
	dc.drawLine( 0, 0, wwidth, hheight );
	dc.drawLine( 0, hheight, wwidth, 0 );
}

void igdeNativeFoxRenderView::DrawErrorRendering( FXDCWindow &dc ){
	const FXColor colorCross = FXRGB( 255, 0, 128 );
	const FXColor colorBg = FXRGB( 64, 64, 64 );
	const int hheight = getHeight();
	const int wwidth = getWidth();
	
	dc.setForeground( colorBg );
	dc.fillRectangle( 0, 0, wwidth, hheight );
	
	dc.setForeground( colorCross );
	dc.drawLine( 0, 0, wwidth, hheight );
	dc.drawLine( 0, hheight, wwidth, 0 );
}

void igdeNativeFoxRenderView::GrabInput(){
	grab();
	grabKeyboard();
}

void igdeNativeFoxRenderView::ReleaseInput(){
	ungrabKeyboard();
	ungrab();
}



// Events
///////////

long igdeNativeFoxRenderView::onResize( FXObject*, FXSelector, void* ){
	if( ! pOwner ){
		return 0;
	}
	
	deRenderWindow * const renderWindow = pOwner->GetRenderWindow();
	if( ! renderWindow ){
		return 0;
	}
	
	pSyncSizes();
	
	const int wwidth = decMath::max( getWidth(), 0 );
	const int hheight = decMath::max( getHeight(), 0 );
	if( wwidth == renderWindow->GetWidth() && hheight == renderWindow->GetHeight() ){
		return 0;
	}
	
	pOwner->OnResize();
	
	return 1;
}

long igdeNativeFoxRenderView::onMap( FXObject*, FXSelector, void* ){
	pProcessMap();
	return 1;
}

long igdeNativeFoxRenderView::onUnmap( FXObject*, FXSelector, void* ){
	if( ! pOwner ){
		return 0;
	}
	
	deRenderWindow * const renderWindow = pOwner->GetRenderWindow();
	if( renderWindow ){
		renderWindow->SetPaint( false );
	}
	pCanAttachRenderWindow = false;
	return 1;
}

long igdeNativeFoxRenderView::onPaint( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner ){
		return 0;
	}
	
	if( ! pOwner->GetEnableRendering() || ! shown()
	|| ! pOwner->GetEngineController().GetRunning() ){
		FXDCWindow dc( this, ( FXEvent* )pdata );
		DrawEngineUnavailable( dc );
		return 1;
	}
	
	if( pErrorRenderWindow || ! pOwner->GetRenderWindow() ){
		FXDCWindow dc( this, ( FXEvent* )pdata );
		DrawErrorRenderWindow( dc );
		return 1;
	}
	
	if( ! pCanRender ){
		FXDCWindow dc( this, ( FXEvent* )pdata );
		DrawErrorRendering( dc );
		return 1;
	}
	
	// render nothing. the game engine renders the content and triggers updates accordingly
	return 1;
}



long igdeNativeFoxRenderView::onKeyPress( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 0; // default handling
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyKeyPress( igdeUIFoxHelper::KeyCodeFromEvent( event ), event.code );
	
	// the focus handling in fox is annoying and produces an impossible to solve situation.
	// when we return 0 somewhere down the line FXComposite will process the key and try
	// to move the focus. this causes this widget to **incorrectly** loose the focus and
	// UI handling breaks. when we return 1 to prevent further processing hotkeys stop
	// working and we are fucked up again. the only solution is to do things veeeeery
	// complicated trying to solve the mess
	
	// check accelerators. we have to walk up the entire widget tree to pick up all
	// accelerators. I do **not** know if this is the correct way to fix this mess but
	// at last it is working (for now)
	FXWindow *walker = this;
	while( walker ){
		if( walker->getAccelTable()
		&& walker->getAccelTable()->handle( walker, FXSEL( SEL_KEYPRESS, 0 ), pdata ) ){
			break;
		}
		walker = walker->getParent();
	}
	
	// skip the entire focus chain handling
	
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onKeyRelease( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 0; // default handling
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyKeyRelease( igdeUIFoxHelper::KeyCodeFromEvent( event ), event.code );
	
	
	// check accelerators. we have to walk up the entire widget tree to pick up all
	// accelerators. I do **not** know if this is the correct way to fix this mess but
	// at last it is working (for now)
	FXWindow *walker = this;
	while( walker ){
		if( walker->getAccelTable()
		&& walker->getAccelTable()->handle( walker, FXSEL( SEL_KEYRELEASE, 0 ), pdata ) ){
			break;
		}
		walker = walker->getParent();
	}
	
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onLeftMouseDown( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	setFocus();
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonPress( deInputEvent::embcLeft, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	
	switch( event.click_count ){
	case 1:
		handle( this, FXSEL( SEL_CLICKED, 0 ), pdata );
		break;
		
	case 2:
		handle( this, FXSEL( SEL_DOUBLECLICKED, 0 ), pdata );
		break;
		
	case 3:
		handle( this, FXSEL( SEL_TRIPLECLICKED, 0 ), pdata );
		break;
		
	default:
		break;
	}
	
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onLeftMouseUp( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonRelease( deInputEvent::embcLeft, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onRightMouseDown( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner ){
		return 1; // prevent further processing
	}
	
	setFocus();
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonPress( deInputEvent::embcRight, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onRightMouseUp( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonRelease( deInputEvent::embcRight, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onMiddleMouseDown( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	setFocus();
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonPress( deInputEvent::embcMiddle, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1;
} // prevent further processing

long igdeNativeFoxRenderView::onMiddleMouseUp( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyButtonRelease( deInputEvent::embcMiddle, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onMouseMove( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyMouseMoved( decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1;
}

long igdeNativeFoxRenderView::onMouseWheel( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyMouseWheeled( decPoint( event.win_x, event.win_y ), decPoint( 0, event.code ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1;
}

long igdeNativeFoxRenderView::onDoubleClicked( FXObject*, FXSelector, void *pdata ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	setFocus();
	const FXEvent &event = *( ( FXEvent* )pdata );
	pOwner->NotifyDoubleClicked( deInputEvent::embcLeft, decPoint( event.win_x, event.win_y ),
		igdeUIFoxHelper::ModifiersFromEvent( event ) );
	return 1; // prevent further processing
}

long igdeNativeFoxRenderView::onMouseEnter( FXObject*, FXSelector, void* ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	pOwner->NotifyMouseEnter();
	return 1;
}

long igdeNativeFoxRenderView::onMouseLeave( FXObject *, FXSelector, void* ){
	if( ! pOwner || ! isEnabled() ){
		return 1; // prevent further processing
	}
	
	pOwner->NotifyMouseLeave();
	return 1;
}

long igdeNativeFoxRenderView::onTimeoutRetryMap( FXObject*, FXSelector, void* ){
	pProcessMap();
	return 0;
}



// Private Functions
//////////////////////

void igdeNativeFoxRenderView::pProcessMap(){
	if( ! pOwner ){
		return;
	}
	
	if( ! id() ){
		// FOX bug. FXFrame calls CreateWindowEx which in turn sends WM_SHOWWINDOW which
		// in turn triggers a SEL_MAP which then has id()==0 because FOX assigns the FXID
		// after CreateWindowEx returns. work around use is to try again
		getApp()->addTimeout( this, ID_TIMEOUT_RETRY_MAP, 0, NULL );
		return;
	}
	
	pCanAttachRenderWindow = true;
	pOwner->CreateAndAttachRenderWindow();
	
	pSyncSizes();
	pOwner->OnResize(); // avoid problems if there is bad timing
}

void igdeNativeFoxRenderView::pSyncSizes(){
	if( ! pOwner ){
		return;
	}
	
	deRenderWindow * const renderWindow = pOwner->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	const int wwidth = decMath::max( getWidth(), 0 );
	const int hheight = decMath::max( getHeight(), 0 );
	
	renderWindow->SetSize( wwidth, hheight );
	
	deCanvasRenderWorld * const canvasRenderWorld = pOwner->GetCanvasRenderWorld();
	if( canvasRenderWorld ){
		canvasRenderWorld->SetSize( decPoint( wwidth, hheight ) );
	}
	
	deCanvasPaint * const canvasBackground = pOwner->GetCanvasBackground();
	if( canvasBackground ){
		canvasBackground->SetSize( decPoint( wwidth, hheight ) );
	}
}

#endif
