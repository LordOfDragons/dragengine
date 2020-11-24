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

#include "igdeWOSOBillboard.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/billboard/igdeGDCBillboard.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/billboard/deBillboardManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>



// Resource loading
/////////////////////

class igdeWOSOBillboardResLoadComponent : public igdeResourceLoaderListener{
private:
	igdeWOSOBillboard *pOwner;
	decString pPathSkin;
	deSkinReference pSkin;
	int pCounter;
	bool pSuccess;
	
public:
	igdeWOSOBillboardResLoadComponent( igdeWOSOBillboard &owner ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ){
	}
	
	virtual ~igdeWOSOBillboardResLoadComponent(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	void LoadSkin( const char *path ){
// 		pOwner->GetWrapper().GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
// 			"igdeWOSOBillboardResLoadComponent.LoadSkin: %p %s [%d]", &pOwner->GetWrapper(), path, pCounter );
		pPathSkin = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSkin, this );
		pCounter++;
	}
	inline deSkin *GetSkin() const{ return pSkin; }
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
// 		pOwner->GetWrapper().GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
// 			"igdeWOSOBillboardResLoadComponent.LoadingFinished: %p %s (%d)[%d,%d]", &pOwner->GetWrapper(),
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
			pSkin = ( deSkin* )resource;
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
// 			"igdeWOSOBillboardResLoadComponent.LoadingFailed: %p %s (%d)[%d,%d]", &pOwner->GetWrapper(),
// 			filename.GetString(), type, pCounter, pSuccess );
		
		if( type == deResourceLoader::ertSkin && pPathSkin == filename ){
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


// Class igdeWOSOBillboard
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOBillboard::igdeWOSOBillboard( igdeWObject &wrapper,
	const igdeGDCBillboard &gdBillboard, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDBillboard( gdBillboard ),
pAddedToWorld( false ),
pAttachment( NULL ),
pRenderEnvMap( false )
{
	pLoadResources();
}

igdeWOSOBillboard::~igdeWOSOBillboard(){
	if( pResLoad ){
		( ( igdeWOSOBillboardResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	pDestroyBillboard();
}



// Management
///////////////

void igdeWOSOBillboard::UpdateParameters(){
	pLoadResources();
}

void igdeWOSOBillboard::OnAllSubObjectsFinishedLoading(){
	pUpdateBillboard();
}

void igdeWOSOBillboard::UpdateGeometry(){
	if( ! pBillboard ){
		return;
	}
	
	if( pGDBillboard.GetDoNotScale() ){
		pBillboard->SetSize( pGDBillboard.GetSize() );
		pBillboard->SetOffset( pBaseOffset );
		
	}else{
		const decVector &scaling = GetWrapper().GetScaling();
		const decVector2 &size = pGDBillboard.GetSize();
		pBillboard->SetSize( decVector2( size.x * scaling.x, size.y * scaling.y ) );
		pBillboard->SetOffset( decVector2( pBaseOffset.x * scaling.x, pBaseOffset.y * scaling.y ) );
	}
}

void igdeWOSOBillboard::UpdateVisibility(){
	if( ! pBillboard ){
		return;
	}
	
	bool visible = GetWrapper().GetVisible();
	const bool partiallyVisible = visible && ! GetWrapper().GetPartiallyHidden();
	
	if( pGDBillboard.GetPartialHide() ){
		visible = partiallyVisible;
	}
	
	pBillboard->SetVisible( visible );
}

void igdeWOSOBillboard::UpdateLayerMasks(){
	if( ! pBillboard ){
		return;
	}
	
	int mask = GetWrapper().GetRenderLayerMask();
	if( pRenderEnvMap ){
		mask |= GetWrapper().GetRenderEnvMapMask();
	}
	
	pBillboard->SetLayerMask( LayerMaskFromInt( mask ) );
}

void igdeWOSOBillboard::CameraChanged(){
}

void igdeWOSOBillboard::Update( float ){
	pUpdateExtends();
}

void igdeWOSOBillboard::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitBillboard( *this );
}

void igdeWOSOBillboard::AsyncLoadFinished( bool success ){
	if( ! pResLoad ){
		return;
	}
	
	( ( igdeWOSOBillboardResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
	
// 	GetWrapper().GetEnvironment().GetLogger()->LogInfoFormat( "DEIGDE",
// 		"igdeWOSOBillboard.AsyncLoadFinished: %p %d", &GetWrapper(), success );
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOBillboard::pLoadResources(){
	if( pResLoad ){
		( ( igdeWOSOBillboardResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	
	pResLoad.TakeOver( new igdeWOSOBillboardResLoadComponent( *this ) );
	igdeWOSOBillboardResLoadComponent &rl =
		( igdeWOSOBillboardResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	const decString pathSkin( GetStringProperty(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epSkin ), pGDBillboard.GetSkinPath() ) );
	if( ! pathSkin.IsEmpty() ){
		rl.LoadSkin( pathSkin );
	}
	
	rl.CheckFinished();
}

void igdeWOSOBillboard::pUpdateBillboard(){
	const igdeWOSOBillboardResLoadComponent &rl =
		( igdeWOSOBillboardResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	deSkin *skin = rl.GetSkin();
	if( ! skin && GetWrapper().GetGDClass() ){
		skin = GetEnvironment().GetErrorSkin();
	}
	
	if( ! skin ){
		pResLoad = NULL;
		pDestroyBillboard();
		return;
	}
	
	if( ! pBillboard ){
		pBillboard.TakeOver( GetEngine().GetBillboardManager()->CreateBillboard() );
		
		pBillboard->SetSizeFixedToScreen( pGDBillboard.GetSizeFixedToScreen() );
		
		UpdateLayerMasks();
		UpdateVisibility();
	}
	
	pBillboard->SetSkin( skin );
	
	pBillboard->SetAxis( GetVectorProperty(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epAxis ),
		pGDBillboard.GetAxis() ) );
	pBaseOffset = GetVector2Property(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epOffset ),
		pGDBillboard.GetOffset() );
	pBillboard->SetLocked( GetBoolProperty(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epLocked ),
		pGDBillboard.GetLocked() ) );
	pBillboard->SetSpherical( GetBoolProperty(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epSpherical ),
		pGDBillboard.GetSpherical() ) );
	
	pRenderEnvMap = GetBoolProperty(
		pGDBillboard.GetPropertyName( igdeGDCBillboard::epRenderEnvMap ),
		pGDBillboard.GetRenderEnvMap() );
	
	UpdateGeometry();
	pUpdateExtends();
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddBillboard( pBillboard );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
	
	pResLoad = NULL;
}

void igdeWOSOBillboard::pDestroyBillboard(){
	if( ! pBillboard ){
		return;
	}
	
	DetachFromCollider();
	ClearBoxExtends();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveBillboard( pBillboard );
	}
	
	pBillboard = NULL;
	pAddedToWorld = false;
}

void igdeWOSOBillboard::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pBillboard ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pBillboard );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDBillboard.GetPropertyName( igdeGDCBillboard::epAttachPosition ),
			pGDBillboard.GetPosition() ) );
		attachment->SetNoScaling( true );
		
		if( colliderComponent ){
			if( ! pGDBillboard.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDBillboard.GetBoneName() );
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

void igdeWOSOBillboard::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}

void igdeWOSOBillboard::pUpdateExtends(){
	if( ! pBillboard || pGDBillboard.GetDoNotScale() ){
		ClearBoxExtends();
		return;
	}
	
	decVector2 boxMinExtend( pBaseOffset - pGDBillboard.GetSize() * 0.5f );
	decVector2 boxMaxExtend( pBaseOffset + pGDBillboard.GetSize() * 0.5f );
	
	if( pGDBillboard.GetSizeFixedToScreen() && GetWrapper().GetCamera() ){
		const deCamera &camera = *GetWrapper().GetCamera();
		const float distance = ( float )( pBillboard->GetPosition() - camera.GetPosition() ).Length();
		const float scale = distance * tanf( camera.GetFov() * 0.5f );
		boxMinExtend *= scale;
		boxMaxExtend *= scale;
	}
	
	SetBoxExtends( decVector( boxMinExtend.x, boxMinExtend.y, boxMinExtend.x ),
		decVector( boxMaxExtend.x, boxMaxExtend.y, boxMaxExtend.x ) );
}
