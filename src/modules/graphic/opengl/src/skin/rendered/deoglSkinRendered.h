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
 * \brief Skin rendered.
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
	/** \brief Create skin rendered. */
	deoglSkinRendered( deoglRenderThread &renderThread, deoglRComponent &component );
	deoglSkinRendered( deoglRenderThread &renderThread, deoglRBillboard &billboard );
	deoglSkinRendered( deoglRenderThread &renderThread, deoglRDecal &decal );
	
	/** \brief Clean up skin rendered. */
	~deoglSkinRendered();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Owner component or NULL. */
	inline deoglRComponent *GetOwnerComponent() const{ return pOwnerComponent; }
	
	/** \brief Owner billboard or NULL. */
	inline deoglRBillboard *GetOwnerBillboard() const{ return pOwnerBillboard; }
	
	/** \brief Owner decal or NULL. */
	inline deoglRDecal *GetOwnerDecal() const{ return pOwnerDecal; }
	
	/** \brief Owner skin or NULL. */
	deoglRSkin *GetOwnerSkin() const;
	
	
	
	/** \brief Number of textures. */
	int GetTexturedCount() const;
	
	/** \brief Texture at index. */
	deoglSkinRenderedTexture *GetTextureAt( int index ) const;
	
	/** \brief Add texture. */
	deoglSkinRenderedTexture *AddTexture( deoglRSkin &skin, int texture, int modelTexture );
	
	/** \brief Remove all textures. */
	void RemoveAllTextures();
	
	
	
	/** \brief Mark dirty. */
	void SetDirty();
	
	/** \brief Prepare if dirty. */
	void Prepare();
	
	/** \brief Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** \brief Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	/*@}*/
	
	
	
private:
	void pAddTextureIfRequired( deoglRSkin &skin, int texture, int modelTexture );
};

#endif
