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

#ifndef _DEOGLSKINRENDERED_H_
#define _DEOGLSKINRENDERED_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglRenderPlan;
class deoglRenderThread;
class deoglRSkin;
class deoglSkinRenderedTexture;
class deoglSkinTexture;
class deoglRComponent;
class deoglRBillboard;
class deoglRDecal;


/**
 * Skin rendered.
 * 
 * Helper class for handling rendered skin textures.
 */
class deoglSkinRendered{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRComponent *pOwnerComponent;
	deoglRBillboard *pOwnerBillboard;
	deoglRDecal *pOwnerDecal;
	
	decPointerList pTextures;
	
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin rendered. */
	deoglSkinRendered(deoglRenderThread &renderThread, deoglRComponent &component);
	deoglSkinRendered(deoglRenderThread &renderThread, deoglRBillboard &billboard);
	deoglSkinRendered(deoglRenderThread &renderThread, deoglRDecal &decal);
	
	/** Clean up skin rendered. */
	~deoglSkinRendered();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	
	
	/** Owner component or NULL. */
	inline deoglRComponent *GetOwnerComponent() const{return pOwnerComponent;}
	
	/** Owner billboard or NULL. */
	inline deoglRBillboard *GetOwnerBillboard() const{return pOwnerBillboard;}
	
	/** Owner decal or NULL. */
	inline deoglRDecal *GetOwnerDecal() const{return pOwnerDecal;}
	
	/** Owner skin or NULL. */
	deoglRSkin *GetOwnerSkin() const;
	
	
	
	/** Number of textures. */
	int GetTexturedCount() const;
	
	/** Texture at index. */
	deoglSkinRenderedTexture *GetTextureAt(int index) const;
	
	/** Add texture. */
	deoglSkinRenderedTexture *AddTexture(deoglRSkin &skin, int texture, int modelTexture);
	
	/** Remove all textures. */
	void RemoveAllTextures();
	
	
	
	/** Mark dirty. */
	void SetDirty();
	
	/** Prepare if dirty. */
	void Prepare();
	
	/** Add render plans. */
	void AddRenderPlans(deoglRenderPlan &plan);
	
	
	
	/** Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	/*@}*/
	
	
	
private:
	void pAddTextureIfRequired(deoglRSkin &skin, int texture, int modelTexture);
};

#endif
