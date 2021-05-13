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

class deoglRenderCanvasContext;
class deoglRenderThread;
class deoglRenderTarget;
class deoglRenderPlanMasked;


/**
 * \brief Render canvas.
 */
class deoglRCanvas : public deObject{
private:
	deoglRenderThread &pRenderThread;
	decVector2 pPosition;
	decVector2 pSize;
	decTexMatrix2 pTransform;
	decColorMatrix pColorTransform;
	float pOrder;
	float pTransparency;
	GLenum pBlendSrc;
	GLenum pBlendDest;
	deoglRCanvas *pMask;
	bool pVisible;
	deoglRenderTarget *pMaskRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvas( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Position. */
	inline const decVector2 &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector2 &position );
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	virtual void SetSize( const decVector2 &size );
	
	/** \brief Transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set transformation matrix. */
	void SetTransform( const decTexMatrix2 &transform );
	
	/** \brief Color transformation matrix. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** \brief Set color transformation matrix. */
	void SetColorTransform( const decColorMatrix &transform );
	
	/** \brief Order used for sorting. */
	inline float GetOrder() const{ return pOrder; }
	
	/** \brief Set order. */
	void SetOrder( float order );
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency. */
	void SetTransparency( float transparency );
	
	/** \brief OpenGL source blend mode. */
	inline GLenum GetBlendSrc() const{ return pBlendSrc; }
	
	/** \brief Set OpenGL source blend mode. */
	void SetBlendSrc( GLenum blendSrc );
	
	/** \brief OpenGL destination blend mode. */
	inline GLenum GetBlendDest() const{ return pBlendDest; }
	
	/** \brief Set OpenGL destination blend mode. */
	void SetBlendDest( GLenum blendDest );
	
	/** \brief Mask. */
	inline deoglRCanvas *GetMask() const{ return pMask; }
	
	/** \brief Set mask. */
	void SetMask( deoglRCanvas *mask );
	
	/** \brief Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set visible. */
	void SetVisible( bool visible );
	
	/** \brief Mask render target or \em NULL if not ready. */
	inline deoglRenderTarget *GetMaskRenderTarget() const{ return pMaskRenderTarget; }
	
	/** \brief Dirty mask render target if present. */
	void DirtyMaskRenderTarget();
	
	
	
	/** \brief Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** \brief Render. */
	virtual void Render( const deoglRenderCanvasContext &context ) = 0;
	/*@}*/
};

#endif
