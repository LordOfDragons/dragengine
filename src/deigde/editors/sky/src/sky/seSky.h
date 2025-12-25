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

#ifndef _SESKY_H_
#define _SESKY_H_

#include "seSkyListener.h"
#include "controller/seController.h"
#include "layer/seLayer.h"
#include "link/seLink.h"

#include <deigde/editableentity/igdeEditableEntity.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/world/deWorld.h>

class seBody;

class igdeCamera;

class deLogger;



/**
 * \brief Sky.
 */
class seSky : public igdeEditableEntity{
public:
	typedef deTObjectReference<seSky> Ref;
	
private:
	deWorld::Ref pEngWorld;
	deSky::Ref pEngSky;
	deSkyInstance::Ref pEngSkyInstance;
	bool pNeedsRebuildSky;
	
	igdeWObject::Ref pEnvObject;
	igdeCamera *pCamera;
	
	deDebugDrawer::Ref pDDHorizon;
	igdeWDebugDrawerShape::Ref pDDSHorizon;
	
	decColor pBgColor;
	
	seController::List pControllers;
	seController::Ref pActiveController;
	
	seLink::List pLinks;
	seLink::Ref pActiveLink;
	
	seLayer::List pLayers;
	seLayer::Ref pActiveLayer;
	
	decTObjectOrderedSet<seSkyListener> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates sky. */
	seSky(igdeEnvironment *environment);
	
protected:
	/** \brief Clean up sky. */
	virtual ~seSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Engine world. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Engine sky. */
	inline const deSky::Ref &GetEngineSky() const{ return pEngSky; }
	
	/** \brief Engine sky instance. */
	inline const deSkyInstance::Ref &GetEngineSkyInstance() const{ return pEngSkyInstance; }
	
	/** \brief Camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Environment wrapper object. */
	inline const igdeWObject::Ref &GetEnvObject() const{ return pEnvObject; }
	
	/** \brief Horizon debug drawer. */
	inline deDebugDrawer *GetDDHorizon(){ return pDDHorizon; }
	
	/** \brief Horizon debug drawer shape. */
	inline const igdeWDebugDrawerShape::Ref &GetDDSHorizon(){ return pDDSHorizon; }
	
	/** \brief Sky compass overlay is drawn. */
	bool GetDrawSkyCompass() const;
	
	/** \brief Set if sky compass overlay is drawn. */
	void SetDrawSkyCompass(bool drawSkyCompass);
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update. */
	void Update(float elapsed);
	
	/** \brief Reset. */
	void Reset();
	
	/** \brief Rebuild engine sky. */
	void RebuildEngineSky();
	
	/** \brief Update relative resources after changing the base path. */
	void UpdateRelativeResources();
	
	
	
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor(const decColor &color);
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Controllers. */
	const seController::List &GetControllers() const{ return pControllers; }
	
	/** \brief Add controller. */
	void AddController(seController *controller);
	
	/** \brief Insert controller. */
	void InsertControllerAt(seController *controller, int index);
	
	/** \brief Move controller. */
	void MoveControllerTo(seController *controller, int index);
	
	/** \brief Remove controller. */
	void RemoveController(seController *controller);
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Active controller or \em nullptr. */
	inline const seController::Ref &GetActiveController() const{ return pActiveController; }
	
	/** \brief Set active controller or \em nullptr. */
	void SetActiveController(seController *controller);
	
	/** \brief Count controller usage. */
	int CountControllerUsage(seController *controller) const;
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** \brief Links. */
	const seLink::List &GetLinks() const{ return pLinks; }
	
	/** \brief Add link. */
	void AddLink(seLink *link);
	
	/** \brief Remove link. */
	void RemoveLink(seLink *link);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Active link or \em nullptr. */
	inline const seLink::Ref &GetActiveLink() const{ return pActiveLink; }
	
	/** \brief Set active link or \em nullptr. */
	void SetActiveLink(seLink *link);
	
	/** \brief Count link usage. */
	int CountLinkUsage(seLink *link) const;
	/*@}*/
	
	
	
	/** \name Layers */
	/*@{*/
	/** \brief Layers. */
	const seLayer::List &GetLayers() const{ return pLayers; }
	
	/** \brief Add layer. */
	void AddLayer(seLayer *layer);
	
	/** \brief Insert layer. */
	void InsertLayerAt(seLayer *layer, int index);
	
	/** \brief Move layer. */
	void MoveLayerTo(seLayer *layer, int index);
	
	/** \brief Remove layer. */
	void RemoveLayer(seLayer *layer);
	
	/** \brief Remove all layers. */
	void RemoveAllLayers();
	
	/** \brief Active layer or \em nullptr. */
	inline const seLayer::Ref &GetActiveLayer() const{ return pActiveLayer; }
	
	/** \brief Set active layer or \em nullptr. */
	void SetActiveLayer(seLayer *layer);
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add listener. */
	void AddListener(seSkyListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(seSkyListener *listener);
	
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
	void NotifyControllerChanged(seController *controller);
	
	/** \brief Notify controller name changed. */
	void NotifyControllerNameChanged(seController *controller);
	
	/** \brief Notify controller value changed. */
	void NotifyControllerValueChanged(seController *controller);
	
	/** \brief Controller selection changed. */
	void NotifyControllerSelectionChanged();
	
	/** \brief Active controller changed. */
	void NotifyActiveControllerChanged();
	
	
	
	/** \brief Notify link added or removed. */
	void NotifyLinkStructureChanged();
	
	/** \brief Notify link changed. */
	void NotifyLinkChanged(seLink *link);
	
	/** \brief Notify link name changed. */
	void NotifyLinkNameChanged(seLink *link);
	
	/** \brief Link link changed. */
	void NotifyLinkSelectionChanged();
	
	/** \brief Active link changed. */
	void NotifyActiveLinkChanged();
	
	
	
	/** \brief Notify layer added or removed. */
	void NotifyLayerStructureChanged();
	
	/** \brief Notify layer changed. */
	void NotifyLayerChanged(seLayer *layer);
	
	/** \brief Notify layer name changed. */
	void NotifyLayerNameChanged(seLayer *layer);
	
	/** \brief Layer selection changed. */
	void NotifyLayerSelectionChanged();
	
	/** \brief Active layer changed. */
	void NotifyActiveLayerChanged();
	
	
	
	/** \brief Notify body added or removed. */
	void NotifyBodyStructureChanged(seLayer *layer);
	
	/** \brief Notify body changed. */
	void NotifyBodyChanged(seLayer *layer, seBody *body);
	
	/** \brief Body selection changed. */
	void NotifyBodySelectionChanged(seLayer *layer);
	
	/** \brief Active body changed. */
	void NotifyActiveBodyChanged(seLayer *layer);
	
	
	
	/** \brief Notify all target changed. */
	void NotifyTargetChanged(seLayer *layer, deSkyLayer::eTargets target);
	
	/** \brief Notify all active target changed. */
    void NotifyActiveTargetChanged(seLayer *layer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pBuildShapeHorizon();
};

#endif
