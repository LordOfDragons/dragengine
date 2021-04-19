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

#ifndef _DEOGLCOMPONENTCOMPONENT_H_
#define _DEOGLCOMPONENTCOMPONENT_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

class deoglComponent;
class deoglDynamicSkin;
class deoglRComponentTexture;
class deoglSkin;
class deoglSkinStateController;
class deComponentTexture;



/**
 * Component texture.
 */
class deoglComponentTexture : public deoglDynamicSkinListener{
private:
	deoglComponent &pComponent;
	const int pIndex;
	deoglRComponentTexture *pRTexture;
	
	deoglSkinStateController *pSkinStateController;
	
	deoglSkin *pSkin;
	deoglDynamicSkin *pDynamicSkin;
	
	bool pDirtyTexture;
	bool pDynamicSkinRenderablesChanged;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component texture. */
	deoglComponentTexture( deoglComponent &component, int index );
	
	/** Clean up component texture. */
	~deoglComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent component. */
	inline deoglComponent &GetComponent() const{ return pComponent; }
	
	/** Texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Skin state controller. */
	inline deoglSkinStateController *GetSkinStateController() const{ return pSkinStateController; }
	
	/** Dynamic skin. */
	inline deoglDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Render component texture. */
	inline deoglRComponentTexture *GetRTexture() const{ return pRTexture; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	inline bool GetDynamicSkinRenderablesChanged() const{ return pDynamicSkinRenderablesChanged; }
	void SetDynamicSkinRenderablesChanged( bool changed );
	
	
	
	/** Init skin state. */
	void InitSkinState();
	
	/** Advance time. */
	void AdvanceTime( float timeStep );
	
	/** Clear skin state controller. */
	void ClearSkinStateController();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged( deoglDSRenderable &renderable );
	virtual void DynamicSkinRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Texture changed. */
	void TextureChanged( const deComponentTexture &texture );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
