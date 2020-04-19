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

#ifndef _DEOGLRWORLD_H_
#define _DEOGLRWORLD_H_

#include "../envmap/deoglEnvironmentMapList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>

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



/**
 * \brief Render world.
 */
class deoglRWorld : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
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
	
	deoglWorldOctree *pOctree;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render world. */
	deoglRWorld( deoglRenderThread &renderThread, const decDVector &octreeSize );
	
	/** \brief Clean up render camera. */
	virtual ~deoglRWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Mark sky order dirty. */
	void MarkSkyOrderDirty();
	
	/** \brief Sky environment map or \em NULL if not existing. */
	inline deoglEnvironmentMap *GetSkyEnvironmentMap() const{ return pSkyEnvMap; }
	
	
	
	/** \brief Height terrain or \em NULL if not set. */
	inline deoglRHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set height terrain or \em NULL if not set. */
	void SetHeightTerrain( deoglRHeightTerrain *heightTerrain );
	
	
	
	
	/** \brief All lights are disabled. */
	inline bool GetDisableLights() const{ return pDisableLights; }
	
	/** \brief Set if all lights are disabled. */
	void SetDisableLights( bool disable );
	
	/** \brief Ambient light color. */
	inline const decColor &GetAmbientLight() const{ return pAmbientLight; }
	
	/** \brief Set ambient light color. */
	void SetAmbientLight( const decColor &color );
	
	/** \brief Light color matrix. */
	inline const decMatrix &GetLightColorMatrix() const{ return pLightColorMatrix; }
	
	/** \brief Set light color matrix. */
	void SetLightColorMatrix( const decMatrix &matrix );
	
	
	
	/** \brief Octree. */
	inline deoglWorldOctree &GetOctree() const{ return *pOctree; }
	
	/** \brief Visit entire content of the world. */
	void VisitAll( deoglWorldOctreeVisitor &visitor );
	
	/** \brief Visit content of the world using an axis aligned box. */
	void VisitRegion( const decDVector &boxMinExtend, const decDVector &boxMaxExtend,
		deoglWorldOctreeVisitor &visitor );
	
	
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	/*@}*/
	
	
	
	/** \name Reference position. */
	/*@{*/
	/** \brief Notify all affected elements that the reference position changed. */
	void NotifyAllReferencePositionChanged();
	
	/** \brief Reference position for world matrices and positions used directly in shaders. */
	inline const decDVector &GetReferencePosition() const{ return pReferencePosition; }
	
	/**
	 * \brief Set reference position for world matrices and positions used directly in shaders.
	 * \details Notifies all affected elements about the change.
	 */
	void SetReferencePosition( const decDVector &position );
	
	/**
	 * \brief Check new reference position against the current reference position.
	 * \details If positions are further apart than the validity distance in any of the vector
	 *          components the reference position is updated and all affected elements notified
	 *          about the change.
	 */
	void CheckReferencePosition( const decDVector &position );
	
	/** \brief Validity distance for reference position checks. */
	inline double GetValidReferenceDistance() const{ return pValidReferenceDistance; }
	
	/** \brief Set validity distance for reference position checks. */
	void SetValidReferenceDistance( double distance );
	/*@}*/
	
	
	
	/** \name Prop fields. */
	/*@{*/
	/** \brief Number of prop fields. */
	int GetPropFieldCount() const;
	
	/** \brief Prop field at index. */
	deoglRPropField *GetPropFieldAt( int index ) const;
	
	/** \brief Add prop field. */
	void AddPropField( deoglRPropField *propField );
	
	/** \brief Remove prop field. */
	void RemovePropField( deoglRPropField *propField );
	
	/** \brief Remove all prop fields. */
	void RemoveAllPropFields();
	
	/** \brief Remove prop fields marked for removal. */
	void RemoveRemovalMarkedPropFields();
	/*@}*/
	
	
	
	/** \name Particle emitter instances. */
	/*@{*/
	/** \brief Number of particle emitter instances. */
	int GetParticleEmitterInstanceCount() const;
	
	/** \brief Particle emitter instance at index. */
	deoglRParticleEmitterInstance *GetParticleEmitterInstanceAt( int index ) const;
	
	/** \brief Add particle emitter instance. */
	void AddParticleEmitterInstance( deoglRParticleEmitterInstance *instance );
	
	/** \brief Remove particle emitter instance. */
	void RemoveParticleEmitterInstance( deoglRParticleEmitterInstance *instance );
	
	/** \brief Remove all particle emitter instances. */
	void RemoveAllParticleEmitterInstances();
	
	/** \brief Remove particle emitter instances marked for removal. */
	void RemoveRemovalMarkedParticleEmitterInstances();
	/*@}*/
	
	
	
	/** \name Components. */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Root component. */
	inline deoglRComponent *GetRootComponent() const{ return pRootComponent; }
	
	/** \brief Add component. */
	void AddComponent( deoglRComponent *component );
	
	/** \brief Remove component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** \brief Remove all component. */
	void RemoveAllComponents();
	
	/** \brief Remove components marked for removal. */
	void RemoveRemovalMarkedComponents();
	/*@}*/
	
	
	
	/** \name Lights. */
	/*@{*/
	/** \brief Number of lights. */
	int GetLightCount() const;
	
	/** \brief Light at index. */
	deoglRLight *GetLightAt( int index ) const;
	
	/** \brief Add light. */
	void AddLight( deoglRLight *light );
	
	/** \brief Remove light. */
	void RemoveLight( deoglRLight *light );
	
	/** \brief Remove all light. */
	void RemoveAllLights();
	
	/** \brief Remove lights marked for removal. */
	void RemoveRemovalMarkedLights();
	/*@}*/
	
	
	
	/** \name Environment Map Probes. */
	/*@{*/
	/** \brief Number of environment map probes. */
	int GetEnvMapProbeCount() const;
	
	/** \brief environment map probe at index. */
	deoglREnvMapProbe *GetEnvMapProbeAt( int index ) const;
	
	/** \brief Add environment map probe. */
	void AddEnvMapProbe( deoglREnvMapProbe *envMapProbe );
	
	/** \brief Remove environment map probe. */
	void RemoveEnvMapProbe( deoglREnvMapProbe *envMapProbe );
	
	/** \brief Remove all environment map probes. */
	void RemoveAllEnvMapProbes();
	
	/**
	 * \brief Remove environment map probes marked for removal.
	 * \details Called from the main thread.
	 */
	void RemoveRemovalMarkedEnvMapProbes();
	/*@}*/
	
	
	
	/** \name Environment maps */
	/*@{*/
	/** \brief Environment map list. */
	inline const deoglEnvironmentMapList &GetEnvMapList() const{ return pEnvMapList; }
	
	/** \brief Add environment map. */
	void AddEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Remove environment map. */
	void RemoveEnvMap( deoglEnvironmentMap *envmap );
	
	/** \brief Remove all environment maps. */
	void RemoveAllEnvMaps();
	
	/** \brief Number of environment maps that can be updated this frame. */
	inline int GetEnvMapUpdateCount() const{ return pEnvMapUpdateCount; }
	
	/** \brief Reset env maps update count. */
	void ResetEnvMapUpdateCount();
	
	/** \brief Decrease environment map update count by one. */
	void DecEnvMapUpdateCount();
	
	/** \brief Notify environment maps that the sky changed. */
	void EnvMapsNotifySkyChanged();
	
	/** \brief Shared environment map render plan. */
	inline deoglRenderPlan *GetEnvMapRenderPlan() const{ return pEnvMapRenderPlan; }
	
	/**
	 * \brief Mark environment map layout dirty.
	 * \details The next time a rendering update is done all components are notified about the change.
	 */
	void InvalidateEnvMapLayout();
	/*@}*/
	
	
	
	/** \name Lumimeters. */
	/*@{*/
	/** \brief Number of lumimeters. */
	int GetLumimeterCount() const;
	
	/** \brief Lumimeter at index. */
	deoglRLumimeter *GetLumimeterAt( int index ) const;
	
	/** \brief Add lumimeter. */
	void AddLumimeter( deoglRLumimeter *lumimeter );
	
	/** \brief Remove lumimeter. */
	void RemoveLumimeter( deoglRLumimeter *lumimeter );
	
	/** \brief Remove all lumimeter. */
	void RemoveAllLumimeters();
	
	/** \brief Remove lumimeters marked for removal. */
	void RemoveRemovalMarkedLumimeters();
	/*@}*/
	
	
	
	/** \name Billboards. */
	/*@{*/
	/** \brief Number of billboards. */
	inline int GetBillboardCount() const{ return pBillboardCount; }
	
	/** \brief Root billboard. */
	inline deoglRBillboard *GetRootBillboard() const{ return pRootBillboard; }
	
	/** \brief Add billboard. */
	void AddBillboard( deoglRBillboard *billboard );
	
	/** \brief Remove billboard. */
	void RemoveBillboard( deoglRBillboard *billboard );
	
	/** \brief Remove all billboard. */
	void RemoveAllBillboards();
	
	/** \brief Remove billboards marked for removal. */
	void RemoveRemovalMarkedBillboards();
	/*@}*/
	
	
	
	/** \name Skies. */
	/*@{*/
	/** \brief Number of skys. */
	int GetSkyCount() const;
	
	/** \brief Sky at index. */
	deoglRSkyInstance *GetSkyAt( int index ) const;
	
	/** \brief Add sky. */
	void AddSky( deoglRSkyInstance *sky );
	
	/** \brief Remove sky. */
	void RemoveSky( deoglRSkyInstance *sky );
	
	/** \brief Remove all skys. */
	void RemoveAllSkies();
	
	/** \brief Remove skys marked for removal. */
	void RemoveRemovalMarkedSkies();
	/*@}*/
	
	
	
	/** \name Debug Drawers. */
	/*@{*/
	/** \brief Number of debug drawers. */
	int GetDebugDrawerCount() const;
	
	/** \brief Debug drawer at index. */
	deoglRDebugDrawer *GetDebugDrawerAt( int index ) const;
	
	/** \brief Add debug drawer. */
	void AddDebugDrawer( deoglRDebugDrawer *debugDrawer );
	
	/** \brief Remove debug drawer. */
	void RemoveDebugDrawer( deoglRDebugDrawer *debugDrawer );
	
	/** \brief Remove all debug drawer. */
	void RemoveAllDebugDrawers();
	
	/** \brief Remove debug drawers marked for removal. */
	void RemoveRemovalMarkedDebugDrawers();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pReorderSkies();
	void pCreateSkyEnvMap();
	void pFreeSkyEnvMap();
};

#endif
