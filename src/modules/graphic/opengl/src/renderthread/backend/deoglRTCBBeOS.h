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

#ifndef _DEOGLRTCBBEOS_H_
#define _DEOGLRTCBBEOS_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_BEOS

#include "deoglRTCBackend.h"

class deOSBeOS;


/**
 * Render thread context backend for BeOS.
 * 
 * Context is managed by deoglRRenderWindow. BGLView is the context.
 * This also means no separate compile contexts are used.
 */
class deoglRTCBBeOS : public deoglRTCBackend{
protected:
	deOSBeOS *pOSBeOS;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBBeOS(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBBeOS() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Assign OS window after render window is created. */
	void AssignOSWindow(deRenderWindow *renderWindow) override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	/** Called before changing the active context window. Unlock old window if applicable. */
	void BeforeDeactivateContext(deoglRRenderWindow &active) override;
	
	/** Activate context for window. */
	void ActivateContext(deoglRRenderWindow &window) override;
	
	/** Swap buffers. */
	void SwapBuffers(deoglRRenderWindow &window) override;
	
	
	/** OS BeOS. */
	inline deOSBeOS *GetOSBeOS() const{ return pOSBeOS; }
	/*@}*/
};

#endif // OS_BEOS
#endif // _DEOGLRTCBBEOS_H_
