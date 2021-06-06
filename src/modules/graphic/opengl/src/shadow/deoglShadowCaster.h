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

#ifndef _DEOGLSHADOWCASTER_H_
#define _DEOGLSHADOWCASTER_H_

#include "deoglSCSolid.h"
#include "deoglSCTransparent.h"
#include "deoglSCAmbient.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglRenderThread;
class deoglRenderableDepthCubeMap;
class deoglRenderableDepthTexture;



/**
 * Shadow caster.
 */
class deoglShadowCaster{
public:
	/** Shadow types. */
	enum eShadowTypes{
		/** No shadows. */
		estNoShadows,
		
		/** Static shadow map only. */
		estStaticOnly,
		
		/** Dynamic shadow map only. */
		estDynamicOnly,
		
		/** Static and dynamic shadow map. */
		estStaticAndDynamic
	};
	
	/** Shadow layers. */
	struct sShadowLayer{
		float layerBorder;
		decVector scale;
		decMatrix matrix;
	};
	
	
	
private:
	deoglSCSolid pSolid;
	deoglSCTransparent pTransparent;
	deoglSCAmbient pAmbient;
	
	eShadowTypes pShadowType;
	
	float pStaticNear;
	float pStaticFar;
	float pStaticScale;
	float pStaticOffset;
	
	float pDynamicNear;
	float pDynamicFar;
	float pDynamicScale;
	float pDynamicOffset;
	
	sShadowLayer *pShadowLayers;
	int pShadowLayerCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shadow caster. */
	deoglShadowCaster( deoglRenderThread &renderThread );
	
	/** Clean up shadow caster. */
	~deoglShadowCaster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Update shadow caster.
	 * \details Checks if cube maps are not been used for the last couple of frames removing them.
	 * \note Has to be done better to avoid cluttering GPU RAM.
	 */
	void Update();
	
	/** Shadow caster requires update. True if timers are running to drop textures. */
	bool RequiresUpdate() const;
	
	/** Solid shadow caster. */
	inline deoglSCSolid &GetSolid(){ return pSolid; }
	inline const deoglSCSolid &GetSolid() const{ return pSolid; }
	
	/** Transparent shadow caster. */
	inline deoglSCTransparent &GetTransparent(){ return pTransparent; }
	inline const deoglSCTransparent &GetTransparent() const{ return pTransparent; }
	
	/** Ambient shadow caster. */
	inline deoglSCAmbient &GetAmbient(){ return pAmbient; }
	inline const deoglSCAmbient &GetAmbient() const{ return pAmbient; }
	
	/** Clear shadow caster. */
	void Clear();
	
	/** Drop dynamic maps. */
	void DropDynamic();
	
	
	
	/** Shadow type. */
	inline eShadowTypes GetShadowType() const{ return pShadowType; }
	
	/** Set shadow type. */
	void SetShadowType( eShadowTypes shadowType );
	
	
	
	/** Near distance for static shadows. */
	inline float GetStaticNear() const{ return pStaticNear; }
	
	/** Far distance for static shadows. */
	inline float GetStaticFar() const{ return pStaticFar; }
	
	/** Scale factor for static shadows. */
	inline float GetStaticScale() const{ return pStaticScale; }
	
	/** Offset for static shadows. */
	inline float GetStaticOffset() const{ return pStaticOffset; }
	
	/** Set static shadows parameters. */
	void SetStaticParams( float near, float far );
	
	
	
	/** Near distance for dynamic shadows. */
	inline float GetDynamicNear() const{ return pDynamicNear; }
	
	/** Far distance for dynamic shadows. */
	inline float GetDynamicFar() const{ return pDynamicFar; }
	
	/** Scale factor for dynamic shadows. */
	inline float GetDynamicScale() const{ return pDynamicScale; }
	
	/** Offset for dynamic shadows. */
	inline float GetDynamicOffset() const{ return pDynamicOffset; }
	
	/** Set dynamic shadows parameters. */
	void SetDynamicParams( float near, float far );
	
	
	
	/** Shadow layer count. */
	inline int GetShadowLayerCount() const{ return pShadowLayerCount; }
	
	/** Set shadow layer count. Clears shadow layer parameters if count changed. */
	void SetShadowLayerCount( int count );
	
	/** Shadow layer at index. */
	sShadowLayer &GetShadowLayerAt( int index );
	const sShadowLayer &GetShadowLayerAt( int index ) const;
	/*@}*/
};

#endif
