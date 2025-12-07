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

#ifndef _DEOGLCANVASCANVASVIEW_H_
#define _DEOGLCANVASCANVASVIEW_H_

#include "deoglCanvas.h"
#include "deoglCanvasViewListener.h"

class deoglRCanvasCanvasView;

class deCanvasCanvasView;


/**
 * Canvas view peer.
 */
class deoglCanvasCanvasView : public deoglCanvas, deoglCanvasViewListener{
private:
	deCanvasCanvasView &pCanvasCanvasView;
	deoglRCanvasCanvasView *pRCanvasCanvasView;
	deoglCanvasView *pCanvasView;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvasCanvasView(deGraphicOpenGl &ogl, deCanvasCanvasView &canvas);
	
	/** Clean up peer. */
	virtual ~deoglCanvasCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	
	/** Canvas view has been destroyed. Owner has to drop weak reference to canvas view. */
	virtual void CanvasViewDestroyed();
	
	/** Canvas view requires sync. */
	virtual void CanvasViewRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
};

#endif
