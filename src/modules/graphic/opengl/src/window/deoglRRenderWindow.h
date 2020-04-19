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

#ifndef _DEOGLRRENDERWINDOW_H_
#define _DEOGLRRENDERWINDOW_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#ifdef OS_W32
#include "../include_windows.h"
#endif

#ifdef OS_BEOS
#include <DirectWindow.h>
#include <GLView.h>
#endif

#ifdef OS_MACOS
#ifdef __OBJC__
@class NSWindow;
@class NSView;
#else
class NSWindow;
class NSView;
#endif
#endif

#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

class deoglRenderThread;
class deoglRCanvasView;
class deoglPixelBuffer;


/**
 * \brief Render window target.
 */
class deoglRRenderWindow : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! OS_MACOS
	Window pHostWindow;
	Window pWindow;
	Cursor pNullCursor;
	#endif
	
	#ifdef OS_BEOS
	class cGLView;
	class cDirectWindow;
	
	class cGLView : public BGLView{
	private:
		cDirectWindow &pWindow;
	public:
		cGLView( cDirectWindow &widow, const BRect &frame );
		virtual void FrameResized( float width, float height );
		virtual void KeyDown( const char *bytes, int32 numBytes );
		virtual void KeyUp( const char *bytes, int32 numBytes );
		virtual void MouseDown( BPoint point );
		virtual void MouseMoved( BPoint point, uint32 transit, const BMessage *message );
		virtual void MouseUp( BPoint point );
	};
	
	class cDirectWindow : public BDirectWindow{
	private:
		deoglRRenderWindow &pWindow;
		cGLView *pGLView;
	public:
		cDirectWindow( deoglRRenderWindow &window );
		inline cGLView *GetGLView() const{ return pGLView; }
		void SendCurMessageToEngine();
		virtual void DirectConnected( direct_buffer_info *info );
		virtual void WindowActivated( bool active );
		virtual void MessageReceived( BMessage *message );
	};
	
	BWindow *pHostWindow;
	cDirectWindow *pWindow;
	#endif
	
	#ifdef OS_W32
	HWND pHostWindow;
	HWND pWindow;
	HDC pWindowDC;
	HICON pWindowIcon;
	#endif
	
	#ifdef OS_MACOS
	NSWindow *pHostWindow;
	NSWindow *pWindow;
	NSView *pView;
	#endif
	
	int pWidth;
	int pHeight;
	decString pTitle;
	bool pFullScreen;
	bool pPaint;
	deoglPixelBuffer *pIcon;
	
	deoglRCanvasView *pRCanvasView;
	
	bool pSwapBuffers;
	bool pNotifySizeChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render render target. */
	deoglRRenderWindow( deoglRenderThread &renderThread );
	
	/** \brief Clean up render render target. */
	virtual ~deoglRRenderWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief OS specific. */
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	inline Window GetHostWindow() const{ return pHostWindow; }
	inline Window GetWindow() const{ return pWindow; }
	void SetHostWindow( Window window );
	#endif
	
	#ifdef ANDROID
	inline void *GetHostWindow() const{ return 0; }
	inline void *GetWindow() const{ return 0; }
	void SetHostWindow( void *window );
	#endif
	
	#ifdef OS_BEOS
	inline BWindow *GetHostWindow() const{ return pHostWindow; }
	inline BDirectWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow( BWindow *window );
	BGLView *GetGLView() const;
	#endif
	
	#ifdef OS_W32
	inline HWND GetHostWindow() const{ return pHostWindow; }
	inline HWND GetWindow() const{ return pWindow; }
	inline HDC GetWindowDC() const{ return pWindowDC; }
	void SetHostWindow( HWND window );
	#endif
	
	#ifdef OS_MACOS
	inline NSWindow *GetHostWindow() const{ return pHostWindow; }
	inline NSWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow( NSWindow *window );
	inline NSView *GetView() const{ return pView; }
	#endif
	
	/** \brief Width of the window. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of the window. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set window size. */
	void SetSize( int width, int height );
	
	/** \brief Window title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set window title. */
	void SetTitle( const char *title );
	
	/** \brief Full screen mode enabled. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if full screen mode is enabled. */
	void SetFullScreen( bool fullScreen );
	
	/** \brief Painting enabled. */
	inline bool GetPaint() const{ return pPaint; }
	
	/** \brief Set painting enabled. */
	void SetPaint( bool paint );
	
	/** \brief Icon. */
	inline deoglPixelBuffer *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon. */
	void SetIcon( deoglPixelBuffer *icon );
	
	
	
	/** \brief Render canvas view or \em NULL if not set. */
	inline deoglRCanvasView *GetRCanvasView() const{ return pRCanvasView; }
	
	/** \brief Set render canvas view or \em NULL if not set. */
	void SetRCanvasView( deoglRCanvasView *rcanvasView );
	
	/** \brief Drop render canvas view if not \em NULL. */
	void DropRCanvasView();
	
	
	
	/** \brief Create render window. */
	void CreateWindow();
	
	/** \brief Swap buffers. Called from render thread. */
	void SwapBuffers();
	
	/** \brief Render window if visible. Called from render thread. */
	void Render();
	
	/** \brief Capture content by capture canvas if any matching one is pending. */
	void Capture();
	
	/** \brief Center window on screen. */
	void CenterOnScreen();
	
	/** \brief Windows resized (render thread event loop). */
	void OnResize( int width, int height );
	
	
	
	/**
	 * \brief Notify scripting module size changed due to OS event.
	 * \note Clears flag before returning if set.
	 */
	bool GetNotifySizeChanged();
	/*@}*/
	
	
	
private:
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	void pCreateNullCursor();
	#endif
	
	void pDestroyWindow();
	
	void pResizeWindow();
	void pSetWindowTitle();
	void pUpdateFullScreen();
	void pSetIcon();
	
	#ifdef OS_MACOS
	void pMacOSCreateWindow();
	void pMacOSDestroyWindow();
	void pMacOSSwapBuffers();
	void pMacOSCenterOnScreen();
	void pMacOSResizeWindow();
	void pMacOSSetWindowTitle();
	void pMacOSUpdateFullscreen();
public:
	void pMacOSDelegateWindowClosed();
	void pMacOSDelegateWindowActivated();
	void pMacOSDelegateWindowDeactivated();
	void pMacOSDelegateWindowResized();
	#endif
};

#endif
