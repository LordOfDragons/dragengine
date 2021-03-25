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
 * @brief Collide List.
 */
class deoglCollideList{
private:
	deoglParticleEmitterInstanceList pParticleEmitterList;
	deoglEnvironmentMapList pEnvMapList;
	
	deoglCollideListComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
	deoglRLight **pLights;
	int pLightCount, pLightSize;
	
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collide list. */
	deoglCollideList();
	/** Cleans up the collide list. */
	~deoglCollideList();
	/*@}*/
	
	/** @name Management */
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
	
	/** Mark all elements visible. */
	void MarkElementsVisible( bool visible );
	/** Removes all elements flagged invisible. */
	void RemoveVisibleElements( bool visible );
	
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
	
	/** @name Components */
	/*@{*/
	/** Retrieves the number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	/** Retrieves the component at the given index. */
	deoglCollideListComponent *GetComponentAt( int index ) const;
	/** Retrieves the index of the given component or -1 if not found. */
	int IndexOfComponent( deoglRComponent *component ) const;
	/** Determines if the given component exists. */
	bool HasComponent( deoglRComponent *component ) const;
	/** Adds a component. */
	void AddComponent( deoglRComponent *component );
	/** Removes a component. */
	void RemoveComponent( deoglRComponent *component );
	/** Removes a component from the given index. */
	void RemoveComponentFrom( int index );
	/** Removes all components. */
	void RemoveAllComponents();
	
	/** Adds all components colliding with the given volume. */
	void AddComponentsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	
	/** Mark all components visible. */
	void MarkComponentsVisible( bool visible );
	/** Removes components matching the visible state. */
	void RemoveVisibleComponents( bool visible );
	/** Removes solid components. */
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
	
	/** @name Lights */
	/*@{*/
	/** Retrieves the number of lights. */
	inline int GetLightCount() const{ return pLightCount; }
	/** Retrieves the light at the given index. */
	deoglRLight *GetLightAt( int index ) const;
	/** Adds a light. */
	void AddLight( deoglRLight *light );
	/** Removes all lights. */
	void RemoveAllLights();
	/** Adds all lights colliding with the given volume. */
	void AddLightsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *volume );
	
	/** Mark all lights visible. */
	void MarkLightsVisible( bool visible );
	/** Removes invisible lights. */
	void RemoveVisibleLights( bool visible );
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** \brief Count of billboards. */
	inline int GetBillboardCount() const{ return pBillboardCount; }
	
	/** \brief Billboard at index. */
	deoglRBillboard *GetBillboardAt( int index ) const;
	
	/** \brief Add billboard. */
	void AddBillboard( deoglRBillboard *billboard );
	
	/** \brief Remove all billboards. */
	void RemoveAllBillboards();
	
	/** \brief Add all billboards colliding with volume. */
	void AddBillboardsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume );
	
	/** \brief Mark all billboards visible. */
	void MarkBillboardsVisible( bool visible );
	
	/** \brief Remove invisible billboards. */
	void RemoveVisibleBillboards( bool visible );
	/*@}*/
	
	
	
	/** @name Height Terrain Sectors */
	/*@{*/
	/** Retrieves the number of height terrain sectors. */
	inline int GetHTSectorCount() const{ return pHTSectorCount; }
	/** Retrieves the height terrain sector at the given index. */
	deoglCollideListHTSector *GetHTSectorAt( int index ) const;
	/** Adds height terrain sector. */
	void AddHTSector( deoglHTViewSector *sector, deoglDCollisionVolume *volume );
	/** Removes all height terrain sectors. */
	void RemoveAllHTSectors();
	/** Adds height terrain sectors colliding with the given volume. */
	void AddHTSectorsColliding( deoglHTView *htview, deoglDCollisionVolume *volume );
	/*@}*/
	
	/** @name Prop Fields */
	/*@{*/
	/** Retrieves the number of prop fields. */
	inline int GetPropFieldCount() const{ return pPropFieldCount; }
	/** Retrieves the prop field at the given index. */
	deoglCollideListPropField *GetPropFieldAt( int index ) const;
	/** Adds a prop field. */
	void AddPropField( deoglRPropField *propField, deoglDCollisionVolume *volume );
	/** Removes all prop fields. */
	void RemoveAllPropFields();
	/** Adds prop fields colliding with the given volume. */
	void AddPropFieldsColliding( deoglRWorld &world, deoglDCollisionVolume *volume );
	/*@}*/
	
private:
	void pCleanUp();
	void pSortCompByDist( int left, int right );
};

// end of include only once
#endif
