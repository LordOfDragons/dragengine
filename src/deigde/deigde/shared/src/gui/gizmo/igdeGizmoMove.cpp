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

#include "igdeGizmoMove.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class igdeGizmoMove
////////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmoMove::igdeGizmoMove( igdeEnvironment &environment ) :
igdeGizmo( environment ),
pAction( eaNone )
{
	SetShapeFromModel( environment.GetStockModel( igdeEnvironment::esmGizmoMove ) );
	SetRig( environment.GetStockRig( igdeEnvironment::esrGizmoMove ) );
}

igdeGizmoMove::~igdeGizmoMove(){
}



// Management
///////////////

decDMatrix igdeGizmoMove::GetObjectMatrix(){
	return decDMatrix::CreateWorld( GetObjectPosition(),
		GetObjectOrientation(), decDVector( GetObjectScale() ) );
}

decDVector igdeGizmoMove::LimitPosition( const decDVector &position ){
	return position;
}



// Protected Functions
////////////////////////

igdeGizmoMove::eActions igdeGizmoMove::pShapeNameToAction( const decString &shapeName ) const{
	if( shapeName == "move x" ){
		return eaMoveX;
		
	}else if( shapeName == "move y" ){
		return eaMoveY;
		
	}else if( shapeName == "move z" ){
		return eaMoveZ;
		
	}else if( shapeName == "move xy" ){
		return eaMoveXY;
		
	}else if( shapeName == "move yz" ){
		return eaMoveYZ;
		
	}else if( shapeName == "move xz" ){
		return eaMoveXZ;
		
	}else if( shapeName == "move view" ){
		return eaMoveView;
		
	}else{
		return eaNone;
	}
}

bool igdeGizmoMove::OnStartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, const decDVector &hitPoint, const decString &shapeName ){
	const eActions action = pShapeNameToAction( shapeName );
	
	const decDMatrix objectMatrix( decDMatrix::CreateFromQuaternion( GetObjectOrientation() ) );
	pViewMatrix = viewMatrix;
	pInteractOrigin = hitPoint;
	pMoveOrigin = GetObjectPosition();
	pPlanePosition = pInteractOrigin;
	
	switch( action ){
	case eaMoveX:
	case eaMoveY:
	case eaMoveZ:
		{
		switch( action ){
		case eaMoveX:
			pInteractAxis = objectMatrix.TransformRight();
			break;
			
		case eaMoveY:
			pInteractAxis = objectMatrix.TransformUp();
			break;
			
		case eaMoveZ:
			pInteractAxis = objectMatrix.TransformView();
			break;
			
		default:
			return false;
		}
		
		pInteractAxis2.SetZero();
		
		const decDVector up( viewMatrix.TransformUp().Normalized() );
		const decDVector right( viewMatrix.TransformRight().Normalized() );
		
		if( fabs( up * pInteractAxis ) < 0.707 ){ // roughly 45 degrees
			pPlaneNormal = ( up % pInteractAxis ).Normalized();
			
		}else{
			pPlaneNormal = ( pInteractAxis % right ).Normalized();
		}
		} return true;
		
	case eaMoveXY:
	case eaMoveYZ:
	case eaMoveXZ:
	case eaMoveView:
		{
		switch( action ){
		case eaMoveXY:
			pInteractAxis = objectMatrix.TransformRight();
			pInteractAxis2 = objectMatrix.TransformUp();
			break;
			
		case eaMoveYZ:
			pInteractAxis = objectMatrix.TransformView();
			pInteractAxis2 = objectMatrix.TransformUp();
			break;
			
		case eaMoveXZ:
			pInteractAxis = objectMatrix.TransformRight();
			pInteractAxis2 = objectMatrix.TransformView();
			break;
			
		case eaMoveView:
			pInteractAxis = viewMatrix.TransformRight().Normalized();
			pInteractAxis2 = viewMatrix.TransformUp().Normalized();
			break;
			
		default:
			return false;
		}
		
		pPlaneNormal = ( pInteractAxis2 % pInteractAxis ).Normalized();
		} return true;
		
	case eaNone:
	default:
		return false;
	}
}

void igdeGizmoMove::OnUpdateEditing( const decDVector &rayOrigin,
const decDVector &rayDirection, const decDMatrix & ){
	const double denom = rayDirection * pPlaneNormal;
	if( fabs( denom ) < DOUBLE_SAFE_EPSILON ){
		return;
	}
	
	const double lambda = ( ( pPlanePosition - rayOrigin ) * pPlaneNormal ) / denom;
	const decDVector hitPoint( rayOrigin + rayDirection * lambda );
	const decDVector direction( hitPoint - pInteractOrigin );
	
	decDVector position( pMoveOrigin );
	position += pInteractAxis * ( pInteractAxis * direction );
	position += pInteractAxis2 * ( pInteractAxis2 * direction );
	position = LimitPosition( position );
	
	SetObjectPosition( position );
}

void igdeGizmoMove::OnFrameUpdateEditing( float ){
}

void igdeGizmoMove::OnStopEditing(){
	pAction = eaNone;
}
