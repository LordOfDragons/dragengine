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

#ifndef _DEOGLREFFECTDISTORTIMAGE_H_
#define _DEOGLREFFECTDISTORTIMAGE_H_

#include "deoglREffect.h"

#include <dragengine/common/math/decMath.h>

class deoglRImage;
class deoglShaderProgram;


/**
 * \brief Render effect distort image.
 */
class deoglREffectDistortImage : public deoglREffect{
private:
	decVector2 pStrength;
	deoglRImage *pImage;
	
	deoglShaderProgram *pShader;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render effect. */
	deoglREffectDistortImage( deoglRenderThread &renderThread );
	
	/** \brief Clean up render effect. */
	virtual ~deoglREffectDistortImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Distortion strength in U and V direction. */
	inline const decVector2 &GetStrength() const{ return pStrength; }
	
	/** \brief Set distortion strength in U and V direction. */
	void SetStrength( const decVector2 &strength );
	
	/** \brief Image or \em NULL to render nothing. */
	inline deoglRImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or \em NULL to render nothing. */
	void SetImage( deoglRImage *image );
	
	
	
	/** \brief Get shader creating it if required. */
	deoglShaderProgram *GetShader();
	
	/** \brief Prepare for render. */
	virtual void PrepareForRender();
	
	/** \brief Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif
