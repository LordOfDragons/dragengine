/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEDSRENDERABLECANVAS_H_
#define _DEDSRENDERABLECANVAS_H_

#include "deDSRenderable.h"
#include "../../../canvas/deCanvasViewReference.h"


/**
 * \brief Dynamic skin renderable canvas.
 *
 * The texture is defined using the content of a temporary render target.
 * Temporary render targets are not created by the user but by the Graphic
 * Module on the fly if required and for as long as it is required. The
 * user only specifies the content using a deCanvasView. This allows the
 * Graphic Module to use GPU memory efficiently. In general the size of
 * the canvas view defines the size of the used render target but the
 * Graphic Module is free to choose a fitting size itself.
 */
class deDSRenderableCanvas : public deDSRenderable{
private:
	int pComponentCount;
	int pBitCount;
	deCanvasViewReference pCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderable. */
	deDSRenderableCanvas( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/**
	 * \brief Set number of components.
	 * \throws deeInvalidParam \em componentCount is less than 1.
	 * \throws deeInvalidParam \em componentCount is greater than 4.
	 */
	void SetComponentCount( int componentCount );
	
	/** \brief Bit count of each component. */
	inline int GetBitCount() const{ return pBitCount; }
	
	/**
	 * \brief Set bit count of each component.
	 * \throws deeInvalidParam \em bitCount is not 8, 16 or 32.
	 */
	void SetBitCount( int bitCount );
	
	/** \brief Canvas view or NULL if not set. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	/** \brief Set canvas view or NULL if not set. */
	void SetCanvas( deCanvasView *canvas );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
