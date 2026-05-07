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

#ifndef _DEOGLRTCBWINDOWS_H_
#define _DEOGLRTCBWINDOWS_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32

#include "deoglRTCBackend.h"
#include "../../include_windows.h"

#include <dragengine/common/string/decString.h>

class deOSWindows;


/**
 * Render thread context backend for Windows (WGL).
 */
class deoglRTCBWindows : public deoglRTCBackend{
protected:
	decString pWindowClassname;
	deOSWindows *pOSWindows;
	HGLRC pContext;
	HGLRC pLoaderContext;
	HGLRC pCompileContext[MaxCompileContextCount];
	int pHdrPixelFormat;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBWindows(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBWindows() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 3. */
	void InitPhase3() override;
	
	/** Initialize phase 4. */
	void InitPhase4() override;
	
	/** Assign OS window after render window is created. */
	void AssignOSWindow(deRenderWindow *renderWindow) override;
	
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
	
	/** Apply VSync mode. */
	void ApplyVSync(deoglRRenderWindow &window, deoglConfiguration::eVSyncMode vsyncMode) override;
	
	
	/** Process window message. */
	LRESULT ProcessWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
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
	
	/** HDR pixel format chosen during initialization or 0 if not available. */
	inline int GetHdrPixelFormat() const{ return pHdrPixelFormat; }
	
	/** True if a 10-bit HDR-capable pixel format was found during initialization. */
	inline bool GetSupportsHdr() const{ return pHdrPixelFormat > 0; }
	
	/** Get function pointer. */
	void *GetFunctionPointer(const char *funcName) override;
	/*@}*/
	
	
protected:
	void pRegisterWindowClass();
	void pCreateContext();
	void pUnregisterWindowClass();
	void pFreeContext();
	void pChooseHdrPixelFormat();
	void pQueryHdrNits(deoglRRenderWindow &window);
};

#endif // OS_W32
#endif // _DEOGLRTCBWINDOWS_H_
