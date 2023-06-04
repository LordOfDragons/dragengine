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

#include "deoglMemoryConsumptionBufferObject.h"
#include "deoglMemoryConsumptionDeferredRendering.h"
#include "deoglMemoryConsumptionShadow.h"
#include "deoglMemoryConsumptionSkin.h"
#include "deoglMemoryConsumptionTexture.h"



/**
 * Keeps track of the memory consumption.
 */
class deoglMemoryConsumption{
public:
	deoglMemoryConsumptionTexture texture2D;
	deoglMemoryConsumptionTexture texture3D;
	deoglMemoryConsumptionTexture textureArray;
	deoglMemoryConsumptionTexture textureCube;
	
	deoglMemoryConsumptionTexture texture2DRenderable;
	deoglMemoryConsumptionTexture texture3DRenderable;
	deoglMemoryConsumptionTexture textureArrayRenderable;
	deoglMemoryConsumptionTexture textureCubeRenderable;
	
	deoglMemoryConsumptionSkin skin;
	deoglMemoryConsumptionBufferObject bufferObject;
	deoglMemoryConsumptionDeferredRendering deferredRendering;
	deoglMemoryConsumptionShadow shadow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption. */
	deoglMemoryConsumption();
	
	/** Clean up memory consumption. */
	~deoglMemoryConsumption();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total memory consumption. */
	unsigned long long Total() const;
	/*@}*/
};

#endif
