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

#ifndef _DERENDERWINDOW_H_
#define _DERENDERWINDOW_H_

#include "../../dragengine_configuration.h"

#include "../deResource.h"
#include "../image/deImageReference.h"
#include "../canvas/deCanvasViewReference.h"
#include "../../common/string/decString.h"

#if defined OS_UNIX && defined HAS_LIB_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#ifdef OS_W32
#include "../../app/include_windows.h"
#endif

#ifdef OS_BEOS
class BWindow;
#endif

#ifdef OS_MACOS
#ifdef __OBJC__
@class NSWindow;
#else
class NSWindow;
#endif
#endif

class deBaseGraphicRenderWindow;
class deRenderWindowManager;


/**
 * \brief Render window.
 *
 * Render windows are used by hosting applications to redirect the rendered
 * output of an embedded engine session into one or more sub windows. The
 * graphic module decides how the window is used for rendering. The window
 * is peered so changing the graphic module is not a problem. The only actions
 * required by the hosting application is to notify the render window about
 * size changes so the graphic module can take the necessary steps.
 *
 * \par Hosting Window Handle
 * 
 * For working with the hosting window handle you can use two different ways.
 * You can either specify the hosting window using a generic function which
 * mangles the window handle into an integer value or use an OS specific
 * function. The generic function has the advantage that you do not have to
 * worry about the operating system your application is running on. Most GUI
 * toolkits provide a generic access to the window handles so you can simply
 * carry over the value. The OS specific functions are convenient if you work
 * with only one OS or you need specific code anyways.
 */
class DE_DLL_EXPORT deRenderWindow : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deRenderWindow> Ref;
	
	
	
private:
#ifdef OS_ANDROID
	void *pHostWindow;
	void *pWindow;
#endif
#ifdef OS_W32
	HWND pHostWindow;
	HWND pWindow;
#endif
#if defined OS_UNIX && defined HAS_LIB_X11
	Window pHostWindow;
	Window pWindow;
#endif
#ifdef OS_BEOS
	BWindow *pHostWindow;
	BWindow *pWindow;
#endif
#ifdef OS_MACOS
	NSWindow *pHostWindow;
	NSWindow *pWindow;
#endif
	decString pTitle;
	int pWidth;
	int pHeight;
	bool pFullScreen;
	bool pNotifyScriptOnResize;
	bool pPaint;
	deImageReference pIcon;
	
	deCanvasViewReference pCanvasView;
	
	deBaseGraphicRenderWindow *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render window. */
	deRenderWindow( deRenderWindowManager *manager );
	
protected:
	/**
	 * \brief Clean up render window.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deRenderWindow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Hosting render window or NULL if not set. */
	#ifdef OS_ANDROID
	inline void *GetHostWindow() const{ return pHostWindow; }
	#endif
	#ifdef OS_W32
	inline HWND GetHostWindow() const{ return pHostWindow; }
	#endif
	#ifdef OS_BEOS
	inline BWindow *GetHostWindow() const{ return pHostWindow; }
	#endif
	#ifdef OS_MACOS
	inline NSWindow *GetHostWindow() const{ return pHostWindow; }
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	inline Window GetHostWindow() const{ return pHostWindow; }
	#endif
	
	/**
	 * \brief Set hosting render window or NULL to let the graphic system create a window.
	 * 
	 * This has to be set before creating the graphic module peer.
	 * For this reason for use only by deRenderWindowManager.
	 */
	#ifdef OS_ANDROID
	void SetHostWindow( void *window );
	#endif
	#ifdef OS_W32
	void SetHostWindow( HWND window );
	#endif
	#ifdef OS_BEOS
	void SetHostWindow( BWindow *window );
	#endif
	#ifdef OS_MACOS
	void SetHostWindow( NSWindow *window );
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	void SetHostWindow( Window window );
	#endif
	
	/** \brief Render window. */
	#ifdef OS_ANDROID
	inline void *GetWindow() const{ return pWindow; }
	#endif
	#ifdef OS_W32
	inline HWND GetWindow() const{ return pWindow; }
	#endif
	#ifdef OS_BEOS
	inline BWindow *GetWindow() const{ return pWindow; }
	#endif
	#ifdef OS_MACOS
	inline NSWindow *GetWindow() const{ return pWindow; }
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	inline Window GetWindow() const{ return pWindow; }
	#endif
	
	/**
	 * \brief Set render window.
	 * 
	 * For use by Graphic Module only.
	 */
	#ifdef OS_ANDROID
	void SetWindow( void *window );
	#endif
	#ifdef OS_W32
	void SetWindow( HWND window );
	#endif
	#ifdef OS_BEOS
	void SetWindow( BWindow *window );
	#endif
	#ifdef OS_MACOS
	void SetWindow( NSWindow *window );
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	void SetWindow( Window window );
	#endif
	
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/**
	 * \brief Set size in pixels.
	 * \throws deeInvalidParam \em width is less than 1.
	 * \throws deeInvalidParam \em height is less than 1.
	 */
	void SetSize( int width, int height );
	
	/** \brief Window is full screen. */
	inline bool GetFullScreen() const{ return pFullScreen; }
	
	/** \brief Set if the window is full screen. */
	void SetFullScreen( bool fullscreen );
	
	/** \brief Script is notified about the window changing size. */
	inline bool GetNotifyScriptOnResize() const{ return pNotifyScriptOnResize; }
	
	/** \brief Set if the script is notified about the window chaning size. */
	void SetNotifyScriptOnResize( bool notify );
	
	/**
	 * \brief Painting enabled.
	 * 
	 * Render windows can be hidden by other windows in a GUI toolkit or otherwise
	 * set invisible due to properties outside of the knowledge of the Graphic
	 * Module. Disabling painting avoids the Graphic Module wasting time rendering
	 * render window content if it is not visible.
	 */
	inline bool GetPaint() const{ return pPaint; }
	
	/**
	 * \brief Set if painting is enabled.
	 * 
	 * Render windows can be hidden by other windows in a GUI toolkit or otherwise
	 * set invisible due to properties outside of the knowledge of the Graphic
	 * Module. Disabling painting avoids the Graphic Module wasting time rendering
	 * render window content if it is not visible.
	 */
	void SetPaint( bool paint );
	
	/** \brief Render window title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set render window title. */
	void SetTitle( const char *title );
	
	/** \brief Icon. */
	inline deImage *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon. */
	void SetIcon( deImage *icon );
	
	/**
	 * \brief Canvas defining the content to render.
	 * 
	 * The canvas view is automatically resized to fill the entire window.
	 */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer or NULL if not set. */
	inline deBaseGraphicRenderWindow *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer or NULL if not set. */
	void SetPeerGraphic( deBaseGraphicRenderWindow *peer );
	/*@}*/
	
};

#endif
