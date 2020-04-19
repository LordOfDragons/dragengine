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

#ifndef _DEOGLRCANVASCANVASVIEW_H_
#define _DEOGLRCANVASCANVASVIEW_H_

#include "deoglRCanvas.h"

class deoglRCanvasView;


/**
 * \brief Render canvas render target.
 */
class deoglRCanvasCanvasView : public deoglRCanvas{
private:
	deoglRCanvasView *pCanvasView;
	decTexMatrix2 pTCTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCanvasCanvasView( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCanvasCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas view or \em NULL if not set. */
	inline deoglRCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Set canvas view or \em NULL if not set. */
	void SetCanvasView( deoglRCanvasView *canvasView );
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTCTransform() const{ return pTCTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTCTransform( const decTexMatrix2 &transform );
	
	/** \brief Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** \brief Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** \brief Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** \brief Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	
	
	/** \brief Prepare for rendering. */
	virtual void PrepareForRender();
	
	/** \brief Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
