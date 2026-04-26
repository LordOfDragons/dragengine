/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLRTCBANDROID_H_
#define _DEOGLRTCBANDROID_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_ANDROID

#include "deoglRTCBackend.h"
#include "../../deoglGL.h"

class deOSAndroid;


/**
 * Render thread context backend for Android.
 * 
 * Android assigns the window differently so AssignOSWindow() is not used.
 */
class deoglRTCBAndroid : public deoglRTCBackend{
protected:
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
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBAndroid(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBAndroid() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 2. */
	void InitPhase2() override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	/** Drop compile contexts above count. */
	void DropCompileContexts(int count) override;
	
	/** Activate context for window. */
	void ActivateContext(deoglRRenderWindow &window) override;
	
	/** Deactivate current context. */
	void DeactivateContext() override;
	
	/** Has loader context ready. */
	bool HasLoaderContext() const override;
	
	/** Activate loader context. */
	void ActivateLoaderContext() override;
	
	/** Deactivate loader context. */
	void DeactivateLoaderContext() override;
	
	/** Activate compile context. */
	void ActivateCompileContext(int index) override;
	
	/** Deactivate compile context. */
	void DeactivateCompileContext(int index) override;
	
	/** Swap buffers. */
	void SwapBuffers(deoglRRenderWindow &window) override;
	
	/** Get function pointer. */
	void *GetFunctionPointer(const char *funcName) override;
	
	
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	
	/** Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** OS Android. */
	inline deOSAndroid *GetOSAndroid() const{ return pOSAndroid; }
	
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
	inline const EGLConfig &GetConfig() const{ return pConfig; }
	
	/** Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
	/*@}*/
	
	
protected:
	void pInitDisplay();
	void pCloseDisplay();
};

#endif // OS_ANDROID
#endif // _DEOGLRTCBANDROID_H_
