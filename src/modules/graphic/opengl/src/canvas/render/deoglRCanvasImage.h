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

#ifndef _DEOGLRCANVASIMAGE_H_
#define _DEOGLRCANVASIMAGE_H_

#include "deoglRCanvas.h"

class deoglRImage;


/**
 * Render canvas image.
 */
class deoglRCanvasImage : public deoglRCanvas{
private:
	deoglRImage *pImage;
	decTexMatrix2 pTCTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvasImage( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvasImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Image or \em NULL if not set. */
	inline deoglRImage *GetImage() const{ return pImage; }
	
	/** Set image or \em NULL if not set. */
	void SetImage( deoglRImage *image );
	
	/** Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTCTransform() const{ return pTCTransform; }
	
	/** Set texture coordinate transformation matrix. */
	void SetTCTransform( const decTexMatrix2 &transform );
	
	/** Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
