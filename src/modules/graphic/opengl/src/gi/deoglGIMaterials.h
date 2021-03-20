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
#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglTexUnitsConfig;



/**
 * Global illumination materials.
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
	
	decPointerList pTUCs;
	
	deoglTexture pTexDiffuse;
	deoglTexture pTexReflectivity;
	deoglTexture pTexEmissivity;
	deoglFramebuffer pFBOMaterial;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGIMaterials( deoglRenderThread &renderThread );
	
	/** Clean up global illumination ray tracing. */
	~deoglGIMaterials();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Maximum material map size. */
	inline int GetMaxMaterialMapSize() const{ return pMaxMaterialMapSize; }
	
	/** Maximum count of materials per row. */
	inline int GetMaxMaterialsPerRow() const{ return pMaxMaterialsPerRow; }
	
	/** Maximum count of rows per image. */
	inline int GetMaxRowsPerImage() const{ return pMaxRowsPerImage; }
	
	/** Current material map size. */
	inline int GetMaterialMapSize() const{ return pMaterialMapSize; }
	
	/** Current count of materials per row. */
	inline int GetMaterialsPerRow() const{ return pMaterialsPerRow; }
	
	/** Current count of rows per image. */
	inline int GetRowsPerImage() const{ return pRowsPerImage; }
	
	/** Current maximum count of materials. */
	inline int GetMaxMaterialCount() const{ return pMaxMaterialCount; }
	
	
	
	/** Add texture units configuration. Assigns material slot to "tuc". */
	void AddTUC( deoglTexUnitsConfig *tuc );
	
	/** Texture units configuration assigned to slot or NULL. */
	deoglTexUnitsConfig *GetTUC( int materialIndex ) const;
	
	
	
	/**
	 * Diffuse texture.
	 * 
	 * Stores texture property "color" as RGB and texture property "color.tint.mask" as A.
	 * Color is combined with texture property "envroom" if present. For environment
	 * rooms the back face is used as texture.
	 */
	inline deoglTexture &GetTextureDiffuse(){ return pTexDiffuse; }
	inline const deoglTexture &GetTextureDiffuse() const{ return pTexDiffuse; }
	
	/**
	 * Reflectivity texture.
	 * 
	 * Stores texture property "reflectivity" as RGB and texture property "roughness" as A.
	 */
	inline deoglTexture &GetTextureReflectivity(){ return pTexReflectivity; }
	inline const deoglTexture &GetTextureReflectivity() const{ return pTexReflectivity; }
	
	/**
	 * Emissivity texture.
	 * 
	 * Stores texture property "emissivity" as RGB and texture property "solidity" as A.
	 * Emissivity is combined with "envroom.emissivity" if present. For environment
	 * rooms the back face is used as texture.
	 */
	inline deoglTexture &GetTextureEmissivity(){ return pTexEmissivity; }
	inline const deoglTexture &GetTextureEmissivity() const{ return pTexEmissivity; }
	
	/** Material FBO. */
	inline deoglFramebuffer &GetFBOMaterial(){ return pFBOMaterial; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBOMaterial();
	void pEnlarge();
	int pFirstUnusedMaterial() const;
};

#endif
