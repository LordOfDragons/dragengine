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

#ifndef _DEOGLRTCBWASM_H_
#define _DEOGLRTCBWASM_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_WEBWASM

#include <emscripten/html5.h>

#include "deoglRTCBackend.h"

class deOSWebWasm;


/**
 * Render thread context backend for WebAssembly.
 */
class deoglRTCBWasm : public deoglRTCBackend{
protected:
	deOSWebWasm *pOSWebWasm;
	
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pContext;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pLoaderContext;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE pCompileContext[MaxCompileContextCount];
	
	int pScreenWidth;
	int pScreenHeight;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBWasm(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBWasm() override;
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
	
	
	/** Application window has been created. */
	void InitAppWindow();
	
	/** Application window has been closed. */
	void TerminateAppWindow();
	
	/** Check if screen configuration changed. */
	void CheckConfigurationChanged();
	
	/** OS Web WASM. */
	inline deOSWebWasm *GetOSWebWasm() const{ return pOSWebWasm; }
	
	/** Context. */
	inline EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetContext() const{ return pContext; }
	
	/** Loader context. */
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetLoaderContext();
	
	/** Compile context. */
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetCompileContextAt(int index);
	
	/** Current screen width. */
	inline int GetScreenWidth() const{ return pScreenWidth; }
	
	/** Current screen height. */
	inline int GetScreenHeight() const{ return pScreenHeight; }
	/*@}*/
	
	
protected:
	void pCreateContext();
	void pFreeContext();
};

#endif // OS_WEBWASM
#endif // _DEOGLRTCBWASM_H_
