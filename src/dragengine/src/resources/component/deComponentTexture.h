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

#ifndef _DECOMPONENTTEXTURE_H_
#define _DECOMPONENTTEXTURE_H_

#include "../../common/math/decMath.h"

class deSkin;
class deImage;
class deDynamicSkin;


/**
 * \brief Component Texture.
 * Texture definition for a component. For simple components a single skin is
 * assigned from which the contained textures are mapped to the model textures.
 * For more control the individual textures can be assigned different skins.
 * This is useful for components used as static terrain geometry and for editing
 * applications as this way individual textures can be assigned at run-time.
 * If the skin is NULL the matching texture of the skin assigned to the component
 * itself is used. Otherwise the first texture in the assigned skin is used to
 * replace the texture.
 */
class DE_DLL_EXPORT deComponentTexture{
private:
	deSkin *pSkin;
	int pTexture;
	decTexMatrix2 pTransform;
	deDynamicSkin *pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new texture. */
	deComponentTexture();
	
	/** \brief Clean up texture. */
	~deComponentTexture();
	/*@}*/
	
	
	
	/** \name Projection */
	/*@{*/
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin( deSkin *skin );
	
	/** \brief Texture number. */
	inline int GetTexture() const{ return pTexture; }
	
	/** \brief Set texture number. */
	void SetTexture( int texture );
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** \brief Dynamic skin or NULL if not set. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or NULL if not set. */
	void SetDynamicSkin( deDynamicSkin *dynamicSkin );
	/*@}*/
};

#endif
