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

#ifndef _DEOGLCANVAS_H_
#define _DEOGLCANVAS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCanvas.h>

#include "render/deoglRCanvas.h"
class deCanvas;
class deGraphicOpenGl;



/**
 * Canvas peer.
 */
class deoglCanvas : public deBaseGraphicCanvas{
private:
	deGraphicOpenGl &pOgl;
	deCanvas &pCanvas;
	
	deoglRCanvas::Ref pRCanvas;
	bool pDirtyGeometry;
	bool pDirtyTransform;
	bool pDirtyColorTransform;
	
	bool pDirtyParameters;
	bool pDirtyContent;
	bool pDirtyParentPaint;
	bool pDirtyMaskContent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvas(deGraphicOpenGl &ogl, deCanvas &canvas);
	
	/** Clean up peer. */
	virtual ~deoglCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOgl(){ return pOgl; }
	inline const deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Canvas engine resource. */
	inline deCanvas &GetCanvas(){ return pCanvas; }
	inline const deCanvas &GetCanvas() const{ return pCanvas; }
	
	/** Render canvas or \em NULL if not set. */
	inline const deoglRCanvas::Ref &GetRCanvas() const{ return pRCanvas; }
	
	/** Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/** Set parent paint dirty. */
	void SetDirtyParentPaint();
	
	/** Set mask content dirty. */
	void SetDirtyMaskContent();
	
	
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
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
	
	/** Mask changed. */
	virtual void MaskChanged();
	
	/** Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas() = 0;
};

#endif
