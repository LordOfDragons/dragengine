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

#include "gdeViewActiveObject.h"
#include "gdeVAOBillboard.h"
#include "gdeVAOComponent.h"
#include "gdeVAOCamera.h"
#include "gdeVAOLight.h"
#include "gdeVAOEnvMapProbe.h"
#include "gdeVAONavSpace.h"
#include "gdeVAONavBlocker.h"
#include "gdeVAOParticleEmitter.h"
#include "gdeVAOForceField.h"
#include "gdeVAOSnapPoint.h"
#include "gdeVAOSpeaker.h"
#include "gdeVAOWorld.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../gamedef/objectClass/camera/gdeOCCamera.h"
#include "../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../../gamedef/objectClass/light/gdeOCLight.h"
#include "../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
#include "../../gamedef/objectClass/forceField/gdeOCForceField.h"
#include "../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../gamedef/skin/gdeSkin.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/loadsave/igdeLoadAnimator.h>
#include <deigde/loadsave/igdeLoadParticleEmitter.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>


// Events
///////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener{
	gdeViewActiveObject &pView;
	
public:
	typedef deTObjectReference<cCameraInteraction> Ref;
	
	cCameraInteraction(gdeViewActiveObject &view) : pView(view){
		SetEnabledAll(true);
	}
	
public:
	virtual void OnCameraChanged(){
		if(pView.GetGameDefinition()){
			pView.GetGameDefinition()->NotifyCameraChanged();
		}
	}
};

}



// Class gdeViewActiveObject
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeViewActiveObject::gdeViewActiveObject(gdeWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),

pWindowMain(windowMain),
pListener(nullptr),

pShowEnvMapProbes(false),
pShowNavBlockers(false)
{
	pListener = gdeViewActiveObjectListener::Ref::New(*this);
	
	const deEngine &engine = *GetEngineController().GetEngine();
	pDebugDrawer = engine.GetDebugDrawerManager()->CreateDebugDrawer();
	pDebugDrawer->SetXRay(true);
	
	pCameraInteraction = cCameraInteraction::Ref::New(*this);
	
	AddListener(pCameraInteraction);
}

gdeViewActiveObject::~gdeViewActiveObject(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeViewActiveObject::ResetView(){
}

void gdeViewActiveObject::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	pCameraInteraction->SetCamera(nullptr);
	SetRenderWorld(nullptr);
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
		
		ClearResources();
		
		pPreviewModelBox = nullptr;
		
		if(pDebugDrawer){
			pGameDefinition->GetWorld()->RemoveDebugDrawer(pDebugDrawer);
		}
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		SetRenderWorld(gameDefinition->GetCamera()->GetEngineCamera());
		pCameraInteraction->SetCamera(gameDefinition->GetCamera());
		
		deVirtualFileSystem * const vfs = gameDefinition->GetEnvironment()->GetFileSystemIGDE();
		deEngine &engine = *gameDefinition->GetEngine();
		deModelManager &modelManager = *engine.GetModelManager();
		
		pPreviewModelBox = modelManager.LoadModel(vfs, "/data/data/models/materialTest/box.demodel", "/");
		
		gameDefinition->GetWorld()->AddDebugDrawer(pDebugDrawer);
		
		gameDefinition->AddListener(pListener);
	}
	
	pUpdateCameraFovRatio();
}



void gdeViewActiveObject::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pGameDefinition){
		pGameDefinition->Update(elapsed);
	}
	
	pOCComponents.Visit([&](gdeVAOComponent &vao){
		vao.Update(elapsed);
	});
	pOCLights.Visit([&](gdeVAOLight &light){
		light.Update(elapsed);
	});
}

void gdeViewActiveObject::OnResize(){
	igdeViewRenderWindow::OnResize();
	pUpdateCameraFovRatio();
}

void gdeViewActiveObject::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pGameDefinition){
		SetRenderWorld(pGameDefinition->GetCamera()->GetEngineCamera());
	}
}



void gdeViewActiveObject::ResetCameraPosition(){
	igdeCamera &camera = *pGameDefinition->GetCamera();
	
	switch(pGameDefinition->GetSelectedObjectType()){
	case gdeGameDefinition::eotNoSelection:
	case gdeGameDefinition::eotCategoryObjectClass:
	case gdeGameDefinition::eotCategorySkin:
	case gdeGameDefinition::eotCategorySky:
	case gdeGameDefinition::eotCategoryParticleEmitter:
		// nothing
		break;
		
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCForceField:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
	case gdeGameDefinition::eotOCWorld:
		pCenterOnObjectClass(camera);
		break;
		
	case gdeGameDefinition::eotSkin:
		pCenterOnSkin(camera);
		break;
		
	case gdeGameDefinition::eotSky:
		pCenterOnSky(camera);
		break;
		
	case gdeGameDefinition::eotParticleEmitter:
		pCenterOnParticleEmitter(camera);
		break;
	}
}

void gdeViewActiveObject::ClearResources(){
	deWorld &world = *pGameDefinition->GetWorld();
	
	pOCBillboards.RemoveAll();
	pOCCameras.RemoveAll();
	pOCEnvMapProbes.RemoveAll();
	pOCLights.RemoveAll();
	pOCParticleEmitters.RemoveAll();
	pOCForceFields.RemoveAll();
	pOCNavBlockers.RemoveAll();
	pOCNavSpaces.RemoveAll();
	pOCSnapPoints.RemoveAll();
	pOCSpeakers.RemoveAll();
	pOCWorlds.RemoveAll();
	pOCComponents.RemoveAll();
	
	if(pPreviewParticleEmitter){
		if(pPreviewParticleEmitter->GetParentWorld()){
			world.RemoveParticleEmitter(pPreviewParticleEmitter);
		}
		pPreviewParticleEmitter.Clear();
	}
	
	if(pPreviewSky){
		if(pPreviewSky->GetParentWorld()){
			world.RemoveSky(pPreviewSky);
		}
		pPreviewSky.Clear();
	}
	
	pPreviewSkin.Clear();
	
	if(pPreviewComponent){
		if(pPreviewComponent->GetParentWorld()){
			world.RemoveComponent(pPreviewComponent);
		}
		pPreviewComponent.Clear();
	}
	
	pObjectClass.Clear();
}

void gdeViewActiveObject::InitSelectedObject(){
	try{
		switch(pGameDefinition->GetSelectedObjectType()){
		case gdeGameDefinition::eotNoSelection:
		case gdeGameDefinition::eotCategoryObjectClass:
		case gdeGameDefinition::eotCategorySkin:
		case gdeGameDefinition::eotCategorySky:
		case gdeGameDefinition::eotCategoryParticleEmitter:
			// nothing
			break;
			
		case gdeGameDefinition::eotObjectClass:
		case gdeGameDefinition::eotOCBillboard:
		case gdeGameDefinition::eotOCCamera:
		case gdeGameDefinition::eotOCComponent:
		case gdeGameDefinition::eotOCEnvMapProbe:
		case gdeGameDefinition::eotOCLight:
		case gdeGameDefinition::eotOCNavigationBlocker:
		case gdeGameDefinition::eotOCNavigationSpace:
		case gdeGameDefinition::eotOCParticleEmitter:
		case gdeGameDefinition::eotOCForceField:
		case gdeGameDefinition::eotOCSnapPoint:
		case gdeGameDefinition::eotOCSpeaker:
		case gdeGameDefinition::eotOCWorld:
			pInitObjectClass();
			break;
			
		case gdeGameDefinition::eotSkin:
			pInitSkin();
			break;
			
		case gdeGameDefinition::eotSky:
			pInitSky();
			break;
			
		case gdeGameDefinition::eotParticleEmitter:
			pInitParticleEmitter();
			break;
		}
		
	}catch(const deException &e){
		ClearResources();
		e.PrintError();
		// TODO mark the error in the panels using warnings for example
	}
}

void gdeViewActiveObject::SelectedSubObjectChanged(){
	pOCBillboards.Visit([](gdeVAOBillboard &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCCameras.Visit([](gdeVAOCamera &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCComponents.Visit([](gdeVAOComponent &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCEnvMapProbes.Visit([](gdeVAOEnvMapProbe &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCLights.Visit([](gdeVAOLight &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCNavBlockers.Visit([](gdeVAONavBlocker &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCNavSpaces.Visit([](gdeVAONavSpace &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCParticleEmitters.Visit([](gdeVAOParticleEmitter &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCForceFields.Visit([](gdeVAOForceField &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCSnapPoints.Visit([](gdeVAOSnapPoint &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCSpeakers.Visit([](gdeVAOSpeaker &vao){
		vao.SelectedObjectChanged();
	});
	
	pOCWorlds.Visit([](gdeVAOWorld &vao){
		vao.SelectedObjectChanged();
	});
}



deCollider *gdeViewActiveObject::GetAttachComponentCollider() const{
	if(pOCComponents.GetCount() == 0){
		return nullptr;
	}
	
	return pOCComponents.First()->GetCollider();
}

void gdeViewActiveObject::RebuildOCComponents(){
	const decTObjectOrderedSet<gdeVAOComponent> oldComponents(pOCComponents);
	pOCComponents.RemoveAll();
	if(pObjectClass){
		pRebuildOCComponents(*pObjectClass, "");
	}
	pOCReattachAllObjects();
}

void gdeViewActiveObject::RebuildOCComponent(gdeOCComponent *occomponent){
	pOCComponents.Visit([&](gdeVAOComponent &vao){
		if(vao.GetOCComponent() == occomponent){
			vao.RebuildResources();
			pOCReattachAllObjects();
		}
	});
}

void gdeViewActiveObject::UpdateOCComponentTexture(gdeOCComponent *occomponent,
gdeOCComponentTexture *texture){
	pOCComponents.Visit([&](gdeVAOComponent &vao){
		if(!occomponent || vao.GetOCComponent() == occomponent){
			vao.UpdateTexture(texture);
		}
	});
}

void gdeViewActiveObject::RebuildOCBillboards(){
	const decTObjectOrderedSet<gdeVAOBillboard> oldBillboards(pOCBillboards);
	pOCBillboards.RemoveAll();
	if(pObjectClass){
		pInitOCBillboards(pObjectClass, "");
	}
	pOCReattachAllObjects();
}

void gdeViewActiveObject::RebuildOCBillboard(gdeOCBillboard *ocbillboard){
	pOCBillboards.Visit([&](gdeVAOBillboard &vao){
		if(vao.GetOCBillboard() == ocbillboard){
			vao.RebuildResources();
			pOCReattachAllObjects();
		}
	});
}

void gdeViewActiveObject::RebuildOCCameras(){
	const decTObjectOrderedSet<gdeVAOCamera> oldCameras(pOCCameras);
	pOCCameras.RemoveAll();
	if(pObjectClass){
		pInitOCCameras(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCCamera(gdeOCCamera *occamera){
	pOCCameras.Visit([&](gdeVAOCamera &vao){
		if(vao.GetOCCamera() == occamera){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCLights(){
	const decTObjectOrderedSet<gdeVAOLight> oldLights(pOCLights);
	pOCLights.RemoveAll();
	if(pObjectClass){
		pInitOCLights(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCLight(gdeOCLight *oclight){
	pOCLights.Visit([&](gdeVAOLight &vao){
		if(vao.GetOCLight() == oclight){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCEnvMapProbes(){
	const decTObjectOrderedSet<gdeVAOEnvMapProbe> oldEnvMapProbes(pOCEnvMapProbes);
	pOCEnvMapProbes.RemoveAll();
	if(pObjectClass){
		pInitOCEnvMapProbes(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCEnvMapProbe(gdeOCEnvMapProbe *ocenvMapProbe){
	pOCEnvMapProbes.Visit([&](gdeVAOEnvMapProbe &vao){
		if(vao.GetOCEnvMapProbe() == ocenvMapProbe){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCNavSpaces(){
	const decTObjectOrderedSet<gdeVAONavSpace> oldNavSpaces(pOCNavSpaces);
	pOCNavSpaces.RemoveAll();
	if(pObjectClass){
		pInitOCNavigationSpaces(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCNavSpace(gdeOCNavigationSpace *ocnavspace){
	pOCNavSpaces.Visit([&](gdeVAONavSpace &vao){
		if(vao.GetOCNavSpace() == ocnavspace){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCNavBlockers(){
	const decTObjectOrderedSet<gdeVAONavBlocker> oldNavBlockers(pOCNavBlockers);
	pOCNavBlockers.RemoveAll();
	if(pObjectClass){
		pInitOCNavigationBlockers(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCNavBlocker(gdeOCNavigationBlocker *ocnavblocker){
	pOCNavBlockers.Visit([&](gdeVAONavBlocker &vao){
		if(vao.GetOCNavBlocker() == ocnavblocker){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCParticleEmitters(){
	const decTObjectOrderedSet<gdeVAOParticleEmitter> oldEmitters(pOCParticleEmitters);
	pOCParticleEmitters.RemoveAll();
	if(pObjectClass){
		pInitOCParticleEmitters(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCParticleEmitter(gdeOCParticleEmitter *ocemitter){
	pOCParticleEmitters.Visit([&](gdeVAOParticleEmitter &vao){
		if(vao.GetOCParticleEmitter() == ocemitter){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCForceFields(){
	const decTObjectOrderedSet<gdeVAOForceField> oldForceFields(pOCForceFields);
	pOCForceFields.RemoveAll();
	if(pObjectClass){
		pInitOCForceFields(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCForceField(gdeOCForceField *ocfield){
	pOCForceFields.Visit([&](gdeVAOForceField &vao){
		if(vao.GetOCForceField() == ocfield){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCSnapPoints(){
	const decTObjectOrderedSet<gdeVAOSnapPoint> oldSnapPoints(pOCSnapPoints);
	pOCSnapPoints.RemoveAll();
	if(pObjectClass){
		pInitOCSnapPoints(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCSnapPoint(gdeOCSnapPoint *ocsnapPoint){
	pOCSnapPoints.Visit([&](gdeVAOSnapPoint &vao){
		if(vao.GetOCSnapPoint() == ocsnapPoint){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCSpeakers(){
	const decTObjectOrderedSet<gdeVAOSpeaker> oldSpeakers(pOCSpeakers);
	pOCSpeakers.RemoveAll();
	if(pObjectClass){
		pInitOCSpeakers(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCSpeaker(gdeOCSpeaker *ocspeaker){
	pOCSpeakers.Visit([&](gdeVAOSpeaker &vao){
		if(vao.GetOCSpeaker() == ocspeaker){
			vao.RebuildResources();
		}
	});
}

void gdeViewActiveObject::RebuildOCWorlds(){
	const decTObjectOrderedSet<gdeVAOWorld> oldWorlds(pOCWorlds);
	pOCWorlds.RemoveAll();
	if(pObjectClass){
		pInitOCWorlds(pObjectClass, "");
	}
}

void gdeViewActiveObject::RebuildOCWorld(gdeOCWorld *ocworld){
	pOCWorlds.Visit([&](gdeVAOWorld &vao){
		if(vao.GetOCWorld() == ocworld){
			vao.RebuildResources();
		}
	});
}


void gdeViewActiveObject::SetShowEnvMapProbes(bool show){
	if(show == pShowEnvMapProbes){
		return;
	}
	
	pShowEnvMapProbes = show;
	
	pOCEnvMapProbes.Visit([&](gdeVAOEnvMapProbe &vao){
		vao.UpdateDDVisibility();
	});
}

void gdeViewActiveObject::SetShowNavBlockers(bool show){
	if(show == pShowNavBlockers){
		return;
	}
	
	pShowNavBlockers = show;
	
	pOCNavSpaces.Visit([&](gdeVAONavSpace &vao){
		vao.UpdateDDVisibility();
	});
	pOCNavBlockers.Visit([&](gdeVAONavBlocker &vao){
		vao.UpdateDDVisibility();
	});
}



// Private Functions
//////////////////////

void gdeViewActiveObject::pCleanUp(){
	SetGameDefinition(nullptr);
}



void gdeViewActiveObject::pUpdateCameraFovRatio(){
	if(!pGameDefinition){
		return;
	}
	
	const decPoint size(GetRenderAreaSize());
	pGameDefinition->SetViewRatio((float)decMath::max(size.x, 1)
		/ (float)decMath::max(size.y, 1));
}



void gdeViewActiveObject::pInitObjectClass(){
	pObjectClass = pGameDefinition->GetActiveObjectClass();
	if(!pObjectClass){
		return;
	}
	pInitObjectClassOCs(*pObjectClass, "", igdeGDClass::FilterSubObjectsAll);
	pAddComponentShadowIgnore();
}

void gdeViewActiveObject::pInitSkin(){
	const gdeSkin * const skin = pGameDefinition->GetActiveSkin();
	if(!skin){
		return;
	}
	
	deVirtualFileSystem * const vfs = pGameDefinition->GetPreviewVFS();
	deEngine &engine = *pGameDefinition->GetEngine();
	pPreviewSkin = engine.GetSkinManager()->LoadSkin(vfs, skin->GetPath(), "/");
	
	pPreviewComponent = engine.GetComponentManager()->CreateComponent(pPreviewModelBox, pPreviewSkin);
	pPreviewComponent->SetEnableGI(false);
	pGameDefinition->GetWorld()->AddComponent(pPreviewComponent);
	
	if(pPreviewComponent->GetTextureCount() == 0){
		// assign skin also to texture directly. skins are supposed to have one texture but
		// the name is not defined. by using a generic model and allowing the user to
		// change the preview model it can be cumbersome to match the texture. by assigning
		// the skin to the texture this problem is solved
		pPreviewComponent->GetTextureAt(0).SetSkin(pPreviewSkin);
		pPreviewComponent->GetTextureAt(0).SetTexture(0);
		pPreviewComponent->NotifyTextureChanged(0);
	}
}

void gdeViewActiveObject::pInitSky(){
	// TODO
}

void gdeViewActiveObject::pInitParticleEmitter(){
	const gdeParticleEmitter * const particleEmitter = pGameDefinition->GetActiveParticleEmitter();
	if(!particleEmitter){
		return;
	}
	
	deVirtualFileSystem * const vfs = pGameDefinition->GetPreviewVFS();
	deEngine &engine = *pGameDefinition->GetEngine();
	deParticleEmitter::Ref engEmitter;
	decBaseFileReader::Ref reader;
	
	igdeLoadParticleEmitter loader(*pGameDefinition->GetEnvironment(),
		pGameDefinition->GetEnvironment()->GetLogger(), "gdeGDEditActiveObject");
	
	decPath path;
	path.SetFromUnix(particleEmitter->GetPath());
	
	engEmitter = engine.GetParticleEmitterManager()->CreateParticleEmitter();
	reader = vfs->OpenFileForReading(path);
	loader.Load(particleEmitter->GetPath(), engEmitter, reader);
	
	pPreviewParticleEmitter = engine.GetParticleEmitterInstanceManager()->CreateInstance();
	pPreviewParticleEmitter->SetEmitter(engEmitter);
	pPreviewParticleEmitter->SetEnableCasting(true);
	
	pGameDefinition->GetWorld()->AddParticleEmitter(pPreviewParticleEmitter);
}



void gdeViewActiveObject::pInitObjectClassOCs(const gdeObjectClass &objectClass,
const decString &propertyPrefix, int filter){
	if((filter & igdeGDClass::efsoComponents) == igdeGDClass::efsoComponents){
		pInitOCComponents(objectClass, propertyPrefix);
	}
	
	if((filter & igdeGDClass::efsoBillboards) == igdeGDClass::efsoBillboards){
		pInitOCBillboards(objectClass, propertyPrefix);
	}
	pInitOCCameras(objectClass, propertyPrefix);
	if((filter & igdeGDClass::efsoEnvMapProbes) == igdeGDClass::efsoEnvMapProbes){
		pInitOCEnvMapProbes(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoLights) == igdeGDClass::efsoLights){
		pInitOCLights(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoNavigationSpaces) == igdeGDClass::efsoNavigationSpaces){
		pInitOCNavigationSpaces(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoNavigationBlockers) == igdeGDClass::efsoNavigationBlockers){
		pInitOCNavigationBlockers(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoParticleEmitters) == igdeGDClass::efsoParticleEmitters){
		pInitOCParticleEmitters(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoForceFields) == igdeGDClass::efsoForceFields){
		pInitOCForceFields(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoSnapPoints) == igdeGDClass::efsoSnapPoints){
		pInitOCSnapPoints(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoSpeakers) == igdeGDClass::efsoSpeakers){
		pInitOCSpeakers(objectClass, propertyPrefix);
	}
	if((filter & igdeGDClass::efsoWorlds) == igdeGDClass::efsoWorlds){
		pInitOCWorlds(objectClass, propertyPrefix);
	}
	
	filter &= objectClass.GetInheritSubObjects();
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pInitObjectClassOCs(oc, p, filter);
	});
}

void gdeViewActiveObject::pInitOCComponents(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetComponents().Visit([&](gdeOCComponent *occomponent){
		pOCComponents.Add(gdeVAOComponent::Ref::New(*this, objectClass, propertyPrefix, occomponent));
	});
}

void gdeViewActiveObject::pInitOCBillboards(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetBillboards().Visit([&](gdeOCBillboard *ocbillboard){
		pOCBillboards.Add(gdeVAOBillboard::Ref::New(*this, objectClass, propertyPrefix, ocbillboard));
	});
}

void gdeViewActiveObject::pInitOCCameras(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetCameras().Visit([&](gdeOCCamera *occamera){
		pOCCameras.Add(gdeVAOCamera::Ref::New(*this, objectClass, propertyPrefix, occamera));
	});
}

void gdeViewActiveObject::pInitOCLights(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetLights().Visit([&](gdeOCLight *oclight){
		pOCLights.Add(gdeVAOLight::Ref::New(*this, objectClass, propertyPrefix, oclight));
	});
}

void gdeViewActiveObject::pInitOCParticleEmitters(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetParticleEmitters().Visit([&](gdeOCParticleEmitter *ocparticleemitter){
		pOCParticleEmitters.Add(gdeVAOParticleEmitter::Ref::New(*this, objectClass, propertyPrefix, ocparticleemitter));
	});
}

void gdeViewActiveObject::pInitOCForceFields(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetForceFields().Visit([&](gdeOCForceField *ocforcefield){
		pOCForceFields.Add(gdeVAOForceField::Ref::New(*this, objectClass, propertyPrefix, ocforcefield));
	});
}

void gdeViewActiveObject::pInitOCEnvMapProbes(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetEnvMapProbes().Visit([&](gdeOCEnvMapProbe *ocenvmapprobe){
		pOCEnvMapProbes.Add(gdeVAOEnvMapProbe::Ref::New(*this, objectClass, propertyPrefix, ocenvmapprobe));
	});
}

void gdeViewActiveObject::pInitOCNavigationSpaces(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetNavigationSpaces().Visit([&](gdeOCNavigationSpace *ocnavspace){
		pOCNavSpaces.Add(gdeVAONavSpace::Ref::New(*this, objectClass, propertyPrefix, ocnavspace));
	});
}

void gdeViewActiveObject::pInitOCNavigationBlockers(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetNavigationBlockers().Visit([&](gdeOCNavigationBlocker *ocnavblocker){
		pOCNavBlockers.Add(gdeVAONavBlocker::Ref::New(*this, objectClass, propertyPrefix, ocnavblocker));
	});
}

void gdeViewActiveObject::pInitOCSnapPoints(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetSnapPoints().Visit([&](gdeOCSnapPoint *ocsnappoint){
		pOCSnapPoints.Add(gdeVAOSnapPoint::Ref::New(*this, objectClass, propertyPrefix, ocsnappoint));
	});
}

void gdeViewActiveObject::pInitOCSpeakers(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetSpeakers().Visit([&](gdeOCSpeaker *ocspeaker){
		pOCSpeakers.Add(gdeVAOSpeaker::Ref::New(*this, objectClass, propertyPrefix, ocspeaker));
	});
}

void gdeViewActiveObject::pInitOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	objectClass.GetWorlds().Visit([&](gdeOCWorld *ocworld){
		pOCWorlds.Add(gdeVAOWorld::Ref::New(*this, objectClass, propertyPrefix, ocworld));
	});
}

void gdeViewActiveObject::pAddComponentShadowIgnore(){
	pOCComponents.Visit([&](gdeVAOComponent &occomp){
		if(occomp.GetOCComponent()->GetLightShadowIgnore()){
			pOCLights.Visit([&](gdeVAOLight &oclight){
				oclight.ShadowIgnoreComponent(occomp.GetComponent());
			});
		}
	});
}

void gdeViewActiveObject::pRebuildOCComponents(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCComponents(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCComponents(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCBillboards(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCBillboards(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCBillboards(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCCameras(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCCameras(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCCameras(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCLights(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCLights(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCLights(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCEnvMapProbes(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCEnvMapProbes(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCEnvMapProbes(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCNavigationSpaces(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCNavigationSpaces(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCNavigationSpaces(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCNavigationBlockers(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCNavigationBlockers(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCNavigationBlockers(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCParticleEmitters(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCParticleEmitters(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCParticleEmitters(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCForceFields(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCForceFields(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCForceFields(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCSnapPoints(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCSnapPoints(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCSnapPoints(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCSpeakers(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCSpeakers(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCSpeakers(oc, p);
	});
}

void gdeViewActiveObject::pRebuildOCWorlds(const gdeObjectClass &objectClass, const decString &propertyPrefix){
	pInitOCWorlds(objectClass, propertyPrefix);
	pAllInheritedApply(objectClass, propertyPrefix, [&](const gdeObjectClass &oc, const decString &p){
		pRebuildOCWorlds(oc, p);
	});
}

void gdeViewActiveObject::pAllInheritedApply(const gdeObjectClass &objectClass, const decString &propertyPrefix,
std::function<void(const gdeObjectClass &, const decString &)> function){
	if(pGameDefinition){
		objectClass.GetInherits().Visit([&](const gdeOCInherit &inherit){
			const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass(inherit.GetName());
			if(ioc){
				function(*ioc, propertyPrefix + inherit.GetPropertyPrefix());
			}
		});
	}
}


void gdeViewActiveObject::pOCReattachAllObjects(){
	// remove all attachments
	pOCComponents.Visit([](gdeVAOComponent &vao){
		deCollider * const collider = vao.GetCollider();
		if(collider){
			collider->RemoveAllAttachments();
		}
	});
	
	// attach all objects
	pOCLights.Visit([](gdeVAOLight &light){
		light.AttachResources();
	});
	pOCParticleEmitters.Visit([](gdeVAOParticleEmitter &emitter){
		emitter.AttachResources();
	});
	pOCForceFields.Visit([](gdeVAOForceField &forceField){
		forceField.AttachResources();
	});
}



void gdeViewActiveObject::pCenterOnObjectClass(igdeCamera &camera){
	decVector boxMinExtend, boxMaxExtend;
	bool first = true;
	
	auto f = [&](const decVector &minExtend, const decVector &maxExtend){
		if(first){
			boxMinExtend = minExtend;
			boxMaxExtend = maxExtend;
			first = false;
			
		}else{
			boxMinExtend.SetSmallest(minExtend);
			boxMaxExtend.SetLargest(maxExtend);
		}
	};
	
	pOCComponents.Visit([&](gdeVAOComponent &component){
		decVector resMinExtend, resMaxExtend;
		component.GetExtends(resMinExtend, resMaxExtend);
		f(resMinExtend, resMaxExtend);
	});
	
	pOCBillboards.Visit([&](gdeVAOBillboard &billboard){
		decVector resMinExtend, resMaxExtend;
		billboard.GetExtends(resMinExtend, resMaxExtend);
		f(resMinExtend, resMaxExtend);
	});
	
	pCenterOnBox(camera, boxMinExtend, boxMaxExtend);
}

void gdeViewActiveObject::pCenterOnSkin(igdeCamera &camera){
	const gdeSkin * const skin = pGameDefinition->GetActiveSkin();
	if(!skin){
		return;
	}
	
	const decVector extends(1.0f, 1.0f, 1.0f);
	pCenterOnBox(camera, -extends, extends);
}

void gdeViewActiveObject::pCenterOnSky(igdeCamera &camera){
	camera.SetPosition(decDVector());
	camera.SetOrientation(decVector());
	camera.SetDistance(0.0f);
}

void gdeViewActiveObject::pCenterOnParticleEmitter(igdeCamera &camera){
	const decVector extends(1.0f, 1.0f, 1.0f);
	pCenterOnBox(camera, -extends, extends);
}

void gdeViewActiveObject::pCenterOnBox(igdeCamera &camera,
const decVector &minExtend, const decVector &maxExtend){
	const decVector viewRotation(-10.0f, 10.0f, 0.0f);
	const decMatrix matrixRotation(decMatrix::CreateRotation(viewRotation * DEG2RAD));
	
	const decVector halfSize((maxExtend - minExtend) * 0.5f);
	const decVector corners[8] = {
		decVector(halfSize.x, halfSize.y, halfSize.z),
		decVector(-halfSize.x, halfSize.y, halfSize.z),
		decVector(halfSize.x, halfSize.y, -halfSize.z),
		decVector(-halfSize.x, halfSize.y, -halfSize.z),
		decVector(halfSize.x, -halfSize.y, halfSize.z),
		decVector(-halfSize.x, -halfSize.y, halfSize.z),
		decVector(halfSize.x, -halfSize.y, -halfSize.z),
		decVector(-halfSize.x, -halfSize.y, -halfSize.z)};
	
	const float halfFovX = tanf(camera.GetFov() * 0.5f * DEG2RAD);
	const float halfFovY = tanf((camera.GetFov()
		/ pGameDefinition->GetViewRatio()) * 0.5f * DEG2RAD);
	
	const float borderScale = 1.1f; // leave 10% space between object and border
	
	float viewDistance = 0.0f;
	int i;
	for(i=0; i<8; i++){
		const decVector corner(matrixRotation * corners[i]);
		if(corner.z < 0.0f){
			continue; // back side vertex are always smaller
		}
		
		viewDistance = decMath::max(viewDistance, corner.z + decMath::max(
			(corner.x * borderScale) / halfFovX,
			(corner.y * borderScale) / halfFovY));
	}
	
	camera.SetPosition(decDVector((minExtend + maxExtend) * 0.5f));
	camera.SetOrientation(viewRotation + decVector(0.0f, 180.0f, 0.0f));
	camera.SetDistance(viewDistance);
}
