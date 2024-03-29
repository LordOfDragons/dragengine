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

#ifndef _DEOGLRRDSRENDERABLEIMAGE_H_
#define _DEOGLRRDSRENDERABLEIMAGE_H_

#include "deoglRDSRenderable.h"
#include "../../../../texture/deoglRImage.h"

#include <dragengine/deObject.h>


/**
 * Render dynamic skin image renderable.
 */
class deoglRDSRenderableImage : public deoglRDSRenderable{
private:
	deoglRImage::Ref pImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin image renderable. */
	deoglRDSRenderableImage( deoglRDynamicSkin &dynamicSkin );
	
	/** Clean up render dynamic skin image renderable. */
	virtual ~deoglRDSRenderableImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Image. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set image. */
	void SetImage( deoglRImage *image );
	
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
