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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "igdeWObject.h"
#include "object/igdeWOSubObject.h"
#include "object/igdeWOSOBillboard.h"
#include "object/igdeWOSOLight.h"
#include "object/igdeWOSOSpeaker.h"
#include "object/igdeWOSOParticleEmitter.h"
#include "object/igdeWOSOForceField.h"
#include "object/igdeWOSOEnvMapProbe.h"
#include "object/igdeWOSONavigationSpace.h"
#include "object/igdeWOSONavigationBlocker.h"
#include "object/igdeWOSOComponent.h"
#include "object/igdeWOSOWorld.h"
#include "../../codec/igdeCodecPropertyString.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../gamedefinition/class/igdeGDClassInherit.h"
#include "../../gamedefinition/class/igdeGDClassManager.h"
#include "../../gamedefinition/class/billboard/igdeGDCBillboard.h"
#include "../../gamedefinition/class/component/igdeGDCCTexture.h"
#include "../../gamedefinition/class/component/igdeGDCComponent.h"
#include "../../gamedefinition/class/envmapprobe/igdeGDCEnvMapProbe.h"
#include "../../gamedefinition/class/light/igdeGDCLight.h"
#include "../../gamedefinition/class/speaker/igdeGDCSpeaker.h"
#include "../../gamedefinition/class/navspace/igdeGDCNavigationSpace.h"
#include "../../gamedefinition/class/navblocker/igdeGDCNavigationBlocker.h"
#include "../../gamedefinition/class/world/igdeGDCWorld.h"
#include "../../gamedefinition/property/igdeGDProperty.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../resourceloader/igdeResourceLoaderListener.h"
#include "../../resourceloader/igdeResourceLoaderTask.h"
#include "../../triggersystem/igdeTriggerTargetList.h"
#include "../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../triggersystem/igdeTriggerExpression.h"
#include "../../triggersystem/igdeTriggerListener.h"
#include "../../triggersystem/igdeTriggerTarget.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableValue.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"


// Collider Listener
//////////////////////

class igdeWObjectColliderListener : public deBaseScriptingCollider{
private:
	igdeWObject *pObject;
	
public:
	igdeWObjectColliderListener(igdeWObject *object){
		pObject = object;
	}
	
	virtual ~igdeWObjectColliderListener(){
	}
	
	virtual void CollisionResponse(deCollider*, deCollisionInfo*){
	}
	
	virtual bool CanHitCollider(deCollider*, deCollider*){
		return true;
	}
	
	virtual void ColliderChanged(deCollider*){
		pObject->OnColliderChanged();
	}
};



// Trigger listener
/////////////////////

class igdeWObjectTriggerListener : public igdeTriggerListener{
	igdeWObject &pWrapper;
	
protected:
	virtual ~igdeWObjectTriggerListener(){
	}
	
public:
	typedef deTObjectReference<igdeWObjectTriggerListener> Ref;
	
	igdeWObjectTriggerListener(igdeWObject &wrapper) : pWrapper(wrapper){
	}
	
	virtual void TriggerTargetChanged(igdeTriggerTarget*){
		pWrapper.UpdateTriggerableProperties();
	}
};



// Asynchronous load finished listener
////////////////////////////////////////

igdeWObject::cAsyncLoadFinished::cAsyncLoadFinished(){
}

igdeWObject::cAsyncLoadFinished::~cAsyncLoadFinished(){
}

void igdeWObject::cAsyncLoadFinished::AnyContentVisibleChanged(igdeWObject&){
}

void igdeWObject::cAsyncLoadFinished::ExtendsChanged(igdeWObject&){
}



// Class igdeWObject
//////////////////////

// Constructor, destructor
////////////////////////////

igdeWObject::igdeWObject(igdeEnvironment &environment) :
pEnvironment(environment),
pColliderUserPointer(nullptr),
pScaling(1.0f, 1.0f, 1.0f),
pRenderLayerMask(0x1),
pRenderEnvMapMask(0x2),
pAudioLayerMask(0x4),
pDynamicCollider(false),
pVisible(true),
pPartiallyHidden(false),
pListenerCollider(nullptr),
pTriggerTable(nullptr),
pHasBoxExtends(false),
pDirtyExtends(false),
pDirtyFallbackColliderShape(false),
pOutlineColor(1.0f, 0.0f, 0.0f),
pAsyncLoadFinished(nullptr),
pAsyncLoadCounter(0),
pAnyContentVisible(false)
{
	try{
		pTriggerListener = igdeWObjectTriggerListener::Ref::New(*this);
		
		// create collider
		pColliderFallback = environment.GetEngineController()->GetEngine()
			->GetColliderManager()->CreateColliderVolume();
		pColliderFallback->SetEnabled(true);
		pColliderFallback->SetResponseType(deCollider::ertStatic);
		pColliderFallback->SetUseLocalGravity(true);
		decShapeList shapeList;
		shapeList.Add(new decShapeBox(decVector(0.1f, 0.1f, 0.1f)));
		pColliderFallback->SetShapes(shapeList);
		pColliderFallback->SetMass(5.0f);
		
		pListenerCollider = new igdeWObjectColliderListener(this);
		environment.SetColliderDelegee(pColliderFallback, pListenerCollider);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeWObject::~igdeWObject(){
	pCleanUp();
}



// Management
///////////////

void igdeWObject::SetWorld(deWorld *world){
	if(world == pWorld){
		return;
	}
	
	pDestroySubObjects();
	DetachCollider();
	if(pWorld){
		pWorld->RemoveCollider(pColliderFallback);
	}
	
	pWorld = world;
	
	if(world){
		world->AddCollider(pColliderFallback);
	}
	
	pUpdateVisiblity();
	
	if(world){
		pCreateSubObjects();
	}
}

void igdeWObject::SetCamera(deCamera *camera){
	if(camera == pCamera){
		return;
	}
	
	pCamera = camera;
	
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.CameraChanged();
	});
}

void igdeWObject::SetGDClass(igdeGDClass *gdClass){
	if(pGDClass == gdClass){
		return;
	}
	
	pDestroySubObjects();
	
	pGDClass = gdClass;
	
	pDirtyExtends = true;
	pDirtyFallbackColliderShape = true;
	
	pUpdateVisiblity();
	pUpdateColliderResponseType();
	pUpdateColliderShapes();
	
	UpdateTriggerTargets();
	
	if(pWorld){
		pCreateSubObjects();
		
		pSubObjectsInitTriggers();
		pSubObjectsUpdateTriggers();
	}
}

void igdeWObject::SetGDClassName(const char *gdClassName){
	SetGDClass(pEnvironment.GetGameProject()->GetGameDefinition()->GetClassManager()->GetNamed(gdClassName));
}

void igdeWObject::SetTriggerTable(igdeTriggerTargetList *triggerTable){
	if(triggerTable == pTriggerTable){
		return;
	}
	
	pTriggerTable = triggerTable;
	UpdateTriggerTargets();
	
	pSubObjectsInitTriggers();
	pSubObjectsUpdateTriggers();
}



void igdeWObject::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	pUpdateMatrices();
	
	pColliderFallback->SetPosition(position);
	pSubObjectsUpdateGeometry();
}

void igdeWObject::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	pUpdateMatrices();
	
	pColliderFallback->SetOrientation(orientation);
	pSubObjectsUpdateGeometry();
}

void igdeWObject::SetScaling(const decVector &scaling){
	if(scaling.IsEqualTo(pScaling)){
		return;
	}
	
	const decVector absScaling(scaling.Absolute());
	
	pScaling.x = absScaling.x > 1e-5f ? scaling.x : (scaling.x > 0.0f ? 1e-5f : -1e-5f);
	pScaling.y = absScaling.y > 1e-5f ? scaling.y : (scaling.y > 0.0f ? 1e-5f : -1e-5f);
	pScaling.z = absScaling.z > 1e-5f ? scaling.z : (scaling.z > 0.0f ? 1e-5f : -1e-5f);
	pDirtyFallbackColliderShape = true;
	
	pUpdateMatrices();
	
	pSubObjectsUpdateGeometry();
	pUpdateColliderShapes();
}



void igdeWObject::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	pUpdateVisiblity();
}

void igdeWObject::SetPartiallyHidden(bool partiallyHidden){
	if(partiallyHidden == pPartiallyHidden){
		return;
	}
	
	pPartiallyHidden = partiallyHidden;
	pUpdateVisiblity();
}



void igdeWObject::SetRenderLayerMask(int mask){
	if(mask == pRenderLayerMask){
		return;
	}
	
	pRenderLayerMask = mask;
	pSubObjectsUpdateLayerMasks();
}

void igdeWObject::SetRenderEnvMapMask(int mask){
	if(mask == pRenderEnvMapMask){
		return;
	}
	
	pRenderEnvMapMask = mask;
	pSubObjectsUpdateLayerMasks();
}

void igdeWObject::SetAudioLayerMask(int mask){
	if(mask == pAudioLayerMask){
		return;
	}
	
	pAudioLayerMask = mask;
	pSubObjectsUpdateLayerMasks();
}

const decVector &igdeWObject::GetBoxMinExtend(){
	pPrepareExtends();
	return pBoxMinExtend;
}

const decVector &igdeWObject::GetBoxMaxExtend(){
	pPrepareExtends();
	return pBoxMaxExtend;
}

bool igdeWObject::GetHasBoxExtends(){
	pPrepareExtends();
	return pHasBoxExtends;
}

void igdeWObject::SetCollisionFilter(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilter){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterParticles(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilterParticles){
		return;
	}
	
	pCollisionFilterParticles = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterForceFields(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilterForceField){
		return;
	}
	
	pCollisionFilterForceField = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterFallback(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilterFallback){
		return;
	}
	
	pCollisionFilterFallback = collisionFilter;
	
	pColliderFallback->SetCollisionFilter(pCollisionFilterFallback);
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterInteract(const decCollisionFilter &collisionFilter){
	if(collisionFilter == pCollisionFilterInteract){
		return;
	}
	
	pCollisionFilterInteract = collisionFilter;
	
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetDynamicCollider(bool dynamic){
	if(dynamic == pDynamicCollider){
		return;
	}
	
	pDynamicCollider = dynamic;
	pUpdateColliderResponseType();
}



void igdeWObject::Update(float elapsed){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.Update(elapsed);
	});
	
	pUpdateColliderShapes();
}

void igdeWObject::ResetComponentTextures(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.ResetComponentTextures();
	});
}

void igdeWObject::ResetPhysics(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.ResetPhysics();
	});
}

void igdeWObject::VisitSubObjects(igdeWOSOVisitor &visitor){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.Visit(visitor);
	});
}

void igdeWObject::SetAsyncLoadFinished(cAsyncLoadFinished *listener){
	pAsyncLoadFinished = listener;
}

bool igdeWObject::AllSubObjectsFinishedLoading() const{
	return pAsyncLoadCounter == 0 && !pSubObjects.HasMatching([](igdeWOSubObject &so){
		return !so.AllSubObjectsFinishedLoading();
	});
}


void igdeWObject::OnGameDefinitionChanged(){
	if(pGDClass){
		SetGDClassName(pGDClass->GetName());
	}
}

void igdeWObject::SetOutlineSkin(deSkin *skin){
	if(pOutlineSkin == skin){
		return;
	}
	
	pOutlineSkin = skin;
	
	if(skin && !pOutlineDynamicSkin){
		pOutlineDynamicSkin = pEnvironment.GetEngineController()->GetEngine()->
			GetDynamicSkinManager()->CreateDynamicSkin();
		
		deDSRenderableColor * const renderableColor = new deDSRenderableColor("color");
		renderableColor->SetColor(pOutlineColor);
		pOutlineDynamicSkin->AddRenderable(renderableColor);
		
		deDSRenderableValue * const renderableThickess = new deDSRenderableValue("thickness");
		renderableThickess->SetValue(0.005f);
		pOutlineDynamicSkin->AddRenderable(renderableThickess);
	}
	
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.OutlineSkinChanged();
	});
}

void igdeWObject::SetOutlineSkinSharedEditing(){
	SetOutlineSkin(pEnvironment.GetStockSkin(igdeEnvironment::essEditOutline));
}

void igdeWObject::SetOutlineColor(const decColor &color){
	if(color.IsEqualTo(pOutlineColor)){
		return;
	}
	
	pOutlineColor = color;
	
	if(!pOutlineDynamicSkin){
		return;
	}
	
	((deDSRenderableColor*)pOutlineDynamicSkin->GetRenderableAt(0))->SetColor(color);
	pOutlineDynamicSkin->NotifyRenderableChanged(0);
}



// Properties
///////////////

void igdeWObject::SetProperty(const char *key, const char *value){
	pProperties.SetAt(key, value);
	pUpdateProperties();
}

void igdeWObject::SetPropertiesFrom(const decStringDictionary &list){
	pProperties = list;
	pUpdateProperties();
}

void igdeWObject::RemoveProperty(const char *key){
	if(pProperties.Has(key)){
		pProperties.Remove(key);
		pUpdateProperties();
	}
}

void igdeWObject::RemoveAllProperties(){
	if(pProperties.GetCount() > 0){
		pProperties.RemoveAll();
		pUpdateProperties();
	}
}



bool igdeWObject::IsAttachedCollider() const{
	return pParentCollider;
}

void igdeWObject::AttachColliderRig(deColliderComponent *parentCollider){
	if(!parentCollider){
		DETHROW(deeInvalidParam);
	}
	
	DetachCollider();
	
	deColliderAttachment *attachment = nullptr;
	try{
		if(pColliderComponent){
			attachment = new deColliderAttachment(pColliderComponent);
			attachment->SetAttachType(deColliderAttachment::eatRig);
			parentCollider->AddAttachment(attachment);
			attachment = nullptr;
		}
		
		attachment = new deColliderAttachment(pColliderFallback);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		parentCollider->AddAttachment(attachment);
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
	
	pParentCollider = parentCollider;
}

void igdeWObject::AttachColliderBone(deColliderComponent *parentCollider, const char *bone,
const decVector &position, const decQuaternion &orientation){
	if(!parentCollider || !bone){
		DETHROW(deeInvalidParam);
	}
	
	DetachCollider();
	
	if(strlen(bone) > 0){
		deColliderAttachment *attachment = nullptr;
		try{
			if(pColliderComponent){
				attachment = new deColliderAttachment(pColliderComponent);
				attachment->SetAttachType(deColliderAttachment::eatBone);
				attachment->SetTrackBone(bone);
				attachment->SetPosition(position);
				attachment->SetOrientation(orientation);
				parentCollider->AddAttachment(attachment);
				attachment = nullptr;
			}
			
			attachment = new deColliderAttachment(pColliderFallback);
			attachment->SetAttachType(deColliderAttachment::eatBone);
			attachment->SetTrackBone(bone);
			attachment->SetPosition(position);
			attachment->SetOrientation(orientation);
			parentCollider->AddAttachment(attachment);
			
		}catch(const deException &){
			if(attachment){
				delete attachment;
			}
			throw;
		}
		
		pAttachToBone = bone;
	}
	
	pParentCollider = parentCollider;
}

void igdeWObject::DetachCollider(){
	if(!pParentCollider){
		return;
	}
	
	deColliderAttachment *attachment = pParentCollider->GetAttachmentWith(pColliderFallback);
	if(attachment){
		pParentCollider->RemoveAttachment(attachment);
	}
	
	if(pColliderComponent){
		attachment = pParentCollider->GetAttachmentWith(pColliderComponent);
		if(attachment){
			pParentCollider->RemoveAttachment(attachment);
		}
	}
	
	pParentCollider = nullptr;
	pAttachToBone.Empty();
	
	// reset position and orientation otherwise our state and the collider state are out of sync
	pColliderFallback->SetPosition(pPosition);
	pColliderFallback->SetOrientation(pOrientation);
	
	pSubObjectsUpdateGeometry();
}

deCollider *igdeWObject::GetCollider() const{
	if(pColliderComponent){
		return pColliderComponent;
		
	}else{
		return pColliderFallback;
	}
}

deComponent *igdeWObject::GetComponent() const{
	return pColliderComponent ? pColliderComponent->GetComponent() : nullptr;
}

void igdeWObject::SetColliderUserPointer(void *userPointer){
	if(userPointer == pColliderUserPointer){
		return;
	}
	
	pColliderUserPointer = userPointer;
	if(pColliderComponent){
		pEnvironment.SetColliderUserPointer(pColliderComponent, userPointer);
	}
	pEnvironment.SetColliderUserPointer(pColliderFallback, userPointer);
	
	pCollidersInteraction.Visit([&](deCollider *c){
		pEnvironment.SetColliderUserPointer(c, userPointer);
	});
	
	pSubObjectsColliderUserPointerChanged();
}

void igdeWObject::OnColliderChanged(){
}



void igdeWObject::UpdateTriggerTargets(){
	pListTriggerTarget.RemoveAll();
	if(!pTriggerTable || !pGDClass){
		return;
	}
	
	const decStringList &keys = pProperties.GetKeys();
	const int keyCount = keys.GetCount();
	int i;
	
	for(i=0; i<keyCount; i++){
		const decString &key = keys.GetAt(i);
		const decString &value = pProperties.GetAt(key);
		if(value.IsEmpty()){
			continue;
		}
		
		igdeGDProperty * const gdproperty = pGDClass->GetPropertyNamed(key);
		if(gdproperty && gdproperty->GetType() == igdeGDProperty::eptTriggerTarget){
			pListTriggerTarget.Add(pTriggerTable->GetNamedAddIfMissing(value));
		}
	}
}

void igdeWObject::UpdateTriggerableProperties(){
	pSubObjectsUpdateTriggers();
}



void igdeWObject::SubObjectFinishedLoading(igdeWOSubObject&, bool){
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
}

void igdeWObject::SubObjectExtendsDirty(){
	pDirtyExtends = true;
	pDirtyFallbackColliderShape = true;
	
	if(pAsyncLoadFinished){
		pAsyncLoadFinished->ExtendsChanged(*this);
	}
}

void igdeWObject::SetInteractCollider(deColliderComponent *collider){
	if(pColliderComponent){
		pEnvironment.SetColliderDelegee(pColliderComponent, nullptr);
		pEnvironment.SetColliderUserPointer(pColliderComponent, nullptr);
		pColliderComponent = nullptr;
		pColliderFallback->SetEnabled(pVisible && !pPartiallyHidden);
	}
	
	pCollidersInteraction.Visit([&](deCollider *c){
		pEnvironment.SetColliderDelegee(c, nullptr);
		pEnvironment.SetColliderUserPointer(c, nullptr);
	});
	
	if(!collider){
		return;
	}
	
	void * const userPointer = pEnvironment.GetColliderUserPointer(pColliderFallback);
	
	pColliderFallback->SetEnabled(false);
	pColliderComponent = collider;
	pEnvironment.SetColliderDelegee(collider, pListenerCollider);
	pEnvironment.SetColliderUserPointer(collider, userPointer);
	
	pCollidersInteraction.Visit([&](deCollider *c){
		pEnvironment.SetColliderDelegee(c, pListenerCollider);
		pEnvironment.SetColliderUserPointer(c, userPointer);
	});
}

void igdeWObject::AddInteractionCollider(deCollider *collider){
	DEASSERT_NOTNULL(collider)
	
	pCollidersInteraction.Add(collider);
	
	pEnvironment.SetColliderDelegee(collider, pListenerCollider);
	pEnvironment.SetColliderUserPointer(collider, pEnvironment.GetColliderUserPointer(pColliderFallback));
}

void igdeWObject::RemoveInteractionCollider(deCollider *collider){
	DEASSERT_NOTNULL(collider);
	
	pEnvironment.SetColliderDelegee(collider, nullptr);
	pEnvironment.SetColliderUserPointer(collider, nullptr);
	
	pCollidersInteraction.Remove(collider);
}

void igdeWObject::UpdateAnyContentVisibile(){
	const bool anyContentVisible = pSubObjects.HasMatching([](igdeWOSubObject &so){
		return so.IsContentVisible();
	});
	
	if(anyContentVisible == pAnyContentVisible){
		return;
	}
	
	pAnyContentVisible = anyContentVisible;
	if(pAsyncLoadFinished){
		pAsyncLoadFinished->AnyContentVisibleChanged(*this);
	}
}



// Private Functions
//////////////////////

void igdeWObject::pCleanUp(){
	DetachCollider();
	pDestroySubObjects();
	SetWorld(nullptr);
	
	pCollidersInteraction.Visit([&](deCollider *c){
		pEnvironment.SetColliderUserPointer(c, nullptr);
	});
	
	if(pColliderFallback){
		pEnvironment.SetColliderDelegee(pColliderFallback, nullptr);
		pColliderFallback = nullptr;
	}
	if(pListenerCollider){
		delete pListenerCollider;
	}
}



void igdeWObject::pUpdateVisiblity(){
	const bool partiallyVisible = (pVisible && !pPartiallyHidden);
	pColliderFallback->SetEnabled(!pColliderComponent && partiallyVisible);
	
	pSubObjectsUpdateVisibility();
}



void igdeWObject::pCreateSubObjects(){
	pAsyncLoadCounter = 1;
	
	if(pGDClass && pWorld){
		pCreateSubObjects("", pGDClass, igdeGDClass::FilterSubObjectsAll);
	}
	
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
}

void igdeWObject::pCreateSubObjects(const decString &prefix, const igdeGDClass &gdclass, int filter){
	// components
	if((filter & igdeGDClass::efsoComponents) != 0){
		const igdeGDCComponent::List &components = gdclass.GetComponentList();
		components.Visit([&](const igdeGDCComponent &c){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOComponent::Ref::New(*this, c, prefix));
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// billboards
	if((filter & igdeGDClass::efsoBillboards) != 0){
		const igdeGDCBillboard::List &billboards = gdclass.GetBillboardList();
		billboards.Visit([&](const igdeGDCBillboard &b){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOBillboard::Ref::New(*this, b, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// lights
	if((filter & igdeGDClass::efsoLights) != 0){
		const igdeGDCLight::List &lights = gdclass.GetLightList();
		lights.Visit([&](const igdeGDCLight &l){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOLight::Ref::New(*this, l, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// speakers
	if((filter & igdeGDClass::efsoSpeakers) != 0){
		const igdeGDCSpeaker::List &speakers = gdclass.GetSpeakerList();
		speakers.Visit([&](const igdeGDCSpeaker &s){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOSpeaker::Ref::New(*this, s, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// particle emitters
	if((filter & igdeGDClass::efsoParticleEmitters) != 0){
		const igdeGDCParticleEmitter::List &particleEmitters = gdclass.GetParticleEmitterList();
		particleEmitters.Visit([&](const igdeGDCParticleEmitter &p){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOParticleEmitter::Ref::New(*this, p, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// force fields
	if((filter & igdeGDClass::efsoForceFields) != 0){
		const igdeGDCForceField::List &forceFields = gdclass.GetForceFieldList();
		forceFields.Visit([&](const igdeGDCForceField &f){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOForceField::Ref::New(*this, f, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// envMapProbes
	if((filter & igdeGDClass::efsoEnvMapProbes) != 0){
		const igdeGDCEnvMapProbe::List &envMapProbes = gdclass.GetEnvironmentMapProbeList();
		envMapProbes.Visit([&](const igdeGDCEnvMapProbe &e){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOEnvMapProbe::Ref::New(*this, e, prefix));
				
			}catch(const deException &ex){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, ex);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// navigation spaces
	if((filter & igdeGDClass::efsoNavigationSpaces) != 0){
		const igdeGDCNavigationSpace::List &navigationSpaces = gdclass.GetNavigationSpaceList();
		navigationSpaces.Visit([&](const igdeGDCNavigationSpace &ns){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSONavigationSpace::Ref::New(*this, ns, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// navigation blockers
	if((filter & igdeGDClass::efsoNavigationBlockers) != 0){
		const igdeGDCNavigationBlocker::List &navigationBlockers = gdclass.GetNavigationBlockerList();
		navigationBlockers.Visit([&](const igdeGDCNavigationBlocker &nb){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSONavigationBlocker::Ref::New(*this, nb, prefix));
				
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// worlds
	if((filter & igdeGDClass::efsoWorlds) != 0){
		const igdeGDCWorld::List &worlds = gdclass.GetWorldList();
		worlds.Visit([&](const igdeGDCWorld &w){
			pAsyncLoadCounter++;
			try{
				pSubObjects.Add(igdeWOSOWorld::Ref::New(*this, w, prefix));
			}catch(const deException &e){
				pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
				pAsyncLoadCounter--;
			}
		});
	}
	
	// inherited classes
	filter &= gdclass.GetInheritSubObjects();
	
	gdclass.GetInheritClasses().Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			pCreateSubObjects(prefix + inherit.GetPropertyPrefix(), *inherit.GetClass(), filter);
		}
	});
}

void igdeWObject::pDestroySubObjects(){
	pSubObjects.RemoveAll();
}
void igdeWObject::pSubObjectsReattachToColliders(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.ReattachToColliders();
	});
}

void igdeWObject::pSubObjectsInitTriggers(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.InitTriggers();
	});
}

void igdeWObject::pSubObjectsUpdateGeometry(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateGeometry();
	});
}

void igdeWObject::pSubObjectsUpdateTriggers(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateTriggers();
	});
}

void igdeWObject::pSubObjectsUpdateVisibility(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateVisibility();
	});
}

void igdeWObject::pSubObjectsUpdateLayerMasks(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateLayerMasks();
	});
}

void igdeWObject::pSubObjectsUpdateCollisionFilter(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateCollisionFilter();
	});
}

void igdeWObject::pSubObjectsColliderUserPointerChanged(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.ColliderUserPointerChanged();
	});
}

void igdeWObject::pSubObjectsAllFinishedLoading(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.OnAllSubObjectsFinishedLoading();
	});
}

void igdeWObject::pCheckAsyncLoadFinished(){
	if(pAsyncLoadCounter != 0){
		return;
	}
	
	pSubObjectsAllFinishedLoading();
	pSubObjectsUpdateTriggers();
	pSubObjectsReattachToColliders();
	UpdateAnyContentVisibile();
	
	if(pAsyncLoadFinished){
		pAsyncLoadFinished->LoadFinished(*this, true);
	}
}

void igdeWObject::pUpdateProperties(){
	UpdateTriggerTargets();
	
	if(!pWorld){
		return;
	}
	
	pAsyncLoadCounter = 1;
	
	pSubObjects.Visit([&](igdeWOSubObject &so){
		pAsyncLoadCounter++;
		try{
			so.UpdateParameters();
			
		}catch(const deException &e){
			pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
			pAsyncLoadCounter--;
		}
	});
	
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
	
	pSubObjectsInitTriggers();
	pSubObjectsUpdateTriggers();
}

void igdeWObject::pUpdateColliderResponseType(){
	pSubObjects.Visit([&](igdeWOSubObject &so){
		so.UpdateColliderResponseType();
	});
	
	deCollider::eResponseType responseType;
	if(pColliderComponent){
		responseType = pColliderComponent->GetResponseType();
		
	}else if(pDynamicCollider){
		responseType = deCollider::ertDynamic;
		
	}else{
		responseType = deCollider::ertKinematic;
	}
	
	pColliderFallback->SetResponseType(responseType);
	pColliderFallback->SetUseLocalGravity(responseType != deCollider::ertDynamic);
}

void igdeWObject::pUpdateColliderShapes(){
	if(!pDirtyFallbackColliderShape){
		return;
	}
	
	pDirtyFallbackColliderShape = false;
	decShapeList shapeList;
	
	pSubObjects.Visit([&](igdeWOSubObject &so){
		if(!so.HasBoxExtends()){
			return;
		}
		
		const decVector &boxMinExtend = so.GetBoxMinExtends();
		const decVector &boxMaxExtend = so.GetBoxMaxExtends();
		decVector boxSize((boxMaxExtend - boxMinExtend) * 0.5f);
		decVector boxCenter((boxMinExtend + boxMaxExtend) * 0.5f);
		
		boxSize.x *= pScaling.x;
		boxSize.y *= pScaling.y;
		boxSize.z *= pScaling.z;
		boxCenter.x *= pScaling.x;
		boxCenter.y *= pScaling.y;
		boxCenter.z *= pScaling.z;
		
		shapeList.Add(new decShapeBox(boxSize, boxCenter));
	});
	
	if(shapeList.GetCount() == 0){
		// ensure the user can still select the object
		shapeList.Add(new decShapeBox(decVector(0.1f, 0.1f, 0.1f)));
	}
	
	pColliderFallback->SetShapes(shapeList);
}

void igdeWObject::pPrepareExtends(){
	if(!pDirtyExtends){
		return;
	}
	
	pHasBoxExtends = false;
	pBoxMinExtend.SetZero();
	pBoxMaxExtend.SetZero();
	
	pSubObjects.Visit([&](igdeWOSubObject &so){
		if(!so.HasBoxExtends()){
			return;
		}
		
		if(pHasBoxExtends){
			pBoxMinExtend.SetSmallest(so.GetBoxMinExtends());
			pBoxMaxExtend.SetLargest(so.GetBoxMaxExtends());
			
		}else{
			pBoxMinExtend = so.GetBoxMinExtends();
			pBoxMaxExtend = so.GetBoxMaxExtends();
			pHasBoxExtends = true;
		}
	});
	
	if(pHasBoxExtends){
		const decVector boxSize(pBoxMaxExtend - pBoxMinExtend);
		if(boxSize.x < 0.001f){
			pBoxMinExtend.x -= 0.0005f;
			pBoxMaxExtend.x += 0.0005f;
		}
		if(boxSize.y < 0.001f){
			pBoxMinExtend.y -= 0.0005f;
			pBoxMaxExtend.y += 0.0005f;
		}
		if(boxSize.z < 0.001f){
			pBoxMinExtend.z -= 0.0005f;
			pBoxMaxExtend.z += 0.0005f;
		}
	}
	
	pDirtyExtends = false;
}

void igdeWObject::pUpdateMatrices(){
	pMatrix.SetWorld(pPosition, pOrientation, pScaling);
	pInvMatrix = pMatrix.QuickInvert();
}
