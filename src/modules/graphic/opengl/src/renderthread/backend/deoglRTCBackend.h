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

#ifndef _DEOGLRTCBACKEND_H_
#define _DEOGLRTCBACKEND_H_

#include "../../configuration/deoglConfiguration.h"

class deoglRTContext;
class deoglRRenderWindow;
class deRenderWindow;


/**
 * Render thread context backend.
 */
class deoglRTCBackend{
public:
	static const int MaxCompileContextCount = 8;
	
	
protected:
	deoglRTContext &pRTContext;
	int pCompileContextCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBackend(deoglRTContext &context);
	
	/** Clean up backend. */
	virtual ~deoglRTCBackend();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Get context. */
	inline deoglRTContext &GetContext(){ return pRTContext; }
	
	/** Initialize phase 2. */
	virtual void InitPhase2();
	
	/** Initialize phase 3. */
	virtual void InitPhase3();
	
	/** Initialize phase 4. */
	virtual void InitPhase4();
	
	/** Assign OS window after render window is created. */
	virtual void AssignOSWindow(deRenderWindow *renderWindow);
	
	/** Clean up all resources. */
	virtual void CleanUp();
	
	/** Drop compile contexts above count. */
	virtual void DropCompileContexts(int count);
	
	/** Called before changing the active context window. */
	virtual void BeforeDeactivateContext(deoglRRenderWindow &active);
	
	/** Activate context for window. */
	virtual void ActivateContext(deoglRRenderWindow &window);
	
	/** Deactivate current context. */
	virtual void DeactivateContext();
	
	/** Has loader context ready. */
	virtual bool HasLoaderContext() const;
	
	/** Activate loader context. */
	virtual void ActivateLoaderContext();
	
	/** Deactivate loader context. */
	virtual void DeactivateLoaderContext();
	
	/** Activate compile context. */
	virtual void ActivateCompileContext(int index);
	
	/** Deactivate compile context. */
	virtual void DeactivateCompileContext(int index);
	
	/** Number of compile contexts. */
	inline int GetCompileContextCount() const{ return pCompileContextCount; }
	
	/** Swap buffers. */
	virtual void SwapBuffers(deoglRRenderWindow &window);
	
	/** Apply VSync mode. */
	virtual void ApplyVSync(deoglRRenderWindow &window, deoglConfiguration::eVSyncMode vsyncMode);
	
	/** Get function pointer. */
	virtual void *GetFunctionPointer(const char *funcName);
	/*@}*/
};

#endif
