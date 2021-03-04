/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLGIMATERIALS_H_
#define _DEOGLGIMATERIALS_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/deoglRenderbuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/deObjectReference.h>

class deoglRenderThread;


/**
 * \brief Global illumination materials.
 * 
 * Materials are stored in texture atlas similar to light maps hence materials maps are square
 * in size and of small size. The default material map size is 64x64 with 32 material maps
 * per row. This is a total of 1024 materials. If the number of materials becomes larger the
 * map size is reduced by factor 2 quadrupling the maximum number of materials.
 */
class deoglGIMaterials{
private:
	deoglRenderThread &pRenderThread;
	
	int pMaxMaterialMapSize;
	int pMaxMaterialsPerRow;
	int pMaxRowsPerImage;
	
	int pMaterialMapSize;
	int pMaterialsPerRow;
	int pRowsPerImage;
	int pMaxMaterialCount;
	
	int pMaterialCount;
	
	deoglTexture pTexDiffuseTransparency;
	deoglTexture pTexReflectivityRoughness;
	deoglTexture pTexEmissivity;
	deoglFramebuffer pFBOMaterial;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create global illumination ray tracing. */
	deoglGIMaterials( deoglRenderThread &renderThread );
	
	/** \brief Clean up global illumination ray tracing. */
	~deoglGIMaterials();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Maximum material map size. */
	inline int GetMaxMaterialMapSize() const{ return pMaxMaterialMapSize; }
	
	/** \brief Maximum count of materials per row. */
	inline int GetMaxMaterialsPerRow() const{ return pMaxMaterialsPerRow; }
	
	/** \brief Maximum count of rows per image. */
	inline int GetMaxRowsPerImage() const{ return pMaxRowsPerImage; }
	
	/** \brief Current material map size. */
	inline int GetMaterialMapSize() const{ return pMaterialMapSize; }
	
	/** \brief Current count of materials per row. */
	inline int GetMaterialsPerRow() const{ return pMaterialsPerRow; }
	
	/** \brief Current count of rows per image. */
	inline int GetRowsPerImage() const{ return pRowsPerImage; }
	
	/** \brief Current maximum count of materials. */
	inline int GetMaxMaterialCount() const{ return pMaxMaterialCount; }
	
	
	
	/** \brief Material count. */
	inline int GetMaterialCount() const{ return pMaterialCount; }
	
	/** \brief Set material count. */
	void SetMaterialCount( int count );
	
	
	
	/** \brief Diffuse/Transparency texture. */
	inline deoglTexture &GetTextureDiffuseTransparency(){ return pTexDiffuseTransparency; }
	
	/** \brief Reflectivity/Roughness texture. */
	inline deoglTexture &GetTextureReflectivityRoughness(){ return pTexReflectivityRoughness; }
	
	/** \brief Emissivity texture. */
	inline deoglTexture &GetTextureEmissivity(){ return pTexEmissivity; }
	
	/** \brief Material FBO. */
	inline deoglFramebuffer &GetFBOMaterial(){ return pFBOMaterial; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBOMaterial();
};

#endif
