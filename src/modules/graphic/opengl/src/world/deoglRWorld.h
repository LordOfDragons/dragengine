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

#ifndef _DEOGLRWORLD_H_
#define _DEOGLRWORLD_H_

#include "deoglWorldCompute.h"
#include "../envmap/deoglEnvironmentMapList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglEnvironmentMapList;
class deoglRBillboard;
class deoglRComponent;
class deoglRDebugDrawer;
class deoglRenderPlan;
class deoglRenderThread;
class deoglREnvMapProbe;
class deoglRHeightTerrain;
class deoglRLight;
class deoglRLumimeter;
class deoglRParticleEmitterInstance;
class deoglRPropField;
class deoglRSkyInstance;
class deoglWorldOctree;
class deoglWorldOctreeVisitor;
class deoglEnvironmentMap;
class deoglRenderPlanMasked;
class deoglGIState;



/**
 * Render world.
 */
class deoglRWorld : public deObject{
public:
	typedef deTObjectReference<deoglRWorld> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decDVector pSize;
	
	bool pDirtyPrepareForRenderEarly;
	bool pDirtyPrepareForRender;
	
	bool pDirtyNotifySkyChanged;
	bool pDirtySkyOrder;
	deoglEnvironmentMap *pSkyEnvMap;
	
	deoglRHeightTerrain *pHeightTerrain;
	
	deoglRComponent *pRootComponent;
	deoglRComponent *pTailComponent;
	int pComponentCount;
	
	deoglRBillboard *pRootBillboard;
	deoglRBillboard *pTailBillboard;
	int pBillboardCount;
	
	decObjectList pSkies;
	decObjectList pDebugDrawers;
	decObjectList pEnvMapProbes;
	decObjectList pLights;
	decObjectList pLumimeters;
	decObjectList pParticleEmitterInstances;
	decObjectList pPropFields;
	
	decColor pAmbientLight;
	bool pDisableLights;
	decMatrix pLightColorMatrix;
	
	decDVector pReferencePosition;
	double pValidReferenceDistance;
	
	deoglEnvironmentMapList pEnvMapList;
	int pEnvMapUpdateCount;
	deoglRenderPlan *pEnvMapRenderPlan;
	bool pDirtyEnvMapLayout;
	
	decPointerSet pGIStates;
	
	deoglWorldOctree *pOctree;
	deoglWorldCompute::Ref pCompute;
	
	decPointerLinkedList pListPrepareForRenderComponents;
	decPointerLinkedList pListPrepareForRenderBillboards;
	decPointerLinkedList pListPrepareForRenderLights;
	decPointerLinkedList pListPrepareForRenderPropFields;
	
	decPointerList pListPrepareRenderComponents;
	decPointerList pListPrepareRenderBillboards;
	decPointerList pListPrepareRenderLights;
	decPointerList pListPrepareRenderPropFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render world. */
	deoglRWorld(deoglRenderThread &renderThread, const decDVector &size);
	
	/** Clean up render camera. */
	virtual ~deoglRWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Size. */
	inline const decDVector &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize(const decDVector &size);
	
	/** Requires prepare for render. */
	void RequiresPrepareForRender();
	
	/** Mark sky order dirty. */
	void MarkSkyOrderDirty();
	
	/** Sky environment map or \em NULL if not existing. */
	inline deoglEnvironmentMap *GetSkyEnvironmentMap() const{ return pSkyEnvMap; }
	
	
	
	/** Height terrain or \em NULL if not set. */
	inline deoglRHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Set height terrain or \em NULL if not set. */
	void SetHeightTerrain(deoglRHeightTerrain *heightTerrain);
	
	
	
	
	/** All lights are disabled. */
	inline bool GetDisableLights() const{ return pDisableLights; }
	
	/** Set if all lights are disabled. */
	void SetDisableLights(bool disable);
	
	/** Ambient light color. */
	inline const decColor &GetAmbientLight() const{ return pAmbientLight; }
	
	/** Set ambient light color. */
	void SetAmbientLight(const decColor &color);
	
	/** Light color matrix. */
	inline const decMatrix &GetLightColorMatrix() const{ return pLightColorMatrix; }
	
	/** Set light color matrix. */
	void SetLightColorMatrix(const decMatrix &matrix);
	
	
	
	/** Octree. */
	inline deoglWorldOctree &GetOctree() const{ return *pOctree; }
	
	/** Visit entire content of the world. */
	void VisitAll(deoglWorldOctreeVisitor &visitor);
	
	/** Visit content of the world using an axis aligned box. */
	void VisitRegion(const decDVector &boxMinExtend, const decDVector &boxMaxExtend,
		deoglWorldOctreeVisitor &visitor);
	
	
	
	/** Compute. */
	inline deoglWorldCompute &GetCompute() const{ return pCompute; }
	
	
	
	/** Early prepare for rendering. */
	void EarlyPrepareForRender(deoglRenderPlan &plan);
	
	/** Prepare for rendering. */
	void PrepareForRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
	
	/** Prepare for rendering render. */
protected:
	void PrepareForRenderRender(deoglRenderPlan &plan, const deoglRenderPlanMasked *mask);
public:
	
	void AddPrepareForRenderComponent(deoglRComponent *component);
	void RemovePrepareForRenderComponent(deoglRComponent *component);
	
	void AddPrepareForRenderBillboard(deoglRBillboard *billboard);
	void RemovePrepareForRenderBillboard(deoglRBillboard *billboard);
	
	void AddPrepareForRenderLight(deoglRLight *light);
	void RemovePrepareForRenderLight(deoglRLight *light);
	
	void AddPrepareForRenderPropField(deoglRPropField *propField);
	void RemovePrepareForRenderPropField(deoglRPropField *propField);
	/*@}*/
	
	
	
	/** \name Reference position. */
	/*@{*/
	/** Notify all affected elements that the reference position changed. */
	void NotifyAllReferencePositionChanged();
	
	/** Reference position for world matrices and positions used directly in shaders. */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/**
	 * Set reference position for world matrices and positions used directly in shaders.
	 * \details Notifies all affected elements about the change.
	 */
	void SetReferencePosition(const decDVector &position);
	
	/**
	 * Check new reference position against the current reference position.
	 * \details If positions are further apart than the validity distance in any of the vector
	 *          components the reference position is updated and all affected elements notified
	 *          about the change.
	 */
	void CheckReferencePosition(const decDVector &position);
	
	/** Validity distance for reference position checks. */
	inline double GetValidReferenceDistance() const{ return pValidReferenceDistance; }
	
	/** Set validity distance for reference position checks. */
	void SetValidReferenceDistance(double distance);
	/*@}*/
	
	
	
	/** \name Prop fields. */
	/*@{*/
	/** Number of prop fields. */
	int GetPropFieldCount() const;
	
	/** Prop field at index. */
	deoglRPropField *GetPropFieldAt(int index) const;
	
	/** Add prop field. */
	void AddPropField(deoglRPropField *propField);
	
	/** Remove prop field. */
	void RemovePropField(deoglRPropField *propField);
	
	/** Remove all prop fields. */
	void RemoveAllPropFields();
	
	/** Remove prop fields marked for removal. */
	void RemoveRemovalMarkedPropFields();
	/*@}*/
	
	
	
	/** \name Particle emitter instances. */
	/*@{*/
	/** Number of particle emitter instances. */
	int GetParticleEmitterInstanceCount() const;
	
	/** Particle emitter instance at index. */
	deoglRParticleEmitterInstance *GetParticleEmitterInstanceAt(int index) const;
	
	/** Add particle emitter instance. */
	void AddParticleEmitterInstance(deoglRParticleEmitterInstance *instance);
	
	/** Remove particle emitter instance. */
	void RemoveParticleEmitterInstance(deoglRParticleEmitterInstance *instance);
	
	/** Remove all particle emitter instances. */
	void RemoveAllParticleEmitterInstances();
	
	/** Remove particle emitter instances marked for removal. */
	void RemoveRemovalMarkedParticleEmitterInstances();
	/*@}*/
	
	
	
	/** \name Components. */
	/*@{*/
	/** Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Root component. */
	inline deoglRComponent *GetRootComponent() const{ return pRootComponent; }
	
	/** Add component. */
	void AddComponent(deoglRComponent *component);
	
	/** Remove component. */
	void RemoveComponent(deoglRComponent *component);
	
	/** Remove all component. */
	void RemoveAllComponents();
	
	/** Remove components marked for removal. */
	void RemoveRemovalMarkedComponents();
	/*@}*/
	
	
	
	/** \name Lights. */
	/*@{*/
	/** Number of lights. */
	int GetLightCount() const;
	
	/** Light at index. */
	deoglRLight *GetLightAt(int index) const;
	
	/** Add light. */
	void AddLight(deoglRLight *light);
	
	/** Remove light. */
	void RemoveLight(deoglRLight *light);
	
	/** Remove all light. */
	void RemoveAllLights();
	
	/** Remove lights marked for removal. */
	void RemoveRemovalMarkedLights();
	/*@}*/
	
	
	
	/** \name Environment Map Probes. */
	/*@{*/
	/** Number of environment map probes. */
	int GetEnvMapProbeCount() const;
	
	/** environment map probe at index. */
	deoglREnvMapProbe *GetEnvMapProbeAt(int index) const;
	
	/**
	 * Add environment map probe.
	 * \note Called during synchronization by main thread.
	 */
	void AddEnvMapProbe(deoglREnvMapProbe *envMapProbe);
	
	/**
	 * Remove environment map probe.
	 * \note Called during synchronization by main thread.
	 */
	void RemoveEnvMapProbe(deoglREnvMapProbe *envMapProbe);
	
	/** Remove all environment map probes. */
	void RemoveAllEnvMapProbes();
	
	/** Remove environment map probes marked for removal. */
	void RemoveRemovalMarkedEnvMapProbes();
	/*@}*/
	
	
	
	/** \name Environment maps */
	/*@{*/
	/** Environment map list. */
	inline const deoglEnvironmentMapList &GetEnvMapList() const{ return pEnvMapList; }
	
	/** Add environment map. */
	void AddEnvMap(deoglEnvironmentMap *envmap);
	
	/** Remove environment map. */
	void RemoveEnvMap(deoglEnvironmentMap *envmap);
	
	/** Remove all environment maps. */
	void RemoveAllEnvMaps();
	
	/** Number of environment maps that can be updated this frame. */
	inline int GetEnvMapUpdateCount() const{ return pEnvMapUpdateCount; }
	
	/** Reset env maps update count. */
	void ResetEnvMapUpdateCount();
	
	/** Decrease environment map update count by one. */
	void DecEnvMapUpdateCount();
	
	/** Notify environment maps that the sky changed. */
	void EnvMapsNotifySkyChanged();
	
	/** Shared environment map render plan. */
	inline deoglRenderPlan *GetEnvMapRenderPlan() const{ return pEnvMapRenderPlan; }
	
	/**
	 * Mark environment map layout dirty.
	 * \details The next time a rendering update is done all components are notified about the change.
	 */
	void InvalidateEnvMapLayout();
	/*@}*/
	
	
	
	/** \name Lumimeters. */
	/*@{*/
	/** Number of lumimeters. */
	int GetLumimeterCount() const;
	
	/** Lumimeter at index. */
	deoglRLumimeter *GetLumimeterAt(int index) const;
	
	/** Add lumimeter. */
	void AddLumimeter(deoglRLumimeter *lumimeter);
	
	/** Remove lumimeter. */
	void RemoveLumimeter(deoglRLumimeter *lumimeter);
	
	/** Remove all lumimeter. */
	void RemoveAllLumimeters();
	
	/** Remove lumimeters marked for removal. */
	void RemoveRemovalMarkedLumimeters();
	/*@}*/
	
	
	
	/** \name Billboards. */
	/*@{*/
	/** Number of billboards. */
	inline int GetBillboardCount() const{ return pBillboardCount; }
	
	/** Root billboard. */
	inline deoglRBillboard *GetRootBillboard() const{ return pRootBillboard; }
	
	/** Add billboard. */
	void AddBillboard(deoglRBillboard *billboard);
	
	/** Remove billboard. */
	void RemoveBillboard(deoglRBillboard *billboard);
	
	/** Remove all billboard. */
	void RemoveAllBillboards();
	
	/** Remove billboards marked for removal. */
	void RemoveRemovalMarkedBillboards();
	/*@}*/
	
	
	
	/** \name Skies. */
	/*@{*/
	/** Number of skys. */
	int GetSkyCount() const;
	
	/** Sky at index. */
	deoglRSkyInstance *GetSkyAt(int index) const;
	
	/** Add sky. */
	void AddSky(deoglRSkyInstance *sky);
	
	/** Remove sky. */
	void RemoveSky(deoglRSkyInstance *sky);
	
	/** Remove all skys. */
	void RemoveAllSkies();
	
	/** Remove skys marked for removal. */
	void RemoveRemovalMarkedSkies();
	
	/** Notify skies render static component changed requiring updates. */
	void SkiesNotifyUpdateStaticComponent(deoglRComponent *component);
	/*@}*/
	
	
	
	/** \name Debug Drawers. */
	/*@{*/
	/** Number of debug drawers. */
	int GetDebugDrawerCount() const;
	
	/** Debug drawer at index. */
	deoglRDebugDrawer *GetDebugDrawerAt(int index) const;
	
	/** Add debug drawer. */
	void AddDebugDrawer(deoglRDebugDrawer *debugDrawer);
	
	/** Remove debug drawer. */
	void RemoveDebugDrawer(deoglRDebugDrawer *debugDrawer);
	
	/** Remove all debug drawer. */
	void RemoveAllDebugDrawers();
	
	/** Remove debug drawers marked for removal. */
	void RemoveRemovalMarkedDebugDrawers();
	/*@}*/
	
	
	
	/** \name GI States */
	/*@{*/
	/** Count of GI States. */
	int GetGICascadeCount() const;
	
	/** GI State at index. */
	const deoglGIState *GetGICascadeAt(int index) const;
	
	/** GI State closest to position. */
	const deoglGIState *ClosestGIState(const decDVector &position) const;
	
	/** Add GI State if absent. */
	void AddGICascade(const deoglGIState *giState);
	
	/** Remove GI State if present. */
	void RemoveGICascade(const deoglGIState *giState);
	
	/** Remove all GI states. */
	void RemoveAllGICascades();
	
	/** Notify GI states component entered world. */
	void GIStatesNotifyComponentEnteredWorld(deoglRComponent *component);
	
	/** Notify GI states component changed layer mask. */
	void GIStatesNotifyComponentChangedLayerMask(deoglRComponent *component);
	
	/** Notify GI states component became visible. */
	void GIStatesNotifyComponentBecameVisible(deoglRComponent *component);
	
	/** Notify GI states component changed gi importance. */
	void GIStatesNotifyComponentChangedGIImportance(deoglRComponent *component);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	decDVector pSanitizeOctreeSize(const decDVector &size) const;
	int pCalcOctreeInsertDepth(const decDVector &size) const;
	void pReorderSkies();
	void pCreateSkyEnvMap();
	void pFreeSkyEnvMap();
};

#endif
