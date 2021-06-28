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

#ifndef _DEOGLRCANVASVIEW_H_
#define _DEOGLRCANVASVIEW_H_

#include "deoglRCanvas.h"

#include <dragengine/common/collection/decObjectList.h>

class deoglRenderTarget;
class deoglRenderPlanMasked;


/**
 * \brief Render canvas view.
 */
class deoglRCanvasView : public deoglRCanvas{
private:
	decObjectList pChildren;
	unsigned int pPaintTracker;
	deoglRenderTarget *pRenderTarget;
	bool pResizeRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvasView( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add children with the correct sorting by order. */
	void AddChild( deoglRCanvas *canvas );
	
	/** \brief Remove all children. */
	void RemoveAllChildren();
	
	/** \brief Has no children. */
	bool HasNoChildren() const;
	
	
	
	/** \brief Paint tracker number. */
	inline unsigned int GetPaintTracker() const{ return pPaintTracker; }
	
	/** \brief Increment paint tracker number by one. */
	void IncrementPaintTracker();
	
	/** \brief Render target or \em NULL if not ready. */
	inline deoglRenderTarget *GetRenderTarget() const{ return pRenderTarget; }
	
	/** \brief Render target has to be resized. */
	void SetResizeRenderTarget();
	
	/**
	 * \brief Prepare render target.
	 * \details If not existing the render target is created. If dirty the render
	 *          target texture is rendered. After this call returns GetRenderTarget()
	 *          returns a render target with a valid texture object suitable for
	 *          rendering unless an error occurred.
	 */
	void PrepareRenderTarget( const deoglRenderPlanMasked *renderPlanMask );
	
	
	
	/** \brief Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * \brief Render.
	 * \details Resets paint dirty flag. Paint tracker is kept unchanged.
	 */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
