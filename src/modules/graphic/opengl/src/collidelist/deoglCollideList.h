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

#ifndef _DEOGLCOLLIDELIST_H_
#define _DEOGLCOLLIDELIST_H_

#include <dragengine/common/math/decMath.h>

#include "../particle/deoglParticleEmitterInstanceList.h"
#include "../envmap/deoglEnvironmentMapList.h"

class deoglCollideListComponent;
class deoglCollideListHTSector;
class deoglCollideListLight;
class deoglCollideListPropField;
class deoglDCollisionFrustum;
class deoglDCollisionVolume;
class deoglHTView;
class deoglHTViewSector;
class deoglRBillboard;
class deoglRComponent;
class deoglRLight;
class deoglRPropField;
class deoglRWorld;
class deoglTMViewFrustum;
class deoglTransformVolume;
class deoglWorldOctree;



/**
 * \brief Collide List.
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
	
	deoglCollideListPropField **pPropFields;
	int pPropFieldCount;
	int pPropFieldSize;
	
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
	void SortLinear( const decVector &pos, const decVector &view );
	/** Sort objects radial. */
	void SortRadial( const decVector &pos );
	/** Flags all components lit or unlit. */
	void FlagAllLit( bool lit );
	/** Adds world elements colliding with the given volume. */
	void AddElementsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *volume );
	/** Adds world elements colliding with the given volume. */
	void AddElementsColliding( deoglWorldOctree *octree, deoglDCollisionFrustum *volume );
	
	/** Remove all culled elements. */
	void RemoveCulledElements();
	
	/**
	 * Update cube face masks and assign them as special flags.
	 * \note Although declared const this method modifies content.
	 */
	void UpdateCubeFaceMasks( const decDVector &position ) const;
	
	/**
	 * Update cube face masks and assign them as special flags.
	 * \note Although declared const this method modifies content.
	 */
	void UpdateOccMeshCubeFaceMasks( const decDVector &position ) const;
	
	/** Retrieves the list of particle emitters. */
	inline deoglParticleEmitterInstanceList &GetParticleEmitterList(){ return pParticleEmitterList; }
	inline const deoglParticleEmitterInstanceList &GetParticleEmitterList() const{ return pParticleEmitterList; }
	/** Adds all particle emitters colliding with the given volume. */
	void AddParticleEmittersColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	
	/** Retrieves the list of environment maps. */
	inline deoglEnvironmentMapList &GetEnvironmentMapList(){ return pEnvMapList; }
	inline const deoglEnvironmentMapList &GetEnvironmentMapList() const{ return pEnvMapList; }
	/** Adds all environment maps colliding with the given volume. */
	void AddEnvironmentMapsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Component at index. */
	deoglCollideListComponent *GetComponentAt( int index ) const;
	
	/** Index of component or -1 if absent. */
	int IndexOfComponent( deoglRComponent *component ) const;
	
	/** Component is present. */
	bool HasComponent( deoglRComponent *component ) const;
	
	/** Add component. */
	deoglCollideListComponent *AddComponent( deoglRComponent *component );
	
	/** Remove component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** Remove component from index. */
	void RemoveComponentFrom( int index );
	
	/** Remove all components. */
	void RemoveAllComponents();
	
	/** Add all components colliding with volume. */
	void AddComponentsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	
	/** Remove culled components. */
	void RemoveCulledComponents();
	
	/** Remove solid components. */
	void RemoveSolidComponents();
	
	/** Sort components by models. */
	void SortComponentsByModels();
	
	/** Sort components by distance. */
	void SortComponentsByDistance( const decVector &pos, const decVector &view );
	
	/** Mark components. */
	void MarkComponents( bool marked );
	
	/** Log components. */
	void LogComponents() const;
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** Count of lights. */
	inline int GetLightCount() const{ return pLightCount; }
	
	/** Light at index. */
	deoglCollideListLight *GetLightAt( int index ) const;
	
	/** Index of light or -1 if absent. */
	int IndexOfLight( deoglRLight *light ) const;
	
	/** Light is present. */
	bool HasLight( deoglRLight *light ) const;
	
	/** Add light. */
	deoglCollideListLight *AddLight( deoglRLight *light );
	
	/** Remove light. */
	void RemoveLight( deoglRLight *light );
	
	/** Remove light from index. */
	void RemoveLightFrom( int index );
	
	/** Remove all lights. */
	void RemoveAllLights();
	
	/** Add all lights colliding with volume. */
	void AddLightsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *volume );
	
	/** Mark all lights culled. */
	void MarkLightsCulled( bool culled );
	
	/** Remove culled lights. */
	void RemoveCulledLights();
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** Count of billboards. */
	inline int GetBillboardCount() const{ return pBillboardCount; }
	
	/** Billboard at index. */
	deoglRBillboard *GetBillboardAt( int index ) const;
	
	/** Add billboard. */
	void AddBillboard( deoglRBillboard *billboard );
	
	/** Remove all billboards. */
	void RemoveAllBillboards();
	
	/** Add all billboards colliding with volume. */
	void AddBillboardsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	
	/** Remove culled billboards. */
	void RemoveCulledBillboards();
	/*@}*/
	
	
	
	/** \name Height Terrain Sectors */
	/*@{*/
	/** Count of height terrain sectors. */
	inline int GetHTSectorCount() const{ return pHTSectorCount; }
	
	/** Height terrain sector at index. */
	deoglCollideListHTSector *GetHTSectorAt( int index ) const;
	
	/** Add height terrain sector. */
	deoglCollideListHTSector *AddHTSector( deoglHTViewSector *sector );
	
	/** Add height terrain sector colliding with volume. */
	void AddHTSector( deoglHTViewSector *sector, deoglDCollisionVolume *volume );
	
	/** Remove all height terrain sectors. */
	void RemoveAllHTSectors();
	
	/** Add height terrain sectors colliding with volume. */
	void AddHTSectorsColliding( deoglHTView *htview, deoglDCollisionVolume *volume );
	/*@}*/
	
	
	
	/** \name Prop Fields */
	/*@{*/
	/** Count of prop fields. */
	inline int GetPropFieldCount() const{ return pPropFieldCount; }
	
	/** Prop field at index. */
	deoglCollideListPropField *GetPropFieldAt( int index ) const;
	
	/** Add prop field. */
	deoglCollideListPropField *AddPropField( deoglRPropField *propField );
	
	/** Add prop field if colliding with volume. */
	void AddPropField( deoglRPropField *propField, deoglDCollisionVolume &volume );
	
	/** Remove all prop fields. */
	void RemoveAllPropFields();
	
	/** Adds prop fields colliding with the given volume. */
	void AddPropFieldsColliding( deoglRWorld &world, deoglDCollisionVolume &volume );
	/*@}*/
	
private:
	void pCleanUp();
	void pSortCompByDist( int left, int right );
};

// end of include only once
#endif
