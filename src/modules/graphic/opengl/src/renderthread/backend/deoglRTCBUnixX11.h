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

#ifndef _DEOGLRTCBUNIXBASE_H_
#define _DEOGLRTCBUNIXBASE_H_

#include "deoglRTCBackend.h"

#ifdef OS_UNIX_X11

#include "deoglRTCBackend.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class deOSUnix;


/**
 * Base class for render thread context backends on Unix X11.
 */
class deoglRTCBUnixX11 : public deoglRTCBackend{
protected:
	deOSUnix *pOSUnix;
	
	Display *pDisplay;
	int pScreen;
	
	Colormap pColMap;
	XVisualInfo *pVisInfo;
	
	Atom pAtomProtocols;
	Atom pAtomDeleteWindow;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create backend. */
	explicit deoglRTCBUnixX11(deoglRTContext &context);
	
	/** Clean up backend. */
	~deoglRTCBUnixX11() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Initialize phase 2. */
	void InitPhase2() override;
	
	/** Clean up all resources. */
	void CleanUp() override;
	
	/** Assign OS window after render window is created. */
	void AssignOSWindow(deRenderWindow *renderWindow) override;
	
	
	/** Process event loop. */
	virtual void ProcessEventLoop();
	
	/** OS Unix. */
	inline deOSUnix *GetOSUnix() const{ return pOSUnix; }
	
	/** Render thread display. */
	inline Display *GetDisplay() const{ return pDisplay; }
	
	/** Main thread display. */
	Display *GetMainThreadDisplay() const;
	
	/** Screen. */
	inline int GetScreen() const{ return pScreen; }
	
	/** Unix visual info. */
	inline XVisualInfo *GetVisualInfo() const{ return pVisInfo; }
	
	/** Atom WM_PROTOCOLS. */
	inline Atom GetAtomProtocols() const{ return pAtomProtocols; }
	
	/** Atom WM_DELETE_WINDOW. */
	inline Atom GetAtomDeleteWindow() const{ return pAtomDeleteWindow; }
	/*@}*/
	
	
protected:
	virtual void pChooseVisual() = 0;
	void pFreeVisualInfo();
	virtual void pOpenDisplay();
	void pCloseDisplay();
	void pCreateColorMap();
	void pCreateAtoms();
};

#endif // OS_UNIX_X11
#endif // _DEOGLRTCBUNIXBASE_H_
