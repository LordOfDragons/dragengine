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

#ifndef _DEOGLWORLD_H_
#define _DEOGLWORLD_H_

#include "deoglRWorld.h"

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicWorld.h>

class deoglHeightTerrain;
class deoglSharedVideoPlayerList;
class deoglSkyInstance;
class deoglComponent;
class deoglBillboard;

class deGraphicOpenGl;
class deWorld;



/**
 * World peer.
 */
class deoglWorld : public deBaseGraphicWorld{
private:
	deGraphicOpenGl &pOgl;
	const deWorld &pWorld;
	deoglRWorld::Ref pRWorld;
	
	deoglHeightTerrain *pHeightTerrain;
	deoglSharedVideoPlayerList *pSharedVideoPlayerList;
	
	bool pDirtySize;
	bool pDirtySkies;
	bool pDirtyBillboards;
	bool pDirtyComponents;
	bool pDirtyDebugDrawers;
	bool pDirtyEnvMapProbes;
	bool pDirtyEnvMapUpdateCount;
	bool pDirtyHeightTerrains;
	bool pDirtyLighting;
	bool pDirtyLights;
	bool pDirtyLumimeters;
	bool pDirtyParticleEmitterInstances;
	bool pDirtyPropFields;
	
	bool pSyncing;
	
	decPointerLinkedList pListSyncComponents;
	decPointerLinkedList pListSyncBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world peer. */
	deoglWorld(deGraphicOpenGl &ogl, const deWorld &world);
	
	/** Clean up world peer. */
	virtual ~deoglWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Retrieves the engine world resource. */
	inline const deWorld &GetWorld() const{ return pWorld; }
	
	/** Render world. */
	inline const deoglRWorld::Ref &GetRWorld() const{ return pRWorld; }
	
	/** \deprecated Height terrain or \em NULL if not set. */
	inline deoglHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Update dynamic parts of world like the skins. */
	virtual void Update(float elapsed);
	
	/** Shared video player list. */
	inline deoglSharedVideoPlayerList &GetSharedVideoPlayerList() const{ return *pSharedVideoPlayerList; }
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Add component as require sync if not present. */
	void AddSyncComponent(deoglComponent *component);
	
	/** Remove component as require sync if present. */
	void RemoveSyncComponent(deoglComponent *component);
	
	/** Add billboard as require sync if not present. */
	void AddSyncBillboard(deoglBillboard *billboard);
	
	/** Remove billboard as require sync if present. */
	void RemoveSyncBillboard(deoglBillboard *billboard);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Size changed. */
	virtual void SizeChanged();
	
	/** Height Terrain has changed. */
	virtual void HeightTerrainChanged();
	/** Global lighting parameters have changed. */
	virtual void LightingChanged();
	
	/** Sky has been added. */
	virtual void SkyAdded(deSkyInstance *sky);
	/** Sky has been removed. */
	virtual void SkyRemoved(deSkyInstance *sky);
	/** All skys have been removed. */
	virtual void AllSkiesRemoved();
	
	/** Billboard has been added. */
	virtual void BillboardAdded(deBillboard *billboard);
	/** Billboard has been removed. */
	virtual void BillboardRemoved(deBillboard *billboard);
	/** All billboards have been removed. */
	virtual void AllBillboardsRemoved();
	
	/** Camera has been added. */
	virtual void CameraAdded(deCamera *camera);
	/** Camera has been removed. */
	virtual void CameraRemoved(deCamera *camera);
	/** All cameras have been removed. */
	virtual void AllCamerasRemoved();
	
	/** Component has been added. */
	virtual void ComponentAdded(deComponent *component);
	/** Component has been removed. */
	virtual void ComponentRemoved(deComponent *component);
	/** All components have been removed. */
	virtual void AllComponentsRemoved();
	
	/** Environment map probe has been added. */
	virtual void EnvMapProbeAdded(deEnvMapProbe *envMapProbe);
	/** Environment map probe has been removed. */
	virtual void EnvMapProbeRemoved(deEnvMapProbe *envMapProbe);
	/** All environment map probes have been removed. */
	virtual void AllEnvMapProbesRemoved();
	
	/** Light has been added. */
	virtual void LightAdded(deLight *light);
	/** Light has been removed. */
	virtual void LightRemoved(deLight *light);
	/** All lights have been removed. */
	virtual void AllLightsRemoved();
	
	/** Lumimeter has been added. */
	virtual void LumimeterAdded(deLumimeter *lumimeter);
	/** Lumimeter has been removed. */
	virtual void LumimeterRemoved(deLumimeter *lumimeter);
	/** All lumimeters have been removed. */
	virtual void AllLumimetersRemoved();
	
	/** Particle emitter has been added. */
	virtual void ParticleEmitterAdded(deParticleEmitterInstance *emitter);
	/** Particle emitter has been removed. */
	virtual void ParticleEmitterRemoved(deParticleEmitterInstance *emitter);
	/** All particle emitters have been removed. */
	virtual void AllParticleEmittersRemoved();
	
	/** PropField has been added. */
	virtual void PropFieldAdded(dePropField *propField);
	/** PropField has been removed. */
	virtual void PropFieldRemoved(dePropField *propField);
	/** All propFields have been removed. */
	virtual void AllPropFieldsRemoved();
	
	/** DebugDrawer has been added. */
	virtual void DebugDrawerAdded(deDebugDrawer *debugDrawer);
	/** DebugDrawer has been removed. */
	virtual void DebugDrawerRemoved(deDebugDrawer *debugDrawer);
	/** All debugDrawers have been removed. */
	virtual void AllDebugDrawersRemoved();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pSyncSkies();
	void pSyncHeightTerrain();
	void pSyncPropFields();
	void pSyncParticleEmitterInstances();
	void pSyncComponents();
	void pSyncLights();
	void pSyncEnvironmentMaps();
	void pSyncLumimeters();
	void pSyncDebugDrawers();
	void pSyncBillboards();
};

#endif
