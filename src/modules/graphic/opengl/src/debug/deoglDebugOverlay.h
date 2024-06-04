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

#ifndef _DEOGLDEBUGOVERLAY_H_
#define _DEOGLDEBUGOVERLAY_H_

#include "deoglDebugOverlay.h"

#include <dragengine/common/collection/decPointerList.h>

class deCanvasView;
class deGraphicOpenGl;


/**
 * Debug overlay manager.
 */
class deoglDebugOverlay{
private:
	deGraphicOpenGl &pOgl;
	decPointerList pList1, pList2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug overlay manager. */
	deoglDebugOverlay( deGraphicOpenGl &ogl );
	
	/** Clean up debug overlay manager. */
	~deoglDebugOverlay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Prepare debug overlay.
	 * 
	 * Aligns the children canvas and synchronizes.
	 */
	void PrepareOverlay( deCanvasView &canvasView );
	/*@}*/
	
	
	
private:
	void pSortViews( deCanvasView &canvasView );
	void pAlignViews();
};

#endif
