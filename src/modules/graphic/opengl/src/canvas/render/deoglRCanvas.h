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

#ifndef _DEOGLRCANVAS_H_
#define _DEOGLRCANVAS_H_

#include "../../deoglGL.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvas.h>

class deoglRenderCanvasContext;
class deoglRenderThread;
class deoglRenderTarget;
class deoglRenderPlanMasked;


/**
 * Render canvas.
 */
class deoglRCanvas : public deObject{
public:
	static const int BlendModeCount = deCanvas::ebmAdd + 1;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	decVector2 pPosition;
	decVector2 pSize;
	decTexMatrix2 pTransform;
	decColorMatrix pColorTransform;
	float pOrder;
	float pTransparency;
	deCanvas::eBlendModes pBlendMode;
	deoglRCanvas *pMask;
	bool pVisible;
	deoglRenderTarget *pMaskRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvas( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Position. */
	inline const decVector2 &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decVector2 &position );
	
	/** Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** Set size. */
	virtual void SetSize( const decVector2 &size );
	
	/** Transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set transformation matrix. */
	void SetTransform( const decTexMatrix2 &transform );
	
	/** Color transformation matrix. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** Set color transformation matrix. */
	void SetColorTransform( const decColorMatrix &transform );
	
	/** Order used for sorting. */
	inline float GetOrder() const{ return pOrder; }
	
	/** Set order. */
	void SetOrder( float order );
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency. */
	void SetTransparency( float transparency );
	
	/** Blend mode. */
	inline deCanvas::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** Set blend mode. */
	void SetBlendMode( deCanvas::eBlendModes mode );
	
	/** Mask. */
	inline deoglRCanvas *GetMask() const{ return pMask; }
	
	/** Set mask. */
	void SetMask( deoglRCanvas *mask );
	
	/** Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set visible. */
	void SetVisible( bool visible );
	
	/** Mask render target or \em NULL if not ready. */
	inline deoglRenderTarget *GetMaskRenderTarget() const{ return pMaskRenderTarget; }
	
	/** Dirty mask render target if present. */
	void DirtyMaskRenderTarget();
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Prepare for rendering render. */
	virtual void PrepareForRenderRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Render. */
	virtual void Render( const deoglRenderCanvasContext &context ) = 0;
	/*@}*/
};

#endif
