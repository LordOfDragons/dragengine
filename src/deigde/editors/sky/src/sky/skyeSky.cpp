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

#include "skyeSky.h"
#include "skyeSkyListener.h"
#include "body/skyeBody.h"
#include "controller/skyeController.h"
#include "controller/skyeControllerTarget.h"
#include "layer/skyeLayer.h"
#include "link/skyeLink.h"

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



// Class skyeSky
////////////////

// Constructor, destructor
////////////////////////////

skyeSky::skyeSky(igdeEnvironment *environment) :
igdeEditableEntity(environment),
pNeedsRebuildSky(true),

pCamera(nullptr)
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
		
		pEnvObject = igdeWObject::Ref::New(*environment);
		pEnvObject->SetWorld(pEngWorld);
		pEnvObject->SetPosition(decDVector(0.0, -1.8, 0.0));
		pEnvObject->SetGDClassName("IGDETestTerrain");
		
		pDDHorizon = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDHorizon->SetXRay(true);
		pEngWorld->AddDebugDrawer(pDDHorizon);
		
		pDDSHorizon = igdeWDebugDrawerShape::Ref::New();
		pDDSHorizon->SetParentDebugDrawer(pDDHorizon);
		pDDSHorizon->SetVisible(true);
		pDDSHorizon->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		pDDSHorizon->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.5f));
		pBuildShapeHorizon();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

skyeSky::~skyeSky(){
	pCleanUp();
}



// Management
///////////////

bool skyeSky::GetDrawSkyCompass() const{
	return pDDHorizon->GetVisible();
}

void skyeSky::SetDrawSkyCompass(bool drawSkyCompass){
	pDDHorizon->SetVisible(drawSkyCompass);
}



void skyeSky::Dispose(){
	SetActiveLayer(nullptr);
	SetActiveLink(nullptr);
	SetActiveController(nullptr);
	
	RemoveAllLayers();
	RemoveAllLinks();
	RemoveAllControllers();
	
	GetUndoSystem()->RemoveAll();
}

void skyeSky::Reset(){
	SetActiveLayer(nullptr);
	SetActiveLink(nullptr);
	SetActiveController(nullptr);
	
	GetUndoSystem()->RemoveAll();
}

void skyeSky::Update(float elapsed){
	if(pNeedsRebuildSky){
		RebuildEngineSky();
	}
	
	pEnvObject->Update(elapsed);
	pEngWorld->Update(elapsed);
}

void skyeSky::RebuildEngineSky(){
	// drop old sky
	pEngSkyInstance->SetSky(nullptr);
	
	if(pEngSky){
		pEngSky = nullptr;
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
			const skyeController &sourceController = *pControllers.GetAt(i);
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
			const skyeLink &sourceLink = *pLinks.GetAt(i);
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
			const skyeLayer &sourceLayer = *pLayers.GetAt(i);
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
			
			const skyeBody::List &bodies = sourceLayer.GetBodies();
			const int bodyCount = bodies.GetCount();
			destLayer.SetBodyCount(bodyCount);
			for(j=0; j<bodyCount; j++){
				const skyeBody &sourceBody = *bodies.GetAt(j);
				deSkyLayerBody &destBody = destLayer.GetBodyAt(j);
				
				destBody.SetOrientation(decQuaternion::CreateFromEuler(
					sourceBody.GetOrientation() * DEG2RAD));
				destBody.SetSize(sourceBody.GetSize() * DEG2RAD);
				destBody.SetColor(sourceBody.GetColor());
				destBody.SetSkin(sourceBody.GetEngineSkin());
			}
			
			for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
				const deSkyLayer::eTargets target = (deSkyLayer::eTargets)j;
				const skyeControllerTarget &sourceTarget = sourceLayer.GetTarget(target);
				deSkyControllerTarget &destTarget = destLayer.GetTarget(target);
				
				const skyeLink::List &ctLinks = sourceTarget.GetLinks();
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
			pEngSky = nullptr;
		}
		throw;
	}
	
	// assign sky
	pEngSkyInstance->SetSky(pEngSky);
	pNeedsRebuildSky = false;
};

void skyeSky::UpdateRelativeResources(){
	const int layerCount = pLayers.GetCount();
	int i;
	for(i=0; i<layerCount; i++){
		pLayers.GetAt(i)->UpdateRelativeResources();
	}
}



void skyeSky::SetBgColor(const decColor &color){
	if(color.IsEqualTo(pBgColor)){
		return;
	}
	
	pBgColor = color;
	NotifySkyChanged();
}



// Controllers
////////////////

void skyeSky::AddController(skyeController *controller){
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

void skyeSky::InsertControllerAt(skyeController *controller, int index){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	
	pControllers.Insert(controller, index);
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

void skyeSky::MoveControllerTo(skyeController *controller, int index){
	pControllers.Move(controller, index);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetIndex(i);
	}
	
	NotifyControllerStructureChanged();
}

void skyeSky::RemoveController(skyeController *controller){
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
			SetActiveController(nullptr);
		}
	}
	
	controller->SetSky(nullptr);
	pControllers.Remove(controller);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetIndex(i);
	}
	
	NotifyControllerStructureChanged();
}

void skyeSky::RemoveAllControllers(){
	const int controllerCount = pControllers.GetCount();
	int i;
	
	SetActiveController(nullptr);
	
	for(i=0; i<controllerCount; i++){
		pControllers.GetAt(i)->SetSky(nullptr);
	}
	pControllers.RemoveAll();
	
	NotifyControllerStructureChanged();
}

void skyeSky::SetActiveController(skyeController *controller){
	if(controller == pActiveController){
		return;
	}
	
	if(pActiveController){
		pActiveController->SetActive(false);
	}
	
	pActiveController = controller;
	
	if(controller){
		controller->SetActive(true);
	}
	
	NotifyActiveControllerChanged();
}

int skyeSky::CountControllerUsage(skyeController *controller) const{
	const int layerCount = pLayers.GetCount();
	int usageCount = 0;
	int i, j, k;
	
	for(i=0; i<layerCount; i++){
		const skyeLayer &layer = *pLayers.GetAt(i);
		for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
			const skyeLink::List &links = layer.GetTarget((deSkyLayer::eTargets)j).GetLinks();
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

void skyeSky::AddLink(skyeLink *link){
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

void skyeSky::RemoveLink(skyeLink *link){
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
			SetActiveLink(nullptr);
		}
	}
	
	link->SetSky(nullptr);
	pLinks.Remove(link);
	
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->SetIndex(i);
	}
	
	NotifyLinkStructureChanged();
}

void skyeSky::RemoveAllLinks(){
	const int linkCount = pLinks.GetCount();
	int i;
	
	SetActiveLink(nullptr);
	
	for(i=0; i<linkCount; i++){
		pLinks.GetAt(i)->SetSky(nullptr);
	}
	pLinks.RemoveAll();
	
	NotifyLinkStructureChanged();
}

void skyeSky::SetActiveLink(skyeLink *link){
	if(link == pActiveLink){
		return;
	}
	
	if(pActiveLink){
		pActiveLink->SetActive(false);
	}
	
	pActiveLink = link;
	
	if(link){
		link->SetActive(true);
	}
	
	NotifyActiveLinkChanged();
}

int skyeSky::CountLinkUsage(skyeLink *link) const{
	const int layerCount = pLayers.GetCount();
	int usageCount = 0;
	int i, j;
	
	for(i=0; i<layerCount; i++){
		const skyeLayer &layer = *pLayers.GetAt(i);
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

void skyeSky::AddLayer(skyeLayer *layer){
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

void skyeSky::InsertLayerAt(skyeLayer *layer, int index){
	if(!layer){
		DETHROW(deeInvalidParam);
	}
	
	pLayers.Insert(layer, index);
	layer->SetSky(this);
	
	NotifyLayerStructureChanged();
	
	if(!pActiveLayer){
		SetActiveLayer(layer);
	}
}

void skyeSky::MoveLayerTo(skyeLayer *layer, int index){
	pLayers.Move(layer, index);
	NotifyLayerStructureChanged();
}

void skyeSky::RemoveLayer(skyeLayer *layer){
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
			SetActiveLayer(nullptr);
		}
	}
	
	layer->SetSky(nullptr);
	pLayers.Remove(layer);
	
	NotifyLayerStructureChanged();
}

void skyeSky::RemoveAllLayers(){
	const int layerCount = pLayers.GetCount();
	int i;
	
	SetActiveLayer(nullptr);
	
	for(i=0; i<layerCount; i++){
		pLayers.GetAt(i)->SetSky(nullptr);
	}
	pLayers.RemoveAll();
	
	NotifyLayerStructureChanged();
}

void skyeSky::SetActiveLayer(skyeLayer *layer){
	if(layer == pActiveLayer){
		return;
	}
	
	if(pActiveLayer){
		pActiveLayer->SetActive(false);
	}
	
	pActiveLayer = layer;
	
	if(layer){
		layer->SetActive(true);
	}
	
	NotifyActiveLayerChanged();
}



// Listeners
//////////////

void skyeSky::AddListener(skyeSkyListener *listener){
	DEASSERT_NOTNULL(listener)
	
	pListeners.Add(listener);
}

void skyeSky::RemoveListener(skyeSkyListener *listener){
	pListeners.Remove(listener);
}


void skyeSky::NotifyStateChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.StateChanged(this);
	});
}

void skyeSky::NotifyUndoChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.UndoChanged(this);
	});
}



void skyeSky::NotifySkyChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.SkyChanged(this);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyEnvObjectChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.EnvObjectChanged(this);
	});
}

void skyeSky::NotifyViewChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ViewChanged(this);
	});
}

void skyeSky::NotifyCameraChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.CameraChanged(this);
	});
}



void skyeSky::NotifyControllerStructureChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ControllerStructureChanged(this);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyControllerChanged(skyeController *controller){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ControllerChanged(this, controller);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyControllerNameChanged(skyeController *controller){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ControllerNameChanged(this, controller);
	});
}

void skyeSky::NotifyControllerValueChanged(skyeController *controller){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ControllerValueChanged(this, controller);
	});
}

void skyeSky::NotifyControllerSelectionChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ControllerSelectionChanged(this);
	});
}

void skyeSky::NotifyActiveControllerChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ActiveControllerChanged(this);
	});
}



void skyeSky::NotifyLinkStructureChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LinkStructureChanged(this);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyLinkChanged(skyeLink *controller){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LinkChanged(this, controller);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyLinkNameChanged(skyeLink *link){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LinkNameChanged(this, link);
	});
	
	SetChanged(true);
}

void skyeSky::NotifyLinkSelectionChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LinkSelectionChanged(this);
	});
}

void skyeSky::NotifyActiveLinkChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ActiveLinkChanged(this);
	});
}



void skyeSky::NotifyLayerStructureChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LayerStructureChanged(this);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyLayerChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LayerChanged(this, layer);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyLayerNameChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LayerNameChanged(this, layer);
	});
	
	SetChanged(true);
}

void skyeSky::NotifyLayerSelectionChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.LayerSelectionChanged(this);
	});
}

void skyeSky::NotifyActiveLayerChanged(){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ActiveLayerChanged(this);
	});
}



void skyeSky::NotifyBodyStructureChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.BodyStructureChanged(this, layer);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyBodyChanged(skyeLayer *layer, skyeBody *body){
	pListeners.Visit([&](skyeSkyListener &l){
		l.BodyChanged(this, layer, body);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyBodySelectionChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.BodySelectionChanged(this, layer);
	});
}

void skyeSky::NotifyActiveBodyChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ActiveBodyChanged(this, layer);
	});
}



void skyeSky::NotifyTargetChanged(skyeLayer *layer, deSkyLayer::eTargets target){
	pListeners.Visit([&](skyeSkyListener &l){
		l.TargetChanged(this, layer, target);
	});
	
	SetChanged(true);
	pNeedsRebuildSky = true;
}

void skyeSky::NotifyActiveTargetChanged(skyeLayer *layer){
	pListeners.Visit([&](skyeSkyListener &l){
		l.ActiveTargetChanged(this, layer);
	});
}



// Private Functions
//////////////////////

void skyeSky::pCleanUp(){
	pListeners.RemoveAll();
	
	if(pCamera){
		delete pCamera;
	}
	
	SetActiveLayer(nullptr);
	RemoveAllLayers();
	
	SetActiveLink(nullptr);
	RemoveAllLinks();
	
	SetActiveController(nullptr);
	RemoveAllControllers();
	
	pEnvObject = nullptr;
	
	if(pEngWorld){
		pDDSHorizon->SetParentDebugDrawer(nullptr);
		
		if(pDDHorizon){
			pEngWorld->RemoveDebugDrawer(pDDHorizon);
		}
		
		if(pEngSkyInstance){
			pEngWorld->RemoveSky(pEngSkyInstance);
		}
	}
}

void skyeSky::pBuildShapeHorizon(){
	deDebugDrawerShapeFace *face = nullptr;
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
	pDDSHorizon->RemoveAllFaces();
	
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
			face->AddVertex(vertex[0]);
			face->AddVertex(vertex[1]);
			face->AddVertex(vertex[2]);
			face->AddVertex(vertex[3]);
			normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[1]);
			normal.Normalize();
			face->SetNormal(normal);
			
			pDDSHorizon->AddFace(face);
			face = nullptr;
			
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
			face->AddVertex(vertex[0]);
			face->AddVertex(vertex[1]);
			face->AddVertex(vertex[2]);
			face->AddVertex(vertex[3]);
			normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[1]);
			normal.Normalize();
			face->SetNormal(normal);
			
			pDDSHorizon->AddFace(face);
			face = nullptr;
		}
		
		
	}catch(const deException &){
		if(face) delete face;
		throw;
	}
}
