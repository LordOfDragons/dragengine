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

#include <dragengine/deTUniqueReference.h>

#ifdef OS_UNIX_X11
#	ifdef BACKEND_OPENGL
#		include "backend/deoglRTCBUnixX11GLX.h"
#		include "backend/deoglRTCBUnixX11EGL.h"
#		include "backend/deoglRTCBUnixWaylandEGL.h"
#	elif defined BACKEND_VULKAN
#		include "backend/deoglRTCBUnixX11Vulkan.h"
#	endif
#elif defined OS_ANDROID
#	include "backend/deoglRTCBAndroid.h"
#elif defined OS_WEBWASM
#	include "backend/deoglRTCBWasm.h"
#elif defined OS_BEOS
#	include "backend/deoglRTCBBeOS.h"
#elif defined OS_MACOS
#	include "backend/deoglRTCBMacOS.h"
#elif defined OS_W32
#	include "backend/deoglRTCBWindows.h"
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
	
#ifdef OS_UNIX_X11
#	ifdef BACKEND_OPENGL
	deTUniqueReference<deoglRTCBUnixX11> pBackend;
#	elif defined(BACKEND_VULKAN)
	deTUniqueReference<deoglRTCBUnixX11Vulkan> pBackend;
#	endif
#elif defined(OS_ANDROID)
	deTUniqueReference<deoglRTCBAndroid> pBackend;
#elif defined(OS_WEBWASM)
	deTUniqueReference<deoglRTCBWasm> pBackend;
#elif defined(OS_BEOS)
	deTUniqueReference<deoglRTCBBeOS> pBackend;
#elif defined(OS_MACOS)
	deTUniqueReference<deoglRTCBMacOS> pBackend;
#elif defined(OS_W32)
	deTUniqueReference<deoglRTCBWindows> pBackend;
#endif
	
	deoglRRenderWindow *pActiveRRenderWindow;
	bool pUserRequestedQuit;
	bool pAppActivated;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread context. */
	explicit deoglRTContext(deoglRenderThread &renderThread);
	
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
	
	
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread(){ return pRenderThread; }
	
	/** Set application activated state. Called by backends during event handling. */
	inline void SetAppActivated(bool activated){ pAppActivated = activated; }
	
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
	
	/**
	 * Set user requested quit.
	 */
	void SetUserRequestedQuit(bool userRequestedQuit);
	
	/** Application is activated. */
	inline bool GetAppActivated() const{ return pAppActivated; }
	
	
	/** Backend. */
#ifdef OS_UNIX_X11
#	ifdef BACKEND_OPENGL
	inline const deTUniqueReference<deoglRTCBUnixX11> &GetBackend() const{ return pBackend; }
#	elif defined BACKEND_VULKAN
	inline const deTUniqueReference<deoglRTCBUnixX11Vulkan> &GetBackend() const{ return pBackend; }
#	endif
#elif defined OS_ANDROID
	inline const deTUniqueReference<deoglRTCBAndroid> &GetBackend() const{ return pBackend; }
#elif defined OS_WEBWASM
	inline const deTUniqueReference<deoglRTCBWasm> &GetBackend() const{ return pBackend; }
#elif defined OS_BEOS
	inline const deTUniqueReference<deoglRTCBBeOS> &GetBackend() const{ return pBackend; }
#elif defined OS_MACOS
	inline const deTUniqueReference<deoglRTCBMacOS> &GetBackend() const{ return pBackend; }
#elif defined OS_W32
	inline const deTUniqueReference<deoglRTCBWindows> &GetBackend() const{ return pBackend; }
#endif
};

#endif
