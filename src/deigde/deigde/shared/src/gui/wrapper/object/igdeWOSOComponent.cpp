/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWOSOComponent.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/igdeGDClassInherit.h"
#include "../../../gamedefinition/class/component/igdeGDCComponent.h"
#include "../../../gamedefinition/class/component/igdeGDCCTexture.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../loadsave/igdeLoadAnimator.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/rig/deRigReference.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinReference.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/world/deWorld.h>



// Resource loading
/////////////////////

class igdeWOSOComponentResLoadComponent : public igdeResourceLoaderListener{
private:
	igdeWOSOComponent *pOwner;
	decString pPathAudioModel;
	decString pPathOcclusionModel;
	decString pPathAnimation;
	decString pPathModel;
	decString pPathSkin;
	decString pPathRig;
	deModelReference pAudioModel;
	deOcclusionMeshReference pOcclusionMesh;
	deAnimationReference pAnimation;
	deModelReference pModel;
	deSkinReference pSkin;
	deRigReference pRig;
	decObjectDictionary pTextureSkins;
	int pCounter;
	bool pSuccess;
	
public:
	igdeWOSOComponentResLoadComponent( igdeWOSOComponent &owner ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ){
	}
	
	virtual ~igdeWOSOComponentResLoadComponent(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	void LoadAudioModel( const char *path ){
		pPathAudioModel = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertModel, this );
		pCounter++;
	}
	inline deModel *GetAudioModel() const{ return pAudioModel; }
	
	void LoadOcclusionMesh( const char *path ){
		pPathOcclusionModel = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertOcclusionMesh, this );
		pCounter++;
	}
	inline deOcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	void LoadAnimation( const char *path ){
		pPathAnimation = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertAnimation, this );
		pCounter++;
	}
	inline deAnimation *GetAnimation() const{ return pAnimation; }
	
	void LoadModel( const char *path ){
		pPathModel = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertModel, this );
		pCounter++;
	}
	inline deModel *GetModel() const{ return pModel; }
	
	void LoadSkin( const char *path ){
		pPathSkin = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSkin, this );
		pCounter++;
	}
	inline deSkin *GetSkin() const{ return pSkin; }
	inline bool HasPathSkin() const{ return ! pPathSkin.IsEmpty(); }
	
	void LoadRig( const char *path ){
		pPathRig = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertRig, this );
		pCounter++;
	}
	inline deRig *GetRig() const{ return pRig; }
	
	void LoadTextureSkin( const char *path ){
		if( pTextureSkins.Has( path ) ){
			return;
		}
		pTextureSkins.SetAt( path, NULL );
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSkin, this );
		pCounter++;
	}
	deSkin *GetTextureSkinFor( const char *path ) const{
		deObject *skin = NULL;
		return pTextureSkins.GetAt( path, &skin ) ? ( deSkin* )skin : NULL;
	}
	inline const decObjectDictionary &GetTextureSkins() const{ return pTextureSkins; }
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
// 		pOwner->GetWrapper().GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
// 			"igdeWOSOComponentResLoadComponent.LoadingFinished: %p %s (%d)[%d,%d]", &pOwner->GetWrapper(),
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertModel && pPathAudioModel == filename ){
			pAudioModel = ( deModel* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertOcclusionMesh && pPathOcclusionModel == filename ){
			pOcclusionMesh = ( deOcclusionMesh* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertAnimation && pPathAnimation == filename ){
			pAnimation = ( deAnimation* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertModel && pPathModel == filename ){
			pModel = ( deModel* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
			pSkin = ( deSkin* )resource;
			pCounter--;
		}
		if( type == deResourceLoader::ertRig && pPathRig == filename ){
			pRig = ( deRig* )resource;
			pCounter--;
		}
		if( pTextureSkins.Has( filename ) ){
			pTextureSkins.SetAt( filename, resource );
			pCounter--;
		}
		
		CheckFinished();
	}
	
	virtual void LoadingFailed( const igdeResourceLoaderTask &task ){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
// 		pOwner->GetWrapper().GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
// 			"igdeWOSOComponentResLoadComponent.LoadingFailed: %p %s (%d)[%d,%d]", &pOwner->GetWrapper(),
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertModel && pPathAudioModel == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertOcclusionMesh && pPathOcclusionModel == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertAnimation && pPathAnimation == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertModel && pPathModel == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( type == deResourceLoader::ertRig && pPathRig == filename ){
			pCounter--;
			pSuccess = false;
		}
		if( pTextureSkins.Has( filename ) ){
			pCounter--;
			pSuccess = false;
		}
		
		CheckFinished();
	}
	
	void CheckFinished(){
		if( pOwner && pCounter == 0 ){
			pOwner->AsyncLoadFinished( pSuccess );
		}
	}
};


// Class igdeWOSOComponent
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOComponent::igdeWOSOComponent( igdeWObject &wrapper,
	const igdeGDCComponent &gdComponent, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDComponent( gdComponent ),
pAddedToWorld( false ),
pAttachment( NULL ),
pPlaybackControllerIndex( -1 ),
pRenderEnvMap( false ),
pAffectsAudio( false ),
pLightShadowIgnore( false ),
pColliderCanInteract( false ),
pColliderAddedToWorld( false )
{
	pCollider.TakeOver( GetEngine().GetColliderManager()->CreateColliderComponent() );
	pCollider->SetEnabled( false );
	pCollider->SetMass( wrapper.GetColliderFallback()->GetMass() );
	
	pColliderInteraction.TakeOver( GetEngine().GetColliderManager()->CreateColliderComponent() );
	pColliderInteraction->SetEnabled( false );
	pColliderInteraction->SetResponseType( deCollider::ertKinematic );
	pColliderInteraction->SetUseLocalGravity( true );
	
	deColliderAttachment *attachment = nullptr;
	try{
		attachment = new deColliderAttachment( pColliderInteraction );
		attachment->SetAttachType( deColliderAttachment::eatRig );
		attachment->SetNoScaling( true );
		pCollider->AddAttachment( attachment );
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
	
	GetWrapper().GetWorld()->AddCollider( pCollider );
	GetWrapper().GetWorld()->AddCollider( pColliderInteraction );
	pColliderAddedToWorld = true;
	
	UpdateCollisionFilter();
	UpdateGeometry();
	UpdateColliderResponseType();
	
	pLoadResources();
}

igdeWOSOComponent::~igdeWOSOComponent(){
	if( pResLoad ){
		( ( igdeWOSOComponentResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	pReleaseOutlineComponent();
	pDestroyComponent();
	
	if( pColliderAddedToWorld ){
		GetWrapper().GetWorld()->RemoveCollider( pColliderInteraction );
		GetWrapper().GetWorld()->RemoveCollider( pCollider );
	}
}



// Management
///////////////

void igdeWOSOComponent::UpdateParameters(){
	pLoadResources();
}

void igdeWOSOComponent::OnAllSubObjectsFinishedLoading(){
	pUpdateComponent();
	UpdateVisibility();
	pUpdateOutlineComponent();
}

void igdeWOSOComponent::UpdateVisibility(){
	const bool visible = pIsVisible();
	pCollider->SetEnabled( visible );
	pColliderInteraction->SetEnabled( visible );
	if( pComponent ){
		pComponent->SetVisible( visible );
	}
}

void igdeWOSOComponent::UpdateLayerMasks(){
	if( ! pComponent ){
		return;
	}
	
	int mask = GetWrapper().GetRenderLayerMask();
	if( pRenderEnvMap ){
		mask |= GetWrapper().GetRenderEnvMapMask();
	}
	if( pAffectsAudio ){
		mask |= GetWrapper().GetAudioLayerMask();
	}
	
	pComponent->SetLayerMask( LayerMaskFromInt( mask ) );
}

void igdeWOSOComponent::UpdateCollisionFilter(){
	pCollider->SetCollisionFilter( GetWrapper().GetCollisionFilter() );
	pColliderInteraction->SetCollisionFilter( GetWrapper().GetCollisionFilterInteract() );
}

void igdeWOSOComponent::UpdateGeometry(){
	if( ! pAttachedToCollider ){
		pCollider->SetPosition( GetWrapper().GetPosition() );
		pCollider->SetOrientation( GetWrapper().GetOrientation() );
	}
	
	if( pGDComponent.GetDoNotScale() ){
		pCollider->SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
		
	}else{
		pCollider->SetScale( GetWrapper().GetScaling() );
	}
	
	if( pComponent ){
		if( pGDComponent.GetDoNotScale() ){
			pComponent->SetScaling( decVector( 1.0f, 1.0f, 1.0f ) );
			
		}else{
			pComponent->SetScaling( GetWrapper().GetScaling() );
		}
	}
	
	/* if( pComponent ){
		pComponent->SetPosition( pCollider->GetPosition() );
		pComponent->SetOrientation( pCollider->GetOrientation() );
		pComponent->SetScaling( pCollider->GetScale() );
	} */
}

void igdeWOSOComponent::UpdateColliderResponseType(){
	deCollider::eResponseType responseType = pGDComponent.GetColliderResponseType();
	if( ! GetWrapper().GetDynamicCollider() && responseType == deCollider::ertDynamic ){
		responseType = deCollider::ertKinematic;
	}
	pCollider->SetResponseType( responseType );
	pCollider->SetUseLocalGravity( responseType != deCollider::ertDynamic );
}

void igdeWOSOComponent::Update( float elapsed ){
	if( ! pComponent ){
		return;
	}
	
// 	pComponent->SetPosition( pCollider->GetPosition() );
// 	pComponent->SetOrientation( pCollider->GetOrientation() );
	
	if( pAnimator ){
		if( pPlaybackControllerIndex != -1 ){
			pAnimator->GetControllerAt( pPlaybackControllerIndex ).IncrementCurrentValue( elapsed );
			pAnimator->NotifyControllerChangedAt( pPlaybackControllerIndex );
		}
		pAnimator->Apply();
	}
}

void igdeWOSOComponent::ResetPhysics(){
	// sync the component position and orientation to the collider and reset the animation state.
	// for this the bone states are reset to the reference position and the animator applied.
	if( pComponent ){
// 		pComponent->SetPosition( pCollider->GetPosition() );
// 		pComponent->SetOrientation( pCollider->GetOrientation() );
		
		const int boneCount = pComponent->GetBoneCount();
		int i;
		for( i=0; i<boneCount; i++ ){
			deComponentBone &bone = pComponent->GetBoneAt( i );
			bone.SetPosition( decVector() );
			bone.SetRotation( decQuaternion() );
			bone.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
		}
		
		if( pAnimator ){
			if( pPlaybackControllerIndex != -1 ){
				pAnimator->GetControllerAt( pPlaybackControllerIndex ).SetCurrentValue(
					pAnimator->GetControllerAt( pPlaybackControllerIndex ).GetMinimumValue() );
			}
			pAnimator->Apply();
		}
		
		pComponent->InvalidateBones();
	}
	
	// sync the collider bones to the component bones. this also resets linear and angular velocities
	pCollider->CopyStatesFromComponent();
	pCollider->AttachmentsForceUpdate();
}

void igdeWOSOComponent::ResetComponentTextures(){
	pUpdateTextures();
}

void igdeWOSOComponent::CameraChanged(){
}

void igdeWOSOComponent::OutlineSkinChanged(){
	pUpdateOutlineComponent();
}

void igdeWOSOComponent::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitComponent( *this );
}

void igdeWOSOComponent::AsyncLoadFinished( bool success ){
	if( ! pResLoad ){
		return;
	}
	
	igdeWOSOComponentResLoadComponent &rl =
		( igdeWOSOComponentResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	pTextureSkins = rl.GetTextureSkins();
	rl.Drop();
	
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOComponent::pLoadResources(){
	if( pResLoad ){
		( ( igdeWOSOComponentResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	
	pResLoad.TakeOver( new igdeWOSOComponentResLoadComponent( *this ) );
	igdeWOSOComponentResLoadComponent &rl =
		( igdeWOSOComponentResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	const decString pathModel( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epModel ), pGDComponent.GetModelPath() ) );
	if( ! pathModel.IsEmpty() ){
		rl.LoadModel( pathModel );
	}
	
	const decString pathSkin( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epSkin ), pGDComponent.GetSkinPath() ) );
	if( ! pathSkin.IsEmpty() ){
		rl.LoadSkin( pathSkin );
	}
	
	const decString pathRig( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epRig ), pGDComponent.GetRigPath() ) );
	if( ! pathRig.IsEmpty() ){
		rl.LoadRig( pathRig );
	}
	
	const decString pathOcclusionMesh( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epOcclusionMesh ), pGDComponent.GetOcclusionMeshPath() ) );
	if( ! pathOcclusionMesh.IsEmpty() ){
		rl.LoadOcclusionMesh( pathOcclusionMesh );
	}
	
	const decString pathAudioModel( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epAudioModel ), pGDComponent.GetAudioModelPath() ) );
	if( ! pathAudioModel.IsEmpty() ){
		rl.LoadAudioModel( pathAudioModel );
	}
	
	const decString pathAnimation( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epAnimation ), pGDComponent.GetAnimationPath() ) );
	if( ! pathAnimation.IsEmpty() ){
		rl.LoadAnimation( pathAnimation );
	}
	
	int textureCount = pGDComponent.GetTextureList().GetCount();
	int i;
	for( i=0; i<textureCount; i++ ){
		const igdeGDCCTexture &gdctexture = *pGDComponent.GetTextureList().GetAt( i );
		
		if( ! gdctexture.GetPathSkin().IsEmpty() ){
			rl.LoadTextureSkin( gdctexture.GetPathSkin() );
		}
	}
	
	const igdeGDClass * const gdclass = GetWrapper().GetGDClass();
	if( gdclass ){
		igdeGDCCTextureList textures;
		gdclass->GetDeepComponentTextures( textures );
		textureCount = textures.GetCount();
		for( i=0; i<textureCount; i++ ){
			const igdeGDCCTexture &gdctexture = *textures.GetAt( i );
			if( ! gdctexture.GetPathSkin().IsEmpty() ){
				rl.LoadTextureSkin( gdctexture.GetPathSkin() );
			}
		}
	}
	
	rl.CheckFinished();
}

void igdeWOSOComponent::pUpdateComponent(){
	const igdeWOSOComponentResLoadComponent &rl =
		( igdeWOSOComponentResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	deModel *model = rl.GetModel();
	if( ! model && GetWrapper().GetGDClass() ){
		model = GetGameDefinition().GetDefaultModel();
	}
	
	if( ! model || model->GetLODAt( 0 )->GetFaceCount() == 0 ){
		pResLoad = NULL;
		pReleaseOutlineComponent();
		pDestroyComponent();
		return;
	}
	
	deSkin::Ref skin( rl.GetSkin() );
	if( ! skin && rl.HasPathSkin() && GetWrapper().GetGDClass() ){
		skin = GetEnvironment().GetStockSkin( igdeEnvironment::essError );
	}
	
	deRig *rig = rl.GetRig();
	pColliderCanInteract = true;
	if( rig ){
		const int boneCount = rig->GetBoneCount();
		if( boneCount == 0 ){
			// if rig shapes are used collision is possible if at least one shape is present
			if( rig->GetShapes().GetCount() == 0 ){
				rig = GetEnvironment().GetSharedModelCollisionRig();
				pColliderCanInteract = model->GetLODAt( 0 )->GetFaceCount() > 0;
			}
			
		}else{
			// if rig uses bone shapes and no bone has shapes use shared model collision rig
			// unless model collision is not possible in which case fallback has to be used
			int i;
			for( i=0; i<boneCount; i++ ){
				if( rig->GetBoneAt( i ).GetShapes().GetCount() > 0 ){
					break;
				}
			}
			if( i == boneCount ){
				rig = GetEnvironment().GetSharedModelCollisionRig();
				pColliderCanInteract = model->GetLODAt( 0 )->GetFaceCount() > 0;
			}
		}
		
	}else{
		rig = GetEnvironment().GetSharedModelCollisionRig();
		pColliderCanInteract = model->GetLODAt( 0 )->GetFaceCount() > 0;
	}
	
	if( pColliderCanInteract && ! GetWrapper().GetColliderComponent() ){
		GetWrapper().SetInteractCollider( pCollider );
		GetWrapper().AddInteractionCollider( pColliderInteraction );
		pCollider->SetEnabled( pIsVisible() );
		pColliderInteraction->SetEnabled( pCollider->GetEnabled() );
	}
	
	deAnimation * const animation = rl.GetAnimation();
	
	deModel *currentModel = nullptr;
	deSkin *currentSkin = nullptr;
	deRig *currentRig = nullptr;
	deAnimation *currentAnimation = nullptr;
	
	if( pComponent ){
		currentModel = pComponent->GetModel();
		currentSkin = pComponent->GetSkin();
		currentRig = pComponent->GetRig();
		
	}else{
		pComponent.TakeOver( GetEngine().GetComponentManager()->CreateComponent() );
		
		pComponent->SetHintMovement( pGDComponent.GetStatic()
			? deComponent::emhStationary : deComponent::emhDynamic );
		
		UpdateGeometry();
		UpdateVisibility();
	}
	
	if( pAnimator ){
		currentAnimation = pAnimator->GetAnimation();
	}
	
	if( ! pComponentInteraction ){
		pComponentInteraction.TakeOver( GetEngine().GetComponentManager()->CreateComponent() );
		pComponentInteraction->SetHintMovement( pComponent->GetHintMovement() );
		pComponentInteraction->SetRig( GetEnvironment().GetStockRig( igdeEnvironment::esrModelCollision ) );
		pComponentInteraction->SetVisible( false );
	}
	
	const bool modelChanged = model != currentModel;
	const bool skinChanged = skin != currentSkin;
	const bool rigChanged = rig != currentRig;
	const bool animationChanged = animation != currentAnimation;
	
	// if the model, skin or rig changed the component has to be removed from certain
	// places first to avoid problems
	if( modelChanged || skinChanged || rigChanged ){
		pCollider->SetComponent( nullptr );
		pColliderInteraction->SetComponent( nullptr );
		if( pAnimator ){
			pAnimator->SetComponent( nullptr ); // otherwise the animator is not reset
		}
	}
	
	// change parameters
	pComponent->SetModelAndSkin( model, skin );
	pComponent->SetRig( rig );
	pComponent->SetOcclusionMesh( rl.GetOcclusionMesh() );
	pComponent->SetAudioModel( rl.GetAudioModel() );
	
	pComponentInteraction->SetModelAndSkin( model, skin );
	
	pRenderEnvMap = GetBoolProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epRenderEnvMap ),
		pGDComponent.GetRenderEnvMap() );
	pAffectsAudio = GetBoolProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epAffectsAudio ),
		pGDComponent.GetAffectsAudio() );
	pLightShadowIgnore = GetBoolProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epLightShadowIgnore ),
		pGDComponent.GetLightShadowIgnore() );
	
	if( pGDComponent.GetDoNotScale() ){
		pComponent->SetScaling( decVector( 1.0f, 1.0f, 1.0f ) );
		
	}else{
		pComponent->SetScaling( GetWrapper().GetScaling() );
	}
	
	UpdateLayerMasks();
	
	// update textures
	if( modelChanged || skinChanged ){
		pUpdateTextures();
	}
	
	// assign component back to the places it has been previously been removed from
	if( modelChanged || skinChanged || rigChanged ){
		pCollider->SetComponent( pComponent );
		pColliderInteraction->SetComponent( pComponentInteraction );
		if( pAnimator ){
			pAnimator->SetComponent( pComponent );
		}
	}
	
	// update extends
	if( modelChanged ){
		if( pGDComponent.GetDoNotScale() ){
			ClearBoxExtends();
			
		}else{
			const deModelLOD &lod = *model->GetLODAt( 0 );
			const int vertexCount = lod.GetVertexCount();
			if( vertexCount > 0 ){
				const deModelVertex * const vertices = lod.GetVertices();
				int i;
				
				decVector boxMinExtend( vertices[ 0 ].GetPosition() );
				decVector boxMaxExtend( boxMinExtend );
				for( i=1; i<vertexCount; i++ ){
					const decVector &position = vertices[ i ].GetPosition();
					boxMinExtend.SetSmallest( position );
					boxMaxExtend.SetLargest( position );
				}
				SetBoxExtends( boxMinExtend, boxMaxExtend );
				
			}else{
				ClearBoxExtends();
			}
		}
	}
	
	// animator
	decString pathAnimator( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epAnimator ),
		pGDComponent.GetAnimatorPath() ) );
	
	decString move( GetStringProperty(
		pGDComponent.GetPropertyName( igdeGDCComponent::epMove ),
		pGDComponent.GetMove() ) );
	
	if( pathAnimator != pPathAnimator || animationChanged || move != pMove ){
		const decString playbackController( GetStringProperty(
				pGDComponent.GetPropertyName( igdeGDCComponent::epPlaybackController ),
				pGDComponent.GetPlaybackController() ) );
		
		deEngine &engine = GetEngine();
		deAnimator::Ref animator;
		
		if( ! pathAnimator.IsEmpty() ){
			igdeLoadAnimator loadAnimator( GetEnvironment(), &GetLogger(), "DEIGDE" );
			const decPath vfsPath( decPath::CreatePathUnix( pathAnimator ) );
			
			if( engine.GetVirtualFileSystem()->ExistsFile( vfsPath ) ){
				try{
					const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
						engine.GetVirtualFileSystem()->OpenFileForReading( vfsPath ) ) );
					animator.TakeOver( engine.GetAnimatorManager()->CreateAnimator() );
					loadAnimator.Load( pathAnimator, animator, reader );
					animator->SetAnimation( animation );
					
				}catch( const deException &e ){
					animator = nullptr;
					GetLogger().LogException( "DEIGDE", e );
				}
			}
			
		}else if ( animation ){
			const int moveIndex = animation->FindMove( move );
			
			if( moveIndex != -1 ){
				animator.TakeOver( engine.GetAnimatorManager()->CreateAnimator() );
				animator->SetAnimation( animation );
				animator->SetRig( rig );
				
				deAnimatorController *controller = nullptr;
				deAnimatorLink *link = nullptr;
				
				try{
					controller = new deAnimatorController;
					controller->SetName( playbackController );
					controller->SetValueRange( 0.0f, animation->GetMove( moveIndex )->GetPlaytime() );
					controller->SetClamp( false );
					animator->AddController( controller );
					
				}catch( const deException &e ){
					if( controller ){
						delete controller;
					}
					animator = nullptr;
					GetLogger().LogException( "DEIGDE", e );
				}
				
				try{
					link = new deAnimatorLink;
					link->SetController( 0 );
					
					decCurveBezier curve;
					curve.SetDefaultLinear();
					link->SetCurve( curve );
					animator->AddLink( link );
					
				}catch( const deException &e ){
					if( link ){
						delete link;
					}
					animator = nullptr;
					GetLogger().LogException( "DEIGDE", e );
				}
				
				try{
					const deAnimatorRuleAnimation::Ref rule(
						deAnimatorRuleAnimation::Ref::New( new deAnimatorRuleAnimation ) );
					rule->SetEnableSize( true );
					rule->SetMoveName( move );
					rule->GetTargetMoveTime().AddLink( 0 );
					animator->AddRule( rule );
			GetLogger().LogInfoFormat( "DEIGDE", "OOPS %p %p '%s' %d", &GetWrapper(), this, pGDComponent.GetPropertyName( igdeGDCComponent::epAnimation ).GetString(), moveIndex );
					
				}catch( const deException &e ){
					animator = nullptr;
					GetLogger().LogException( "DEIGDE", e );
				}
			}
		}
		
		pPathAnimator = pathAnimator;
		pMove = move;
		pPlaybackControllerIndex = -1;
		
		if( animator ){
			if( ! pAnimator ){
				pAnimator.TakeOver( engine.GetAnimatorInstanceManager()->CreateAnimatorInstance() );
				pAnimator->SetComponent( pComponent );
			}
			pAnimator->SetAnimator( animator );
			
			pPlaybackControllerIndex = pAnimator->IndexOfControllerNamed( playbackController );
			
		}else{
			pAnimator = nullptr;
		}
	}
	
	// reset the component and collider bones
	if( modelChanged || rigChanged ){
		const int boneCount = pComponent->GetBoneCount();
		int i;
		
		for( i=0; i<boneCount; i++ ){
			deComponentBone &bone = pComponent->GetBoneAt( i );
			bone.SetPosition( decVector() );
			bone.SetRotation( decQuaternion() );
			bone.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
		}
		pComponent->InvalidateBones();
		pComponent->PrepareBones();
		
		pCollider->CopyStatesFromComponent();
		pCollider->AttachmentsForceUpdate();
	}
	
	// to be on the safe side the animator is always applied
	if( pAnimator ){
		pAnimator->Apply();
	}
	
	// add to game world and optionally attach
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddComponent( pComponent );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
	
	pResLoad = NULL;
}

void igdeWOSOComponent::pUpdateTextures(){
	if( ! pComponent || ! pComponent->GetModel() ){
		return;
	}
	
	const igdeGDClass * const gdclass = GetWrapper().GetGDClass();
	const deModel &model = *pComponent->GetModel();
	const int textureCount = model.GetTextureCount();
	deEngine &engine = GetEngine();
	int i;
	
	igdeGDCCTextureList textures;
	if( gdclass ){
		gdclass->GetDeepComponentTextures( textures );
	}
	
	for( i=0; i<textureCount; i++ ){
		deComponentTexture &componentTexture = pComponent->GetTextureAt( i );
		const decString &name = model.GetTextureAt( i )->GetName();
		const igdeGDCCTexture *gdctexture = textures.GetNamed( name );
		if( ! gdctexture ){
			gdctexture = pGDComponent.GetTextureList().GetNamed( name );
		}
		
		deDynamicSkinReference gdctDynamicSkin;
		deSkin *useSkin = NULL;
		int useTexture = 0;
		deDynamicSkin *useDynamicSkin = NULL;
		decVector2 texCoordOffset( 0.0f, 0.0f );
		decVector2 texCoordScale( 1.0f, 1.0f );
		float texCoordRotation = 0.0f;
		
		if( gdctexture ){
			texCoordOffset = gdctexture->GetOffset();
			texCoordScale = gdctexture->GetScale();
			texCoordRotation = gdctexture->GetRotation();
		}
		
		if( ! useSkin && gdctexture ){
			const decString &pathToLoad = gdctexture->GetPathSkin().GetString();
			if( ! pathToLoad.IsEmpty() ){
				deObject *object;
				if( pTextureSkins.GetAt( pathToLoad, &object ) && object ){
					useSkin = ( deSkin* )object;
					useTexture = 0;
				}
			}
		}
		
		if( ! useDynamicSkin && gdctexture ){
			const decColor &gdctColorTint = gdctexture->GetColorTint();
			const bool gdctHasTint = ! gdctColorTint.IsEqualTo( decColor( 1.0f, 1.0f, 1.0f ) );
			bool gdctRequiresDynamicSkin = false;
			
			if( gdctHasTint ){
				gdctRequiresDynamicSkin = true;
			}
			
			if( gdctRequiresDynamicSkin ){
				gdctDynamicSkin.TakeOver( engine.GetDynamicSkinManager()->CreateDynamicSkin() );
				if( gdctHasTint ){
					deDSRenderableColor * const renderable = new deDSRenderableColor( "tint" );
					renderable->SetColor( gdctColorTint );
					gdctDynamicSkin->AddRenderable( renderable );
				}
			}
			
			useDynamicSkin = gdctDynamicSkin;
		}
		
		if( texCoordScale.x == 0.0f ){
			texCoordScale.x = 1.0f;
		}
		if( texCoordScale.y == 0.0f ){
			texCoordScale.y = 1.0f;
		}
		
		const decTexMatrix2 texCoordTransform(
			decTexMatrix2::CreateScale( texCoordScale.x, texCoordScale.y ) *
			decTexMatrix2::CreateRotation( texCoordRotation * DEG2RAD ) *
			decTexMatrix2::CreateTranslation( texCoordOffset.x, texCoordOffset.y ) );
		
		if( useSkin != componentTexture.GetSkin()
		|| useTexture != componentTexture.GetTexture()
		|| useDynamicSkin != componentTexture.GetDynamicSkin()
		|| ! texCoordTransform.IsEqualTo( componentTexture.GetTransform() ) ){
			componentTexture.SetSkin( useSkin );
			componentTexture.SetTexture( useTexture );
			componentTexture.SetTransform( texCoordTransform );
			componentTexture.SetDynamicSkin( useDynamicSkin );
			pComponent->NotifyTextureChanged( i );
		}
	}
}

void igdeWOSOComponent::pDestroyComponent(){
	if( pComponent ){
		if( pCollider == GetWrapper().GetColliderComponent() ){
			GetWrapper().SetInteractCollider( nullptr );
			GetWrapper().RemoveInteractionCollider( pColliderInteraction );
			pCollider->SetEnabled( false );
			pColliderInteraction->SetEnabled( false );
		}
		pColliderCanInteract = false;
		
		ClearBoxExtends();
		DetachFromCollider();
		pPlaybackControllerIndex = -1;
		pAnimator = NULL;
		pPathAnimator.Empty();
		pMove.Empty();
		
		if( pAddedToWorld ){
			GetWrapper().GetWorld()->RemoveComponent( pComponent );
			pAddedToWorld = false;
		}
		
		pCollider->SetComponent( nullptr );
		pComponent = nullptr;
	}
	
	if( pComponentInteraction ){
		pColliderInteraction->SetComponent( nullptr );
		pComponentInteraction = nullptr;
	}
}

void igdeWOSOComponent::AttachToCollider(){
	DetachFromCollider();
	
	if( /*! pComponent ||*/ pGDComponent.GetAttachTarget() ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( /*pComponent*/ pCollider );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDComponent.GetPropertyName( igdeGDCComponent::epAttachPosition ),
			pGDComponent.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDComponent.GetPropertyName( igdeGDCComponent::epAttachRotation ),
			pGDComponent.GetOrientation() ) );
		attachment->SetNoScaling( true );
		
		if( colliderComponent ){
			if( ! pGDComponent.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDComponent.GetBoneName() );
			}
			colliderComponent->AddAttachment( attachment );
			pAttachedToCollider = colliderComponent;
			
		}else{
			colliderFallback->AddAttachment( attachment );
			pAttachedToCollider = colliderFallback;
		}
		
		pAttachment = attachment;
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

void igdeWOSOComponent::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}

bool igdeWOSOComponent::pIsVisible() const{
	bool visible = GetWrapper().GetVisible();
	const bool partiallyVisible = visible && ! GetWrapper().GetPartiallyHidden();
	
	if( pGDComponent.GetPartialHide() ){
		visible = partiallyVisible;
	}
	
	//return pCollider == GetWrapper().GetColliderComponent() && partiallyVisible;
	return GetWrapper().GetColliderComponent() && partiallyVisible;
}

void igdeWOSOComponent::pUpdateOutlineComponent(){
	pReleaseOutlineComponent();
	
	// get outline skin and check if an outline is required
	deSkin * const outlineSkin = GetWrapper().GetOutlineSkin();
	if( ! outlineSkin || ! pComponent || ! pComponent->GetModel() || ! pCollider || ! GetWrapper().GetWorld() ){
		return;
	}
	
	// create outline component
	deEngine &engine = *GetWrapper().GetEnvironment().GetEngineController()->GetEngine();
	
	pOutlineComponent.TakeOver( engine.GetComponentManager()->CreateComponent( pComponent->GetModel(), outlineSkin ) );
	pOutlineComponent->SetRig( pComponent->GetRig() );
	pOutlineComponent->SetDynamicSkin( GetWrapper().GetOutlineDynamicSkin() );
	
	// there are two possible solutions here. either we use the movement hint from the component
	// or we use dynamic movement hint. using the dynamic movement avoids triggering updates
	// of static caches in graphic modules which makes the editor more fluid
// 	pOutlineComponent->SetHintMovement( pComponent->GetHintMovement() );
	pOutlineComponent->SetHintMovement( deComponent::emhDynamic );
	
	const int textureCount = pComponent->GetTextureCount();
	int i;
	for( i=0; i<textureCount; i++ ){
		pOutlineComponent->GetTextureAt( i ).SetSkin( outlineSkin );
		pOutlineComponent->NotifyTextureChanged( i );
	}
	
	GetWrapper().GetWorld()->AddComponent( pOutlineComponent );
	
	// attach outline component
	deColliderAttachment * const attachment = new deColliderAttachment( pOutlineComponent );
	attachment->SetAttachType( deColliderAttachment::eatRig );
	pCollider->AddAttachment( attachment );
}

void igdeWOSOComponent::pReleaseOutlineComponent(){
	if( ! pOutlineComponent ){
		return;
	}
	
	if( pCollider ){
		deColliderAttachment * const attachment = pCollider->GetAttachmentWith( pOutlineComponent );
		if( attachment ){
			pCollider->RemoveAttachment( attachment );
		}
	}
	if( pOutlineComponent->GetParentWorld() ){
		pOutlineComponent->GetParentWorld()->RemoveComponent( pOutlineComponent );
	}
	pOutlineComponent = NULL;
}
