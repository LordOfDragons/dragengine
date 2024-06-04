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

#ifndef _DEOGLSHADOWCASTER_H_
#define _DEOGLSHADOWCASTER_H_

#include "deoglSCSolid.h"
#include "deoglSCTransparent.h"
#include "deoglSCAmbient.h"
#include "../renderthread/deoglRTFrameCounterTracker.h"

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
	deoglRTFrameCounterTracker pFrameCounterTracker;
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
	
	/** Drop temporary maps. */
	void DropTemporary();
	
	
	
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
