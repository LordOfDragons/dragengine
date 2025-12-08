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

#include "gdeVAOComponent.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../gamedef/objectClass/component/gdeOCComponentTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/loadsave/igdeLoadAnimator.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOComponent
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOComponent::gdeVAOComponent(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCComponent *occomponent) :
gdeVAOSubObject(view, objectClass, propertyPrefix),
pOCComponent(occomponent),
pPlayback(false)
{
	if(!occomponent){
		DETHROW(deeInvalidParam);
	}
	try{
		pCreateComponent();
		pCreateCollider();
		pCreateAnimator();
		pAttachComponent();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeVAOComponent::~gdeVAOComponent(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOComponent::Update(float elapsed){
	if(!pAnimator){
		return;
	}
	
	if(pPlayback && pAnimator->GetControllerCount() > 0){
		pAnimator->GetControllerAt(0).IncrementCurrentValue(elapsed);
	}
	
	pAnimator->Apply();
}

void gdeVAOComponent::RebuildResources(){
	// TODO improve this by keeping old resources until the new ones are fully created.
	//      this reduces update time making UI more fluid
	pReleaseResources();
	
	pCreateComponent();
	pCreateCollider();
	pCreateAnimator();
	pAttachComponent();
}

void gdeVAOComponent::UpdateTexture(gdeOCComponentTexture *texture){
	if(!pComponent){
		return;
	}
	
	const deModel &model = *pComponent->GetModel();
	const int index = model.IndexOfTextureNamed(texture->GetName());
	if(index == -1){
		return;
	}
	
	pUpdateComponentTexture(texture, pComponent->GetTextureAt(index), index);
}

void gdeVAOComponent::SelectedObjectChanged(){
}

void gdeVAOComponent::GetExtends(decVector &minExtend, decVector &maxExtend) const{
	minExtend.SetZero();
	maxExtend.SetZero();
	if(!pComponent || !pComponent->GetModel()){
		return;
	}
	
	const deModelLOD &lod = *pComponent->GetModel()->GetLODAt(0);
	const int vertexCount = lod.GetVertexCount();
	if(vertexCount > 0){
		const deModelVertex * const vertices = lod.GetVertices();
		int i;
		minExtend = maxExtend = vertices[0].GetPosition();
		for(i=1; i<vertexCount; i++){
			const decVector &position = vertices[i].GetPosition();
			minExtend.SetSmallest(position);
			maxExtend.SetLargest(position);
		}
	}
}



// Private functions
//////////////////////

void gdeVAOComponent::pCleanUp(){
	pReleaseResources();
}


void gdeVAOComponent::pCreateComponent(){
	deOcclusionMesh::Ref occlusionMesh;
	deModel::Ref audioModel;
	deModel::Ref model;
	deSkin::Ref skin;
	deRig::Ref rig;
	
	// load the new resources. if the resource is already in use it is not loaded again just the
	// reference count increased by one. loading of individual resources is allowed to fail. in
	// this case the exception is logged and the resource simply not loaded or replaced with a
	// placeholder. this behavior can be changed later on
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	decString path;
	
	// model
	path = PropertyString(pOCComponent->GetPropertyName(gdeOCComponent::epModel), pOCComponent->GetModelPath());
	if(!path.IsEmpty()){
		try{
			model.TakeOver(engine.GetModelManager()->LoadModel(vfs, path, "/"));
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
		}
	}
	
	// skin
	path = PropertyString(pOCComponent->GetPropertyName(gdeOCComponent::epSkin), pOCComponent->GetSkinPath());
	if(!path.IsEmpty()){
		try{
			skin.TakeOver(engine.GetSkinManager()->LoadSkin(vfs, path, "/"));
			
		}catch(const deException &){
			skin = environment.GetStockSkin(igdeEnvironment::essError);
		}
	}
	
	// rig
	path = PropertyString(pOCComponent->GetPropertyName(gdeOCComponent::epRig), pOCComponent->GetRigPath());
	if(!path.IsEmpty()){
		try{
			rig.TakeOver(engine.GetRigManager()->LoadRig(vfs, path, "/"));
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
		}
	}
	
	// occlusion mesh
	path = PropertyString(pOCComponent->GetPropertyName(gdeOCComponent::epOcclusionMesh), pOCComponent->GetOcclusionMeshPath());
	if(!path.IsEmpty()){
		try{
			occlusionMesh.TakeOver(engine.GetOcclusionMeshManager()->LoadOcclusionMesh(vfs, path, "/"));
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
		}
	}
	
	// audio model
	path = PropertyString(pOCComponent->GetPropertyName(gdeOCComponent::epAudioModel), pOCComponent->GetAudioModelPath());
	if(!path.IsEmpty()){
		try{
			audioModel.TakeOver(engine.GetModelManager()->LoadModel(vfs, path, "/"));
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
		}
	}
	
	// if the model or skin are missing use the default ones unless we have no component defined
	/*if( ! model ){
		model = pView.GetGameDefinition()->GetDefaultModel();
	}*/
	
	/*if( ! skin ){
		skin = pView.GetGameDefinition()->GetDefaultSkin();
	}*/
	
	// create component if model and skin are present
	if(model && skin){
		pComponent.TakeOver(engine.GetComponentManager()->CreateComponent(model, skin));
		pView.GetGameDefinition()->GetWorld()->AddComponent(pComponent);
	}
	if(pComponent){
		pComponent->SetRig(rig);
		pComponent->SetOcclusionMesh(occlusionMesh);
		pComponent->SetAudioModel(audioModel);
	}
	
	pCreateComponentTextures();
}

void gdeVAOComponent::pCreateComponentTextures(){
	if(!pComponent){
		return;
	}
	
	const deModel &model = *pComponent->GetModel();
	const int textureCount = model.GetTextureCount();
	int i;
	
	for(i=0; i<textureCount; i++){
		const decString &textureName = model.GetTextureAt(i)->GetName();
		const gdeOCComponentTexture * const componentTexture = pOCComponent->GetTextures().GetNamed(textureName);
		const gdeOCComponentTexture * const objectClassTexture = pView.GetObjectClass()->GetTextures().GetNamed(textureName);
		
		pUpdateComponentTexture(objectClassTexture ? objectClassTexture : componentTexture,
			pComponent->GetTextureAt(i), i);
	}
}

void gdeVAOComponent::pUpdateComponentTexture(const gdeOCComponentTexture *texture,
deComponentTexture &engTexture, int engTextureIndex){
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	deSkin::Ref occtextureSkin;
	deDynamicSkin::Ref gdctDynamicSkin;
	deSkin *useSkin = NULL;
	int useTexture = 0;
	deDynamicSkin *useDynamicSkin = NULL;
	decVector2 texCoordOffset(0.0f, 0.0f);
	decVector2 texCoordScale(1.0f, 1.0f);
	float texCoordRotation = 0.0f;
	
	if(texture){
		texCoordOffset = texture->GetOffset();
		texCoordScale = texture->GetScale();
		texCoordRotation = texture->GetRotation();
	}
	
	if(!useSkin && texture){
		try{
			if(!texture->GetPathSkin().IsEmpty()){
				occtextureSkin.TakeOver(engine.GetSkinManager()->LoadSkin(texture->GetPathSkin(), "/"));
			}
			
		}catch(const deException &){
			occtextureSkin = environment.GetStockSkin(igdeEnvironment::essError);
		}
		
		if(occtextureSkin){
			useSkin = occtextureSkin;
			useTexture = 0;
		}
	}
	
	if(!useDynamicSkin && texture){
		const decColor &gdctColorTint = texture->GetColorTint();
		const bool gdctHasTint = !gdctColorTint.IsEqualTo(decColor(1.0f, 1.0f, 1.0f));
		bool gdctRequiresDynamicSkin = false;
		
		if(gdctHasTint){
			gdctRequiresDynamicSkin = true;
		}
		
		if(gdctRequiresDynamicSkin){
			gdctDynamicSkin.TakeOver(engine.GetDynamicSkinManager()->CreateDynamicSkin());
			if(gdctHasTint){
				deDSRenderableColor * const renderable = new deDSRenderableColor("tint");
				renderable->SetColor(gdctColorTint);
				gdctDynamicSkin->AddRenderable(renderable);
			}
		}
		
		useDynamicSkin = gdctDynamicSkin;
	}
	
	if(fabsf(texCoordScale.x) < FLOAT_SAFE_EPSILON){
		texCoordScale.x = 1.0f;
	}
	if(fabsf(texCoordScale.y) < FLOAT_SAFE_EPSILON){
		texCoordScale.y = 1.0f;
	}
	const decTexMatrix2 texCoordTransform =
		decTexMatrix2::CreateScale(texCoordScale.x, texCoordScale.y) *
		decTexMatrix2::CreateRotation(texCoordRotation * DEG2RAD) *
		decTexMatrix2::CreateTranslation(texCoordOffset.x, texCoordOffset.y);
	
	if(useSkin != engTexture.GetSkin()
	|| useTexture != engTexture.GetTexture()
	|| useDynamicSkin != engTexture.GetDynamicSkin()
	|| !texCoordTransform.IsEqualTo(engTexture.GetTransform())){
		engTexture.SetSkin(useSkin);
		engTexture.SetTexture(useTexture);
		engTexture.SetTransform(texCoordTransform);
		engTexture.SetDynamicSkin(useDynamicSkin);
		pComponent->NotifyTextureChanged(engTextureIndex);
	}
}

void gdeVAOComponent::pCreateCollider(){
	if(pComponent){
		pCollider.TakeOver(pView.GetGameDefinition()->GetEngine()->GetColliderManager()->CreateColliderComponent());
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(pOCComponent->GetColliderResponseType());
		pCollider->SetUseLocalGravity(pOCComponent->GetColliderResponseType() != deCollider::ertDynamic);
		pCollider->SetMass(5.0f);
		((deColliderComponent&)(deCollider&)pCollider).SetComponent(pComponent);
		
	}else{
		decShapeList shapeList;
		shapeList.Add(new decShapeBox(decVector(0.1f, 0.1f, 0.1f)));
		
		pCollider.TakeOver(pView.GetGameDefinition()->GetEngine()->GetColliderManager()->CreateColliderVolume());
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertStatic);
		pCollider->SetUseLocalGravity(true);
		pCollider->SetMass(5.0f);
		((deColliderVolume&)(deCollider&)pCollider).SetShapes(shapeList);
	}
	
	decLayerMask collisionMask;
	collisionMask.SetBit(0);
	pCollider->SetCollisionFilter(decCollisionFilter(collisionMask));
	
	pView.GetGameDefinition()->GetWorld()->AddCollider(pCollider);
}

void gdeVAOComponent::pCreateAnimator(){
	if(!pComponent){
		return;
	}
	
	deVirtualFileSystem &vfs = *pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	deAnimation::Ref animation;
	deAnimator::Ref animator;
	
	const decString &pathAnimator = pOCComponent->GetAnimatorPath();
	const decString &pathAnimation = pOCComponent->GetAnimationPath();
	const decString &move = pOCComponent->GetMove();
	
	if(pathAnimator.IsEmpty() && pathAnimation.IsEmpty()){
		return;
	}
	
	if(!pathAnimation.IsEmpty() && vfs.ExistsFile(decPath::CreatePathUnix(pathAnimation))){
		try{
			animation.TakeOver(engine.GetAnimationManager()->LoadAnimation(&vfs, pathAnimation, "/"));
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
			animation = nullptr;
		}
	}
	
	if(!pathAnimator.IsEmpty()){
		const decPath vfsPath(decPath::CreatePathUnix(pathAnimator));
		if(!vfs.ExistsFile(vfsPath)){
			return;
		}
		
		igdeLoadAnimator loader(environment, environment.GetLogger(), LOGSOURCE);
		try{
			const decBaseFileReader::Ref reader(
				decBaseFileReader::Ref::New(vfs.OpenFileForReading(vfsPath)));
			animator.TakeOver(engine.GetAnimatorManager()->CreateAnimator());
			loader.Load(pathAnimator, animator, reader);
			animator->SetAnimation(animation);
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
			return;
		}
		
	}else if(animation){
		const int moveIndex = animation->FindMove(move);
		if(moveIndex == -1){
			return;
		}
		
		animator.TakeOver(engine.GetAnimatorManager()->CreateAnimator());
		animator->SetAnimation(animation);
		
		deAnimatorController *controller = nullptr;
		deAnimatorLink *link = nullptr;
		
		try{
			controller = new deAnimatorController;
			controller->SetName(pOCComponent->GetPlaybackController());
			controller->SetValueRange(0.0f, animation->GetMove(moveIndex)->GetPlaytime());
			animator->AddController(controller);
			
		}catch(const deException &e){
			if(controller){
				delete controller;
			}
			environment.GetLogger()->LogException(LOGSOURCE, e);
			return;
		}
		
		try{
			link = new deAnimatorLink;
			link->SetController(0);
			
			decCurveBezier curve;
			curve.SetDefaultLinear();
			link->SetCurve(curve);
			animator->AddLink(link);
			
		}catch(const deException &e){
			if(link){
				delete link;
			}
			environment.GetLogger()->LogException(LOGSOURCE, e);
			return;
		}
		
		try{
			const deAnimatorRuleAnimation::Ref rule(
				deAnimatorRuleAnimation::Ref::NewWith());
			rule->SetEnableSize(true);
			rule->SetMoveName(move);
			rule->GetTargetMoveTime().AddLink(0);
			animator->AddRule(rule);
			
		}catch(const deException &e){
			environment.GetLogger()->LogException(LOGSOURCE, e);
			return;
		}
	}
	
	pAnimator.TakeOver(engine.GetAnimatorInstanceManager()->CreateAnimatorInstance());
	pAnimator->SetComponent(pComponent);
	pAnimator->SetAnimator(animator);
	
	pPlayback = true;
}

void gdeVAOComponent::pAttachComponent(){
	if(!pComponent || !pCollider){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	try{
		attachment = new deColliderAttachment(pCollider);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		//attachment->SetPosition( pOCComponent->GetPosition() );
		//attachment->SetOrientation( pOCComponent->GetOrientation() );
		pCollider->AddAttachment(attachment);
		attachment = NULL;
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}



void gdeVAOComponent::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	pAnimator = NULL;
	
	if(pCollider){
		pCollider->RemoveAllAttachments(); // because otherwise cyclic loop with component
		world.RemoveCollider(pCollider);
		pCollider = NULL;
	}
	
	if(pComponent){
		world.RemoveComponent(pComponent);
		pComponent = NULL;
	}
}
