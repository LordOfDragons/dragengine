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

#ifndef _DEOGLMEMORYCONSUMPTION_H_
#define _DEOGLMEMORYCONSUMPTION_H_

#include "deoglMemoryConsumptionTexture.h"
#include "deoglMemoryConsumptionVBO.h"



/**
 * @brief Memory Consumption.
 * Keeps track of the current memory consumption.
 */
class deoglMemoryConsumption{
private:
	deoglMemoryConsumptionTexture pTexture1D;
	deoglMemoryConsumptionTexture pTexture1DRenderable;
	
	deoglMemoryConsumptionTexture pTexture2D;
	deoglMemoryConsumptionTexture pTexture2DRenderable;
	
	deoglMemoryConsumptionTexture pTexture3D;
	deoglMemoryConsumptionTexture pTexture3DRenderable;
	
	deoglMemoryConsumptionTexture pTextureArray;
	deoglMemoryConsumptionTexture pTextureArrayRenderable;
	
	deoglMemoryConsumptionTexture pTextureCube;
	deoglMemoryConsumptionTexture pTextureCubeRenderable;
	
	deoglMemoryConsumptionTexture pRenderbuffer;
	deoglMemoryConsumptionTexture pSkin;
	deoglMemoryConsumptionVBO pVBO;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new memory consumption. */
	deoglMemoryConsumption();
	/** Cleans up the memory consumption. */
	~deoglMemoryConsumption();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Retrieves the 1d texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture1D(){ return pTexture1D; }
	/** \brief Retrieves the 1d renderable texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture1DRenderable(){ return pTexture1DRenderable; }
	
	/** Retrieves the 2d texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture2D(){ return pTexture2D; }
	/** Retrieves the 2d renderable texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture2DRenderable(){ return pTexture2DRenderable; }
	
	/** Retrieves the 3d texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture3D(){ return pTexture3D; }
	/** Retrieves the 3d renderable texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTexture3DRenderable(){ return pTexture3DRenderable; }
	
	/** Retrieves the array texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTextureArray(){ return pTextureArray; }
	/** Retrieves the renderable array texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTextureArrayRenderable(){ return pTextureArrayRenderable; }
	
	/** Retrieves the cube texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTextureCube(){ return pTextureCube; }
	/** Retrieves the renderable cube texture memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetTextureCubeRenderable(){ return pTextureCubeRenderable; }
	
	/** Retrieves the render buffer memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetRenderbuffer(){ return pRenderbuffer; }
	/** Retrieves the skin memory consumption tracker. */
	inline deoglMemoryConsumptionTexture &GetSkin(){ return pSkin; }
	/** Retrieves the vbo memory consumption tracker. */
	inline deoglMemoryConsumptionVBO &GetVBO(){ return pVBO; }
	/*@}*/
};

#endif
