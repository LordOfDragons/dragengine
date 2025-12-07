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

#ifndef _DEOGLCOLLIDELIST_H_
#define _DEOGLCOLLIDELIST_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

#include "../particle/deoglParticleEmitterInstanceList.h"
#include "../envmap/deoglEnvironmentMapList.h"

class deoglCollideListComponent;
class deoglCollideListHTSector;
class deoglCollideListHTSCluster;
class deoglCollideListLight;
class deoglCollideListPropField;
class deoglCollideListPropFieldCluster;
class deoglDCollisionFrustum;
class deoglDCollisionVolume;
class deoglHTView;
class deoglHTViewSector;
class deoglHTViewSectorCluster;
class deoglRBillboard;
class deoglRComponent;
class deoglRLight;
class deoglRPropField;
class deoglPropFieldCluster;
class deoglRWorld;
class deoglTMViewFrustum;
class deoglTransformVolume;
class deoglWorldOctree;
class deoglRTLogger;



/**
 * Collide List.
 */
class deoglCollideList{
private:
	deoglParticleEmitterInstanceList pParticleEmitterList;
	deoglEnvironmentMapList pEnvMapList;
	
	deoglCollideListComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
	deoglCollideListLight **pLights;
	int pLightCount;
	int pLightSize;
	
	deoglRBillboard **pBillboards;
	int pBillboardCount;
	int pBillboardSize;
	
	deoglCollideListHTSector **pHTSectors;
	int pHTSectorCount;
	int pHTSectorSize;
	
	deoglCollideListHTSCluster **pHTSClusters;
	int pHTSClusterCount;
	int pHTSClusterSize;
	
	deoglCollideListPropField **pPropFields;
	int pPropFieldCount;
	int pPropFieldSize;
	
	deoglCollideListPropFieldCluster **pPropFieldClusters;
	int pPropFieldClusterCount;
	int pPropFieldClusterSize;
	
	deoglTransformVolume *pTransformVolume;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collide list. */
	deoglCollideList();
	/** Cleans up the collide list. */
	~deoglCollideList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Clears all objects from the list. */
	void Clear();
	/** Sort objects linear. */
	void SortLinear(const decVector &pos, const decVector &view);
	/** Sort objects radial. */
	void SortRadial(const decVector &pos);
	/** Flags all components lit or unlit. */
	void FlagAllLit(bool lit);
	/** Adds world elements colliding with the given volume. */
	void AddElementsColliding(deoglWorldOctree *octree, deoglDCollisionVolume *volume);
	/** Adds world elements colliding with the given volume. */
	void AddElementsColliding(deoglWorldOctree *octree, deoglDCollisionFrustum *volume);
	
	/** Remove all culled elements. */
	void RemoveCulledElements();
	
	/**
	 * Update cube face masks and assign them as special flags.
	 * \note Although declared const this method modifies content.
	 */
	void UpdateCubeFaceMasks(const decDVector &position) const;
	
	/**
	 * Update cube face masks and assign them as special flags.
	 * \note Although declared const this method modifies content.
	 */
	void UpdateOccMeshCubeFaceMasks(const decDVector &position) const;
	
	/** Retrieves the list of particle emitters. */
	inline deoglParticleEmitterInstanceList &GetParticleEmitterList(){return pParticleEmitterList;}
	inline const deoglParticleEmitterInstanceList &GetParticleEmitterList() const{return pParticleEmitterList;}
	/** Adds all particle emitters colliding with the given volume. */
	void AddParticleEmittersColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume);
	
	/** Retrieves the list of environment maps. */
	inline deoglEnvironmentMapList &GetEnvironmentMapList(){return pEnvMapList;}
	inline const deoglEnvironmentMapList &GetEnvironmentMapList() const{return pEnvMapList;}
	/** Adds all environment maps colliding with the given volume. */
	void AddEnvironmentMapsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume);
	
	/** Single line debug. */
	void DebugSingleLine(deoglRTLogger &logger, bool sorted) const;
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** Count of components. */
	inline int GetComponentCount() const{return pComponentCount;}
	
	/** Component at index. */
	deoglCollideListComponent *GetComponentAt(int index) const;
	
	/** Index of component or -1 if absent. */
	int IndexOfComponent(deoglRComponent *component) const;
	
	/** Component is present. */
	bool HasComponent(deoglRComponent *component) const;
	
	/** Add component. */
	deoglCollideListComponent *AddComponent(deoglRComponent *component);
	
	/** Remove component. */
	void RemoveComponent(deoglRComponent *component);
	
	/** Remove component from index. */
	void RemoveComponentFrom(int index);
	
	/** Remove all components. */
	void RemoveAllComponents();
	
	/** Add all components colliding with volume. */
	void AddComponentsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume);
	
	/** Remove culled components. */
	void RemoveCulledComponents();
	
	/** Remove solid components. */
	void RemoveSolidComponents();
	
	/** Sort components by models. */
	void SortComponentsByModels();
	
	/** Sort components by distance. */
	void SortComponentsByDistance(const decVector &pos, const decVector &view);
	
	/** Mark components. */
	void MarkComponents(bool marked) const;
	
	/** Log components. */
	void LogComponents() const;
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** Count of lights. */
	inline int GetLightCount() const{return pLightCount;}
	
	/** Light at index. */
	deoglCollideListLight *GetLightAt(int index) const;
	
	/** Index of light or -1 if absent. */
	int IndexOfLight(deoglRLight *light) const;
	
	/** Light is present. */
	bool HasLight(deoglRLight *light) const;
	
	/** Add light. */
	deoglCollideListLight *AddLight(deoglRLight *light);
	
	/** Remove light. */
	void RemoveLight(deoglRLight *light);
	
	/** Remove light from index. */
	void RemoveLightFrom(int index);
	
	/** Remove all lights. */
	void RemoveAllLights();
	
	/** Add all lights colliding with volume. */
	void AddLightsColliding(deoglWorldOctree *octree, deoglDCollisionVolume *volume);
	
	/** Mark all lights culled. */
	void MarkLightsCulled(bool culled);
	
	/** Remove culled lights. */
	void RemoveCulledLights();
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** Count of billboards. */
	inline int GetBillboardCount() const{return pBillboardCount;}
	
	/** Billboard at index. */
	deoglRBillboard *GetBillboardAt(int index) const;
	
	/** Add billboard. */
	void AddBillboard(deoglRBillboard *billboard);
	
	/** Remove all billboards. */
	void RemoveAllBillboards();
	
	/** Add all billboards colliding with volume. */
	void AddBillboardsColliding(deoglWorldOctree &octree, deoglDCollisionVolume *volume);
	
	/** Remove culled billboards. */
	void RemoveCulledBillboards();
	/*@}*/
	
	
	
	/** \name Height Terrain Sectors */
	/*@{*/
	/** Count of height terrain sectors. */
	inline int GetHTSectorCount() const{return pHTSectorCount;}
	
	/** Height terrain sector at index. */
	deoglCollideListHTSector *GetHTSectorAt(int index) const;
	
	/** Add height terrain sector. */
	deoglCollideListHTSector *AddHTSector(deoglHTViewSector *sector);
	
	/** Add height terrain sector colliding with volume. */
	void AddHTSector(deoglHTViewSector *sector, deoglDCollisionVolume *volume);
	
	/** Remove all height terrain sectors. */
	void RemoveAllHTSectors();
	
	/** Add height terrain sectors colliding with volume. */
	void AddHTSectorsColliding(deoglHTView *htview, deoglDCollisionVolume *volume);
	/*@}*/
	
	
	
	/** \name Height Terrain Sector Clusters */
	/*@{*/
	/** Count of height terrain sector clusters. */
	inline int GetHTSClusterCount() const{return pHTSClusterCount;}
	
	/** Height terrain sector cluster at index. */
	deoglCollideListHTSCluster *GetHTSClusterAt(int index) const;
	
	/** Add height terrain sector cluster. */
	deoglCollideListHTSCluster *AddHTSCluster(deoglHTViewSectorCluster *cluster);
	
	/** Remove all height terrain sector clusters. */
	void RemoveAllHTSClusters();
	/*@}*/
	
	
	
	/** \name Prop Fields */
	/*@{*/
	/** Count of prop fields. */
	inline int GetPropFieldCount() const{return pPropFieldCount;}
	
	/** Prop field at index. */
	deoglCollideListPropField *GetPropFieldAt(int index) const;
	
	/** Add prop field. */
	deoglCollideListPropField *AddPropField(deoglRPropField *propField);
	
	/** Add prop field if colliding with volume. */
	void AddPropField(deoglRPropField *propField, deoglDCollisionVolume &volume);
	
	/** Remove all prop fields. */
	void RemoveAllPropFields();
	
	/** Adds prop fields colliding with the given volume. */
	void AddPropFieldsColliding(deoglRWorld &world, deoglDCollisionVolume &volume);
	/*@}*/
	
	
	
	/** \name Prop Field Clusters */
	/*@{*/
	/** Count of prop field clusters. */
	inline int GetPropFieldClusterCount() const{return pPropFieldClusterCount;}
	
	/** Prop field cluster at index. */
	deoglCollideListPropFieldCluster *GetPropFieldClusterAt(int index) const;
	
	/** Add prop field cluster. */
	deoglCollideListPropFieldCluster *AddPropFieldCluster(deoglPropFieldCluster *cluster);
	
	/** Remove all prop field clusters. */
	void RemoveAllPropFieldClusters();
	/*@}*/
	
private:
	void pCleanUp();
	void pSortCompByDist(int left, int right);
};

// end of include only once
#endif
