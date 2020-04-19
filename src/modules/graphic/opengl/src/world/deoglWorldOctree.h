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

#ifndef _DEOGLWORLDOCTREE_H_
#define _DEOGLWORLDOCTREE_H_

#include "../billboard/deoglBillboardList.h"
#include "../envmap/deoglEnvironmentMapList.h"
#include "../particle/deoglParticleEmitterInstanceList.h"
#include "../utils/octree/deoglDOctree.h"

#include <dragengine/common/collection/decPointerList.h>

class deoglDCollisionVolume;
class deoglRComponent;
class deoglRLight;
class deoglRLumimeter;
class deoglWorldOctreeVisitor;



/**
 * \brief World octree.
 */
class deoglWorldOctree : public deoglDOctree{
private:
	deoglBillboardList pBillboards;
	deoglEnvironmentMapList pEnvMaps;
	deoglParticleEmitterInstanceList pParticleEmitters;
	
	decPointerList pComponents;
	decPointerList pLights;
	decPointerList pLumimeters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world octree. */
	deoglWorldOctree( const decDVector &center, const decDVector &halfSize );
	
	/** \brief Clean up world octree. */
	virtual ~deoglWorldOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	virtual deoglDOctree *CreateOctree( int octant ) const;
	
	
	
	/** \brief Clear content of this node. */
	virtual void ClearNodeContent();
	
	/** \brief Clear all billboards from the tree. */
	void ClearBillboards();
	
	/** \brief Clear all components from the tree. */
	void ClearComponents();
	
	/** \brief Clear all environment maps from the tree. */
	void ClearEnvMaps();
	
	/** \brief Clear all lights from the tree. */
	void ClearLights();
	
	/** \brief Clear all lumimeters from the tree. */
	void ClearLumimeters();
	
	/** \brief Clear all particle emitters from the tree. */
	void ClearParticleEmitters();
	
	
	
	/** \brief Add billboard into the octree. */
	void InsertBillboardIntoTree( deoglRBillboard *billboard, int maxDepth );
	
	/** \brief Add component into the octree. */
	void InsertComponentIntoTree( deoglRComponent *component, int maxDepth );
	
	/** \brief Add environment map into the octree. */
	void InsertEnvMapIntoTree( deoglEnvironmentMap *envmap, int maxDepth );
	
	/** \brief Add light into the octree. */
	void InsertLightIntoTree( deoglRLight *light, int maxDepth );
	
	/** \brief Add particle emitter into the octree. */
	void InsertParticleEmitterIntoTree( deoglRParticleEmitterInstance *instance, int maxDepth );
	
	/**
	 * \brief Add lumimeter into the octree. Returns the node where the lumimeter has been placed into.
	 * \todo Change to match other insert calls which set node instead of returning it.
	 */
	deoglWorldOctree *InsertLumimeterIntoTree( deoglRLumimeter *lumimeter, int maxDepth );
	
	/** TEMPORARY HACK!. */
	void VisitNodesCollidingVolume( deoglWorldOctreeVisitor *visitor, deoglDCollisionVolume *volume );
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Number of components. */
	int GetComponentCount() const;
	
	/** \brief Component at index. */
	deoglRComponent *GetComponentAt( int index ) const;
	
	/** \brief Index of the given component or -1 if absent. */
	int IndexOfComponent( deoglRComponent *component ) const;
	
	/** \brief Add component. */
	void AddComponent( deoglRComponent *component );
	
	/** \brief Remove component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** \brief Remove all components. */
	void RemoveAllComponents();
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** \brief Number of lights. */
	int GetLightCount() const;
	
	/** \brief Light at the given index. */
	deoglRLight *GetLightAt( int index ) const;
	
	/** \brief Index of light or -1 if absent. */
	int IndexOfLight( deoglRLight *light ) const;
	
	/** \brief Add light. */
	void AddLight( deoglRLight *light );
	
	/** \brief Remove light. */
	void RemoveLight( deoglRLight *light );
	
	/** \brief Remove all lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Lumimeters */
	/*@{*/
	/** \brief Number of lumimeters. */
	int GetLumimeterCount() const;
	
	/** \brief Lumimeter at index. */
	deoglRLumimeter *GetLumimeterAt( int index ) const;
	
	/** \brief Index of lumimeter or -1 if absent. */
	int IndexOfLumimeter( deoglRLumimeter *lumimeter ) const;
	
	/** \brief Add lumimeter. */
	void AddLumimeter( deoglRLumimeter *lumimeter );
	
	/** \brief Remove lumimeter. */
	void RemoveLumimeter( deoglRLumimeter *lumimeter );
	
	/** \brief Remove all lumimeters. */
	void RemoveAllLumimeters();
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** \brief List of billboards. */
	inline const deoglBillboardList &GetBillboardList() const{ return pBillboards; }
	
	/** \brief Add billboard. */
	void AddBillboard( deoglRBillboard *billboard );
	
	/** \brief Remove billboard. */
	void RemoveBillboard( deoglRBillboard *billboard );
	
	/** \brief Remove all billboards. */
	void RemoveAllBillboards();
	/*@}*/
	
	
	
	/** \name EnvMaps */
	/*@{*/
	/** \brief List of environment maps. */
	inline const deoglEnvironmentMapList &GetEnvMapList() const{ return pEnvMaps; }
	
	/** \brief Add envmap. */
	void AddEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Remove envmap. */
	void RemoveEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Remove all envmaps. */
	void RemoveAllEnvMaps();
	/*@}*/
	
	
	
	/** \name ParticleEmitters */
	/*@{*/
	/** \brief List of particle emitters. */
	inline const deoglParticleEmitterInstanceList &GetParticleEmittersList() const{ return pParticleEmitters; }
	
	/** \brief Add emitter. */
	void AddParticleEmitter( deoglRParticleEmitterInstance *emitter );
	
	/** \brief Remove emitter. */
	void RemoveParticleEmitter( deoglRParticleEmitterInstance *emitter );
	
	/** \brief Remove all emitters. */
	void RemoveAllParticleEmitters();
	/*@}*/
	
	
	
private:
	deoglWorldOctree *pGetNodeFor( const decDVector &center, const decDVector &halfSize, int maxDepth );
	deoglWorldOctree *pGetNodeFor( const decDVector &position, int maxDepth );
};

#endif
