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

// include only once
#ifndef _DEOGLOPTIMIZERLIGHT_H_
#define _DEOGLOPTIMIZERLIGHT_H_

// includes
#include "dragengine/common/math/decMath.h"
#include "dragengine/common/utils/decTimer.h"
#include "deoglOptimizer.h"

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
 * @brief Light Optimizer.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new optimizer. */
	deoglOptimizerLight( deoglRLight *light, deoglWorld *world );
	/** Cleans up the optimizer. */
	virtual ~deoglOptimizerLight();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the light to optimize. */
	inline deoglRLight *GetLight() const{ return pLight; }
	/** Runs optimizer until the given time slice has elapsed. */
	virtual bool Run( int timeSlice );
	
	/** Retrieves the initial warm up time in ms. */
	inline int GetInitialWarmUpTime() const{ return pInitWarmUpTime; }
	/** Sets the initial warm up time in ms. */
	void SetInitialWarmUpTime( int warmUpTime );
	
	/** Determines if the light volume will be optimized. */
	inline bool GetOptimizeLightVolume() const{ return pDoOptimizeLightVolume; }
	/** Sets if the light volume will be optimized. */
	void SetOptimizeLightVolume( bool optimizeLightVolume );
	/** Resets the light volume optimization. */
	void ResetLightVolume();
	/** Retrieves the maximal volume count. */
	inline int GetMaximalVolumeCount() const{ return pMaxVolumeCount; }
	/** Sets the maximal volume count. */
	void SetMaximalVolumeCount( int maximalVolumeCount );
	
	/** Determines if the shadow casters will be optimized. */
	inline bool GetOptimizeShadowCaster() const{ return pDoOptimizeShadowCaster; }
	/** Sets if the shadow casters will be optimized. */
	void SetOptimizeShadowCaster( bool optimizeShadowCaster );
	/** Resets the shadow caster optimization. */
	void ResetShadowCaster();
	
	/** Resets all optimizations. */
	void ResetAllOptimizations();
	/*@}*/
	
private:
	void pCleanUp();
	bool pOptimizeLightVolume( int timeSlice );
	void pBuildTriangleList();
	void pPrepareCroping();
	bool pCropLightVolume( int timeSlice );
	bool pOptimizeShadowCaster( int timeSlice );
	void pSCBuildCollideList();
	void pSCBuildShadowCaster();
	decVector pGetSectorOffset( const decVector &sectorSize, const decPoint3 &sectorFrom, const decPoint3 &sectorTo ) const;
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal );
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal );
};

// end of include only once
#endif
