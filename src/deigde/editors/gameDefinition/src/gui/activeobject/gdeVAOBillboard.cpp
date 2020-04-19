/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeVAOBillboard.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../gdEditor.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/billboard/deBillboardManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOBillboard::gdeVAOBillboard( gdeViewActiveObject &view, gdeOCBillboard *ocbillboard ) :
pView( view ),
pOCBillboard( ocbillboard )
{
	if( ! ocbillboard ){
		DETHROW( deeInvalidParam );
	}
	ocbillboard->AddReference();
	
	pCreateBillboard();
	pCreateCollider();
	pAttachBillboard();
}

gdeVAOBillboard::~gdeVAOBillboard(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOBillboard::RebuildResources(){
	const deBillboardReference oldBillboard( pBillboard );
	
	pReleaseResources();
	
	pCreateBillboard();
	pCreateCollider();
	pAttachBillboard();
}

void gdeVAOBillboard::SelectedObjectChanged(){
}

void gdeVAOBillboard::GetExtends( decVector &minExtend, decVector &maxExtend ) const{
	minExtend.SetZero();
	maxExtend.SetZero();
	if( ! pBillboard ){
		return;
	}
	
	const decDVector &position = pOCBillboard->GetPosition();
	const decVector2 &size = pOCBillboard->GetSize();
	const float width = size.x * 0.5f;
	const float height = size.y * 0.5f;
	
	if( width > height ){
		minExtend.x = position.x - width;
		minExtend.y = position.y - width;
		minExtend.z = position.z - width;
		maxExtend.x = position.x + width;
		maxExtend.y = position.y + width;
		maxExtend.z = position.z + width;
		
	}else{
		minExtend.x = position.x - height;
		minExtend.y = position.y - height;
		minExtend.z = position.z - height;
		maxExtend.x = position.x + height;
		maxExtend.y = position.y + height;
		maxExtend.z = position.z + height;
	}
}



// Private functions
//////////////////////

void gdeVAOBillboard::pCleanUp(){
	pReleaseResources();
	
	if( pOCBillboard ){
		pOCBillboard->FreeReference();
	}
}

void gdeVAOBillboard::pCreateBillboard(){
	// load the new resources. if the resource is already in use it is not loaded again just the
	// reference count increased by one. loading of individual resources is allowed to fail. in
	// this case the exception is logged and the resource simply not loaded or replaced with a
	// placeholder. this behavior can be changed later on
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	deSkinReference skin;
	if( ! pOCBillboard->GetSkinPath().IsEmpty() ){
		try{
			skin.TakeOver( engine.GetSkinManager()->LoadSkin( vfs, pOCBillboard->GetSkinPath(), "/" ) );
			
		}catch( const deException &e ){
			skin = environment.GetErrorSkin();
		}
	}
	
	// create billboard if skin is present
	if( skin ){
		pBillboard.TakeOver( engine.GetBillboardManager()->CreateBillboard() );
		pBillboard->SetSkin( skin );
		pBillboard->SetAxis( pOCBillboard->GetAxis() );
		pBillboard->SetSize( pOCBillboard->GetSize() );
		pBillboard->SetOffset( pOCBillboard->GetOffset() );
		pBillboard->SetLocked( pOCBillboard->GetLocked() );
		pBillboard->SetSpherical( pOCBillboard->GetSpherical() );
		pBillboard->SetSizeFixedToScreen( pOCBillboard->GetSizeFixedToScreen() );
		pView.GetGameDefinition()->GetWorld()->AddBillboard( pBillboard );
	}
}

void gdeVAOBillboard::pCreateCollider(){
	decShapeList shapeList;
	if( pBillboard ){
		shapeList.Add( new decShapeBox( decVector(
			pOCBillboard->GetSize().x * 0.5f, pOCBillboard->GetSize().y * 0.5f, 0.02f ) ) );
		
	}else{
		shapeList.Add( new decShapeBox( decVector( 0.1f, 0.1f, 0.1f ) ) );
	}
	
	pCollider.TakeOver( pView.GetGameDefinition()->GetEngine()->GetColliderManager()->CreateColliderVolume() );
	pCollider->SetEnabled( true );
	pCollider->SetResponseType( deCollider::ertKinematic );
	pCollider->SetUseLocalGravity( true );
	( ( deColliderVolume& )( deCollider& )pCollider ).SetShapes( shapeList );
	
	decLayerMask collisionMask;
	collisionMask.SetBit( 0 );
	pCollider->SetCollisionFilter( decCollisionFilter( collisionMask ) );
	
	pView.GetGameDefinition()->GetWorld()->AddCollider( pCollider );
}

void gdeVAOBillboard::pAttachBillboard(){
	if( ! pBillboard || ! pCollider ){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pCollider );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		pCollider->AddAttachment( attachment );
		attachment = NULL;
		
	}catch( const deException &e ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

void gdeVAOBillboard::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if( pCollider ){
		pCollider->RemoveAllAttachments(); // because otherwise cyclic loop with component
		world.RemoveCollider( pCollider );
		pCollider = NULL;
	}
	
	if( pBillboard ){
		world.RemoveBillboard( pBillboard );
		pBillboard = NULL;
	}
}
