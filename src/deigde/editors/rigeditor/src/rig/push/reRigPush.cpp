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

#include "reRigPush.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class reRigPush
////////////////////

// Constructor, destructor
////////////////////////////

reRigPush::reRigPush( deEngine *engine ){
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	pEngine = engine;
	pRig = NULL;
	
	pDebugDrawer = NULL;
	pDDSPush = NULL;
	pCollider = NULL;
	
	pType = reRigPush::eptSimple;
	pImpuls = 1.0f;
	pRayCount = 1;
	pConeAngle = 180.0f;
	
	pSelected = false;
	pActive = false;
	
	try{
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled( true );
		pCollider->SetResponseType( deCollider::ertKinematic );
		pCollider->SetUseLocalGravity( true );
		
		decLayerMask layerMask;
		layerMask.SetBit( reRig::eclmPushes );
		
		pCollider->SetCollisionFilter( decCollisionFilter( layerMask ) );
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSPush = new igdeWDebugDrawerShape;
		pUpdateDDPushGeometry();
		pDDSPush->SetVisible( true );
		pDDSPush->SetParentDebugDrawer( pDebugDrawer );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reRigPush::reRigPush( const reRigPush &push ){
	pEngine = push.pEngine;
	pRig = NULL;
	
	pDebugDrawer = NULL;
	pDDSPush = NULL;
	pCollider = NULL;
	
	pType = push.pType;
	pPosition = push.pPosition;
	pOrientation = push.pOrientation;
	pImpuls = push.pImpuls;
	pRayCount = push.pRayCount;
	pConeAngle = push.pConeAngle;
	
	pSelected = false;
	pActive = false;
	
	try{
		pCollider = pEngine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled( true );
		pCollider->SetResponseType( deCollider::ertKinematic );
		pCollider->SetUseLocalGravity( true );
		
		decLayerMask layerMask;
		layerMask.SetBit( reRig::eclmPushes );
		
		pCollider->SetCollisionFilter( decCollisionFilter( layerMask ) );
		
		// create debug drawer and shapes
		pDebugDrawer = pEngine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSPush = new igdeWDebugDrawerShape;
		pUpdateDDPushGeometry();
		pDDSPush->SetVisible( true );
		pDDSPush->SetParentDebugDrawer( pDebugDrawer );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reRigPush::~reRigPush(){
	pCleanUp();
}



// Management
///////////////

void reRigPush::SetRig( reRig *rig ){
	if( pRig ){
		pRig->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
		pRig->GetEngineWorld()->RemoveCollider( pCollider );
	}
	
	pRig = rig;
	
	if( rig ){
		rig->GetEngineWorld()->AddCollider( pCollider );
		rig->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
	}
	
	pUpdateDDPush();
	pUpdateColliderShape();
}



void reRigPush::SetType( reRigPush::ePushTypes type ){
	if( type == pType ){
		return;
	}
	
	pType = type;
	
	if( pRig ){
		pRig->NotifyPushChanged( this );
	}
}

void reRigPush::SetPosition( const decVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pRig ){
		pRig->NotifyPushChanged( this );
	}
	
	pDebugDrawer->SetPosition( position );
	pUpdateColliderShape();
}

void reRigPush::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pRig ){
		pRig->NotifyPushChanged( this );
	}
	
	pDebugDrawer->SetOrientation( decQuaternion::CreateFromEuler( orientation * DEG2RAD ) );
	pUpdateColliderShape();
}

void reRigPush::SetImpuls( float impuls ){
	if( fabs( impuls - pImpuls ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pImpuls = impuls;
	
	if( pRig ){
		pRig->NotifyPushChanged( this );
	}
}

void reRigPush::SetRayCount( int rayCount ){
	if( rayCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rayCount != pRayCount ){
		pRayCount = rayCount;
		
		if( pRig ){
			pRig->NotifyPushChanged( this );
		}
	}
}

void reRigPush::SetConeAngle( float angle ){
	if( fabsf( angle - pConeAngle ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pConeAngle = angle;
	
	if( pRig ){
		pRig->NotifyPushChanged( this );
	}
}



void reRigPush::SetSelected( bool selected ){
	if( selected == pSelected ){
		return;
	}
	
	pSelected = selected;
	
	pUpdateDDPush();
}

void reRigPush::SetActive( bool active ){
	if( active == pActive ){
		return;
	}
	
	pActive = active;
	
	pUpdateDDPush();
}



void reRigPush::ShowStateChanged(){
	pUpdateDDPush();
	
	pCollider->SetEnabled( IsVisible() );
}



bool reRigPush::IsVisible() const{
	if( pRig ){
		return pRig->GetShowRigPushes();
		
	}else{
		return false;
	}
}



// Private Functions
//////////////////////

void reRigPush::pCleanUp(){
	SetRig( NULL );
	
	if( pCollider ){
		pCollider->FreeReference();
	}
	
	if( pDDSPush ){
		delete pDDSPush;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}

void reRigPush::pUpdateDDPush(){
	pDebugDrawer->SetVisible( IsVisible() );
	
	if( pActive ){
		pDDSPush->SetEdgeColor( decColor( 0.0f, 0.8f, 0.0f, 1.0 ) );
		pDDSPush->SetFillColor( decColor( 0.0f, 0.8f, 0.0f, 0.25 ) );
		
	}else if( pSelected ){
		pDDSPush->SetEdgeColor( decColor( 0.5f, 0.8f, 1.0f, 1.0 ) );
		pDDSPush->SetFillColor( decColor( 0.5f, 0.8f, 1.0f, 0.25 ) );
		
	}else{
		pDDSPush->SetEdgeColor( decColor( 0.0f, 0.5f, 0.0f, 1.0f ) );
		pDDSPush->SetFillColor( decColor( 0.0f, 0.5f, 0.0f, 0.25f ) );
	}
}

void reRigPush::pUpdateDDPushGeometry(){
	igdeShapeBuilder builder;
	
	builder.CreateArrow( *pDDSPush, decVector( 0.0f, 0.0f, -0.5f ), decVector(), 0.05f, 0.08f, 0.01f );
}

void reRigPush::pUpdateColliderShape(){
	if( pCollider && pRig ){
		decMatrix matrix = decMatrix::CreateRotation( pOrientation * DEG2RAD );
		decShapeBox *box = NULL;
		decVector arrowStart;
		decShapeList shapeList;
		
		arrowStart = pPosition - matrix.TransformView() * 0.5f;
		
		pCollider->SetPosition( ( arrowStart + pPosition ) * 0.5f );
		pCollider->SetOrientation( matrix.ToQuaternion() );
		pCollider->SetEnabled( IsVisible() );
		
		try{
			box = new decShapeBox( decVector( 0.05f, 0.05f, 0.5f ) );
			shapeList.Add( box );
			
		}catch( const deException & ){
			if( box ){
				delete box;
			}
		}
		
		pCollider->SetShapes( shapeList );
	}
}
