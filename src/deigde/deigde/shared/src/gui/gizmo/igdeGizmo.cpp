/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "igdeGizmo.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/rig/deRig.h>



// Class igdeGizmo
////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmo::igdeGizmo( igdeEnvironment &environment ) :
pEnvironment( environment ),
pEdgeTransparency( 0.0f ),
pFillTransparency( 0.6f ),
pIsEditing( false )
{
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	pDebugDrawer.TakeOver( engine.GetDebugDrawerManager()->CreateDebugDrawer() );
	
	pCollider.TakeOver( engine.GetColliderManager()->CreateColliderVolume() );
	pCollider->SetUseLocalGravity( true );
	pCollider->SetResponseType( deCollider::ertKinematic );
	
	decLayerMask cfcategory, cffilter;
	
	deColliderAttachment *attachment = nullptr;
	try{
		attachment = new deColliderAttachment( pDebugDrawer );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		pCollider->AddAttachment( attachment );
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

igdeGizmo::~igdeGizmo(){
	if( pCollider ){
		pEnvironment.SetColliderUserPointer( pCollider, nullptr );
	}
	
	SetWorld( nullptr );
}



// Management
///////////////

void igdeGizmo::SetWorld( deWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->RemoveCollider( pCollider );
		pWorld->RemoveDebugDrawer( pDebugDrawer );
		OnRemoveFromWorld();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddDebugDrawer( pDebugDrawer );
		world->AddCollider( pCollider );
		OnAddToWorld();
	}
}

void igdeGizmo::SetShapeFromModel( const deModel &model ){
	pShapeNames.RemoveAll();
	pDebugDrawer->RemoveAllShapes();
	
	const deModelLOD &lod = *model.GetLODAt( 0 );
	const deModelVertex * const vertices = lod.GetVertices();
	const deModelFace * const faces = lod.GetFaces();
	const int textureCount = model.GetTextureCount();
	deDebugDrawerShapeFace *ddsface = nullptr;
	const int faceCount = lod.GetFaceCount();
	deDebugDrawerShape *ddshape = nullptr;
	int i;
	
	try{
		for( i=0; i<textureCount; i++ ){
			ddshape = new deDebugDrawerShape;
			pDebugDrawer->AddShape( ddshape );
			ddshape = nullptr;
			
			pShapeNames.Add( model.GetTextureAt( i )->GetName() );
		}
		
	}catch( const deException & ){
		if( ddshape ){
			delete ddshape;
		}
		pDebugDrawer->NotifyShapeLayoutChanged();
		throw;
	}
	
	try{
		for( i=0; i<faceCount; i++ ){
			const deModelFace &face = faces[ i ];
			ddsface = new deDebugDrawerShapeFace;
			ddsface->AddVertex( vertices[ face.GetVertex1() ].GetPosition() );
			ddsface->AddVertex( vertices[ face.GetVertex2() ].GetPosition() );
			ddsface->AddVertex( vertices[ face.GetVertex3() ].GetPosition() );
			ddsface->CalculateNormal();
			pDebugDrawer->GetShapeAt( face.GetTexture() )->AddFace( ddsface );
			ddsface = nullptr;
		}
		
	}catch( const deException & ){
		if( ddsface ){
			delete ddsface;
		}
		pDebugDrawer->NotifyShapeLayoutChanged();
		throw;
	}
	
	pDebugDrawer->NotifyShapeLayoutChanged();
}

decString igdeGizmo::GetRigShapeName( int rigShapeIndex ) const{
	if( rigShapeIndex >= 0 && rigShapeIndex <= pRigShapeNames.GetCount() ){
		return pRigShapeNames.GetAt( rigShapeIndex );
		
	}else{
		return decString();
	}
}

decColor igdeGizmo::GetShapeColor( const char *name ) const{
	decColor color( pDebugDrawer->GetShapeAt( pShapeNames.IndexOf( name ) )->GetFillColor() );
	color.a = 1.0f;
	return color;
}

void igdeGizmo::SetShapeColor( const char *name, const decColor &color ){
	const int index = pShapeNames.IndexOf( name );
	if( index == -1 ){
		return;
	}
	
	deDebugDrawerShape &shape = *pDebugDrawer->GetShapeAt( index );
	shape.SetEdgeColor( decColor( color, pEdgeTransparency ) );
	shape.SetFillColor( decColor( color, pFillTransparency ) );
	pDebugDrawer->NotifyShapeColorChanged();
}

void igdeGizmo::SetRig( deRig *rig ){
	if( rig ){
		pCollider->SetShapes( rig->GetShapes() );
		pRigShapeNames = rig->GetShapeProperties();
		
	}else{
		pCollider->SetShapes( decShapeList() );
		pRigShapeNames.RemoveAll();
	}
}

const decCollisionFilter &igdeGizmo::GetCollisionFilter() const{
	return pCollider->GetCollisionFilter();
}

void igdeGizmo::SetCollisionFilter( const decCollisionFilter &filter ){
	pCollider->SetCollisionFilter( filter );
}

void igdeGizmo::SetColliderUserPointer( void *userPointer ){
	pEnvironment.SetColliderUserPointer( pCollider, userPointer );
}

bool igdeGizmo::GetVisible() const{
	return pDebugDrawer->GetVisible();
}

void igdeGizmo::SetVisible( bool visible ){
	pCollider->SetEnabled( visible );
	pDebugDrawer->SetVisible( visible );
}

const decDVector &igdeGizmo::GetPosition() const{
	return pCollider->GetPosition();
}

void igdeGizmo::SetPosition( const decDVector &position ){
	pCollider->SetPosition( position );
}

const decQuaternion &igdeGizmo::GetOrientation() const{
	return pCollider->GetOrientation();
}

void igdeGizmo::SetOrientation( const decQuaternion &orientation ){
	pCollider->SetOrientation( orientation );
}

const decVector &igdeGizmo::GetScale() const{
	return pCollider->GetScale();
}

void igdeGizmo::SetScale( const decVector &scale ){
	pCollider->SetScale( scale );
}

void igdeGizmo::SetGeometry( const decDVector &position, const decQuaternion &orientation ){
	pCollider->SetGeometry( position, orientation );
}

void igdeGizmo::SetGeometry( const decDMatrix &matrix ){
	pCollider->SetGeometry( matrix.GetPosition(), matrix.Normalized().ToQuaternion() );
}

decDMatrix igdeGizmo::GetMatrix() const{
	return decDMatrix::CreateWorld( pCollider->GetPosition(), pCollider->GetOrientation() );
}



// Interaction
////////////////

bool igdeGizmo::StartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, double distance, int shape ){
	if( shape == -1 ){
		return false;
	}
	
	const decString name( GetRigShapeName( shape ) );
	if( name.IsEmpty() ){
		return false;
	}
	
	const decDVector hitPoint( rayOrigin + rayDirection * distance );
	
	if( ! OnStartEditing( rayOrigin, rayDirection, viewMatrix, hitPoint, name ) ){
		return false;
	}
	
	pIsEditing = true;
	return true;
}

void igdeGizmo::UpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix ){
	DEASSERT_TRUE( pIsEditing )
	OnUpdateEditing( rayOrigin, rayDirection, viewMatrix );
}

void igdeGizmo::OnFrameUpdate( float elapsed ){
	DEASSERT_TRUE( pIsEditing )
	OnFrameUpdateEditing( elapsed );
}

void igdeGizmo::StopEditing( bool cancel ){
	DEASSERT_TRUE( pIsEditing )
	OnStopEditing( cancel );
	pIsEditing = false;
}



// Protected Functions
////////////////////////

void igdeGizmo::OnUpdateEditing( const decDVector &, const decDVector &, const decDMatrix & ){
}

void igdeGizmo::OnFrameUpdateEditing( float ){
}

void igdeGizmo::OnStopEditing( bool ){
}

void igdeGizmo::OnAddToWorld(){
}

void igdeGizmo::OnRemoveFromWorld(){
}
