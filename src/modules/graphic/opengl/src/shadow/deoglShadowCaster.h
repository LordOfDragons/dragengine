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
 * \brief Shadow caster.
 */
class deoglShadowCaster{
public:
	/** \brief Shadow types. */
	enum eShadowTypes{
		/** \brief No shadows. */
		estNoShadows,
		
		/** \brief Static shadow map only. */
		estStaticOnly,
		
		/** \brief Dynamic shadow map only. */
		estDynamicOnly,
		
		/** \brief Static and dynamic shadow map. */
		estStaticAndDynamic
	};
	
	
	
private:
	deoglSCSolid pSolid;
	deoglSCTransparent pTransparent;
	deoglSCAmbient pAmbient;
	
	decVector pShadowOrigin;
	eShadowTypes pShadowType;
	
	float pStaticNear;
	float pStaticFar;
	float pStaticScale;
	float pStaticOffset;
	float pStaticCutOff;
	
	float pDynamicNear;
	float pDynamicFar;
	float pDynamicScale;
	float pDynamicOffset;
	float pDynamicCutOff;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shadow caster. */
	deoglShadowCaster( deoglRenderThread &renderThread );
	
	/** \brief Clean up shadow caster. */
	~deoglShadowCaster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Update shadow caster.
	 * \details Checks if cube maps are not been used for the last couple of frames removing them.
	 * \note Has to be done better to avoid cluttering GPU RAM.
	 */
	void Update();
	
	/** \brief Solid shadow caster. */
	inline deoglSCSolid &GetSolid(){ return pSolid; }
	inline const deoglSCSolid &GetSolid() const{ return pSolid; }
	
	/** \brief Transparent shadow caster. */
	inline deoglSCTransparent &GetTransparent(){ return pTransparent; }
	inline const deoglSCTransparent &GetTransparent() const{ return pTransparent; }
	
	/** \brief Ambient shadow caster. */
	inline deoglSCAmbient &GetAmbient(){ return pAmbient; }
	inline const deoglSCAmbient &GetAmbient() const{ return pAmbient; }
	
	/** \brief Clear shadow caster. */
	void Clear();
	
	/** \brief Drop dynamic maps. */
	void DropDynamic();
	
	
	
	/** \brief Shadow origin for 180-point lights. */
	inline const decVector &GetShadowOrigin() const{ return pShadowOrigin; }
	
	/** \brief Set shadow orign for 180point lights. */
	void SetShadowOrigin( const decVector &origin );
	
	/** \brief Shadow type. */
	inline eShadowTypes GetShadowType() const{ return pShadowType; }
	
	/** \brief Set shadow type. */
	void SetShadowType( eShadowTypes shadowType );
	
	
	
	/** \brief Near distance for static shadows. */
	inline float GetStaticNear() const{ return pStaticNear; }
	
	/** \brief Far distance for static shadows. */
	inline float GetStaticFar() const{ return pStaticFar; }
	
	/** \brief Scale factor for static shadows. */
	inline float GetStaticScale() const{ return pStaticScale; }
	
	/** \brief Offset for static shadows. */
	inline float GetStaticOffset() const{ return pStaticOffset; }
	
	/** \brief Set static shadows parameters. */
	void SetStaticParams( float near, float far );
	
	/** \brief Set cut-off for static shadows of 180-point lights. */
	inline float GetStaticCutOff() const{ return pStaticCutOff; }
	
	/** \brief Set cut-off for static shadows of 180-point lights. */
	void SetStaticCutOff( float cutoff );
	
	
	
	/** \brief Near distance for dynamic shadows. */
	inline float GetDynamicNear() const{ return pDynamicNear; }
	
	/** \brief Far distance for dynamic shadows. */
	inline float GetDynamicFar() const{ return pDynamicFar; }
	
	/** \brief Scale factor for dynamic shadows. */
	inline float GetDynamicScale() const{ return pDynamicScale; }
	
	/** \brief Offset for dynamic shadows. */
	inline float GetDynamicOffset() const{ return pDynamicOffset; }
	
	/** \brief Set dynamic shadows parameters. */
	void SetDynamicParams( float near, float far );
	
	/** \brief Set cut-off for dynamic shadows of 180-point lights. */
	inline float GetDynamicCutOff() const{ return pDynamicCutOff; }
	
	/** \brief Set cut-off for dynamic shadows of 180-point lights. */
	void SetDynamicCutOff( float cutoff );
	/*@}*/
};

#endif
