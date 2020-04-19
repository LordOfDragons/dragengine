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

#ifndef _DEOGLENVIONMENTMAP_H_
#define _DEOGLENVIONMENTMAP_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include "../utils/collision/deoglDCollisionBox.h"
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>

#include "../billboard/deoglBillboardList.h"
#include "../component/deoglComponentList.h"
#include "../particle/deoglParticleEmitterInstanceList.h"
#include "../rendering/plan/deoglRenderPlanList.h"

class deoglRLight;
class decConvexVolumeList;
class deoglCubeMap;
class deoglLightVolume;
class deoglRenderThread;
class deoglRWorld;
class deoglTexture;
class deoglWorldOctree;



/**
 * @brief Environment Map.
 * Stores an environment map. Provides support for updating the environment map.
 * Individual world elements can be included or excluded.
 */
class deoglEnvironmentMap : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pWorld;
	decDVector pPosition;
	deoglWorldOctree *pOctreeNode;
	
	bool pSkyOnly;
	
	int pSize;
	bool pIsFloat;
	
	deoglCubeMap *pEnvMap;
	deoglCubeMap *pEnvMapDepth;
	deoglTexture *pEnvMapEqui;
	deoglTexture *pEnvMapEquiDepth;
	int pMaxMipMapLevel;
	
	decLayerMask pLayerMask;
	
	decDMatrix pMatrixInfluenceBox;
	decVector pInfluenceBoxBorderFactor;
	decVector pInfluenceBoxScale;
	bool pHasInfluenceBox;
	deoglDCollisionBox pInfluenceCollisionBox;
	int pInfluencePriority;
	
	decDMatrix pMatrixReflectionBox;
	bool pHasReflectionBox;
	decDMatrix *pReflectionMaskBoxMatrices;
	int pReflectionMaskBoxMatrixCount;
	
	bool pDirty;
	bool pDirtyInit;
	bool pDirtyOctreeNode;
	bool pReady;
	int pNextUpdateFace;
	
	int pPlanUsageCount;
	bool pDestroyIfUnused;
	
	deoglBillboardList pBillboardList;
	deoglComponentList pComponentList;
	deoglParticleEmitterInstanceList pParticleEmitterInstanceList;
	deoglRenderPlanList pRenderPlanList;
	
	bool pMarked;
	
	int pSlotIndex;
	
	decConvexVolumeList *pConvexVolumeList;
	deoglLightVolume *pLightVolume;
	bool pDirtyConvexVolumeList;
	bool pDirtyCubeMapHardLimit;
	bool pDirtyCubeMapSoftLimit;
	deoglCubeMap *pCubeMapHardLimit;
	deoglCubeMap *pCubeMapSoftLimit;
	float pCubeMapLimitScale;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new environment map. */
	deoglEnvironmentMap( deoglRenderThread &renderThread );
	/** Cleans up the environment map. */
	virtual ~deoglEnvironmentMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent world or NULL if there is none. */
	deoglRWorld *GetWorld() const{ return pWorld; }
	/** Sets the parent world or NULL if there is none. */
	void SetWorld( deoglRWorld *world );
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decDVector &position );
	
	/** Updates the octree position. */
	void UpdateOctreePosition();
	/** Retrieves the octree node or NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	/** Sets the octree node or NULL if there is none. */
	void SetOctreeNode( deoglWorldOctree *node );
	
	/** Determines if the sky only is rendered hence the env map is positionless. */
	inline bool GetSkyOnly() const{ return pSkyOnly; }
	/** Sets if the sky only is rendered hence the env map is positionless. */
	void SetSkyOnly( bool skyOnly );
	
	/** Retrieves the size. */
	inline int GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize( int size );
	/** Determines if the environment map is of float format. */
	inline bool GetIsFloat() const{ return pIsFloat; }
	/** Sets if the environment map is of float format. */
	void SetIsFloat( bool isFloat );
	/** Retrieves the maximum mip map level. */
	inline int GetMaxMipMapLevel() const{ return pMaxMipMapLevel; }
	
	/** Retrieves the layer mask. */
	inline decLayerMask &GetLayerMask(){ return pLayerMask; }
	/** Sets the layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Retrieves the influence box matrix. */
	inline const decDMatrix &GetMatrixInfluenceBox() const{ return pMatrixInfluenceBox; }
	/** Sets the influence box matrix. */
	void SetMatrixInfluenceBox( const decDMatrix &matrix );
	/** Retrieves the influence box border factor. */
	inline const decVector &GetInfluenceBoxBorderFactor() const{ return pInfluenceBoxBorderFactor; }
	/** Sets the influence box border factor. */
	void SetInfluenceBoxBorderFactor( const decVector &factor );
	/** Retrieves the influence box scale. */
	inline const decVector &GetInfluenceBoxScale() const{ return pInfluenceBoxScale; }
	/** Sets the influence box scale. */
	void SetInfluenceBoxScale( const decVector &scale );
	/** Determines if an influence box exists. */
	inline bool GetHasInfluenceBox() const{ return pHasInfluenceBox; }
	/** Sets if a influence box exists. */
	void SetHasInfluenceBox( bool hasInfluenceBox );
	/** Retrieves the influence collision box. */
	inline deoglDCollisionBox &GetInfluenceCollisionBox(){ return pInfluenceCollisionBox; }
	inline const deoglDCollisionBox &GetInfluenceCollisionBox() const{ return pInfluenceCollisionBox; }
	/** Retrieves the influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	/** Sets the influence priority. */
	void SetInfluencePriority( int priority );
	
	/** Retrieves the reflection box matrix. */
	inline const decDMatrix &GetMatrixReflectionBox() const{ return pMatrixReflectionBox; }
	/** Sets the reflection box matrix. */
	void SetMatrixReflectionBox( const decDMatrix &matrix );
	/** Determines if a reflection box exists. */
	inline bool GetHasReflectionBox() const{ return pHasReflectionBox; }
	/** Sets if a reflection box exists. */
	void SetHasReflectionBox( bool hasReflectionBox );
	
	/** Retrieves the number of reflection mask box matrices. */
	inline int GetReflectionMaskBoxMatrixCount() const{ return pReflectionMaskBoxMatrixCount; }
	/** Retrieves a reflection mask box matrix. */
	const decDMatrix &GetReflectionMaskBoxMatrixAt( int index ) const;
	/** Removes all reflection mask boxes matrices. */
	void RemoveAllReflectionMaskBoxMatrices();
	/** Adds a reflection mask box matrix. */
	void AddReflectionMaskBoxMatrix( const decDMatrix &matrix );
	
	/** Determines if the environment map is dirty. */
	inline bool GetDirty() const{ return pDirty; }
	/** Sets if the environment map is dirty. */
	void SetDirty( bool dirty );
	
	/** Determines if the texture is ready for rendering. */
	inline bool GetReady() const{ return pReady; }
	
	/** Retrieves the component list. */
	inline deoglComponentList &GetComponentList(){ return pComponentList; }
	inline const deoglComponentList &GetComponentList() const{ return pComponentList; }
	
	/** \brief Billboard list. */
	inline deoglBillboardList &GetBillboardList(){ return pBillboardList; }
	inline const deoglBillboardList &GetBillboardList() const{ return pBillboardList; }
	
	/** Retrieves the particle emitter instance list. */
	inline deoglParticleEmitterInstanceList &GetParticleEmitterInstanceList(){ return pParticleEmitterInstanceList; }
	inline const deoglParticleEmitterInstanceList &GetParticleEmitterInstanceList() const{ return pParticleEmitterInstanceList; }
	/** Retrieves the render plan list. */
	inline deoglRenderPlanList &GetRenderPlanList(){ return pRenderPlanList; }
	inline const deoglRenderPlanList &GetRenderPlanList() const{ return pRenderPlanList; }
	
	/** Retrieves the render plan usage count. */
	inline int GetPlanUsageCount() const{ return pPlanUsageCount; }
	/** Increases the render plan usage count by one. */
	void AddPlanUsage();
	/** Decreases the render plan usage count by one dropping the textures if reaching 0. */
	void RemovePlanUsage();
	/** Destroy the environment map. */
	void Destroy();
	/** Determines if the environment map is destroy if becoming unused. */
	inline bool GetDestroyIfUnused() const{ return pDestroyIfUnused; }
	/** Sets if the environment map is destroy if becoming unused. */
	void SetDestroyIfUnused( bool destroyIfUnused );
	
	/** Determines if the environment map is marked. */
	inline bool GetMarked() const{ return pMarked; }
	/** Sets if the environment map is marked. */
	void SetMarked( bool marked );
	
	/** Retrieves the index of the slot in the global environment map slot manager or -1 if not in a slot. */
	inline int GetSlotIndex() const{ return pSlotIndex; }
	/** Sets the index of the slot in the global environment map slot manager or -1 if not in a slot. */
	void SetSlotIndex( int slotIndex );
	
	/** Prepare for rendering. */
	void PrepareForRender();
	/** Updates the environment map if dirty. */
	void Update();
	/** Render an environment cube map. */
	void RenderEnvCubeMap();
	/** Retrieves the environment map or NULL if not existing. */
	inline deoglCubeMap *GetEnvironmentMap() const{ return pEnvMap; }
	/** Retrieves the environment depth map or NULL if not existing. */
	inline deoglCubeMap *GetEnvironmentMapDepth() const{ return pEnvMapDepth; }
	/** Retrieves the equi environment map or NULL if not existing. */
	inline deoglTexture *GetEquiEnvMap() const{ return pEnvMapEqui; }
	/** Retrieves the equi environment depth map or NULL if not existing. */
	inline deoglTexture *GetEquiEnvMapDepth() const{ return pEnvMapEquiDepth; }
	
	/** Retrieves the convex colume list. */
	inline decConvexVolumeList *GetConvexVolumeList() const{ return pConvexVolumeList; }
	/** Retrieves the light volume. */
	inline deoglLightVolume *GetLightVolume() const{ return pLightVolume; }
	/** Update the convex volume list. */
	void UpdateConvexVolumeList();
	/** Prepare hard limit cube map. */
	void PrepareCubeMapHardLimit();
	/** Prepare soft limit cube map. */
	void PrepareCubeMapSoftLimit();
	/** Retrieves the hard limit cube map. */
	inline deoglCubeMap *GetCubeMapHardLimit() const{ return pCubeMapHardLimit; }
	/** Retrieves the soft limit cube map. */
	inline deoglCubeMap *GetCubeMapSoftLimit() const{ return pCubeMapSoftLimit; }
	/** Retrieves the cube map limit scale. */
	inline float GetCubeMapLimitScale() const{ return pCubeMapLimitScale; }
	
	/** Sky changed. */
	void SkyChanged();
	
	/** \brief Light changed. */
	void LightChanged( deoglRLight *light );
	
	/** \brief Prepare for quick disposal of environment map. */
	void PrepareQuickDispose();
	/*@}*/
	
private:
	void pCleanUp();
	void pRemoveFromAllRenderPlans();
};

#endif
