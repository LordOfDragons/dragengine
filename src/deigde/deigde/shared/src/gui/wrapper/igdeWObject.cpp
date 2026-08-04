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
#include "../igdeUIHelper.h"
#include "../browse/igdeDialogBrowserObjectClass.h"
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


// Class igdeWObject::MetaContext
///////////////////////////////////

namespace{

class cActionMPSelectClass : public igdeMetaProperty::Action{
	igdeWObject::MetaProperties::ObjectClass &pPropertyClass;
	
public:
	cActionMPSelectClass(igdeWObject::MetaProperties::ObjectClass &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
	Action(owner, context, "@Igde.MetaPropertyPath.Action.BrowseGameDef",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSearch),
		"@Igde.MetaPropertyPath.Action.BrowseGameDef.ToolTip"),
	pPropertyClass(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyClass.IsValid(context)){
			return;
		}
		
		igdeGDClass *gdClass = pPropertyClass.Owner(context).GetGDClass();
		if(igdeDialogBrowserObjectClass::SelectObjectClass(&GetOwner(), gdClass)){
			pPropertyClass.ChangePropertyValue(context, gdClass->GetName());
		}
	}
};

}

igdeWObject::MetaContext::Ref igdeWObject::CreateMetaContext(igdeWObject *wrapper){
	return igdeWObject::MetaContext::Ref::New("igde.wobject",
		"Object Wrapper", "Object Wrapper properties",
		igdeWObject::MetaProperties::global.properties, wrapper);
}


// Class igdeWObject::MetaProperties
//////////////////////////////////////

// ObjectClass

igdeWObject::MetaProperties::ObjectClass::ObjectClass() :
igdeMetaPropertyMCT("igde.wobject.class", "Igde.WPWObject.Class"){
	SetEnableAllowed(true);
}

igdeWObject::MetaProperties::ObjectClass::~ObjectClass() = default;

igdeMetaPropertyStringStorage::Storage &igdeWObject::MetaProperties::ObjectClass::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPObjectClass();
}

decStringSet igdeWObject::MetaProperties::ObjectClass::GetPropertyAllowedStrings(const ContextRef &context) const{
	decStringSet set;
	Owner(context).GetEnvironment().GetGameProject()->GetGameDefinition()->GetClassManager()->
		GetClasses().Visit([&](const igdeGDClass &gdClass){
			set.Add(gdClass.GetName());
		});
	return set;
}

void igdeWObject::MetaProperties::ObjectClass::AddContextMenuEntries(
igdeMenuCascade &menu, const ContextRef &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, deTObjectReference<cActionMPSelectClass>::New(*this, owner, context));
}


// World

igdeWObject::MetaProperties::World::World() :
igdeMetaPropertyMCT("igde.wobject.world", "Igde.WPWObject.World", igdeEnvironment::eFilePatternListTypes::efpltWorld){
}

igdeWObject::MetaProperties::World::~World() = default;

igdeMetaPropertyPathStorage::Storage &igdeWObject::MetaProperties::World::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPWorld();
}


// Position

igdeWObject::MetaProperties::Position::Position() :
igdeMetaPropertyMCT("igde.wobject.position", "Igde.WPWObject.Position"){
}

igdeWObject::MetaProperties::Position::~Position() = default;

igdeMetaPropertyDVectorStorage::Storage &igdeWObject::MetaProperties::Position::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPPosition();
}


// Rotation

igdeWObject::MetaProperties::Rotation::Rotation() :
igdeMetaPropertyMCT("igde.wobject.rotation", "Igde.WPWObject.Orientation"){
}

igdeWObject::MetaProperties::Rotation::~Rotation() = default;

igdeMetaPropertyVectorStorageQuaternion::Storage &igdeWObject::MetaProperties::Rotation::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPRotation();
}


// Scaling

igdeWObject::MetaProperties::Scaling::Scaling() :
igdeMetaPropertyMCT("igde.wobject.scaling", "Igde.WPWObject.Scaling"){
	SetDefaultValue(decVector(1.0f, 1.0f, 1.0f));
}

igdeWObject::MetaProperties::Scaling::~Scaling() = default;

igdeMetaPropertyVectorStorage::Storage &igdeWObject::MetaProperties::Scaling::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPScaling();
}

igdeMetaPropertyVectorUndo::Ref igdeWObject::MetaProperties::Scaling::ChangePropertyValue(
const ContextRef &context, const decVector &newValue, const char *undoInfo, const char *undoInfoLong){
	const auto absScaling = newValue.Absolute();
	return igdeMetaPropertyMCT::ChangePropertyValue(context, decVector(
		absScaling.x > 1e-5f ? newValue.x : (newValue.x > 0.0f ? 1e-5f : -1e-5f),
		absScaling.y > 1e-5f ? newValue.y : (newValue.y > 0.0f ? 1e-5f : -1e-5f),
		absScaling.z > 1e-5f ? newValue.z : (newValue.z > 0.0f ? 1e-5f : -1e-5f)),
			undoInfo, undoInfoLong);
}


// Visible

igdeWObject::MetaProperties::Visible::Visible() :
igdeMetaPropertyMCT("igde.wobject.visible", "Igde.WPWObject.Visible"){
	SetDefaultValue(true);
}

igdeWObject::MetaProperties::Visible::~Visible() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeWObject::MetaProperties::Visible::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPVisible();
}


// DynamicCollider

igdeWObject::MetaProperties::DynamicCollider::DynamicCollider() :
igdeMetaPropertyMCT("igde.wobject.dynamicCollider", "Igde.WPWObject.DynamicCollider"){
}

igdeWObject::MetaProperties::DynamicCollider::~DynamicCollider() = default;

igdeMetaPropertyBooleanStorage::Storage &igdeWObject::MetaProperties::DynamicCollider::GetStorage(
const igdeMetaContext::Ref &context) const{
	return Owner(context).GetMPDynamicCollider();
}


// Properties

igdeWObject::MetaProperties igdeWObject::MetaProperties::global;

igdeWObject::MetaProperties::MetaProperties() :
objectClass(deTObjectReference<ObjectClass>::New()),
world(deTObjectReference<World>::New()),
position(deTObjectReference<Position>::New()),
rotation(deTObjectReference<Rotation>::New()),
scaling(deTObjectReference<Scaling>::New()),
visible(deTObjectReference<Visible>::New()),
dynamicCollider(deTObjectReference<DynamicCollider>::New()),
properties(igdeMetaContext::PropertyList::Ref::New(decTObjectOrderedSet<igdeMetaProperty>(devctag,
	objectClass, world, position, rotation, scaling, visible, dynamicCollider))){
}


// Class igdeWObject
//////////////////////

// Constructor, destructor
////////////////////////////

igdeWObject::igdeWObject(igdeEnvironment &environment) :
pEnvironment(environment),
pUndoSystem(nullptr),
pMetaContext(CreateMetaContext(this)),
pColliderUserPointer(nullptr),
pRenderLayerMask(0x1),
pRenderEnvMapMask(0x2),
pAudioLayerMask(0x4),
pRequiresInteraction(false),
pPartiallyHidden(false),
pListenerCollider(nullptr),
pTriggerTable(nullptr),
pHasBoxExtends(false),
pDirtyExtends(false),
pDirtyFallbackColliderShape(false),
pOutlineColor(1.0f, 0.0f, 0.0f),
pAsyncLoadFinished(nullptr),
pAsyncLoadCounter(0),
pAnyContentVisible(false),
pMPObjectClass(MetaProperties::global.objectClass, pMetaContext),
pMPWorld(MetaProperties::global.world, pMetaContext),
pMPPosition(MetaProperties::global.position, pMetaContext),
pMPRotation(MetaProperties::global.rotation, pMetaContext),
pMPScaling(MetaProperties::global.scaling, pMetaContext),
pMPVisible(MetaProperties::global.visible, pMetaContext),
pMPDynamicCollider(MetaProperties::global.dynamicCollider, pMetaContext)
{
	try{
		pTriggerListener = igdeWObjectTriggerListener::Ref::New(*this);
		
		// create collider
		pColliderFallback = environment.GetEngineController()->GetEngine()
			->GetColliderManager()->CreateColliderVolume();
		pColliderFallback->SetEnabled(true);
		pColliderFallback->SetResponseType(deCollider::ertStatic);
		pColliderFallback->SetUseLocalGravity(true);
		decShape::List shapeList;
		shapeList.Add(decShapeBox::Ref::New(decVector(0.1f, 0.1f, 0.1f)));
		pColliderFallback->SetShapes(shapeList);
		pColliderFallback->SetMass(5.0f);
		
		pListenerCollider = new igdeWObjectColliderListener(this);
		environment.SetColliderDelegee(pColliderFallback, pListenerCollider);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pMPObjectClass.onValueChanged = [this](){
		SetGDClass(pEnvironment.GetGameProject()->GetGameDefinition()->
			GetClassManager()->GetClasses().FindNamed(pMPObjectClass));
		onChanged();
	};
	
	pMPWorld.onValueChanged = [this](){
		pDestroySubObjects();
		DetachCollider();
		pWorldGDClass.Clear();
		pUpdateContent();
		onChanged();
	};
	
	pMPPosition.onValueChanged = [this](){
		pUpdateMatrices();
		pColliderFallback->SetPosition(pMPPosition);
		pSubObjectsUpdateGeometry();
		onChanged();
	};
	
	pMPRotation.onValueChanged = [this](){
		pUpdateMatrices();
		pColliderFallback->SetOrientation(pMPRotation);
		pSubObjectsUpdateGeometry();
		onChanged();
	};
	
	pMPScaling.onValueChanged = [this](){
		pDirtyFallbackColliderShape = true;
		pUpdateMatrices();
		pSubObjectsUpdateGeometry();
		pUpdateColliderShapes();
		onChanged();
	};
	
	pMPVisible.onValueChanged = [this](){
		pUpdateVisiblity();
		onChanged();
	};
	
	pMPDynamicCollider.onValueChanged = [this](){
		pUpdateColliderResponseType();
		onChanged();
	};
}

igdeWObject::~igdeWObject(){
	pCleanUp();
}


// Management
///////////////

void igdeWObject::SetUndoSystem(igdeUndoSystem *undoSystem){
	pUndoSystem = undoSystem;
}

void igdeWObject::SetWorld(deWorld *value){
	if(value == pWorld){
		return;
	}
	
	pDestroySubObjects();
	DetachCollider();
	if(pWorld){
		pWorld->RemoveCollider(pColliderFallback);
	}
	
	pWorld = value;
	
	if(pWorld){
		pWorld->AddCollider(pColliderFallback);
	}
	
	pUpdateVisiblity();
	
	if(pWorld){
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
	DetachCollider();
	
	pGDClass = gdClass;
	
	pUpdateContent();
}

void igdeWObject::SetGDClassName(const char *gdClassName){
	pMPObjectClass = gdClassName;
}

void igdeWObject::SetPathWorld(const char *path){
	pMPWorld = path;
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



void igdeWObject::SetPosition(const decDVector &value){
	pMPPosition = value;
}

void igdeWObject::SetOrientation(const decQuaternion &value){
	pMPRotation = value;
}

void igdeWObject::SetScaling(const decVector &value){
	pMPScaling = value;
}



void igdeWObject::SetVisible(bool value){
	pMPVisible = value;
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
	pMPDynamicCollider = dynamic;
}

void igdeWObject::SetRequiresInteraction(bool requiresInteraction){
	pRequiresInteraction = requiresInteraction;
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
	pMPObjectClass.Property().NotifyAllowedStringsChanged(pMetaContext);
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
		
		auto renderableColor = deDSRenderableColor::Ref::New("color");
		renderableColor->SetColor(pOutlineColor);
		pOutlineDynamicSkin->AddRenderable(std::move(renderableColor));
		
		auto renderableThickess = deDSRenderableValue::Ref::New("thickness");
		renderableThickess->SetValue(0.005f);
		pOutlineDynamicSkin->AddRenderable(std::move(renderableThickess));
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
	
	pOutlineDynamicSkin->GetRenderableAt(0).ReferenceDynamicCast<deDSRenderableColor>().SetColor(color);
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
	
	if(pColliderComponent){
		auto attachment = deColliderAttachment::Ref::New(pColliderComponent);
		attachment->SetAttachType(deColliderAttachment::eatRig);
		parentCollider->AddAttachment(std::move(attachment));
	}
	
	auto attachment = deColliderAttachment::Ref::New(pColliderFallback);
	attachment->SetAttachType(deColliderAttachment::eatStatic);
	parentCollider->AddAttachment(std::move(attachment));
	
	pParentCollider = parentCollider;
}

void igdeWObject::AttachColliderBone(deColliderComponent *parentCollider, const char *bone,
const decVector &aposition, const decQuaternion &aorientation){
	if(!parentCollider || !bone){
		DETHROW(deeInvalidParam);
	}
	
	DetachCollider();
	
	if(strlen(bone) > 0){
		if(pColliderComponent){
			auto attachment = deColliderAttachment::Ref::New(pColliderComponent);
			attachment->SetAttachType(deColliderAttachment::eatBone);
			attachment->SetTrackBone(bone);
			attachment->SetPosition(aposition);
			attachment->SetOrientation(aorientation);
			parentCollider->AddAttachment(std::move(attachment));
		}
		
		auto attachment = deColliderAttachment::Ref::New(pColliderFallback);
		attachment->SetAttachType(deColliderAttachment::eatBone);
		attachment->SetTrackBone(bone);
		attachment->SetPosition(aposition);
		attachment->SetOrientation(aorientation);
		parentCollider->AddAttachment(std::move(attachment));
		
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
	
	pParentCollider.Clear();
	pAttachToBone.Empty();
	
	// reset position and orientation otherwise our state and the collider state are out of sync
	pColliderFallback->SetPosition(pMPPosition);
	pColliderFallback->SetOrientation(pMPRotation);
	
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
	return pColliderComponent ? pColliderComponent->GetComponent().Pointer() : nullptr;
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
	
	pProperties.GetKeys().Visit([&](const decString &key){
		const decString &value = pProperties.GetAt(key);
		if(value.IsEmpty()){
			return;
		}
		
		igdeGDProperty * const gdproperty = pGDClass->GetPropertyNamed(key);
		if(gdproperty && gdproperty->GetType() == igdeGDProperty::eptTriggerTarget){
			pListTriggerTarget.Add(pTriggerTable->GetNamedAddIfMissing(value));
		}
	});
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
	if(pColliderComponent == collider){
		return;
	}
	
	DetachCollider();
	
	if(pColliderComponent){
		pEnvironment.SetColliderDelegee(pColliderComponent, nullptr);
		pEnvironment.SetColliderUserPointer(pColliderComponent, nullptr);
		pColliderComponent = nullptr;
		pColliderFallback->SetEnabled(pMPVisible && !pPartiallyHidden);
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
	pDestroySubObjects();
	DetachCollider();
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
	
	if(pMetaContext){
		pMetaContext->Dispose();
	}
}



void igdeWObject::pUpdateContent(){
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

void igdeWObject::pUpdateVisiblity(){
	const bool partiallyVisible = (pMPVisible && !pPartiallyHidden);
	pColliderFallback->SetEnabled(!pColliderComponent && partiallyVisible);
	
	pSubObjectsUpdateVisibility();
}

igdeGDClass::Ref igdeWObject::pCreateWorldGDClass(){
	auto gdclass = igdeGDClass::Ref::New("_World");
	auto gdcworld = igdeGDCWorld::Ref::New();
	gdcworld->SetPath(pMPWorld);
	gdclass->AddWorld(gdcworld);
	return gdclass;
}

void igdeWObject::pCreateSubObjects(){
	pAsyncLoadCounter = 1;
	
	if(pWorld){
		if(pGDClass){
			pCreateSubObjects("", pGDClass, igdeGDClass::FilterSubObjectsAll);
			
		}else if(!pMPWorld->IsEmpty()){
			if(!pWorldGDClass){
				pWorldGDClass = pCreateWorldGDClass();
			}
			pCreateSubObjects("", pWorldGDClass, igdeGDClass::FilterSubObjectsAll);
		}
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
		
	}else if(pMPDynamicCollider){
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
	decShape::List shapeList;
	
	const decVector &s = pMPScaling;
	pSubObjects.Visit([&](igdeWOSubObject &so){
		if(!so.HasBoxExtends()){
			return;
		}
		
		const decVector &boxMinExtend = so.GetBoxMinExtends();
		const decVector &boxMaxExtend = so.GetBoxMaxExtends();
		decVector boxSize((boxMaxExtend - boxMinExtend) * 0.5f);
		decVector boxCenter((boxMinExtend + boxMaxExtend) * 0.5f);
		
		boxSize.x *= s.x;
		boxSize.y *= s.y;
		boxSize.z *= s.z;
		boxCenter.x *= s.x;
		boxCenter.y *= s.y;
		boxCenter.z *= s.z;
		
		shapeList.Add(decShapeBox::Ref::New(boxSize, boxCenter));
	});
	
	if(shapeList.GetCount() == 0){
		// ensure the user can still select the object
		shapeList.Add(decShapeBox::Ref::New(decVector(0.1f, 0.1f, 0.1f)));
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
	pMatrix.SetWorld(pMPPosition.GetValue(), pMPRotation.GetQuaternion(), pMPScaling.GetValue());
	pInvMatrix = pMatrix.QuickInvert();
}
