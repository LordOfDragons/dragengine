/* 
 * Drag[en]gine IGDE World Editor
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

#include "meCLSnapPoint.h"
#include "../world/meColliderOwner.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSnapPoint.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/snappoint/igdeGDCSnapPoint.h>
#include <deigde/gamedefinition/class/snappoint/igdeGDCSnapPointList.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class meCLSnapPoint
////////////////////////

// Constructor, destructor
////////////////////////////

meCLSnapPoint::meCLSnapPoint( meWorld &world, meObject *object ) :
pWorld( world ),
pObject( object ),
pDistance( 0.0f ),
pSourceSnapPoint( NULL ),
pTargetObject( NULL ),
pTargetSnapPoint( NULL ){
}

meCLSnapPoint::~meCLSnapPoint(){
}



// Management
///////////////

bool meCLSnapPoint::IsSnappingEnabled() const{
	return pWorld.GetGuiParameters().GetSnapToSnapPoints();
}

void meCLSnapPoint::CalcBoundingBoxShape( decShapeList &shape ){
	decVector boxMin, boxMax;
	
	if( pObject->GetGDClass() ){
		const igdeGDCSnapPointList &list = pObject->GetGDClass()->GetSnapPointList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			const decVector &position = list.GetAt( i )->GetPosition();
			boxMin.SetSmallest( position );
			boxMax.SetLargest( position );
		}
	}
	
	shape.RemoveAll();
	
	decShapeBox *shapeBox = NULL;
	
	try{
		shapeBox = new decShapeBox(
			( boxMax - boxMin ) * 0.5f + decVector( 0.01f, 0.01f, 0.01f ),
			( boxMax + boxMin ) * 0.5f );
		shape.Add( shapeBox );
		
	}catch( const deException & ){
		if( shapeBox ){
			delete shapeBox;
		}
		throw;
	}
}



void meCLSnapPoint::AddIgnoreObject( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	pIgnoreObjects.AddIfAbsent( object );
}

void meCLSnapPoint::Reset(){
	pDistance = 0.0f;
	pSourceSnapPoint = NULL;
	pTargetObject = NULL;
	pTargetSnapPoint = NULL;
}



decDMatrix meCLSnapPoint::CalcCorrectionMatrix() const{
	if( ! pTargetSnapPoint || ! pTargetObject ){
		return decDMatrix();
	}
	
	const decDMatrix matrixTarget(
		decDMatrix::CreateRT(
			decDVector( pTargetSnapPoint->GetRotation() * DEG2RAD ),
			decDVector( pTargetSnapPoint->GetPosition() ) )
		* pTargetObject->GetObjectMatrix() );
	
	decDMatrix matrixSource( pObject->GetObjectMatrix() );
	if( pSourceSnapPoint ){
		matrixSource = decDMatrix::CreateRT(
			decDVector( pSourceSnapPoint->GetRotation() * DEG2RAD ),
			decDVector( pSourceSnapPoint->GetPosition() ) )
				* matrixSource;
	}
	
	const decDVector targetPosition( matrixTarget.GetPosition() );
	const decDVector sourcePosition( matrixSource.GetPosition() );
	
	if( pTargetSnapPoint->GetSnapToRotation() ){
		return decDMatrix::CreateTranslation( -sourcePosition )
			* decDMatrix::CreateFromQuaternion(
				matrixTarget.ToQuaternion() * matrixSource.ToQuaternion().Conjugate() )
			* decDMatrix::CreateTranslation( targetPosition );
		
	}else{
		return decDMatrix::CreateTranslation( targetPosition - sourcePosition );
	}
}



// Notifications
//////////////////

void meCLSnapPoint::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! info->IsCollider() ){
		return;
	}
	
	const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
		*pWorld.GetEnvironment(), info->GetCollider() );
	if( ! colliderOwner ){
		return;
	}
	
	meObjectSnapPoint * const snapPoint = colliderOwner->GetSnapPoint();
	if( ! snapPoint ){
		return;
	}
	
	meObject * const object = snapPoint->GetObject();
	if( object == pObject ){
		return;
	}
	if( pIgnoreObjects.Has( object ) ){
		return;
	}
	
	// world position of target snap point
	const igdeGDCSnapPoint * const gdcSnapPoint = snapPoint->GetSnapPoint();
	const decDVector targetPosition( object->GetObjectMatrix()
		* decDVector( gdcSnapPoint->GetPosition() ) );
	const float snapDistance = gdcSnapPoint->GetSnapDistance();
	
	// find closest snap point if object has snap points
	if( pObject->GetGDClass() ){
		const igdeGDCSnapPointList &list = pObject->GetGDClass()->GetSnapPointList();
		const decDMatrix matrix( pObject->GetObjectMatrix() );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			igdeGDCSnapPoint * const snappoint = list.GetAt( i );
			const decDVector sourcePosition( matrix * decDVector( snappoint->GetPosition() ) );
			const float distance = ( targetPosition - sourcePosition ).Length();
			
			if( distance > snapDistance ){
				continue;
			}
			if( pTargetSnapPoint && distance >= pDistance ){
				continue;
			}
			
			pSourceSnapPoint = snappoint;
			pTargetObject = object;
			pTargetSnapPoint = gdcSnapPoint;
			pDistance = distance;
		}
	}
	
	// use object position as snap point
	const float distance = ( targetPosition - pObject->GetPosition() ).Length();
	
	if( distance <= snapDistance ){
		if( ! pTargetSnapPoint || distance < pDistance ){
			pSourceSnapPoint = NULL;
			pTargetObject = object;
			pTargetSnapPoint = gdcSnapPoint;
			pDistance = distance;
		}
	}
}

bool meCLSnapPoint::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void meCLSnapPoint::ColliderChanged( deCollider *owner ){
}
