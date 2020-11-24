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

#include "igdeWOSONavigationSpace.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../codec/igdeCodecPropertyString.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/navspace/igdeGDCNavigationSpace.h"
#include "../../../loadsave/igdeLoadSaveNavSpace.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWOSONavigationSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSONavigationSpace::igdeWOSONavigationSpace( igdeWObject &wrapper,
	const igdeGDCNavigationSpace &gdNavigationSpace, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDNavigationSpace( gdNavigationSpace ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	wrapper.SubObjectFinishedLoading( *this, true );
}

igdeWOSONavigationSpace::~igdeWOSONavigationSpace(){
	pDestroyNavigationSpace();
}



// Management
///////////////

void igdeWOSONavigationSpace::UpdateParameters(){
	GetWrapper().SubObjectFinishedLoading( *this, true );
}

void igdeWOSONavigationSpace::OnAllSubObjectsFinishedLoading(){
	pUpdateNavigationSpace();
}

void igdeWOSONavigationSpace::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitNavigationSpace( *this );
}



// Private Functions
//////////////////////

void igdeWOSONavigationSpace::pUpdateNavigationSpace(){
	if( ! pNavigationSpace ){
		pNavigationSpace.TakeOver( GetEngine().GetNavigationSpaceManager()->CreateNavigationSpace() );
		pNavigationSpace->SetType( pGDNavigationSpace.GetType() );
	}
	
	decString pathNavSpace( GetStringProperty(
		pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epPath ),
		pGDNavigationSpace.GetPath() ) );
		
	if( pathNavSpace != pPathNavigationSpace ){
		pNavigationSpace->SetRoomCount( 0 );
		pNavigationSpace->SetWallCount( 0 );
		pNavigationSpace->SetFaceCount( 0 );
		pNavigationSpace->SetCornerCount( 0 );
		pNavigationSpace->SetEdgeCount( 0 );
		pNavigationSpace->SetVertexCount( 0 );
		pNavigationSpace->SetType( pGDNavigationSpace.GetType() );
		
		if( ! pathNavSpace.IsEmpty() ){
			igdeLoadSaveNavSpace loadNavSpace( &GetEnvironment(), "DEIGDE" );
			const decPath vfsPath( decPath::CreatePathUnix( pathNavSpace ) );
			deEngine &engine = GetEngine();
			
			if( engine.GetVirtualFileSystem()->ExistsFile( vfsPath ) ){
				try{
					decBaseFileReaderReference reader;
					reader.TakeOver( engine.GetVirtualFileSystem()->OpenFileForReading( vfsPath ) );
					loadNavSpace.Load( pNavigationSpace, reader );
					
				}catch( const deException &e ){
					pNavigationSpace->SetRoomCount( 0 );
					pNavigationSpace->SetWallCount( 0 );
					pNavigationSpace->SetFaceCount( 0 );
					pNavigationSpace->SetCornerCount( 0 );
					pNavigationSpace->SetEdgeCount( 0 );
					pNavigationSpace->SetVertexCount( 0 );
					pNavigationSpace->SetType( pGDNavigationSpace.GetType() );
					GetLogger().LogException( "DEIGDE", e );
				}
			}
		}
		
		pNavigationSpace->NotifyLayoutChanged();
		pPathNavigationSpace = pathNavSpace;
	}
	
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decString value;
	
	pNavigationSpace->SetLayer( GetIntProperty(
		pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epLayer ),
		pGDNavigationSpace.GetLayer() ) );
	pNavigationSpace->SetBlockingPriority( GetIntProperty(
		pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epBlockingPriority ),
		pGDNavigationSpace.GetBlockingPriority() ) );
	pNavigationSpace->SetSnapDistance( GetFloatProperty(
		pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epSnapDistance ),
		pGDNavigationSpace.GetSnapDistance() ) );
	pNavigationSpace->SetSnapAngle( GetFloatProperty(
		pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epSnapAngle ),
		pGDNavigationSpace.GetSnapAngle() ) * DEG2RAD );
	
	// shape property
	if( GetPropertyValue( pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epBlockerShape ), value ) ){
		codec.DecodeShapeList( value, pNavigationSpace->GetBlockerShapeList() );
		
	}else{
		pNavigationSpace->GetBlockerShapeList() = pGDNavigationSpace.GetBlockerShapeList();
	}
	pNavigationSpace->NotifyBlockerShapeListChanged();
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddNavigationSpace( pNavigationSpace );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
}

void igdeWOSONavigationSpace::pDestroyNavigationSpace(){
	if( ! pNavigationSpace ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveNavigationSpace( pNavigationSpace );
	}
	
	pNavigationSpace = NULL;
	pPathNavigationSpace.Empty();
	pAddedToWorld = false;
}

void igdeWOSONavigationSpace::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pNavigationSpace ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pNavigationSpace );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epAttachPosition ),
			pGDNavigationSpace.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDNavigationSpace.GetPropertyName( igdeGDCNavigationSpace::epAttachRotation ),
			pGDNavigationSpace.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDNavigationSpace.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDNavigationSpace.GetBoneName() );
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

void igdeWOSONavigationSpace::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
