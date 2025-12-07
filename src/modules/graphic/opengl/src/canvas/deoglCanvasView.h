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

#ifndef _DEOGLCANVASVIEW_H_
#define _DEOGLCANVASVIEW_H_

#include "deoglCanvas.h"

#include <dragengine/common/collection/decPointerSet.h>

class deoglRCanvasView;
class deCanvasView;
class deoglCanvasViewListener;


/**
 * Canvas peer.
 */
class deoglCanvasView : public deoglCanvas{
private:
	deCanvasView &pCanvasView;
	deoglRCanvasView *pRCanvasView;
	
	bool pDirtyPaint;
	bool pResizeRenderTarget;
	
	decPointerSet pListeners;
	bool pSyncRequestSend;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvasView(deGraphicOpenGl &ogl, deCanvasView &canvas);
	
	/** Clean up peer. */
	virtual ~deoglCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render canvas view or \em NULL if not existing. */
	inline deoglRCanvasView *GetRCanvasView() const{ return pRCanvasView; }
	
	/** Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/** Order of a child changed. */
	void ChildOrderChanged();
	
	/** Paint dirty. */
	inline bool GetDirtyPaint() const{ return pDirtyPaint; }
	
	/** Set paint dirty. */
	void SetDirtyPaint();
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener(deoglCanvasViewListener *listener);
	
	/** Remove listener if existing. */
	void RemoveListener(deoglCanvasViewListener *listener);
	
	/** Notify all canvas view has been destroyed. */
	void NotifyDestroyed();
	
	/** Notify all canvas view requires sync. */
	void NotifyRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Size changed. */
	virtual void SizeChanged();
	
	/** Transform changed. */
	virtual void TransformChanged();
	
	/** Color transform changed. */
	virtual void ColorTransformChanged();
	
	/** Visible changed. */
	virtual void VisibleChanged();
	
	/** Render order changed. */
	virtual void OrderChanged();
	
	/** Transparency changed. */
	virtual void TransparencyChanged();
	
	/** Blend mode changed. */
	virtual void BlendModeChanged();
	
	/** Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
	
	
	
private:
	void pRequiresSync();
};

#endif
