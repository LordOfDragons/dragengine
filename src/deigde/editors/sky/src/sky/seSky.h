/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SESKY_H_
#define _SESKY_H_

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>

#include "controller/seControllerList.h"
#include "layer/seLayerList.h"
#include "link/seLinkList.h"

class seBody;
class seSkyListener;

class igdeCamera;
class igdeWObject;

class deDebugDrawer;
class deLogger;
class deSky;
class deSkyInstance;
class deWorld;



/**
 * \brief Sky.
 */
class seSky : public igdeEditableEntity{
private:
	deWorld *pEngWorld;
	deSky *pEngSky;
	deSkyInstance *pEngSkyInstance;
	bool pNeedsRebuildSky;
	
	igdeWObject *pEnvObject;
	igdeCamera *pCamera;
	
	deDebugDrawer *pDDHorizon;
	igdeWDebugDrawerShape pDDSHorizon;
	
	decColor pBgColor;
	
	seControllerList pControllers;
	seController *pActiveController;
	
	seLinkList pLinks;
	seLink *pActiveLink;
	
	seLayerList pLayers;
	seLayer *pActiveLayer;
	
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates sky. */
	seSky( igdeEnvironment *environment );
	
protected:
	/** \brief Clean up sky. */
	virtual ~seSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Engine sky. */
	inline deSky *GetEngineSky() const{ return pEngSky; }
	
	/** \brief Engine sky instance. */
	inline deSkyInstance *GetEngineSkyInstance() const{ return pEngSkyInstance; }
	
	/** \brief Camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** \brief Horizon debug drawer. */
	inline deDebugDrawer *GetDDHorizon(){ return pDDHorizon; }
	
	/** \brief Horizon debug drawer shape. */
	inline igdeWDebugDrawerShape &GetDDSHorizon(){ return pDDSHorizon; }
	
	/** \brief Sky compass overlay is drawn. */
	bool GetDrawSkyCompass() const;
	
	/** \brief Set if sky compass overlay is drawn. */
	void SetDrawSkyCompass( bool drawSkyCompass );
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Reset. */
	void Reset();
	
	/** \brief Rebuild engine sky. */
	void RebuildEngineSky();
	
	/** \brief Update relative resources after changing the base path. */
	void UpdateRelativeResources();
	
	
	
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Controllers. */
	const seControllerList &GetControllers() const{ return pControllers; }
	
	/** \brief Add controller. */
	void AddController( seController *controller );
	
	/** \brief Insert controller. */
	void InsertControllerAt( seController *controller, int index );
	
	/** \brief Move controller. */
	void MoveControllerTo( seController *controller, int index );
	
	/** \brief Remove controller. */
	void RemoveController( seController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Active controller or \em NULL. */
	inline seController *GetActiveController() const{ return pActiveController; }
	
	/** \brief Set active controller or \em NULL. */
	void SetActiveController( seController *controller );
	
	/** \brief Count controller usage. */
	int CountControllerUsage( seController *controller ) const;
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** \brief Links. */
	const seLinkList &GetLinks() const{ return pLinks; }
	
	/** \brief Add link. */
	void AddLink( seLink *link );
	
	/** \brief Remove link. */
	void RemoveLink( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Active link or \em NULL. */
	inline seLink *GetActiveLink() const{ return pActiveLink; }
	
	/** \brief Set active link or \em NULL. */
	void SetActiveLink( seLink *link );
	
	/** \brief Count link usage. */
	int CountLinkUsage( seLink *link ) const;
	/*@}*/
	
	
	
	/** \name Layers */
	/*@{*/
	/** \brief Layers. */
	const seLayerList &GetLayers() const{ return pLayers; }
	
	/** \brief Add layer. */
	void AddLayer( seLayer *layer );
	
	/** \brief Insert layer. */
	void InsertLayerAt( seLayer *layer, int index );
	
	/** \brief Move layer. */
	void MoveLayerTo( seLayer *layer, int index );
	
	/** \brief Remove layer. */
	void RemoveLayer( seLayer *layer );
	
	/** \brief Remove all layers. */
	void RemoveAllLayers();
	
	/** \brief Active layer or \em NULL. */
	inline seLayer *GetActiveLayer() const{ return pActiveLayer; }
	
	/** \brief Set active layer or \em NULL. */
	void SetActiveLayer( seLayer *layer );
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add listener. */
	void AddListener( seSkyListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( seSkyListener *listener );
	
	/** \brief Notify listeners changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify sky parameter changed. */
	void NotifySkyChanged();
	
	/** \brief Notify environment object changed. */
	void NotifyEnvObjectChanged();
	
	/** \brief Notify view changed. */
	void NotifyViewChanged();
	
	/** \brief Notify camera changed. */
	void NotifyCameraChanged();
	
	
	
	/** \brief Notify controller added or removed. */
	void NotifyControllerStructureChanged();
	
	/** \brief Notify controller changed. */
	void NotifyControllerChanged( seController *controller );
	
	/** \brief Notify controller name changed. */
	void NotifyControllerNameChanged( seController *controller );
	
	/** \brief Notify controller value changed. */
	void NotifyControllerValueChanged( seController *controller );
	
	/** \brief Controller selection changed. */
	void NotifyControllerSelectionChanged();
	
	/** \brief Active controller changed. */
	void NotifyActiveControllerChanged();
	
	
	
	/** \brief Notify link added or removed. */
	void NotifyLinkStructureChanged();
	
	/** \brief Notify link changed. */
	void NotifyLinkChanged( seLink *link );
	
	/** \brief Notify link name changed. */
	void NotifyLinkNameChanged( seLink *link );
	
	/** \brief Link link changed. */
	void NotifyLinkSelectionChanged();
	
	/** \brief Active link changed. */
	void NotifyActiveLinkChanged();
	
	
	
	/** \brief Notify layer added or removed. */
	void NotifyLayerStructureChanged();
	
	/** \brief Notify layer changed. */
	void NotifyLayerChanged( seLayer *layer );
	
	/** \brief Notify layer name changed. */
	void NotifyLayerNameChanged( seLayer *layer );
	
	/** \brief Layer selection changed. */
	void NotifyLayerSelectionChanged();
	
	/** \brief Active layer changed. */
	void NotifyActiveLayerChanged();
	
	
	
	/** \brief Notify body added or removed. */
	void NotifyBodyStructureChanged( seLayer *layer );
	
	/** \brief Notify body changed. */
	void NotifyBodyChanged( seLayer *layer, seBody *body );
	
	/** \brief Body selection changed. */
	void NotifyBodySelectionChanged( seLayer *layer );
	
	/** \brief Active body changed. */
	void NotifyActiveBodyChanged( seLayer *layer );
	
	
	
	/** \brief Notify all target changed. */
	void NotifyTargetChanged( seLayer *layer, deSkyLayer::eTargets target );
	
	/** \brief Notify all active target changed. */
    void NotifyActiveTargetChanged( seLayer *layer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pBuildShapeHorizon();
};

#endif
