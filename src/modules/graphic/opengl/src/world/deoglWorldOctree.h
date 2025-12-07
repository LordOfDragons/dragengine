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

#ifndef _DEOGLWORLDOCTREE_H_
#define _DEOGLWORLDOCTREE_H_

#include <stdint.h>

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
class deoglWorldCSOctree;



/**
 * World octree.
 */
class deoglWorldOctree : public deoglDOctree{
private:
	int pInsertDepth;
	
	deoglBillboardList pBillboards;
	deoglEnvironmentMapList pEnvMaps;
	deoglParticleEmitterInstanceList pParticleEmitters;
	
	decPointerList pComponents;
	decPointerList pLights;
	decPointerList pLumimeters;
	
	int pCSChildCount;
	int pCSLocalElementCount;
	int pCSNodeCount;
	int pCSElementCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world octree. */
	deoglWorldOctree(const decDVector &center, const decDVector &halfSize, int insertDepth);
	
	/** Clean up world octree. */
	~deoglWorldOctree() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create octree for octant. */
	deoglDOctree *CreateOctree(int octant) const override;
	
	
	
	/** Clear content of this node. */
	void ClearNodeContent() override;
	
	/** Clear all billboards from the tree. */
	void ClearBillboards();
	
	/** Clear all components from the tree. */
	void ClearComponents();
	
	/** Clear all environment maps from the tree. */
	void ClearEnvMaps();
	
	/** Clear all lights from the tree. */
	void ClearLights();
	
	/** Clear all lumimeters from the tree. */
	void ClearLumimeters();
	
	/** Clear all particle emitters from the tree. */
	void ClearParticleEmitters();
	
	
	
	/** Add billboard into the octree. */
	void InsertBillboardIntoTree(deoglRBillboard *billboard);
	
	/** Add component into the octree. */
	void InsertComponentIntoTree(deoglRComponent *component);
	
	/** Add environment map into the octree. */
	void InsertEnvMapIntoTree(deoglEnvironmentMap *envmap);
	
	/** Add light into the octree. */
	void InsertLightIntoTree(deoglRLight *light);
	
	/** Add particle emitter into the octree. */
	void InsertParticleEmitterIntoTree(deoglRParticleEmitterInstance *instance);
	
	/**
	 * Add lumimeter into the octree. Returns the node where the lumimeter has been placed into.
	 * \todo Change to match other insert calls which set node instead of returning it.
	 */
	deoglWorldOctree *InsertLumimeterIntoTree(deoglRLumimeter *lumimeter);
	
	/** TEMPORARY HACK!. */
	void VisitNodesCollidingVolume(deoglWorldOctreeVisitor *visitor, deoglDCollisionVolume *volume);
	
	
	
	/** Compute shader node count. Valid after UpdateCSCounts() has been called. */
	inline int GetCSNodeCount() const{ return pCSNodeCount; }
	
	/** Compute shader element count. Valid after UpdateCSCounts() has been called. */
	inline int GetCSElementCount() const{ return pCSElementCount; }
	
	/** Update compute shader node and element counts. */
	void UpdateCSCounts();
	
	/** Write compute shader data. */
	void WriteCSData(deoglWorldCSOctree &csoctree, int nodeIndex);
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** Number of components. */
	int GetComponentCount() const;
	
	/** Component at index. */
	deoglRComponent *GetComponentAt(int index) const;
	
	/** Index of the given component or -1 if absent. */
	int IndexOfComponent(deoglRComponent *component) const;
	
	/** Add component. */
	void AddComponent(deoglRComponent *component);
	
	/** Remove component. */
	void RemoveComponent(deoglRComponent *component);
	
	/** Remove all components. */
	void RemoveAllComponents();
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** Number of lights. */
	int GetLightCount() const;
	
	/** Light at the given index. */
	deoglRLight *GetLightAt(int index) const;
	
	/** Index of light or -1 if absent. */
	int IndexOfLight(deoglRLight *light) const;
	
	/** Add light. */
	void AddLight(deoglRLight *light);
	
	/** Remove light. */
	void RemoveLight(deoglRLight *light);
	
	/** Remove all lights. */
	void RemoveAllLights();
	/*@}*/
	
	
	
	/** \name Lumimeters */
	/*@{*/
	/** Number of lumimeters. */
	int GetLumimeterCount() const;
	
	/** Lumimeter at index. */
	deoglRLumimeter *GetLumimeterAt(int index) const;
	
	/** Index of lumimeter or -1 if absent. */
	int IndexOfLumimeter(deoglRLumimeter *lumimeter) const;
	
	/** Add lumimeter. */
	void AddLumimeter(deoglRLumimeter *lumimeter);
	
	/** Remove lumimeter. */
	void RemoveLumimeter(deoglRLumimeter *lumimeter);
	
	/** Remove all lumimeters. */
	void RemoveAllLumimeters();
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** List of billboards. */
	inline const deoglBillboardList &GetBillboardList() const{ return pBillboards; }
	
	/** Add billboard. */
	void AddBillboard(deoglRBillboard *billboard);
	
	/** Remove billboard. */
	void RemoveBillboard(deoglRBillboard *billboard);
	
	/** Remove all billboards. */
	void RemoveAllBillboards();
	/*@}*/
	
	
	
	/** \name EnvMaps */
	/*@{*/
	/** List of environment maps. */
	inline const deoglEnvironmentMapList &GetEnvMapList() const{ return pEnvMaps; }
	
	/** Add envmap. */
	void AddEnvMap(deoglEnvironmentMap *envmap);
	
	/** Remove envmap. */
	void RemoveEnvMap(deoglEnvironmentMap *envmap);
	
	/** Remove all envmaps. */
	void RemoveAllEnvMaps();
	/*@}*/
	
	
	
	/** \name ParticleEmitters */
	/*@{*/
	/** List of particle emitters. */
	inline const deoglParticleEmitterInstanceList &GetParticleEmittersList() const{ return pParticleEmitters; }
	
	/** Add emitter. */
	void AddParticleEmitter(deoglRParticleEmitterInstance *emitter);
	
	/** Remove emitter. */
	void RemoveParticleEmitter(deoglRParticleEmitterInstance *emitter);
	
	/** Remove all emitters. */
	void RemoveAllParticleEmitters();
	/*@}*/
	
	
	
private:
	deoglWorldOctree *pGetNodeFor(const decDVector &minExtend, const decDVector &maxExtend, int maxDepth);
	deoglWorldOctree *pGetNodeFor(const decDVector &position, int maxDepth);
};

#endif
