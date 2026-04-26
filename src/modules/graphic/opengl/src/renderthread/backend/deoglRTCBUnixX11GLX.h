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

#ifndef _DEOGLRTCBUNIXX11GLX_H_
#define _DEOGLRTCBUNIXX11GLX_H_

#include <dragengine/dragengine_configuration.h>

#if defined(OS_UNIX_X11) && defined(BACKEND_OPENGL)

#include <GL/glx.h>
#include "deoglRTCBUnixX11.h"
#include "../../extensions/glxext.h"


/**
 * GLX-based OpenGL context backend for Unix X11.
 *
 * Subclass of deoglRTCBUnixX11OpenGL that implements OpenGL context creation and
 * management using the dynamically loaded libGLX.so library. X11 state (display,
 * screen, visual info, colormap) is inherited from deoglRTCBUnixX11 via the base class.
 */
class deoglRTCBUnixX11GLX : public deoglRTCBUnixX11{
protected:
	void *pLibHandle;
	
	GLXContext pGLXContext;
	GLXContext pGLXLoaderContext;
	GLXContext pGLXCompileContext[MaxCompileContextCount];
	GLXFBConfig pGLXBestFBConfig;
	Display *pCompileContextDisplay[MaxCompileContextCount];
	
	typedef GLXContext (*PFNGLXCREATENEWCONTEXTPROC)(Display*, GLXFBConfig, int, GLXContext, Bool);
	typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC2)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	typedef void (*PFNGLXDESTROYCONTEXTPROC)(Display*, GLXContext);
	typedef Bool (*PFNGLXMAKECURRENTPROC)(Display*, GLXDrawable, GLXContext);
	typedef Bool (*PFNGLXISDIRECTPROC)(Display*, GLXContext);
	typedef GLXFBConfig* (*PFNGLXCHOOSEFBCONFIGPROC)(Display*, int, const int*, int*);
	typedef void (*PFNGLXGETFBCONFIGATTRIBPROC)(Display*, GLXFBConfig, int, int*);
	typedef XVisualInfo* (*PFNGLXCHOOSEVISUALPROC)(Display*, int, int*);
	typedef void (*PFNGLXGETCONFIGPROC)(Display*, XVisualInfo*, int, int*);
	typedef void* (*PFNGLXGETPROCADDRESSPROC)(const GLubyte*);
	typedef void (*PFNGLXSWAPBUFFERSPROC)(Display*, GLXDrawable);
	
	PFNGLXCREATENEWCONTEXTPROC pGlXCreateNewContext;
	PFNGLXCREATECONTEXTATTRIBSARBPROC2 pGlXCreateContextAttribs;
	PFNGLXDESTROYCONTEXTPROC pGlXDestroyContext;
	PFNGLXMAKECURRENTPROC pGlXMakeCurrent;
	PFNGLXISDIRECTPROC pGlXIsDirect;
	PFNGLXCHOOSEFBCONFIGPROC pGlXChooseFBConfig;
	PFNGLXGETFBCONFIGATTRIBPROC pGlXGetFBConfigAttrib;
	PFNGLXCHOOSEVISUALPROC pGlXChooseVisual;
	PFNGLXGETCONFIGPROC pGlXGetConfig;
	PFNGLXGETPROCADDRESSPROC pGlXGetProcAddress;
	PFNGLXSWAPBUFFERSPROC pGlxSwapBuffers;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create GLX backend. */
	explicit deoglRTCBUnixX11GLX(deoglRTContext &context);
	
	/** Clean up GLX backend. */
	~deoglRTCBUnixX11GLX() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Try to initialize. Returns false if the required library is not available
	 * or required symbols are missing. Throws on other failures.
	 */
	bool TryInit();
	
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
	
	
	inline GLXFBConfig &GetGLXBestFBConfig(){ return pGLXBestFBConfig; }
	inline const GLXFBConfig &GetGLXBestFBConfig() const{ return pGLXBestFBConfig; }
	inline GLXContext GetGLXContext() const{ return pGLXContext; }
	inline GLXContext GetGLXLoaderContext() const{ return pGLXLoaderContext; }
	GLXContext GetGLXCompileContextAt(int index) const;
	/*@}*/
	
	
protected:
	void pFreeContext();
	void pChooseVisual() override;
	void pPrintVisualInfo();
	void pChooseFBConfig();
	void pCreateContext();
	void pUnloadLibrary();
};

#endif // OS_UNIX_X11 && BACKEND_OPENGL
#endif // _DEOGLRTCBUNIXOPENGLUSINGGL_H_
