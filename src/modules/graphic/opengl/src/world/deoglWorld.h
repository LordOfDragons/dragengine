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

#include <dragengine/common/collection/decTLinkedList.h>
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
	
	decTLinkedList<deoglComponent> pListSyncComponents;
	decTLinkedList<deoglBillboard> pListSyncBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world peer. */
	deoglWorld(deGraphicOpenGl &ogl, const deWorld &world);
	
	/** Clean up world peer. */
	~deoglWorld() override;
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
	void Update(float elapsed) override;
	
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
	void SizeChanged() override;
	
	/** Height Terrain has changed. */
	void HeightTerrainChanged() override;
	/** Global lighting parameters have changed. */
	void LightingChanged() override;
	
	/** Sky has been added. */
	void SkyAdded(deSkyInstance *sky) override;
	/** Sky has been removed. */
	void SkyRemoved(deSkyInstance *sky) override;
	/** All skys have been removed. */
	void AllSkiesRemoved() override;
	
	/** Billboard has been added. */
	void BillboardAdded(deBillboard *billboard) override;
	/** Billboard has been removed. */
	void BillboardRemoved(deBillboard *billboard) override;
	/** All billboards have been removed. */
	void AllBillboardsRemoved() override;
	
	/** Camera has been added. */
	void CameraAdded(deCamera *camera) override;
	/** Camera has been removed. */
	void CameraRemoved(deCamera *camera) override;
	/** All cameras have been removed. */
	void AllCamerasRemoved() override;
	
	/** Component has been added. */
	void ComponentAdded(deComponent *component) override;
	/** Component has been removed. */
	void ComponentRemoved(deComponent *component) override;
	/** All components have been removed. */
	void AllComponentsRemoved() override;
	
	/** Environment map probe has been added. */
	void EnvMapProbeAdded(deEnvMapProbe *envMapProbe) override;
	/** Environment map probe has been removed. */
	void EnvMapProbeRemoved(deEnvMapProbe *envMapProbe) override;
	/** All environment map probes have been removed. */
	void AllEnvMapProbesRemoved() override;
	
	/** Light has been added. */
	void LightAdded(deLight *light) override;
	/** Light has been removed. */
	void LightRemoved(deLight *light) override;
	/** All lights have been removed. */
	void AllLightsRemoved() override;
	
	/** Lumimeter has been added. */
	void LumimeterAdded(deLumimeter *lumimeter) override;
	/** Lumimeter has been removed. */
	void LumimeterRemoved(deLumimeter *lumimeter) override;
	/** All lumimeters have been removed. */
	void AllLumimetersRemoved() override;
	
	/** Particle emitter has been added. */
	void ParticleEmitterAdded(deParticleEmitterInstance *emitter) override;
	/** Particle emitter has been removed. */
	void ParticleEmitterRemoved(deParticleEmitterInstance *emitter) override;
	/** All particle emitters have been removed. */
	void AllParticleEmittersRemoved() override;
	
	/** PropField has been added. */
	void PropFieldAdded(dePropField *propField) override;
	/** PropField has been removed. */
	void PropFieldRemoved(dePropField *propField) override;
	/** All propFields have been removed. */
	void AllPropFieldsRemoved() override;
	
	/** DebugDrawer has been added. */
	void DebugDrawerAdded(deDebugDrawer *debugDrawer) override;
	/** DebugDrawer has been removed. */
	void DebugDrawerRemoved(deDebugDrawer *debugDrawer) override;
	/** All debugDrawers have been removed. */
	void AllDebugDrawersRemoved() override;
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
