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

#ifndef _DEOGLRRTCONTEXT_H_
#define _DEOGLRRTCONTEXT_H_

#include "../deoglBasics.h"

#ifdef OS_UNIX_X11
#ifdef BACKEND_OPENGL
	#include <GL/glx.h>
	#include "../extensions/glxext.h"
#elif defined BACKEND_VULKAN
	#include <deSharedVulkan.h>
	#include <devkDevice.h>
	#include <queue/devkCommandPool.h>
	#include <queue/devkQueue.h>
#endif
class deOSUnix;

#elif defined OS_ANDROID
class deOSAndroid;

#elif defined OS_WEBWASM
class deOSWebWasm;

#elif defined OS_BEOS
//#include <GL/glx.h> ???
//#include <GL/glext.h> ???
class deOSBeOS;

#elif defined OS_MACOS
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

#elif defined OS_W32
#include <dragengine/common/string/decString.h>
#include "../include_windows.h"
class deOSWindows;
#endif

#ifdef OS_UNIX_X11
#include <dragengine/app/deOSUnix.h>

#elif defined OS_ANDROID
#include <dragengine/app/deOSAndroid.h>

#elif defined OS_WEBWASM
#include <dragengine/app/deOSWebWasm.h>
#include <emscripten/html5.h>

#elif defined OS_BEOS
#include <dragengine/app/deOSBeOS.h>

#elif defined OS_MACOS
#include <dragengine/app/deOSMacOS.h>

#elif defined OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

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
	
	static const int MaxCompileContextCount = 8;
	int pCompileContextCount;
	
	
#ifdef OS_UNIX_X11
	deOSUnix *pOSUnix;
	
	Display *pDisplay;
	int pScreen;
	
#ifdef BACKEND_OPENGL
	GLXContext pContext;
	GLXContext pLoaderContext;
	GLXContext pCompileContext[MaxCompileContextCount];
#elif defined BACKEND_VULKAN
	deSharedVulkan::Ref pVulkan;
	devkDevice::Ref pDevice;
	devkQueue *pQueueGraphic;
	devkQueue *pQueueCompute;
	devkQueue *pQueueTransfer;
	devkCommandPool::Ref pCommandPoolGraphic;
	devkCommandPool::Ref pCommandPoolCompute;
	devkCommandPool::Ref pCommandPoolTransfer;
#endif
	
	Colormap pColMap;
	XVisualInfo *pVisInfo;
#ifdef BACKEND_OPENGL
	GLXFBConfig pBestFBConfig;
#endif
	
	Atom pAtomProtocols;
	Atom pAtomDeleteWindow;
	
#elif defined OS_ANDROID
	deOSAndroid *pOSAndroid;
	
	EGLDisplay pDisplay;
	EGLSurface pSurface;
	EGLContext pContext;
	EGLSurface pLoaderSurface;
	EGLContext pLoaderContext;
	EGLSurface pCompileSurface[MaxCompileContextCount];
	EGLContext pCompileContext[MaxCompileContextCount];
	EGLConfig pConfig;
	
	int pScreenWidth;
	int pScreenHeight;
	
#elif defined OS_WEBWASM
	deOSWebWasm *pOSWebWasm;
	
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pContext;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pLoaderContext;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pCompileContext[MaxCompileContextCount];
	
	int pScreenWidth;
	int pScreenHeight;
	
#elif defined OS_BEOS
	deOSBeOS *pOSBeOS;
	// GLXContet pContent; ???
	
#elif defined OS_MACOS
	deOSMacOS *pOSMacOS;
	NSOpenGLPixelFormat *pPixelFormat;
	NSOpenGLContext *pContext;
	NSOpenGLContext *pLoaderContext;
	NSOpenGLContext *pCompileContext[MaxCompileContextCount];
	
#elif defined OS_W32
	decString pWindowClassname;
	deOSWindows *pOSWindows;
	HGLRC pContext;
	HGLRC pLoaderContext;
	HGLRC pCompileContext[MaxCompileContextCount];
#endif
	
	deoglRRenderWindow *pActiveRRenderWindow;
	bool pUserRequestedQuit;
	bool pAppActivated;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread context. */
	deoglRTContext(deoglRenderThread &renderThread);
	
	/** Clean up render thread context. */
	~deoglRTContext();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 1 called in main thread. */
	void InitPhase1(deRenderWindow *renderWindow);
	
	/** Initialize phase 2 called in render thread. */
	void InitPhase2(deRenderWindow *renderWindow);
	
	/** Initialize phase 3 called in main thread. */
	void InitPhase3(deRenderWindow *renderWindow);
	
	/** Initialize phase 4 called in render thread. */
	void InitPhase4(deRenderWindow *renderWindow);
	
	/** Clean up. Separate call to make sure everything is fine before deleting. */
	void CleanUp();
	
	
	
	/** Active render window. */
	inline deoglRRenderWindow *GetActiveRRenderWindow() const{ return pActiveRRenderWindow; }
	
	/** Active render render window current. */
	void ActivateRRenderWindow(deoglRRenderWindow *rrenderWindow, bool forceIfNull = false);
	
	/**
	 * User requests quit.
	 * 
	 * Value is cleared after queried so this can be called only once.
	 */
	bool GetUserRequestedQuit();
	
	/** Application is activated. */
	inline bool GetAppActivated() const{ return pAppActivated; }
	
#ifdef BACKEND_OPENGL
	/** Special call for module to get a function pointer before extensions can be properly initialized. */
	void *GetFunctionPointer(const char *funcName);
#endif
	
	/** Count of compile contexts. */
	inline int GetCompileContextCount() const{ return pCompileContextCount; }
	
	/** Release all compile contexts above count. */
	void DropCompileContexts(int count);
	
	
	
#ifdef OS_UNIX_X11
	/** OS Unix. */
	inline deOSUnix *GetOSUnix(){ return pOSUnix; }
	
	/** Unix visual. */
	inline XVisualInfo *GetVisualInfo() const{ return pVisInfo; }
	
	/** Render thread display. */
	inline Display *GetDisplay() const{ return pDisplay; }
	
	/** Main thread display. */
	Display *GetMainThreadDisplay() const;
	
#ifdef BACKEND_OPENGL
	/** Unix best framebuffer configuration. */
	inline GLXFBConfig &GetBestFBConfig(){ return pBestFBConfig; }
	inline const GLXFBConfig &GetBestFBConfig() const{ return pBestFBConfig; }
	
	/** Context. */
	inline GLXContext GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline GLXContext GetLoaderContext() const{ return pLoaderContext; }
	
	/** Compile context or nullptr. */
	inline GLXContext GetCompileContextAt(int index) const{ return pCompileContext[index]; }
	
#elif defined BACKEND_VULKAN
	/** Vulkan. */
	inline deSharedVulkan &GetVulkan() const{ return pVulkan; }
	
	/** Vulkan device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** queues. */
	inline devkQueue &GetQueueGraphic() const{ return *pQueueGraphic; }
	inline devkQueue &GetQueueCompute() const{ return *pQueueCompute; }
	inline devkQueue &GetQueueTransfer() const{ return *pQueueTransfer; }
	
	/** Command pools. */
	inline devkCommandPool &GetCommandPoolGraphic() const{ return pCommandPoolGraphic; }
	inline devkCommandPool &GetCommandPoolCompute() const{ return pCommandPoolCompute; }
	inline devkCommandPool &GetCommandPoolTransfer() const{ return pCommandPoolTransfer; }
#endif
	
	/** Atoms. */
	inline Atom GetAtomProtocols() const{ return pAtomProtocols; }
	inline Atom GetAtomDeleteWindow() const{ return pAtomDeleteWindow; }
	
	/** Process event loop. */
	void ProcessEventLoop();
#endif
	
#ifdef OS_ANDROID
	/** OS Android. */
	inline deOSAndroid *GetOSAndroid(){ return pOSAndroid; }
	
	/** Display. */
	inline EGLDisplay GetDisplay() const{ return pDisplay; }
	
	/** Surface. */
	inline EGLSurface GetSurface() const{ return pSurface; }
	
	/** Context. */
	inline EGLContext GetContext() const{ return pContext; }
	
	/** Loader surface. */
	inline EGLSurface GetLoaderSurface() const{ return pLoaderSurface; }
	
	/** Loader context. */
	inline EGLContext GetLoaderContext() const{ return pLoaderContext; }
	
	/** Compile surface or EGL_NO_SURFACE. */
	inline EGLSurface GetCompileSurfaceAt(int index) const{ return pCompileSurface[index]; }
	
	/** Compile context or EGL_NO_CONTEXT. */
	inline EGLContext GetCompileContextAt(int index) const{ return pCompileContext[index]; }
	
	/** Configuration. */
	inline const EGLConfig& GetConfig() const{ return pConfig; }
	
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	
	/** Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
	
#elif defined OS_WEBWASM
	/** OS Web WASM. */
	inline deOSWebWasm *GetOSWebWasm(){ return pOSWebWasm; }
	
	/** Context. */
	inline EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetContext() const{ return pContext; }
	
	/** Loader context. */
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetLoaderContext();
	
	/** Compile context. */
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetCompileContextAt(int index);
	
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	
	/** Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
	
#elif defined OS_BEOS
	/** OS BeOS. */
	inline deOSBeOS *GetOSBeOS() const{ return pOSBeOS; }
	
	/** Context. */
//	inline GLXContext getContext() const{ return pContext; }   ???

#elif defined OS_MACOS
	/** OS MacOS. */
	inline deOSMacOS *GetOSMacOS() const{ return pOSMacOS; }
	
	/** Pixel format. */
	inline NSOpenGLPixelFormat *GetPixelFormat() const{ return pPixelFormat; }
	
	/** Context. */
	inline NSOpenGLContext *GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline NSOpenGLContext *GetLoaderContext() const{ return pLoaderContext; }
	
	/** Loader context or nullptr. */
	inline NSOpenGLContext *GetCompileContextAt(int index) const{ return pCompileContext[index]; }
	
#elif defined OS_W32
	/** Registered window class name. */
	inline const decString &GetWindowClassname() const{ return pWindowClassname; }
	
	/** OS Windows. */
	inline deOSWindows *GetOSWindow() const{ return pOSWindows; }
	
	/** Context. */
	inline HGLRC GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline HGLRC GetLoaderContext() const{ return pLoaderContext; }
	
	/** Compile context or NULL. */
	inline HGLRC GetCompileContextAt(int index) const{ return pCompileContext[index]; }
	
	LRESULT ProcessWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
	/*@}*/
	
	
	
private:
#ifdef OS_UNIX_X11
	void pOpenDisplay();
#ifdef BACKEND_OPENGL
	void pPrintVisualInfo();
	void pChooseFBConfig();
	void pChooseVisual();
#endif
	void pCreateColorMap();
	void pCreateAtoms();
	void pCreateContext();
	void pFreeContext();
	void pFreeVisualInfo();
	void pCloseDisplay();
	
#elif defined OS_ANDROID
	void pInitDisplay();
	void pCloseDisplay();
	
#elif defined OS_WEBWASM
	void pCreateContext();
	void pFreeContext();
	
#elif defined OS_BEOS
	void pCreateContext();
	void pFreeContext();
	
#elif defined OS_W32
	void pRegisterWindowClass();
	void pCreateContext();
	void pUnregisterWindowClass();
	void pFreeContext();
	
#elif defined OS_MACOS
	void pCreateContext();
	void pContextMakeCurrent(NSView *view);
	void pFreeContext();
#endif
};

#endif
