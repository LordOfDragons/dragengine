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

#ifndef _DEOGLRTCBUNIXEGL_H_
#define _DEOGLRTCBUNIXEGL_H_

#include <dragengine/dragengine_configuration.h>

#if defined(OS_UNIX_X11) && defined(BACKEND_OPENGL)

#include "deoglRTCBUnixX11.h"
#include "../../extensions/egl.h"
#include "../../extensions/eglext.h"


/**
 * EGL-based OpenGL context backend for Unix X11.
 */
class deoglRTCBUnixX11EGL : public deoglRTCBUnixX11{
protected:
	void *pLibHandle;
	
	EGLDisplay pEGLDisplay;
	EGLConfig pEGLConfig;
	EGLContext pEGLContext;
	EGLContext pEGLLoaderContext;
	EGLContext pEGLCompileContext[MaxCompileContextCount];
	EGLSurface pEGLLoaderSurface;
	EGLSurface pEGLCompileSurface[MaxCompileContextCount];
	
	PFNEGLGETDISPLAYPROC pEglGetDisplay;
	PFNEGLINITIALIZEPROC pEglInitialize;
	PFNEGLTERMINATEPROC pEglTerminate;
	PFNEGLCHOOSECONFIGPROC pEglChooseConfig;
	PFNEGLCREATEWINDOWSURFACEPROC pEglCreateWindowSurface;
	PFNEGLCREATEPBUFFERSURFACEPROC pEglCreatePbufferSurface;
	PFNEGLDESTROYSURFACEPROC pEglDestroySurface;
	PFNEGLCREATECONTEXTPROC pEglCreateContext;
	PFNEGLDESTROYCONTEXTPROC pEglDestroyContext;
	PFNEGLMAKECURRENTPROC pEglMakeCurrent;
	PFNEGLGETPROCADDRESSPROC pEglGetProcAddress;
	PFNEGLQUERYSURFACEPROC pEglQuerySurface;
	typedef EGLBoolean (*PFNEGLBINDAPIPROC)(EGLenum api);
	PFNEGLBINDAPIPROC pEglBindAPI;
	PFNEGLSWAPBUFFERSPROC pEglSwapBuffers;
	PFNEGLSWAPINTERVALPROC pEglSwapInterval;
	PFNEGLGETCONFIGATTRIBPROC pEglGetConfigAttrib;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create EGL backend. */
	explicit deoglRTCBUnixX11EGL(deoglRTContext &context);
	
	/** Clean up EGL backend. */
	~deoglRTCBUnixX11EGL() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Try to initialize. Returns false if the required library is not available
	 * or required symbols are missing. Throws on other failures.
	 */
	virtual bool TryInit();
	
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
	
	/** Get function pointer. */
	void *GetFunctionPointer(const char *funcName) override;
	
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
	
	/** Apply VSync mode. */
	void ApplyVSync(deoglRRenderWindow &window, deoglConfiguration::eVSyncMode vsyncMode) override;
	
	/** Create surface for window. */
	virtual void CreateWindowSurface(deoglRRenderWindow &window);
	
	/** Destroy surface for window. */
	virtual void DestroyWindowSurface(deoglRRenderWindow &window);
	
	
	/** Library functions. */
	inline EGLDisplay GetEGLDisplay() const{ return pEGLDisplay; }
	inline EGLConfig GetEGLConfig() const{ return pEGLConfig; }
	inline EGLContext GetEGLContext() const{ return pEGLContext; }
	inline EGLContext GetEGLLoaderContext() const{ return pEGLLoaderContext; }
	EGLContext GetEGLCompileContextAt(int index) const;
	inline EGLSurface GetEGLLoaderSurface() const{ return pEGLLoaderSurface; }
	EGLSurface GetEGLCompileSurfaceAt(int index) const;
	inline PFNEGLGETPROCADDRESSPROC GetEGLGetProcAddressFunc() const{ return pEglGetProcAddress; }
	/*@}*/
	
	
protected:
	void pFreeContext();
	virtual void pChooseConfig();
	void pChooseVisual() override;
	void pCreateContext();
	void pUnloadLibrary();
};

#endif // OS_UNIX_X11 && BACKEND_OPENGL
#endif // _DEOGLRTCBUNIXOPENGLUSINGEGL_H_
