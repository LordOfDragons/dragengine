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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seSky.h"
#include "seSkyListener.h"
#include "body/seBody.h"
#include "controller/seController.h"
#include "controller/seControllerTarget.h"
#include "layer/seLayer.h"
#include "link/seLink.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLink.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Class seSky
////////////////

// Constructor, destructor
////////////////////////////

seSky::seSky(igdeEnvironment *environment) :
igdeEditableEntity(environment),

pEngWorld(NULL),
pEngSky(NULL),
pEngSkyInstance(NULL),
pNeedsRebuildSky(true),

pCamera(NULL),

pDDHorizon(NULL),

pActiveController(NULL),

pActiveLink(NULL),

pActiveLayer(NULL)
{
	deEngine * const engine = GetEngine();
	
	try{
		SetFilePath("new.desky");
		
		pEngWorld = engine->GetWorldManager()->CreateWorld();
		pEngWorld->SetGravity(decVector());
		pEngWorld->SetDisableLights(false);
		pEngWorld->SetAmbientLight(decColor());
		
		pCamera = new igdeCamera(engine);
		pCamera->SetEngineWorld(pEngWorld);
		pCamera->Reset();
		pCamera->SetHighestIntensity(20.0f);
		pCamera->SetLowestIntensity(1.0f);
		pCamera->SetAdaptionTime(1.0f);
		
		pEngSkyInstance = engine->GetSkyInstanceManager()->CreateSkyInstance();
		pEngWorld->AddSky(pEngSkyInstance);
		
		pEnvObject.TakeOver(new igdeWObject(*environment));
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, -1.8, 0.0));
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		pDDHorizon = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDHorizon->SetXRay(true);
		pEngWorld->AddDebugDrawer(pDDHorizon);
		
		pDDSHorizon.SetParentDebugDrawer(pDDHorizon);
		pDDSHorizon.SetVisible(true);
		pDDSHorizon.SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		pDDSHorizon.SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.5f));
		pBuildShapeHorizon();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

seSky::~seSky(){
	pCleanUp();
}



// Management
///////////////

bool seSky::GetDrawSkyCompass() const{
	return pDDHorizon->GetVisible();
}

void seSky::SetDrawSkyCompass(bool drawSkyCompass){
	pDDHorizon->SetVisible(drawSkyCompass);
}



void seSky::Dispose(){
	SetActiveLayer(NULL);
	SetActiveLink(NULL);
	SetActiveController(NULL);
	
	RemoveAllLayers();
	RemoveAllLinks();
	RemoveAllControllers();
	
	GetUndoSystem()->RemoveAll();
}

void seSky::Reset(){
	SetActiveLayer(NULL);
	SetActiveLink(NULL);
	SetActiveController(NULL);
	
	GetUndoSystem()->RemoveAll();
}

void seSky::Update(float elapsed){
	if(pNeedsRebuildSky){
		RebuildEngineSky();
	}
	
	pEnvObject->Update(elapsed);
	pEngWorld->Update(elapsed);
}

void seSky::RebuildEngineSky(){
	// drop old sky
	pEngSkyInstance->SetSky(NULL);
	
	if(pEngSky){
		pEngSky->FreeReference();
		pEngSky = NULL;
	}
	
	// create sky
	int i, j, k;
	
	try{
		pEngSky = GetEngine()->GetSkyManager()->CreateSky();
		
		pEngSky->SetBgColor(pBgColor);
		
		// controllers
		const int controllerCount = pControllers.GetCount();
		pEngSky->SetControllerCount(controllerCount);
		
		for(i=0; i<controllerCount; i++){
			const seController &sourceController = *pControllers.GetAt(i);
			deSkyController &destController = pEngSky->GetControllerAt(i);
			
			destController.SetValueRange(sourceController.GetMinimumValue(),
				sourceController.GetMaximumValue());
			destController.SetCurrentValue(sourceController.GetCurrentValue());
			destController.SetClamp(sourceController.GetClamp());
			destController.SetFrozen(sourceController.GetFrozen());
		}
		
		// links
		const int linkCount = pLinks.GetCount();
		pEngSky->SetLinkCount(linkCount);
		
		for(i=0; i<linkCount; i++){
			const seLink &sourceLink = *pLinks.GetAt(i);
			deSkyLink &destLink = pEngSky->GetLinkAt(i);
			
			destLink.SetController(sourceLink.GetController()
				? sourceLink.GetController()->GetIndex() : -1);
			destLink.GetCurve() = sourceLink.GetCurve();
			destLink.SetRepeat(sourceLink.GetRepeat());
		}
		
		// layers
		const int layerCount = pLayers.GetCount();
		pEngSky->SetLayerCount(layerCount);
		
		for(i=0; i<layerCount; i++){
			const seLayer &sourceLayer = *pLayers.GetAt(i);
			deSkyLayer &destLayer = pEngSky->GetLayerAt(i);
			
			destLayer.SetOffset(sourceLayer.GetOffset());
			destLayer.SetOrientation(sourceLayer.GetOrientation() * DEG2RAD);
			destLayer.SetColor(sourceLayer.GetColor());
			destLayer.SetIntensity(sourceLayer.GetIntensity());
			destLayer.SetTransparency(sourceLayer.GetTransparency());
			destLayer.SetSkin(sourceLayer.GetEngineSkin());
			destLayer.SetLightOrientation(decQuaternion::CreateFromEuler(
				sourceLayer.GetLightOrientation() * DEG2RAD));
			destLayer.SetLightColor(sourceLayer.GetLightColor());
			destLayer.SetLightIntensity(sourceLayer.GetLightIntensity());
			destLayer.SetAmbientIntensity(sourceLayer.GetAmbientIntensity());
			
			const seBodyList &bodies = sourceLayer.GetBodies();
			const int bodyCount = bodies.GetCount();
			destLayer.SetBodyCount(bodyCount);
			for(j=0; j<bodyCount; j++){
				const seBody &sourceBody = *bodies.GetAt(j);
				deSkyLayerBody &destBody = destLayer.GetBodyAt(j);
				
				destBody.SetOrientation(decQuaternion::CreateFromEuler(
					sourceBody.GetOrientation() * DEG2RAD));
				destBody.SetSize(sourceBody.GetSize() * DEG2RAD);
				destBody.SetColor(sourceBody.GetColor());
				destBody.SetSkin(sourceBody.GetEngineSkin());
			}
			
			for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
				const deSkyLayer::eTargets target = (deSkyLayer::eTargets)j;
				const seControllerTarget &sourceTarget = sourceLayer.GetTarget(target);
				deSkyControllerTarget &destTarget = destLayer.GetTarget(target);
				
				const seLinkList &ctLinks = sourceTarget.GetLinks();
				const int ctLinkCount = ctLinks.GetCount();
				for(k=0; k<ctLinkCount; k++){
					destTarget.AddLink(ctLinks.GetAt(k)->GetIndex());
				}
			}
			
			destLayer.SetMultiplyBySkyLight(sourceLayer.GetMuliplyBySkyLight());
			destLayer.SetMultiplyBySkyColor(sourceLayer.GetMuliplyBySkyColor());
		}
		
		pEngSky->NotifyParametersChanged();
		
	}catch(const deException &){
		if(pEngSky){
			pEngSky->FreeReference();
			pEngSky = NULL;
		}
		throw;
	}
	
	// assign sky
	pEngSkyInstance->SetSky(pEngSky);
	pNeedsRebuildSky = false;
};

void seSky::UpdateRelativeResources(){
	const int layerCount = pLayers.GetCount();
	int i;
	for(i=0; i<layerCount; i++){
		pLayers.GetAt(i)->UpdateRelativeResources();
	}
}



void seSky::SetBgColor(const decColor &color){
	if(color.IsEqualTo(pBgColor)){
		return;
	}
	
	pBgColor = color;
	NotifySkyChanged();
}



// Controllers
////////////////

void seSky::AddController(seController *controller){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.Add(controller);
	controller->SetSky(this);
	controller->SetIndex(pControllers.GetCount() - 1);
	
	NotifyControllerStructureChanged();
	
	if(!pActiveController){
		SetActiveController(controller);
	}
}

void seSky::InsertControllerAt(seController *controller, int index){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.InsertAt(controller, index);
	controller->SetSky(this);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetIndex(i);
	}
	
	NotifyControllerStructureChanged();
	
	if(!pActiveController){
		SetActiveController(controller);
	}
}

void seSky::MoveControllerTo(seController *controller, int index){
	pControllers.MoveTo(controller, index);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetIndex(i);
	}
	
	NotifyControllerStructureChanged();
}

void seSky::RemoveController(seController *controller){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	
	if(controller->GetActive()){
		int controllerCount = pControllers.GetCount();
		int controllerIndex = controller->GetIndex();
		
		if(controllerIndex < controllerCount - 1){
			SetActiveController(pControllers.GetAt(controllerIndex + 1));
			
		}else if(controllerIndex > 0){
			SetActiveController(pControllers.GetAt(controllerIndex - 1));
			
		}else{
			SetActiveController(NULL);
		}
	}
	
	controller->SetSky(NULL);
	pControllers.Remove(controller);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetIndex(i);
	}
	
	NotifyControllerStructureChanged();
}

void seSky::RemoveAllControllers(){
	const int controllerCount = pControllers.GetCount();
	int i;
	
	SetActiveController(NULL);
	
	for(i=0; i<controllerCount; i++){
		pControllers.GetAt(i)->SetSky(NULL);
	}
	pControllers.RemoveAll();
	
	NotifyControllerStructureChanged();
}

void seSky::SetActiveController(seController *controller){
	if(controller == pActiveController){
		return;
	}
	
	if(pActiveController){
		pActiveController->SetActive(false);
		pActiveController->FreeReference();
	}
	
	pActiveController = controller;
	
	if(controller){
		controller->AddReference();
		controller->SetActive(true);
	}
	
	NotifyActiveControllerChanged();
}

int seSky::CountControllerUsage(seController *controller) const{
	const int layerCount = pLayers.GetCount();
	int usageCount = 0;
	int i, j, k;
	
	for(i=0; i<layerCount; i++){
		const seLayer &layer = *pLayers.GetAt(i);
		for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
			const seLinkList &links = layer.GetTarget((deSkyLayer::eTargets)j).GetLinks();
			const int linkCount = links.GetCount();
			for(k=0; k<linkCount; k++){
				if(links.GetAt(k)->GetController() == controller){
					usageCount++;
				}
			}
		}
	}
	
	return usageCount;
}



// Links
//////////

void seSky::AddLink(seLink *link){
	if(!link){
		DETHROW(deeInvalidParam);
	}
	
	pLinks.Add(link);
	link->SetSky(this);
	link->SetIndex(pLinks.GetCount() - 1);
	
	NotifyLinkStructureChanged();
	
	if(!pActiveLink){
		SetActiveLink(link);
	}
}

void seSky::RemoveLink(seLink *link){
	if(!link){
		DETHROW(deeInvalidParam);
	}
	
	if(link->GetActive()){
		int controllerCount = pLinks.GetCount();
		int controllerIndex = link->GetIndex();
		
		if(controllerIndex < controllerCount - 1){
			SetActiveLink(pLinks.GetAt(controllerIndex + 1));
			
		}else if(controllerIndex > 0){
			SetActiveLink(pLinks.GetAt(controllerIndex - 1));
			
		}else{
			SetActiveLink(NULL);
		}
	}
	
	link->SetSky(NULL);
	pLinks.Remove(link);
	
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->SetIndex(i);
	}
	
	NotifyLinkStructureChanged();
}

void seSky::RemoveAllLinks(){
	const int linkCount = pLinks.GetCount();
	int i;
	
	SetActiveLink(NULL);
	
	for(i=0; i<linkCount; i++){
		pLinks.GetAt(i)->SetSky(NULL);
	}
	pLinks.RemoveAll();
	
	NotifyLinkStructureChanged();
}

void seSky::SetActiveLink(seLink *link){
	if(link == pActiveLink){
		return;
	}
	
	if(pActiveLink){
		pActiveLink->SetActive(false);
		pActiveLink->FreeReference();
	}
	
	pActiveLink = link;
	
	if(link){
		link->AddReference();
		link->SetActive(true);
	}
	
	NotifyActiveLinkChanged();
}

int seSky::CountLinkUsage(seLink *link) const{
	const int layerCount = pLayers.GetCount();
	int usageCount = 0;
	int i, j;
	
	for(i=0; i<layerCount; i++){
		const seLayer &layer = *pLayers.GetAt(i);
		for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
			if(layer.GetTarget((deSkyLayer::eTargets)j).GetLinks().Has(link)){
				usageCount++;
			}
		}
	}
	
	return usageCount;
}



// Layers
///////////

void seSky::AddLayer(seLayer *layer){
	if(!layer){
		DETHROW(deeInvalidParam);
	}
	
	pLayers.Add(layer);
	layer->SetSky(this);
	
	NotifyLayerStructureChanged();
	
	if(!pActiveLayer){
		SetActiveLayer(layer);
	}
}

void seSky::InsertLayerAt(seLayer *layer, int index){
	if(!layer){
		DETHROW(deeInvalidParam);
	}
	
	pLayers.InsertAt(layer, index);
	layer->SetSky(this);
	
	NotifyLayerStructureChanged();
	
	if(!pActiveLayer){
		SetActiveLayer(layer);
	}
}

void seSky::MoveLayerTo(seLayer *layer, int index){
	pLayers.MoveTo(layer, index);
	NotifyLayerStructureChanged();
}

void seSky::RemoveLayer(seLayer *layer){
	if(!layer){
		DETHROW(deeInvalidParam);
	}
	
	if(layer->GetActive()){
		int layerCount = pLayers.GetCount();
		int layerIndex = pLayers.IndexOf(layer);
		
		if(layerIndex < layerCount - 1){
			SetActiveLayer(pLayers.GetAt(layerIndex + 1));
			
		}else if(layerIndex > 0){
			SetActiveLayer(pLayers.GetAt(layerIndex - 1));
			
		}else{
			SetActiveLayer(NULL);
		}
	}
	
	layer->SetSky(NULL);
	pLayers.Remove(layer);
	
	NotifyLayerStructureChanged();
}

void seSky::RemoveAllLayers(){
	const int layerCount = pLayers.GetCount();
	int i;
	
	SetActiveLayer(NULL);
	
	for(i=0; i<layerCount; i++){
		pLayers.GetAt(i)->SetSky(NULL);
	}
	pLayers.RemoveAll();
	
	NotifyLayerStructureChanged();
}

void seSky::SetActiveLayer(seLayer *layer){
	if(layer == pActiveLayer){
		return;
	}
	
	if(pActiveLayer){
		pActiveLayer->SetActive(false);
		pActiveLayer->FreeReference();
	}
	
	pActiveLayer = layer;
	
	if(layer){
		layer->AddReference();
		layer->SetActive(true);
	}
	
	NotifyActiveLayerChanged();
}



// Listeners
//////////////

void seSky::AddListener(seSkyListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void seSky::RemoveListener(seSkyListener *listener){
	pListeners.RemoveIfPresent(listener);
}



void seSky::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->StateChanged(this);
	}
}

void seSky::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->UndoChanged(this);
	}
}



void seSky::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->SkyChanged(this);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyEnvObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->EnvObjectChanged(this);
	}
}

void seSky::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ViewChanged(this);
	}
}

void seSky::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->CameraChanged(this);
	}
}



void seSky::NotifyControllerStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ControllerStructureChanged(this);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyControllerChanged(seController *controller){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ControllerChanged(this, controller);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyControllerNameChanged(seController* controller){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ControllerNameChanged(this, controller);
	}
}

void seSky::NotifyControllerValueChanged(seController *controller){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ControllerValueChanged(this, controller);
	}
}

void seSky::NotifyControllerSelectionChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ControllerSelectionChanged(this);
	}
}

void seSky::NotifyActiveControllerChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ActiveControllerChanged(this);
	}
}



void seSky::NotifyLinkStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LinkStructureChanged(this);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyLinkChanged(seLink *controller){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LinkChanged(this, controller);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyLinkNameChanged(seLink* link){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LinkNameChanged(this, link);
	}
	
	SetChanged(true);
}

void seSky::NotifyLinkSelectionChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LinkSelectionChanged(this);
	}
}

void seSky::NotifyActiveLinkChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ActiveLinkChanged(this);
	}
}



void seSky::NotifyLayerStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LayerStructureChanged(this);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyLayerChanged(seLayer *layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LayerChanged(this, layer);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyLayerNameChanged(seLayer* layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LayerNameChanged(this, layer);
	}
	
	SetChanged(true);
}

void seSky::NotifyLayerSelectionChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->LayerSelectionChanged(this);
	}
}

void seSky::NotifyActiveLayerChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ActiveLayerChanged(this);
	}
}



void seSky::NotifyBodyStructureChanged(seLayer *layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->BodyStructureChanged(this, layer);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyBodyChanged(seLayer *layer, seBody *body){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->BodyChanged(this, layer, body);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyBodySelectionChanged(seLayer *layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->BodySelectionChanged(this, layer);
	}
}

void seSky::NotifyActiveBodyChanged(seLayer *layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ActiveBodyChanged(this, layer);
	}
}



void seSky::NotifyTargetChanged(seLayer *layer, deSkyLayer::eTargets target){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->TargetChanged(this, layer, target);
	}
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void seSky::NotifyActiveTargetChanged(seLayer *layer){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for(i=0; i<listenerCount; i++){
		((seSkyListener*)pListeners.GetAt(i))->ActiveTargetChanged(this, layer);
	}
}



// Private Functions
//////////////////////

void seSky::pCleanUp(){
	pListeners.RemoveAll();
	
	if(pCamera){
		delete pCamera;
	}
	
	SetActiveLayer(NULL);
	RemoveAllLayers();
	
	SetActiveLink(NULL);
	RemoveAllLinks();
	
	SetActiveController(NULL);
	RemoveAllControllers();
	
	pEnvObject = nullptr;
	
	if(pEngWorld){
		pDDSHorizon.SetParentDebugDrawer(NULL);
		
		if(pDDHorizon){
			pEngWorld->RemoveDebugDrawer(pDDHorizon);
			pDDHorizon->FreeReference();
		}
		
		if(pEngSkyInstance){
			pEngWorld->RemoveSky(pEngSkyInstance);
			pEngSkyInstance->FreeReference();
		}
		
		if(pEngSky){
			pEngSky->FreeReference();
		}
		
		pEngWorld->FreeReference();
	}
}

void seSky::pBuildShapeHorizon(){
	deDebugDrawerShapeFace *face = NULL;
	int s, stepCount, subStepCount;
	decVector vertex[4], normal;
	float bandHeight = 0.005f;
	float markHeightSmall = 0.02f;
	float markHeightLarge = 0.04f;
	float markWidth = DEG2RAD * 0.2f;
	float markWidthPrime = DEG2RAD * 0.6f;
	float markHeightPrime = 0.12f;
	float angle1, angle2;
	float factor;
	float radius = 10.0f;
	
	// build a horizon ring that helps the user to examine the sky. the ring is
	// composed of a band of small height all around as well as degree marks.
	// the degree marks compose of heigher marks at 45 degree steps as well as
	// smaller marks at 5 degree steps.
	
	// just to make sure clear the shape
	pDDSHorizon.RemoveAllFaces();
	
	try{
		stepCount = 72; // 5 degree steps
		subStepCount = 9; // stepCount div-by 8 (45 degree large marks)
		factor = PI * 2.0f / (float)stepCount;
		
		// add the all around band
		vertex[0].y = radius * bandHeight;
		vertex[1].y = radius * bandHeight;
		vertex[2].y = -vertex[0].y;
		vertex[3].y = -vertex[1].y;
		
		angle1 = 0.0f;
		
		for(s=0; s<stepCount; s++){
			angle2 = factor * (float)((s + 1) % stepCount);
			
			vertex[0].x = radius * sinf(angle1);
			vertex[1].x = radius * sinf(angle2);
			vertex[2].x = vertex[1].x;
			vertex[3].x = vertex[0].x;
			
			vertex[0].z = radius * cosf(angle1);
			vertex[1].z = radius * cosf(angle2);
			vertex[2].z = vertex[1].z;
			vertex[3].z = vertex[0].z;
			
			face = new deDebugDrawerShapeFace;
			if(!face) DETHROW(deeOutOfMemory);
			
			face->AddVertex(vertex[0]);
			face->AddVertex(vertex[1]);
			face->AddVertex(vertex[2]);
			face->AddVertex(vertex[3]);
			normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[1]);
			normal.Normalize();
			face->SetNormal(normal);
			
			pDDSHorizon.AddFace(face);
			face = NULL;
			
			angle1 = angle2;
		}
		
		// add marks
		vertex[2].y = radius * bandHeight;
		vertex[3].y = vertex[2].y;
		
		for(s=0; s<stepCount; s++){
			angle1 = factor * (float)(s % stepCount);
			
			if(s == 0){
				angle2 = angle1 + markWidthPrime;
				angle1 -= markWidthPrime;
				
				vertex[0].y = radius * markHeightPrime;
				
			}else{
				angle2 = angle1 + markWidth;
				angle1 -= markWidth;
				
				if(s % subStepCount == 0){
					vertex[0].y = radius * markHeightLarge;
					
				}else{
					vertex[0].y = radius * markHeightSmall;
				}
			}
			vertex[1].y = vertex[0].y;
			
			vertex[0].x = radius * sinf(angle1);
			vertex[1].x = radius * sinf(angle2);
			vertex[2].x = vertex[1].x;
			vertex[3].x = vertex[0].x;
			
			vertex[0].z = radius * cosf(angle1);
			vertex[1].z = radius * cosf(angle2);
			vertex[2].z = vertex[1].z;
			vertex[3].z = vertex[0].z;
			
			face = new deDebugDrawerShapeFace;
			if(!face) DETHROW(deeOutOfMemory);
			
			face->AddVertex(vertex[0]);
			face->AddVertex(vertex[1]);
			face->AddVertex(vertex[2]);
			face->AddVertex(vertex[3]);
			normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[1]);
			normal.Normalize();
			face->SetNormal(normal);
			
			pDDSHorizon.AddFace(face);
			face = NULL;
		}
		
		
	}catch(const deException &){
		if(face) delete face;
		throw;
	}
}
