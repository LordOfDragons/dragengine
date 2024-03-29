/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLCANVAS_H_
#define _DEOGLCANVAS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCanvas.h>

class deoglRCanvas;
class deCanvas;
class deGraphicOpenGl;



/**
 * Canvas peer.
 */
class deoglCanvas : public deBaseGraphicCanvas{
private:
	deGraphicOpenGl &pOgl;
	deCanvas &pCanvas;
	
	deoglRCanvas *pRCanvas;
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
	deoglCanvas( deGraphicOpenGl &ogl, deCanvas &canvas );
	
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
	inline deoglRCanvas *GetRCanvas() const{ return pRCanvas; }
	
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
