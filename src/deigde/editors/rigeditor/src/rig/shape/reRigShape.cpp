/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShape.h"
#include "reSelectionShapes.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>



// Class reRigShape
/////////////////////

// Constructor, destructor
////////////////////////////

reRigShape::reRigShape( deEngine *engine, reRigShape::eShapeTypes shapeType ){
	if( ! engine || shapeType < estSphere || shapeType > estCapsule ){
		DETHROW( deeInvalidParam );
	}
	
	pEngine = engine;
	pRig = NULL;
	pRigBone = NULL;
	
	pDebugDrawer = NULL;
	pDDSShape = NULL;
	pCollider = NULL;
	
	pShapeType = shapeType;
	
	pSelected = false;
	pActive = false;
	
	pDirtyShape = true;
	
	try{
		
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled( true );
		pCollider->SetResponseType( deCollider::ertKinematic );
		pCollider->SetUseLocalGravity( true );
		
		decLayerMask layerMask;
		layerMask.SetBit( reRig::eclmShapes );
		
		pCollider->SetCollisionFilter( decCollisionFilter( layerMask ) );
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSShape = new igdeWDebugDrawerShape;
		pUpdateDDSColor();
		pDDSShape->SetParentDebugDrawer( pDebugDrawer );
		
	}catch( const deException & ){
		pRSCleanUp();
		throw;
	}
}

reRigShape::~reRigShape(){
	pRSCleanUp();
}



// Management
///////////////

void reRigShape::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	if( pRig ){
		pRig->GetEngineWorld()->RemoveCollider( pCollider );
		pRig->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
	}
	
	pRig = rig;
	
	if( rig ){
		rig->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
		rig->GetEngineWorld()->AddCollider( pCollider );
	}
	
	ShowStateChanged();
	pUpdateShapes();
}

void reRigShape::SetRigBone( reRigBone *rigBone ){
	if( rigBone == pRigBone ){
		return;
	}
	
	pRigBone = rigBone;
	
	ShowStateChanged();
}

void reRigShape::SetPosition( const decVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	NotifyShapeChanged();
}

void reRigShape::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	NotifyShapeChanged();
}

void reRigShape::SetProperty( const char *property ){
	if( pProperty == property ){
		return;
	}
	
	pProperty = property;
	
	NotifyShapeChanged();
}

void reRigShape::SetSelected( bool selected ){
	if( selected == pSelected ){
		return;
	}
	
	pSelected = selected;
	
	pUpdateDDSColor();
}

void reRigShape::SetActive( bool active ){
	if( active == pActive ){
		return;
	}
	
	pActive = active;
	
	pUpdateDDSColor();
}



void reRigShape::ShowStateChanged(){
	const bool visible = IsVisible();
	
	if( pRig ){
		pDebugDrawer->SetXRay( pRig->GetShapeXRay() );
	}
	
	pDebugDrawer->SetVisible( visible );
	pCollider->SetEnabled( visible );
}

void reRigShape::Update(){
	pUpdateShapes();
	pRepositionShapes();
}

void reRigShape::NotifyShapeChanged(){
	if( pRig ){
		pRig->NotifyAllShapeChanged( this );
	}
	
	InvalidateShape();
	InvalidatePosition();
}

void reRigShape::InvalidateShape(){
	pDirtyShape = true;
}

void reRigShape::InvalidatePosition(){
	pDirtyShape = true;
}



bool reRigShape::IsVisible() const{
	if( pRig ){
		if( pRigBone ){
			return pRig->GetShowAllBoneShapes() || pRigBone->GetSelected();
		}
		
		return pRig->GetShowRigShapes();
	}
	
	return false;
}



// Private Functions
//////////////////////

void reRigShape::pRSCleanUp(){
	SetRigBone( NULL );
	SetRig( NULL );
	
	if( pCollider ){
		pCollider->FreeReference();
	}
	
	if( pDDSShape ){
		delete pDDSShape;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}



void reRigShape::pRepositionShapes(){
	decDMatrix matrix;
	
	if( pRigBone ){
		matrix = pRigBone->GetPoseMatrix();
		
	}else if( pRig ){
		matrix = pRig->GetPoseMatrix();
	}
	
	const decDVector position = matrix.GetPosition();
	const decQuaternion orientation = matrix.ToQuaternion();
	
	pDebugDrawer->SetPosition( position );
	pDebugDrawer->SetOrientation( orientation );
	
	pCollider->SetPosition( position );
	pCollider->SetOrientation( orientation );
}

void reRigShape::pUpdateDDSColor(){
	if( pActive ){
		pDDSShape->SetEdgeColor( decColor( 1.0f, 0.5f, 0.0f, 1.0 ) );
		pDDSShape->SetFillColor( decColor( 1.0f, 0.5f, 0.0f, 0.1 ) );
		
	}else if( pSelected ){
		pDDSShape->SetEdgeColor( decColor( 0.75f, 0.75f, 0.0f, 1.0 ) );
		pDDSShape->SetFillColor( decColor( 0.75f, 0.75f, 0.0f, 0.1 ) );
		
	}else{
		pDDSShape->SetEdgeColor( decColor( 0.5f, 0.5f, 0.0f, 1.0f ) );
		pDDSShape->SetFillColor( decColor( 0.5f, 0.5f, 0.0f, 0.1f ) );
	}
}

void reRigShape::pUpdateShapes(){
	if( ! pDirtyShape ){
		return;
	}
	
	// debug drawer shape
	decShape *shape = NULL;
	pDDSShape->RemoveAllShapes();
	
	try{
		shape = CreateShape();
		pDDSShape->AddShape( shape );
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
	
	// collider
	shape = NULL;
	decShapeList shapeList;
	
	try{
		shape = CreateShape();
		shapeList.Add( shape );
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
	
	pCollider->SetShapes( shapeList );
	
	// no more dirty
	pDirtyShape = false;
}
