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

#ifndef _DEOGLRRENDERWINDOW_H_
#define _DEOGLRRENDERWINDOW_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../target/deoglRenderTarget.h"
#include "../configuration/deoglConfiguration.h"

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


/**
 * Render window target.
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
	class cGLWindow;
	
	class cGLView : public BGLView{
	private:
		cGLWindow &pWindow;
	public:
		cGLView( cGLWindow &widow, const BRect &frame );
		virtual void KeyDown( const char *bytes, int32 numBytes );
		virtual void KeyUp( const char *bytes, int32 numBytes );
		virtual void MouseDown( BPoint point );
		virtual void MouseMoved( BPoint point, uint32 transit, const BMessage *message );
		virtual void MouseUp( BPoint point );
	};
	
	class cGLWindow : public BDirectWindow{
	private:
		deoglRRenderWindow &pWindow;
		cGLView *pGLView;
		BCursor *pCursor;
		bool pBlockQuitRequested;
	public:
		cGLWindow( deoglRRenderWindow &window );
		virtual ~cGLWindow();
		inline deoglRRenderWindow &GetWindow() const{ return pWindow; }
		inline cGLView *GetGLView() const{ return pGLView; }
		void SendCurMessageToEngine();
		virtual void DirectConnected( direct_buffer_info *info );
		virtual void WindowActivated( bool active );
		virtual void MessageReceived( BMessage *message );
		virtual	void FrameResized( float newWidth, float newHeight );
		virtual	bool QuitRequested();
		void SetBlockQuitRequested( bool blockQuitRequested );
	};
	
	BWindow *pHostWindow;
	cGLWindow *pWindow;
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
	deoglPixelBuffer::Ref pIcon;
	
	deoglRCanvasView *pRCanvasView;
	
	bool pSwapBuffers;
	bool pNotifySizeChanged;
	
	deoglConfiguration::eVSyncMode pVSyncMode;
	bool pInitSwapInterval;
	
	deoglRenderTarget::Ref pRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render render target. */
	deoglRRenderWindow( deoglRenderThread &renderThread );
	
	/** Clean up render render target. */
	virtual ~deoglRRenderWindow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** OS specific. */
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
	inline cGLWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow( BWindow *window );
	BGLView *GetGLView() const;
	#endif
	
	#ifdef OS_W32
	inline HWND GetHostWindow() const{ return pHostWindow; }
	inline HWND GetWindow() const{ return pWindow; }
	inline HDC GetWindowDC() const{ return pWindowDC; }
	void SetHostWindow( HWND window );
	decPoint GetInnerSize() const;
	#endif
	
	#ifdef OS_MACOS
	inline NSWindow *GetHostWindow() const{ return pHostWindow; }
	inline NSWindow *GetWindow() const{ return pWindow; }
	void SetHostWindow( NSWindow *window );
	inline NSView *GetView() const{ return pView; }
	#endif
	
	/** Width of the window. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of the window. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Set window size. */
	void SetSize( int width, int height );
	
	/** Window title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** Set window title. */
	void SetTitle( const char *title );
	
	/** Full screen mode enabled. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** Set if full screen mode is enabled. */
	void SetFullScreen( bool fullScreen );
	
	/** Painting enabled. */
	inline bool GetPaint() const{ return pPaint; }
	
	/** Set painting enabled. */
	void SetPaint( bool paint );
	
	/** Icon. */
	inline const deoglPixelBuffer::Ref &GetIcon() const{ return pIcon; }
	
	/** Set icon. */
	void SetIcon( deoglPixelBuffer *icon );
	
	
	
	/** Render canvas view or \em NULL if not set. */
	inline deoglRCanvasView *GetRCanvasView() const{ return pRCanvasView; }
	
	/** Set render canvas view or \em NULL if not set. */
	void SetRCanvasView( deoglRCanvasView *rcanvasView );
	
	/** Drop render canvas view if not \em NULL. */
	void DropRCanvasView();
	
	
	
	/** Create render window. */
	void CreateWindow();
	
	/** Swap buffers. Called from render thread. */
	void SwapBuffers();
	
	/** Render window if visible. Called from render thread. */
	void Render();
	
	/** Capture content by capture canvas if any matching one is pending. */
	void Capture();
	
	/** Center window on screen. */
	void CenterOnScreen();
	
	/** Windows resized (render thread event loop). */
	void OnResize( int width, int height );
	
	
	
	/**
	 * Notify scripting module size changed due to OS event.
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
	
	void pUpdateVSync();
};

#endif
