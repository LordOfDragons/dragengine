/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEVIEWACTIVEOBJECT_H_
#define _GDEVIEWACTIVEOBJECT_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/component/deComponentReference.h>
#include <dragengine/resources/debug/deDebugDrawerReference.h>
#include <dragengine/resources/model/deModelReference.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceReference.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/sky/deSkyInstanceReference.h>

class gdeWindowMain;
class gdeViewActiveObjectListener;

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCBillboard;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeOCCamera;
class gdeOCLight;
class gdeOCEnvMapProbe;
class gdeOCNavigationSpace;
class gdeOCNavigationBlocker;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;

class igdeCamera;
class igdeWDebugDrawerShape;

class deCollider;



/**
 * \brief Active object view.
 * 
 * \todo Add DebugDrawerShape to show ObjectClass bounding box if an object class is selected.
 */
class gdeViewActiveObject : public igdeViewRenderWindow{
private:
	gdeWindowMain &pWindowMain;
	gdeViewActiveObjectListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	gdeObjectClass *pObjectClass;
	
	deComponentReference pPreviewComponent;
	deModelReference pPreviewModelBox;
	deSkinReference pPreviewSkin;
	deSkyInstanceReference pPreviewSky;
	deParticleEmitterInstanceReference pPreviewParticleEmitter;
	
	decObjectList pOCBillboards;
	decObjectList pOCCameras;
	decObjectList pOCComponents;
	decObjectList pOCParticleEmitters;
	decObjectList pOCForceFields;
	decObjectList pOCLights;
	decObjectList pOCEnvMapProbes;
	decObjectList pOCNavSpaces;
	decObjectList pOCNavBlockers;
	decObjectList pOCSnapPoints;
	decObjectList pOCSpeakers;
	
	deDebugDrawerReference pDebugDrawer;
	
	bool pShowEnvMapProbes;
	bool pShowNavBlockers;
	
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	gdeViewActiveObject( gdeWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~gdeViewActiveObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	
	
	/** \brief Window main. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Monitored game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition to monitor. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Debug drawer. */
	inline deDebugDrawer *GetDebugDrawer() const{ return pDebugDrawer; }
	
	
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	
	
	/** \brief Selected object class or \em NULL. */
	inline gdeObjectClass *GetObjectClass() const{ return pObjectClass; }
	
	/** \brief Set camera position to show active object properly. */
	void ResetCameraPosition();
	
	/** \brief Clear resources. */
	void ClearResources();
	
	/** \brief Init selected object. */
	void InitSelectedObject();
	
	/** \brief Selected sub object changed. */
	void SelectedSubObjectChanged();
	
	
	
	/** \brief Attach component collider or \em NULL. */
	deCollider *GetAttachComponentCollider() const;
	
	
	
	/** \brief Rebuild object class billboard. */
	void RebuildOCBillboards();
	
	/** \brief Rebuild object class billboard. */
	void RebuildOCBillboard( gdeOCBillboard *ocbillboard );
	
	/** \brief Rebuild object class cameras. */
	void RebuildOCCameras();
	
	/** \brief Rebuild object class camera. */
	void RebuildOCCamera( gdeOCCamera *occamera );
	
	/** \brief Rebuild object class components. */
	void RebuildOCComponents();
	
	/** \brief Rebuild object class component. */
	void RebuildOCComponent( gdeOCComponent *occomponent );
	
	/** \brief Update object class component texture. */
	void UpdateOCComponentTexture( gdeOCComponent *occomponent, gdeOCComponentTexture *texture );
	
	/** \brief Rebuild object class lights. */
	void RebuildOCLights();
	
	/** \brief Rebuild object class light. */
	void RebuildOCLight( gdeOCLight *oclight );
	
	/** \brief Rebuild object class environment map probes. */
	void RebuildOCEnvMapProbes();
	
	/** \brief Rebuild object class environment map probe. */
	void RebuildOCEnvMapProbe( gdeOCEnvMapProbe *ocenvMapProbe );
	
	/** \brief Rebuild object class navigation spaces. */
	void RebuildOCNavSpaces();
	
	/** \brief Rebuild object class navigation spaces. */
	void RebuildOCNavSpace( gdeOCNavigationSpace *ocnavspace );
	
	/** \brief Rebuild object class navigation blockers. */
	void RebuildOCNavBlockers();
	
	/** \brief Rebuild object class navigation blockers. */
	void RebuildOCNavBlocker( gdeOCNavigationBlocker *ocnavblocker );
	
	/** \brief Rebuild object class particle emitters. */
	void RebuildOCParticleEmitters();
	
	/** \brief Rebuild object class particle emitter. */
	void RebuildOCParticleEmitter( gdeOCParticleEmitter *ocemitter );
	
	/** \brief Rebuild object class force fields. */
	void RebuildOCForceFields();
	
	/** \brief Rebuild object class force field. */
	void RebuildOCForceField( gdeOCForceField *ocfield );
	
	/** \brief Rebuild object class snap points. */
	void RebuildOCSnapPoints();
	
	/** \brief Rebuild object class snap points. */
	void RebuildOCSnapPoint( gdeOCSnapPoint *ocsnapPoint );
	
	/** \brief Rebuild object class speakers. */
	void RebuildOCSpeakers();
	
	/** \brief Rebuild object class speaker. */
	void RebuildOCSpeaker( gdeOCSpeaker *ocspeaker );
	
	
	
	/** \brief Show all environment map probes. */
	inline bool GetShowEnvMapProbes() const{ return pShowEnvMapProbes; }
	
	/** \brief Set show all environment map probes. */
	void SetShowEnvMapProbes( bool show );
	
	/** \brief Show all navigation blockers. */
	inline bool GetShowNavBlockers() const{ return pShowNavBlockers; }
	
	/** \brief Set show all navigation blockers. */
	void SetShowNavBlockers( bool show );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateCameraFovRatio();
	
	void pInitObjectClass();
	void pInitSkin();
	void pInitSky();
	void pInitParticleEmitter();
	
	void pInitObjectClassOCs( const gdeObjectClass &objectClass );
	void pInitOCBillboards( const gdeObjectClass &objectClass );
	void pInitOCCameras( const gdeObjectClass &objectClass );
	void pInitOCComponents( const gdeObjectClass &objectClass );
	void pInitOCLights( const gdeObjectClass &objectClass );
	void pInitOCParticleEmitters( const gdeObjectClass &objectClass );
	void pInitOCForceFields( const gdeObjectClass &objectClass );
	void pInitOCEnvMapProbes( const gdeObjectClass &objectClass );
	void pInitOCNavigationSpaces( const gdeObjectClass &objectClass );
	void pInitOCNavigationBlockers( const gdeObjectClass &objectClass );
	void pInitOCSnapPoints( const gdeObjectClass &objectClass );
	void pInitOCSpeakers( const gdeObjectClass &objectClass );
	void pRebuildOCBillboards( const gdeObjectClass &objectClass );
	void pRebuildOCCameras( const gdeObjectClass &objectClass );
	void pRebuildOCComponents( const gdeObjectClass &objectClass );
	void pRebuildOCLights( const gdeObjectClass &objectClass );
	void pRebuildOCParticleEmitters( const gdeObjectClass &objectClass );
	void pRebuildOCForceFields( const gdeObjectClass &objectClass );
	void pRebuildOCEnvMapProbes( const gdeObjectClass &objectClass );
	void pRebuildOCNavigationSpaces( const gdeObjectClass &objectClass );
	void pRebuildOCNavigationBlockers( const gdeObjectClass &objectClass );
	void pRebuildOCSnapPoints( const gdeObjectClass &objectClass );
	void pRebuildOCSpeakers( const gdeObjectClass &objectClass );
	
	void pOCReattachAllObjects();
	
	void pCenterOnObjectClass( igdeCamera &camera );
	void pCenterOnSkin( igdeCamera &camera );
	void pCenterOnSky( igdeCamera &camera );
	void pCenterOnParticleEmitter( igdeCamera &camera );
	void pCenterOnBox( igdeCamera &camera, const decVector &minExtend, const decVector &maxExtend );
};

#endif
