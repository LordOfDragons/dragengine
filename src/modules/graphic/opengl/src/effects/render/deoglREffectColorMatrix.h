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

#ifndef _DEOGLREFFECTCOLORMATRIX_H_
#define _DEOGLREFFECTCOLORMATRIX_H_

#include "deoglREffect.h"

#include <dragengine/common/math/decMath.h>

class deoglShaderProgram;


/**
 * \brief Render effect color matrix.
 */
class deoglREffectColorMatrix : public deoglREffect{
private:
	decColorMatrix pColorMatrix;
	
	deoglShaderProgram *pShader;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render effect. */
	deoglREffectColorMatrix( deoglRenderThread &renderThread );
	
	/** \brief Clean up render effect. */
	virtual ~deoglREffectColorMatrix();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Color matrix. */
	inline const decColorMatrix &GetColorMatrix() const{ return pColorMatrix; }
	
	/** \brief Set color matrix. */
	void SetColorMatrix( const decColorMatrix &colorMatrix );
	
	
	
	/** \brief Get shader creating it if required. */
	deoglShaderProgram *GetShader();
	
	/** \brief Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif
