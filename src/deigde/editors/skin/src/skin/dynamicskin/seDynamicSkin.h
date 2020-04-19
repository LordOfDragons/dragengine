/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEDYNAMICSKIN_H_
#define _SEDYNAMICSKIN_H_

#include "seDynamicSkinRenderableList.h"

class seSkin;

class deDynamicSkin;



/**
 * @brief Dynamic Skin.
 * Manages a dynamic skin used to test the dynamic part of skin definitions.
 */
class seDynamicSkin{
private:
	seSkin *pParentSkin;
	
	deDynamicSkin *pEngDynamicSkin;
	
	seDynamicSkinRenderableList pRenderableList;
	seDynamicSkinRenderable *pActiveRenderable;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dynamic skin. */
	seDynamicSkin( seSkin *parentSkin );
	/** Cleans up the dynamic skin. */
	~seDynamicSkin();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent skin. */
	inline seSkin *GetParentSkin() const{ return pParentSkin; }
	
	/** Retrieves the engine dynamic skin. */
	inline deDynamicSkin *GetEngineDynamicSkin() const{ return pEngDynamicSkin; }
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the dynamic skin. */
	void Update( float elapsed );
	/** Resets the dynamic skin. */
	void Reset();
	/*@}*/
	
	/** @name Renderables */
	/*@{*/
	/** Retrieves the renderable list read-only. */
	inline const seDynamicSkinRenderableList &GetRenderableList() const{ return pRenderableList; }
	/** Adds a new renderable. */
	void AddRenderable( seDynamicSkinRenderable *renderable );
	/** Removes a renderable. */
	void RemoveRenderable( seDynamicSkinRenderable *renderable );
	/** Removes all renderables. */
	void RemoveAllRenderables();
	/** Retrieves the active renderable or NULL if none is active. */
	inline seDynamicSkinRenderable *GetActiveRenderable() const{ return pActiveRenderable; }
	/** Determines if there is an active renderable or not. */
	bool HasActiveRenderable() const;
	/** Sets the active renderable or NULL if none is active. */
	void SetActiveRenderable( seDynamicSkinRenderable *renderable );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
