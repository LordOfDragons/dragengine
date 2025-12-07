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
	deoglRenderPlanLight(deoglRenderPlan &plan);
	
	/** Clean up render plan light. */
	~deoglRenderPlanLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{return pPlan;}
	
	/** Light. */
	inline deoglCollideListLight *GetLight() const{return pLight;}
	
	/** Set light. */
	void SetLight(deoglCollideListLight *light);
	
	/** Light position in world space. */
	inline const decDVector &GetPosition() const{return pPosition;}
	
	/** Distance of light to camera. */
	inline float GetDistance() const{return pDistance;}
	
	/** Init. */
	void Init();
	
	/** Plan shadow casting. */
	void PlanShadowCasting();
	
	/** Shadow size. */
	inline int GetShadowSizeStatic() const{return pShadowSizeStatic;}
	inline int GetTranspShadowSizeStatic() const{return pTranspShadowSizeStatic;}
	inline int GetAmbientShadowSizeStatic() const{return pAmbientShadowSizeStatic;}
	
	inline int GetShadowSizeDynamic() const{return pShadowSizeDynamic;}
	inline int GetTranspShadowSizeDynamic() const{return pTranspShadowSizeDynamic;}
	inline int GetAmbientShadowSizeDynamic() const{return pAmbientShadowSizeDynamic;}
	
	inline int GetGIShadowSizeDynamic() const{return pGIShadowSizeDynamic;}
	
	/** Set shadow size. */
	void SetShadowSizeStatic(int size);
	void SetTranspShadowSizeStatic(int size);
	void SetAmbientShadowSizeStatic(int size);
	
	void SetShadowSizeDynamic(int size);
	void SetTranspShadowSizeDynamic(int size);
	void SetAmbientShadowSizeDynamic(int size);
	
	/** Render switches. */
	inline bool GetUseShadow() const{return pUseShadow;}
	inline bool GetUseShadowTemporary() const{return pUseShadowTemporary;}
	inline bool GetUseAmbient() const{return pUseAmbient;}
	inline bool GetRefilterShadows() const{return pRefilterShadows;}
	/*@}*/
	
	
	
private:
	void pCalcReductionFactorStatic();
	void pCalcReductionFactorDynamic();
	void pDetermineUseShadow();
	void pDetermineUseAmbient();
	void pDetermineRefilterShadows();
};

#endif
