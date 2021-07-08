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

#ifndef _DEOGLRENDERPLANLIGHT_H_
#define _DEOGLRENDERPLANLIGHT_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderPlan;
class deoglCollideListLight;



/**
 * Render plan light.
 */
class deoglRenderPlanLight{
private:
	deoglRenderPlan &pPlan;
	deoglCollideListLight *pLight;
	
	decDVector pPosition;
	float pDistance;
	int pReductionFactor;
	
	int pSolidShadowSizeStatic;
	int pSolidShadowSizeDynamic;
	int pSolidShadowSizeTransp;
	
	int pTranspShadowSizeStatic;
	int pTranspShadowSizeDynamic;
	int pTranspShadowSizeTransp;
	
	int pAmbientShadowSizeStatic;
	int pAmbientShadowSizeDynamic;
	int pAmbientShadowSizeTransp;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan light. */
	deoglRenderPlanLight( deoglRenderPlan &plan );
	
	/** Clean up render plan light. */
	~deoglRenderPlanLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return pPlan; }
	
	/** Light. */
	inline deoglCollideListLight *GetLight() const{ return pLight; }
	
	/** Set light. */
	void SetLight( deoglCollideListLight *light );
	
	/** Light position in world space. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Distance of light to camera. */
	inline float GetDistance() const{ return pDistance; }
	
	/** Shadow map size reduction factor. */
	inline int GetReductionFactor() const{ return pReductionFactor; }
	
	/** Init. */
	void Init();
	
	/** Plan shadow casting. */
	void PlanShadowCasting();
	
	/** Shadow size. */
	inline int GetSolidShadowSizeStatic() const{ return pSolidShadowSizeStatic; }
	inline int GetSolidShadowSizeDynamic() const{ return pSolidShadowSizeDynamic; }
	inline int GetSolidShadowSizeTransp() const{ return pSolidShadowSizeTransp; }
	
	inline int GetTranspShadowSizeStatic() const{ return pTranspShadowSizeStatic; }
	inline int GetTranspShadowSizeDynamic() const{ return pTranspShadowSizeDynamic; }
	inline int GetTranspShadowSizeTransp() const{ return pTranspShadowSizeTransp; }
	
	inline int GetAmbientShadowSizeStatic() const{ return pAmbientShadowSizeStatic; }
	inline int GetAmbientShadowSizeDynamic() const{ return pAmbientShadowSizeDynamic; }
	inline int GetAmbientShadowSizeTransp() const{ return pAmbientShadowSizeTransp; }
	
	/** Set shadow size. */
	void SetSolidShadowSizeStatic( int size );
	void SetSolidShadowSizeDynamic( int size );
	void SetSolidShadowSizeTransp( int size );
	
	void SetTranspShadowSizeStatic( int size );
	void SetTranspShadowSizeDynamic( int size );
	void SetTranspShadowSizeTransp( int size );
	
	void SetAmbientShadowSizeStatic( int size );
	void SetAmbientShadowSizeDynamic( int size );
	void SetAmbientShadowSizeTransp( int size );
	/*@}*/
	
	
	
private:
	void pCalcReductionFactor();
};

#endif
