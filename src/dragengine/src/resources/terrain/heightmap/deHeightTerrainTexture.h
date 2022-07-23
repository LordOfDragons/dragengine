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

#ifndef _DEHEIGHTTERRAINTEXTURE_H_
#define _DEHEIGHTTERRAINTEXTURE_H_

#include "../../../common/math/decMath.h"

class deSkin;
class deImage;


/**
 * \brief Height Terrain Mesh Texture.
 *
 * Texture definition for a height terrain. Textures with an alpha mask assigned
 * are blended ontop of the other layers using the alpha mask. Textures with
 * splats assigned are handled differently. Each splat is rendered using the skin
 * of the parent texture. This allows for very complex and rich terrains with a
 * small amount of disk space required but requires powerful Graphic Modules.
 * Alpha mask textures are produced quick and render typically faster than splat
 * textures which require more work and tend to require a lot more memory
 * especially with high resolutions. The order of the texture defines the order
 * in which the textures are applied to the height terrain. Planar projection
 * along the negative y axis is used for mapping the textures. All other
 * parameters are free to be modified.
 */
class DE_DLL_EXPORT deHeightTerrainTexture{
private:
	deSkin *pSkin;
	
	decVector2 pProjOffset;
	decVector2 pProjScaling;
	float pProjRotation;
	
	deImage *pMaskImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new texture. */
	deHeightTerrainTexture();
	
	/** \brief Clean up texture. */
	~deHeightTerrainTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin( deSkin *skin );
	
	/** \brief Projection offset. */
	inline const decVector2 &GetProjectionOffset() const{ return pProjOffset; }
	
	/** \brief Set projection offset. */
	void SetProjectionOffset( const decVector2 &offset );
	
	/** \brief Projection scaling. */
	inline const decVector2 &GetProjectionScaling() const{ return pProjScaling; }
	
	/** \brief Set projection scaling. */
	void SetProjectionScaling( const decVector2 &scaling );
	
	/** \brief Projection rotation. */
	inline float GetProjectionRotation() const{ return pProjRotation; }
	
	/** \brief Set projection rotation. */
	void SetProjectionRotation( float rotation );
	
	/** \brief Mask image or NULL if not set. */
	inline deImage *GetMaskImage() const{ return pMaskImage; }
	
	/** \brief Set mask image or NULL if not set. */
	void SetMaskImage( deImage *image );
	/*@}*/
};

#endif
