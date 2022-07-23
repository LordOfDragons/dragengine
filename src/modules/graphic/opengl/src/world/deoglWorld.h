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

#ifndef _DEOGLWORLD_H_
#define _DEOGLWORLD_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicWorld.h>

class deoglHeightTerrain;
class deoglRWorld;
class deoglSharedVideoPlayerList;
class deoglSkyInstance;
class deoglComponent;
class deoglBillboard;

class deGraphicOpenGl;
class deWorld;



/**
 * \brief World peer.
 */
class deoglWorld : public deBaseGraphicWorld{
private:
	deGraphicOpenGl &pOgl;
	const deWorld &pWorld;
	deoglRWorld *pRWorld;
	
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
	/** \brief Create world peer. */
	deoglWorld( deGraphicOpenGl &ogl, const deWorld &world );
	
	/** \brief Clean up world peer. */
	virtual ~deoglWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenGl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Retrieves the engine world resource. */
	inline const deWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Render world. */
	inline deoglRWorld *GetRWorld() const{ return pRWorld; }
	
	/** \deprecated Height terrain or \em NULL if not set. */
	inline deoglHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Update dynamic parts of world like the skins. */
	virtual void Update( float elapsed );
	
	/** \brief Shared video player list. */
	inline deoglSharedVideoPlayerList &GetSharedVideoPlayerList() const{ return *pSharedVideoPlayerList; }
	
	
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Add component as require sync if not present. */
	void AddSyncComponent( deoglComponent *component );
	
	/** \brief Remove component as require sync if present. */
	void RemoveSyncComponent( deoglComponent *component );
	
	/** \brief Add billboard as require sync if not present. */
	void AddSyncBillboard( deoglBillboard *billboard );
	
	/** \brief Remove billboard as require sync if present. */
	void RemoveSyncBillboard( deoglBillboard *billboard );
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** Size changed. */
	virtual void SizeChanged();
	
	/** Height Terrain has changed. */
	virtual void HeightTerrainChanged();
	/** Global lighting parameters have changed. */
	virtual void LightingChanged();
	
	/** Sky has been added. */
	virtual void SkyAdded( deSkyInstance *sky );
	/** Sky has been removed. */
	virtual void SkyRemoved( deSkyInstance *sky );
	/** All skys have been removed. */
	virtual void AllSkiesRemoved();
	
	/** Billboard has been added. */
	virtual void BillboardAdded( deBillboard *billboard );
	/** Billboard has been removed. */
	virtual void BillboardRemoved( deBillboard *billboard );
	/** All billboards have been removed. */
	virtual void AllBillboardsRemoved();
	
	/** Camera has been added. */
	virtual void CameraAdded( deCamera *camera );
	/** Camera has been removed. */
	virtual void CameraRemoved( deCamera *camera );
	/** All cameras have been removed. */
	virtual void AllCamerasRemoved();
	
	/** Component has been added. */
	virtual void ComponentAdded( deComponent *component );
	/** Component has been removed. */
	virtual void ComponentRemoved( deComponent *component );
	/** All components have been removed. */
	virtual void AllComponentsRemoved();
	
	/** Environment map probe has been added. */
	virtual void EnvMapProbeAdded( deEnvMapProbe *envMapProbe );
	/** Environment map probe has been removed. */
	virtual void EnvMapProbeRemoved( deEnvMapProbe *envMapProbe );
	/** All environment map probes have been removed. */
	virtual void AllEnvMapProbesRemoved();
	
	/** Light has been added. */
	virtual void LightAdded( deLight *light );
	/** Light has been removed. */
	virtual void LightRemoved( deLight *light );
	/** All lights have been removed. */
	virtual void AllLightsRemoved();
	
	/** Lumimeter has been added. */
	virtual void LumimeterAdded( deLumimeter *lumimeter );
	/** Lumimeter has been removed. */
	virtual void LumimeterRemoved( deLumimeter *lumimeter );
	/** All lumimeters have been removed. */
	virtual void AllLumimetersRemoved();
	
	/** Particle emitter has been added. */
	virtual void ParticleEmitterAdded( deParticleEmitterInstance *emitter );
	/** Particle emitter has been removed. */
	virtual void ParticleEmitterRemoved( deParticleEmitterInstance *emitter );
	/** All particle emitters have been removed. */
	virtual void AllParticleEmittersRemoved();
	
	/** PropField has been added. */
	virtual void PropFieldAdded( dePropField *propField );
	/** PropField has been removed. */
	virtual void PropFieldRemoved( dePropField *propField );
	/** All propFields have been removed. */
	virtual void AllPropFieldsRemoved();
	
	/** DebugDrawer has been added. */
	virtual void DebugDrawerAdded( deDebugDrawer *debugDrawer );
	/** DebugDrawer has been removed. */
	virtual void DebugDrawerRemoved( deDebugDrawer *debugDrawer );
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
