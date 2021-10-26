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

#ifndef _DEOGLRRDSRENDERABLECANVAS_H_
#define _DEOGLRRDSRENDERABLECANVAS_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"

class deoglRCanvasView;
class deDSRenderableCanvas;



/**
 * Render dynamic skin canvas renderable.
 */
class deoglRDSRenderableCanvas : public deoglRDSRenderable{
private:
	deoglRCanvasView *pCanvas;
	int pComponentCount;
	int pBitCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin canvas renderable. */
	deoglRDSRenderableCanvas( deoglRDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable );
	
	/** Clean up render dynamic skin canvas  renderable. */
	virtual ~deoglRDSRenderableCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set canvas. */
	void SetCanvas( deoglRCanvasView *canvas );
	
	/** Set component count. */
	void SetComponentCount( int componentCount );
	
	/** Set bit count. */
	void SetBitCount( int bitCount );
	
	/** Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	/*@}*/
};

#endif
