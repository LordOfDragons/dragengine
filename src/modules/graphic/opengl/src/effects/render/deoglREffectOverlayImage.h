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

#ifndef _DEOGLREFFECTOVERLAYIMAGE_H_
#define _DEOGLREFFECTOVERLAYIMAGE_H_

#include "deoglREffect.h"
#include "../../pipeline/deoglPipeline.h"

#include <dragengine/common/math/decMath.h>

class deoglRImage;


/**
 * Render effect overlay image.
 */
class deoglREffectOverlayImage : public deoglREffect{
private:
	float pTransparency;
	deoglRImage *pImage;
	
	const deoglPipeline *pPipeline;
	const deoglPipeline *pPipelineStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render effect. */
	deoglREffectOverlayImage( deoglRenderThread &renderThread );
	
	/** Clean up render effect. */
	virtual ~deoglREffectOverlayImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency. */
	void SetTransparency( float transparency );
	
	/** Image or \em NULL to render nothing. */
	inline deoglRImage *GetImage() const{ return pImage; }
	
	/** Set image or \em NULL to render nothing. */
	void SetImage( deoglRImage *image );
	
	
	
	/** Get pipeline creating it if required. */
	const deoglPipeline *GetPipeline();
	const deoglPipeline *GetPipelineStereo();
	
	/** Prepare for render. */
	virtual void PrepareForRender();
	
	/** Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif
