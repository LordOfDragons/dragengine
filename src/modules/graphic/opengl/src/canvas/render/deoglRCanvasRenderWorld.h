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

#ifndef _DEOGLRCANVASRENDERWORLD_H_
#define _DEOGLRCANVASRENDERWORLD_H_

#include "deoglRCanvas.h"

class deoglRWorld;
class deoglRCamera;


/**
 * \brief Render canvas render world.
 */
class deoglRCanvasRenderWorld : public deoglRCanvas{
private:
	deoglRCamera *pCamera;
	bool pForceToneMapAdaption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvasRenderWorld( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvasRenderWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Camera or \em NULL if not set. */
	inline deoglRCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or \em NULL if not set. */
	void SetCamera( deoglRCamera *camera );
	
	
	
	/** \brief Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** \brief Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
