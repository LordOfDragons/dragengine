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

#ifndef _DEOGLRTCBMACOS_H_
#define _DEOGLRTCBMACOS_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_MACOS

#include "deoglRTCBackend.h"

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


/**
 * Render thread context backend for MacOS.
 * 
 * MacOS uses NSOpenGLContext. Compile context management is deferred.
 */
class deoglRTCBMacOS : public deoglRTCBackend{
protected:
	deOSMacOS *pOSMacOS;
	NSOpenGLPixelFormat *pPixelFormat;
	NSOpenGLContext *pContext;
	NSOpenGLContext *pLoaderContext;
	NSOpenGLContext *pCompileContext[MaxCompileContextCount];
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBMacOS(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBMacOS() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 2. */
	void InitPhase2() override;
	
	/** Assign OS window after render window is created. */
	void AssignOSWindow(deRenderWindow *renderWindow) override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	/** Activate context for window. */
	void ActivateContext(deoglRRenderWindow *window) override;
	
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
	
	
	/** OS MacOS. */
	inline deOSMacOS *GetOSMacOS() const{ return pOSMacOS; }
	
	/** Pixel format. */
	inline NSOpenGLPixelFormat *GetPixelFormat() const{ return pPixelFormat; }
	
	/** Context. */
	inline NSOpenGLContext *GetContext() const{ return pContext; }
	
	/** Loader context. */
	inline NSOpenGLContext *GetLoaderContext() const{ return pLoaderContext; }
	
	/** Compile context or nullptr. */
	inline NSOpenGLContext *GetCompileContextAt(int index) const{ return pCompileContext[index]; }
	
	/** Get function pointer. */
	void *GetFunctionPointer(const char *funcName) override;
	/*@}*/
	
	
protected:
	void pCreateContext();
	void pContextMakeCurrent(NSView *view);
	void pFreeContext();
};

#endif // OS_MACOS
#endif // _DEOGLRTCBMACOS_H_
