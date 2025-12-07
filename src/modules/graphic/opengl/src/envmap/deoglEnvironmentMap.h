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

#ifndef _DEOGLENVIONMENTMAP_H_
#define _DEOGLENVIONMENTMAP_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include "../utils/collision/deoglDCollisionBox.h"
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>

#include "../billboard/deoglBillboardList.h"
#include "../component/deoglComponentSet.h"
#include "../particle/deoglParticleEmitterInstanceList.h"
#include "../rendering/plan/deoglRenderPlanList.h"

class deoglRLight;
class decConvexVolumeList;
class deoglCubeMap;
class deoglArrayTexture;
class deoglLightVolume;
class deoglRenderThread;
class deoglRWorld;
class deoglTexture;
class deoglWorldOctree;



/**
 * Environment Map. Provides support for updating the environment map. Individual world
 * elements can be included or excluded.
 */
class deoglEnvironmentMap : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglEnvironmentMap> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pWorld;
	decDVector pPosition;
	deoglWorldOctree *pOctreeNode;
	
	bool pSkyOnly;
	
	int pSize;
	bool pIsFloat;
	
	deoglCubeMap *pEnvMap;
	deoglArrayTexture *pEnvMapPosition;
	deoglArrayTexture *pEnvMapDiffuse;
	deoglArrayTexture *pEnvMapNormal;
	deoglArrayTexture *pEnvMapEmissive;
	deoglTexture *pEnvMapEqui;
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
	bool pMaterialReady;
	int pNextUpdateFace;
	int pLastGILightUpdate;
	
	int pPlanUsageCount;
	bool pDestroyIfUnused;
	
	deoglBillboardList pBillboardList;
	deoglComponentSet pComponentList;
	deoglParticleEmitterInstanceList pParticleEmitterInstanceList;
	deoglRenderPlanList pRenderPlanList;
	
	bool pMarked;
	
	int pSlotIndex;
	
	decConvexVolumeList *pConvexVolumeList;
	deoglLightVolume *pLightVolume;
	bool pDirtyConvexVolumeList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new environment map. */
	deoglEnvironmentMap(deoglRenderThread &renderThread);
	/** Cleans up the environment map. */
	virtual ~deoglEnvironmentMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent world or NULL if there is none. */
	deoglRWorld *GetWorld() const{return pWorld;}
	/** Sets the parent world or NULL if there is none. */
	void SetWorld(deoglRWorld *world);
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{return pPosition;}
	/** Sets the position. */
	void SetPosition(const decDVector &position);
	
	/** Updates the octree position. */
	void UpdateOctreePosition();
	/** Retrieves the octree node or NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{return pOctreeNode;}
	/** Sets the octree node or NULL if there is none. */
	void SetOctreeNode(deoglWorldOctree *node);
	
	/** Determines if the sky only is rendered hence the env map is positionless. */
	inline bool GetSkyOnly() const{return pSkyOnly;}
	/** Sets if the sky only is rendered hence the env map is positionless. */
	void SetSkyOnly(bool skyOnly);
	
	/** Retrieves the size. */
	inline int GetSize() const{return pSize;}
	/** Sets the size. */
	void SetSize(int size);
	/** Determines if the environment map is of float format. */
	inline bool GetIsFloat() const{return pIsFloat;}
	/** Sets if the environment map is of float format. */
	void SetIsFloat(bool isFloat);
	/** Retrieves the maximum mip map level. */
	inline int GetMaxMipMapLevel() const{return pMaxMipMapLevel;}
	
	/** Retrieves the layer mask. */
	inline decLayerMask &GetLayerMask(){return pLayerMask;}
	/** Sets the layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** Retrieves the influence box matrix. */
	inline const decDMatrix &GetMatrixInfluenceBox() const{return pMatrixInfluenceBox;}
	/** Sets the influence box matrix. */
	void SetMatrixInfluenceBox(const decDMatrix &matrix);
	/** Retrieves the influence box border factor. */
	inline const decVector &GetInfluenceBoxBorderFactor() const{return pInfluenceBoxBorderFactor;}
	/** Sets the influence box border factor. */
	void SetInfluenceBoxBorderFactor(const decVector &factor);
	/** Retrieves the influence box scale. */
	inline const decVector &GetInfluenceBoxScale() const{return pInfluenceBoxScale;}
	/** Sets the influence box scale. */
	void SetInfluenceBoxScale(const decVector &scale);
	/** Determines if an influence box exists. */
	inline bool GetHasInfluenceBox() const{return pHasInfluenceBox;}
	/** Sets if a influence box exists. */
	void SetHasInfluenceBox(bool hasInfluenceBox);
	/** Retrieves the influence collision box. */
	inline deoglDCollisionBox &GetInfluenceCollisionBox(){return pInfluenceCollisionBox;}
	inline const deoglDCollisionBox &GetInfluenceCollisionBox() const{return pInfluenceCollisionBox;}
	/** Retrieves the influence priority. */
	inline int GetInfluencePriority() const{return pInfluencePriority;}
	/** Sets the influence priority. */
	void SetInfluencePriority(int priority);
	
	/** Retrieves the reflection box matrix. */
	inline const decDMatrix &GetMatrixReflectionBox() const{return pMatrixReflectionBox;}
	/** Sets the reflection box matrix. */
	void SetMatrixReflectionBox(const decDMatrix &matrix);
	/** Determines if a reflection box exists. */
	inline bool GetHasReflectionBox() const{return pHasReflectionBox;}
	/** Sets if a reflection box exists. */
	void SetHasReflectionBox(bool hasReflectionBox);
	
	/** Retrieves the number of reflection mask box matrices. */
	inline int GetReflectionMaskBoxMatrixCount() const{return pReflectionMaskBoxMatrixCount;}
	/** Retrieves a reflection mask box matrix. */
	const decDMatrix &GetReflectionMaskBoxMatrixAt(int index) const;
	/** Removes all reflection mask boxes matrices. */
	void RemoveAllReflectionMaskBoxMatrices();
	/** Adds a reflection mask box matrix. */
	void AddReflectionMaskBoxMatrix(const decDMatrix &matrix);
	
	/** Determines if the environment map is dirty. */
	inline bool GetDirty() const{return pDirty;}
	/** Sets if the environment map is dirty. */
	void SetDirty(bool dirty);
	
	/** Texture is ready for rendering. */
	inline bool GetReady() const{return pReady;}
	
	/** Material textures are ready. */
	inline bool GetMaterialReady() const{return pMaterialReady;}
	
	/** Count of update cycles since the last time this envmap has been GI lit. */
	inline int GetLastGILightUpdate() const{return pLastGILightUpdate;}
	
	/** Count of update cycles since the last time this envmap has been GI lit is at maximum. */
	int IsLastGILightUpdateAtMax() const;
	
	/** Increment count of update cycles since the last time this envmap has been GI lit. */
	void IncLastGILightUpdate();
	
	/** Set count of update cycles since the last time this envmap has been GI lit to maximum. */
	void SetMaxLastGILightUpdate();
	
	/** Set count of update cycles since the last time this envmap has been GI lit to 0. */
	void ResetLastGILightUpdate();
	
	
	
	/** Retrieves the component list. */
	inline deoglComponentSet &GetComponentList(){return pComponentList;}
	inline const deoglComponentSet &GetComponentList() const{return pComponentList;}
	
	/** Billboard list. */
	inline deoglBillboardList &GetBillboardList(){return pBillboardList;}
	inline const deoglBillboardList &GetBillboardList() const{return pBillboardList;}
	
	/** Retrieves the particle emitter instance list. */
	inline deoglParticleEmitterInstanceList &GetParticleEmitterInstanceList(){return pParticleEmitterInstanceList;}
	inline const deoglParticleEmitterInstanceList &GetParticleEmitterInstanceList() const{return pParticleEmitterInstanceList;}
	/** Retrieves the render plan list. */
	inline deoglRenderPlanList &GetRenderPlanList(){return pRenderPlanList;}
	inline const deoglRenderPlanList &GetRenderPlanList() const{return pRenderPlanList;}
	
	/** Retrieves the render plan usage count. */
	inline int GetPlanUsageCount() const{return pPlanUsageCount;}
	/** Increases the render plan usage count by one. */
	void AddPlanUsage();
	/** Decreases the render plan usage count by one dropping the textures if reaching 0. */
	void RemovePlanUsage();
	/** Destroy the environment map. */
	void Destroy();
	/** Determines if the environment map is destroy if becoming unused. */
	inline bool GetDestroyIfUnused() const{return pDestroyIfUnused;}
	/** Sets if the environment map is destroy if becoming unused. */
	void SetDestroyIfUnused(bool destroyIfUnused);
	
	/** Determines if the environment map is marked. */
	inline bool GetMarked() const{return pMarked;}
	/** Sets if the environment map is marked. */
	void SetMarked(bool marked);
	
	/** Retrieves the index of the slot in the global environment map slot manager or -1 if not in a slot. */
	inline int GetSlotIndex() const{return pSlotIndex;}
	/** Sets the index of the slot in the global environment map slot manager or -1 if not in a slot. */
	void SetSlotIndex(int slotIndex);
	
	/** Prepare for rendering. */
	void PrepareForRender();
	/** Updates the environment map if dirty. */
	void Update(deoglRenderPlan &parentPlan);
	/** Render an environment cube map. */
	void RenderEnvCubeMap(deoglRenderPlan &parentPlan);
	
	/** Environment map or NULL. */
	inline deoglCubeMap *GetEnvironmentMap() const{return pEnvMap;}
	
	/** Environment position map or NULL. */
	inline deoglArrayTexture *GetEnvironmentMapPosition() const{return pEnvMapPosition;}
	
	/** Environment diffuse map or NULL. */
	inline deoglArrayTexture *GetEnvironmentMapDiffuse() const{return pEnvMapDiffuse;}
	
	/**
	 * Environment normal map or NULL.
	 * \warning This is Int-Shifted format.
	 */
	inline deoglArrayTexture *GetEnvironmentMapNormal() const{return pEnvMapNormal;}
	
	/** Environment GI map or NULL. */
	inline deoglArrayTexture *GetEnvironmentMapEmissive() const{return pEnvMapEmissive;}
	
	/** Equi environment map or NULL. */
	inline deoglTexture *GetEquiEnvMap() const{return pEnvMapEqui;}
	
	/** Retrieves the convex colume list. */
	inline decConvexVolumeList *GetConvexVolumeList() const{return pConvexVolumeList;}
	/** Retrieves the light volume. */
	inline deoglLightVolume *GetLightVolume() const{return pLightVolume;}
	/** Update the convex volume list. */
	void UpdateConvexVolumeList();
	
	/** Sky changed. */
	void SkyChanged();
	
	/** Light changed. */
	void LightChanged(deoglRLight *light);
	
	/** Prepare for quick disposal of environment map. */
	void PrepareQuickDispose();
	/*@}*/
	
private:
	void pCleanUp();
	void pRemoveFromAllRenderPlans();
};

#endif
