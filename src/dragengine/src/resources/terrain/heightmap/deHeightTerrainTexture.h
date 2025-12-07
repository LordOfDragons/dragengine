/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEHEIGHTTERRAINTEXTURE_H_
#define _DEHEIGHTTERRAINTEXTURE_H_

#include "../../../common/math/decMath.h"
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>



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
	deSkin::Ref pSkin;
	
	decVector2 pProjOffset;
	decVector2 pProjScaling;
	float pProjRotation;
	
	deImage::Ref pMaskImage;
	
	
	
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
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin(deSkin *skin);
	
	/** \brief Projection offset. */
	inline const decVector2 &GetProjectionOffset() const{ return pProjOffset; }
	
	/** \brief Set projection offset. */
	void SetProjectionOffset(const decVector2 &offset);
	
	/** \brief Projection scaling. */
	inline const decVector2 &GetProjectionScaling() const{ return pProjScaling; }
	
	/** \brief Set projection scaling. */
	void SetProjectionScaling(const decVector2 &scaling);
	
	/** \brief Projection rotation. */
	inline float GetProjectionRotation() const{ return pProjRotation; }
	
	/** \brief Set projection rotation. */
	void SetProjectionRotation(float rotation);
	
	/** \brief Mask image or NULL if not set. */
	inline const deImage::Ref &GetMaskImage() const{ return pMaskImage; }
	
	/** \brief Set mask image or NULL if not set. */
	void SetMaskImage(deImage *image);
	/*@}*/
};

#endif
