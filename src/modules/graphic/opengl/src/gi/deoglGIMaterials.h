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

#ifndef _DEOGLGIMATERIALS_H_
#define _DEOGLGIMATERIALS_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"

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
 * 
 * This class keeps a usage reference to all stored TUCs to avoid accessing deleted ones.
 * This keeps TUCs alive if nobody uses them anymore. These slots can be reclaimed if an
 * unused slot is requested.
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
	
	deoglTexture *pTexDiffuse;
	deoglTexture *pTexReflectivity;
	deoglTexture *pTexEmissivity;
	const deoglFramebuffer::Ref pFBOMaterial;
	
	
	
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
	inline deoglTexture &GetTextureDiffuse(){ return *pTexDiffuse; }
	inline const deoglTexture &GetTextureDiffuse() const{ return *pTexDiffuse; }
	
	/**
	 * Reflectivity texture.
	 * 
	 * Stores texture property "reflectivity" as RGB and texture property "roughness" as A.
	 */
	inline deoglTexture &GetTextureReflectivity(){ return *pTexReflectivity; }
	inline const deoglTexture &GetTextureReflectivity() const{ return *pTexReflectivity; }
	
	/**
	 * Emissivity texture.
	 * 
	 * Stores texture property "emissivity" as RGB and texture property "solidity" as A.
	 * Emissivity is combined with "envroom.emissivity" if present. For environment
	 * rooms the back face is used as texture.
	 */
	inline deoglTexture &GetTextureEmissivity(){ return *pTexEmissivity; }
	inline const deoglTexture &GetTextureEmissivity() const{ return *pTexEmissivity; }
	
	/** Material FBO. */
	inline const deoglFramebuffer::Ref &GetFBOMaterial(){ return pFBOMaterial; }
	/*@}*/
// 	void DEBUG();
	
	
private:
	void pCleanUp();
	void pCreateFBOMaterial();
	void pEnlarge();
	int pFirstUnusedMaterial() const;
};

#endif
