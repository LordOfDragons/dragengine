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
#include "../extensions/glxext.h"
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
 * Render thread context.
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
	GLXContext pLoaderContext;
	
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
	EGLContext pLoaderContext;
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
	NSOpenGLContext *pLoaderContext;
#endif

#ifdef OS_W32
	decString pWindowClassname;
	deOSWindows *pOSWindows;
	HGLRC pContext;
	HGLRC pLoaderContext;
#endif
	
	deoglRRenderWindow *pActiveRRenderWindow;
	bool pUserRequestedQuit;
	bool pAppActivated;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread context. */
	deoglRTContext( deoglRenderThread &renderThread );
	
	/** Clean up render thread context. */
	~deoglRTContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 1 called in main thread. */
	void InitPhase1( deRenderWindow *renderWindow );
	
	/** Initialize phase 2 called in render thread. */
	void InitPhase2( deRenderWindow *renderWindow );
	
	/** Initialize phase 3 called in main thread. */
	void InitPhase3( deRenderWindow *renderWindow );
	
	/** Initialize phase 4 called in render thread. */
	void InitPhase4( deRenderWindow *renderWindow );
	
	/** Clean up. Separate call to make sure everything is fine before deleting. */
	void CleanUp();
	
	
	
	/** Active render window. */
	inline deoglRRenderWindow *GetActiveRRenderWindow() const{ return pActiveRRenderWindow; }
	
	/** Active render render window current. */
	void ActivateRRenderWindow( deoglRRenderWindow *rrenderWindow, bool forceIfNull = false );
	
	/**
	 * User requests quit.
	 * 
	 * Value is cleared after queried so this can be called only once.
	 */
	bool GetUserRequestedQuit();
	
	/** Application is activated. */
	inline bool GetAppActivated() const{ return pAppActivated; }
	
	/** Special call for module to get a function pointer before extensions can be properly initialized. */
	void *GetFunctionPointer( const char *funcName );
	
	

#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	/** OS Unix. */
	inline deOSUnix *GetOSUnix(){ return pOSUnix; }
	
	/** Unix visual. */
	inline XVisualInfo *GetVisualInfo() const{ return pVisInfo; }
	
	/** Render thread display. */
	inline Display *GetDisplay() const{ return pDisplay; }
	
	/** Main thread display. */
	Display *GetMainThreadDisplay() const;
	
	/** Unix best framebuffer configuration. */
	inline GLXFBConfig &GetBestFBConfig(){ return pBestFBConfig; }
	inline const GLXFBConfig &GetBestFBConfig() const{ return pBestFBConfig; }
	
	/** Context. */
	inline GLXContext GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline GLXContext GetLoaderContext() const{ return pLoaderContext; }
	
	/** Atoms. */
	inline Atom GetAtomProtocols() const{ return pAtomProtocols; }
	inline Atom GetAtomDeleteWindow() const{ return pAtomDeleteWindow; }
	
	/** Process event loop. */
	void ProcessEventLoop();
#endif

#ifdef ANDROID
	/** OS Android. */
	inline deOSAndroid *GetOSAndroid(){ return pOSAndroid; }
	
	/** Display. */
	inline EGLDisplay GetDisplay() const{ return pDisplay; }
	
	/** Surface. */
	inline EGLSurface GetSurface() const{ return pSurface; }
	
	/** Context. */
	inline EGLContext GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline EGLContext GetLoaderContext() const{ return pLoaderContext; }
	
	/** Configuration. */
	inline const EGLConfig& GetConfig() const{ return pConfig; }
	
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	
	/** Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** Screen size changed. */
	inline bool GetScreenResized() const{ return pScreenResized; }
	
	/** Clear screen size changed. */
	void ClearScreenResized();
	
	/** Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
#endif
	
#ifdef OS_BEOS
	/** OS BeOS. */
	inline deOSBeOS *GetOSBeOS() const{ return pOSBeOS; }
	
	/** Context. */
//	inline GLXContext getContext() const{ return pContext; }   ???
#endif
	
#ifdef OS_MACOS
	/** OS MacOS. */
	inline deOSMacOS *GetOSMacOS() const{ return pOSMacOS; }
	
	/** Pixel format. */
	inline NSOpenGLPixelFormat *GetPixelFormat() const{ return pPixelFormat; }
	
	/** Context. */
	inline NSOpenGLContext *GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline NSOpenGLContext *GetLoaderContext() const{ return pLoaderContext; }
#endif

#ifdef OS_W32
	/** Registered window class name. */
	inline const decString &GetWindowClassname() const{ return pWindowClassname; }
	
	/** OS Windows. */
	inline deOSWindows *GetOSWindow() const{ return pOSWindows; }
	
	/** Context. */
	inline HGLRC GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline HGLRC GetLoaderContext() const{ return pLoaderContext; }
	
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
