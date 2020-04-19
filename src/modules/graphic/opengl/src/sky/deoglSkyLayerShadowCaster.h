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

#ifndef _DEOGLSKYLAYERSHADOWCASTER_H_
#define _DEOGLSKYLAYERSHADOWCASTER_H_

#include <dragengine/common/math/decMath.h>

class deoglArrayTexture;
class deoglRenderThread;



/**
 * @brief Sky Layer Shadow Caster.
 */
class deoglSkyLayerShadowCaster{
public:
	struct sLayer{
		float frustumNear;
		float frustumFar;
		float layerBorder;
		float zscale;
		float zoffset;
		decVector minExtend;
		decVector maxExtend;
		decVector position;
		decVector scale;
		decMatrix matrix;
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	sLayer *pLayers;
	int pLayerCount;
	
	deoglArrayTexture *pSolidShadowMap;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new sky layer shadow caster. */
	deoglSkyLayerShadowCaster( deoglRenderThread &renderThread );
	/** Cleans up the sky layer shadow caster. */
	~deoglSkyLayerShadowCaster();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread, */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the layers. */
	sLayer *GetLayers() const{ return pLayers; }
	/** Retrieves the number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	/** Sets the number of layers. */
	void SetLayerCount( int layerCount );
	
	/** Retrieves the solid shadow map. */
	deoglArrayTexture *GetSolidShadowMap( int size );
	/** Drops the solid shadow map. */
	void DropSolidShadowMap();
	/*@}*/
};

#endif
