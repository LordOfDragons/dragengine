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
#include <dragengine/common/utils/decLayerMask.h>

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
	int pReductionFactorStatic;
	int pReductionFactorDynamic;
	
	int pShadowSizeStatic;
	int pTranspShadowSizeStatic;
	int pAmbientShadowSizeStatic;
	
	int pShadowSizeDynamic;
	int pTranspShadowSizeDynamic;
	int pAmbientShadowSizeDynamic;
	
	int pGIShadowSizeDynamic;
	
	bool pUseShadow;
	bool pUseShadowTemporary;
	bool pUseAmbient;
	bool pRefilterShadows;
	
	
	
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
	
	/** Init. */
	void Init();
	
	/** Plan shadow casting. */
	void PlanShadowCasting();
	
	/** Shadow size. */
	inline int GetShadowSizeStatic() const{ return pShadowSizeStatic; }
	inline int GetTranspShadowSizeStatic() const{ return pTranspShadowSizeStatic; }
	inline int GetAmbientShadowSizeStatic() const{ return pAmbientShadowSizeStatic; }
	
	inline int GetShadowSizeDynamic() const{ return pShadowSizeDynamic; }
	inline int GetTranspShadowSizeDynamic() const{ return pTranspShadowSizeDynamic; }
	inline int GetAmbientShadowSizeDynamic() const{ return pAmbientShadowSizeDynamic; }
	
	inline int GetGIShadowSizeDynamic() const{ return pGIShadowSizeDynamic; }
	
	/** Set shadow size. */
	void SetShadowSizeStatic( int size );
	void SetTranspShadowSizeStatic( int size );
	void SetAmbientShadowSizeStatic( int size );
	
	void SetShadowSizeDynamic( int size );
	void SetTranspShadowSizeDynamic( int size );
	void SetAmbientShadowSizeDynamic( int size );
	
	/** Render switches. */
	inline bool GetUseShadow() const{ return pUseShadow; }
	inline bool GetUseShadowTemporary() const{ return pUseShadowTemporary; }
	inline bool GetUseAmbient() const{ return pUseAmbient; }
	inline bool GetRefilterShadows() const{ return pRefilterShadows; }
	/*@}*/
	
	
	
private:
	void pCalcReductionFactorStatic();
	void pCalcReductionFactorDynamic();
	void pDetermineUseShadow();
	void pDetermineUseAmbient();
	void pDetermineRefilterShadows();
};

#endif
