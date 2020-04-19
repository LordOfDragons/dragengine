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

#ifndef _DEOGLRRTCONTEXT_H_
#define _DEOGLRRTCONTEXT_H_

#include "../deoglBasics.h"

#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
#include <GL/glx.h>
#include <GL/glxext.h>
class deOSUnix;
#endif

#ifdef ANDROID
class deOSAndroid;
#endif

#ifdef OS_BEOS
//#include <GL/glx.h> ???
//#include <GL/glext.h> ???
class deOSBeOS;
#endif

#ifdef OS_MACOS
class deOSMacOS;
#ifdef __OBJC__
@class NSOpenGLPixelFormat;
@class NSOpenGLContext;
@class NSView;
#else
class NSOpenGLPixelFormat;
class NSOpenGLContext;
class NSView;
#endif
#endif

#ifdef OS_W32
#include <dragengine/common/string/decString.h>
#include "../include_windows.h"
class deOSWindows;
#endif

// HACK HACK HACK
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
#include <dragengine/app/deOSUnix.h>
#endif

#ifdef ANDROID
#include <dragengine/app/deOSAndroid.h>
#endif

#ifdef OS_BEOS
#include <dragengine/app/deOSBeOS.h>
#endif

#ifdef OS_MACOS
#include <dragengine/app/deOSMacOS.h>
#endif

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif
// HACK HACK HACK

class deRenderWindow;
class deoglRRenderWindow;



/**
 * \brief Render thread context.
 * 
 * Operating system specific objects and context.
 */
class deoglRTContext{
private:
	deoglRenderThread &pRenderThread;
	
	
	
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	deOSUnix *pOSUnix;
	
	Display *pDisplay;
	int pScreen;
	
	GLXContext pContext;
	
	Colormap pColMap;
	XVisualInfo *pVisInfo;
	GLXFBConfig pBestFBConfig;
	
	Atom pAtomProtocols;
	Atom pAtomDeleteWindow;
#endif
	
#ifdef ANDROID
	deOSAndroid *pOSAndroid;
	
	EGLDisplay pDisplay;
	EGLSurface pSurface;
	EGLContext pContext;
	EGLConfig pConfig;
	
	int pScreenWidth;
	int pScreenHeight;
	bool pScreenResized;
#endif
	
#ifdef OS_BEOS
	deOSBeOS *pOSBeOS;
	// GLXContet pContent; ???
#endif
	
#ifdef OS_MACOS
	deOSMacOS *pOSMacOS;
	NSOpenGLPixelFormat *pPixelFormat;
	NSOpenGLContext *pContext;
#endif

#ifdef OS_W32
	decString pWindowClassname;
	deOSWindows *pOSWindows;
	HGLRC pContext;
#endif
	
	deoglRRenderWindow *pActiveRRenderWindow;
	bool pUserRequestedQuit;
	bool pAppActivated;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread context. */
	deoglRTContext( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread context. */
	~deoglRTContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize phase 1 called in main thread. */
	void InitPhase1( deRenderWindow *renderWindow );
	
	/** \brief Initialize phase 2 called in render thread. */
	void InitPhase2( deRenderWindow *renderWindow );
	
	/** \brief Initialize phase 3 called in main thread. */
	void InitPhase3( deRenderWindow *renderWindow );
	
	/** \brief Initialize phase 4 called in render thread. */
	void InitPhase4( deRenderWindow *renderWindow );
	
	/** \brief Clean up. Separate call to make sure everything is fine before deleting. */
	void CleanUp();
	
	
	
	/** \brief Active render window. */
	inline deoglRRenderWindow *GetActiveRRenderWindow() const{ return pActiveRRenderWindow; }
	
	/** \brief Active render render window current. */
	void ActivateRRenderWindow( deoglRRenderWindow *rrenderWindow, bool forceIfNull = false );
	
	/**
	 * \brief User requests quit.
	 * 
	 * Value is cleared after queried so this can be called only once.
	 */
	bool GetUserRequestedQuit();
	
	/** \brief Application is activated. */
	inline bool GetAppActivated() const{ return pAppActivated; }
	
	/** \brief Special call for module to get a function pointer before extensions can be properly initialized. */
	void *GetFunctionPointer( const char *funcName );
	
	

#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	/** \brief OS Unix. */
	inline deOSUnix *GetOSUnix(){ return pOSUnix; }
	
	/** \brief Unix visual. */
	inline XVisualInfo *GetVisualInfo() const{ return pVisInfo; }
	
	/** \brief Render thread display. */
	inline Display *GetDisplay() const{ return pDisplay; }
	
	/** \brief Main thread display. */
	Display *GetMainThreadDisplay() const;
	
	/** \brief Unix best framebuffer configuration. */
	inline GLXFBConfig &GetBestFBConfig(){ return pBestFBConfig; }
	
	/** \brief Context. */
	inline GLXContext GetContext() const{ return pContext; }
	
	/** \brief Atoms. */
	inline Atom GetAtomProtocols() const{ return pAtomProtocols; }
	inline Atom GetAtomDeleteWindow() const{ return pAtomDeleteWindow; }
	
	/** \brief Process event loop. */
	void ProcessEventLoop();
#endif

#ifdef ANDROID
	/** \brief OS Android. */
	inline deOSAndroid *GetOSAndroid(){ return pOSAndroid; }
	
	/** \brief Display. */
	inline EGLDisplay GetDisplay() const{ return pDisplay; }
	
	/** \brief Surface. */
	inline EGLSurface GetSurface() const{ return pSurface; }
	
	/** \brief Context. */
	inline EGLContext GetContext() const{ return pContext; }
	
	/** \brief Configuration. */
	inline const EGLConfig& GetConfig() const{ return pConfig; }
	
	/** \brief Application window has been created. */
	void InitAppWindow();
	
	/** \brief Application window has been closed. */
	void TerminateAppWindow();
	
	/** \brief Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** \brief Screen size changed. */
	inline bool GetScreenResized() const{ return pScreenResized; }
	
	/** \brief Clear screen size changed. */
	void ClearScreenResized();
	
	/** \brief Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** \brief Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
#endif
	
#ifdef OS_BEOS
	/** \brief OS BeOS. */
	inline deOSBeOS *GetOSBeOS() const{ return pOSBeOS; }
	
	/** \brief Context. */
//	inline GLXContext getContext() const{ return pContext; }   ???
#endif
	
#ifdef OS_MACOS
	/** \brief OS MacOS. */
	inline deOSMacOS *GetOSMacOS() const{ return pOSMacOS; }
	
	/** \brief Pixel format. */
	inline NSOpenGLPixelFormat *GetPixelFormat() const{ return pPixelFormat; }
	
	/** \brief Context. */
	inline NSOpenGLContext *GetContext() const{ return pContext; }
#endif

#ifdef OS_W32
	/** \brief Registered window class name. */
	inline const decString &GetWindowClassname() const{ return pWindowClassname; }
	
	/** \brief OS Windows. */
	inline deOSWindows *GetOSWindow() const{ return pOSWindows; }
	
	/** \brief Context. */
	inline HGLRC GetContext() const{ return pContext; }
	
	LRESULT ProcessWindowMessage( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
#endif
	/*@}*/
	
	
	
private:
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	void pOpenDisplay();
	void pChooseVisual();
	void pPrintVisualInfo();
	void pChooseFBConfig();
	void pCreateColorMap();
	void pCreateAtoms();
	void pCreateGLContext();
	void pFreeContext();
	void pFreeVisualInfo();
	void pCloseDisplay();
	#endif
	
	#ifdef ANDROID
	void pInitDisplay();
	void pCloseDisplay();
	#endif
	
	#ifdef OS_BEOS
	void pCreateGLContext();
	void pFreeContext();
	#endif
	
	#ifdef OS_W32
	void pRegisterWindowClass();
	void pCreateGLContext();
	void pUnregisterWindowClass();
	void pFreeContext();
	#endif
	
	#ifdef OS_MACOS
	void pCreateGLContext();
	void pGLContextMakeCurrent( NSView *view );
	void pFreeContext();
	#endif
};

#endif
