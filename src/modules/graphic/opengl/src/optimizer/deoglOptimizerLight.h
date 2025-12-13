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

// include only once
#ifndef _DEOGLOPTIMIZERLIGHT_H_
#define _DEOGLOPTIMIZERLIGHT_H_

// includes
#include "deoglOptimizer.h"
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

// predefinitions
class deoglRLight;
class deoglWorld;
class deoglTriangleSorter;
class deoglCollideList;
class deoglShadowCaster;
class decTimer;
class decConvexVolumeList;
class decConvexVolume;



/**
 * Light Optimizer.
 * Optimizer for Scene Light objects. The Light Optimizer is able to
 * refine the light volume as well as narrowing down the list of
 * shadow casting faces.
 */
class deoglOptimizerLight : public deoglOptimizer{
private:
	deoglRLight *pLight;
	deoglWorld *pWorld;
	
	// warm up
	decTimer pWarmUpTimer;
	int pInitWarmUpTime;
	int pWarmUpTime;
	
	// light volume optimization
	bool pDoOptimizeLightVolume;
	deoglTriangleSorter *pTriangleSorter;
	decConvexVolumeList *pConvexVolumeList;
	int pMaxVolumeCount;
	int pNextTriangle;
	float pSafeRadius;
	int pCropStage;
	
	// shadow caster optimization
	bool pDoOptimizeShadowCaster;
	deoglCollideList *pCollideList;
	deoglCollideList *pFinalCollideList;
	deoglShadowCaster *pShadowCaster;
	int pNextTexture;
	int pNextFace;
	int pCasterStage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new optimizer. */
	deoglOptimizerLight(deoglRLight *light, deoglWorld *world);
	/** Cleans up the optimizer. */
	~deoglOptimizerLight() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the light to optimize. */
	inline deoglRLight *GetLight() const{ return pLight; }
	/** Runs optimizer until the given time slice has elapsed. */
	bool Run(int timeSlice) override;
	
	/** Retrieves the initial warm up time in ms. */
	inline int GetInitialWarmUpTime() const{ return pInitWarmUpTime; }
	/** Sets the initial warm up time in ms. */
	void SetInitialWarmUpTime(int warmUpTime);
	
	/** Determines if the light volume will be optimized. */
	inline bool GetOptimizeLightVolume() const{ return pDoOptimizeLightVolume; }
	/** Sets if the light volume will be optimized. */
	void SetOptimizeLightVolume(bool optimizeLightVolume);
	/** Resets the light volume optimization. */
	void ResetLightVolume();
	/** Retrieves the maximal volume count. */
	inline int GetMaximalVolumeCount() const{ return pMaxVolumeCount; }
	/** Sets the maximal volume count. */
	void SetMaximalVolumeCount(int maximalVolumeCount);
	
	/** Determines if the shadow casters will be optimized. */
	inline bool GetOptimizeShadowCaster() const{ return pDoOptimizeShadowCaster; }
	/** Sets if the shadow casters will be optimized. */
	void SetOptimizeShadowCaster(bool optimizeShadowCaster);
	/** Resets the shadow caster optimization. */
	void ResetShadowCaster();
	
	/** Resets all optimizations. */
	void ResetAllOptimizations();
	/*@}*/
	
private:
	void pCleanUp();
	bool pOptimizeLightVolume(int timeSlice);
	void pBuildTriangleList();
	void pPrepareCroping();
	bool pCropLightVolume(int timeSlice);
	bool pOptimizeShadowCaster(int timeSlice);
	void pSCBuildCollideList();
	void pSCBuildShadowCaster();
	decVector pGetSectorOffset(const decVector &sectorSize, const decPoint3 &sectorFrom, const decPoint3 &sectorTo) const;
	void pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal);
	void pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal);
};

// end of include only once
#endif
