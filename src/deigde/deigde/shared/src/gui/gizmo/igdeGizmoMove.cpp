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

const char * const igdeGizmoMove::ShapeNameMoveX = "move x";
const char * const igdeGizmoMove::ShapeNameMoveY = "move y";
const char * const igdeGizmoMove::ShapeNameMoveZ = "move z";
const char * const igdeGizmoMove::ShapeNameMoveXY = "move xy";
const char * const igdeGizmoMove::ShapeNameMoveYZ = "move yz";
const char * const igdeGizmoMove::ShapeNameMoveXZ = "move xz";
const char * const igdeGizmoMove::ShapeNameMoveView = "move view";

// Constructor, destructor
////////////////////////////

igdeGizmoMove::igdeGizmoMove( igdeEnvironment &environment ) :
igdeGizmo( environment ),
pAction( eaNone )
{
	SetShapeFromModel( environment.GetStockModel( igdeEnvironment::esmGizmoMove ) );
	SetRig( environment.GetStockRig( igdeEnvironment::esrGizmoMove ) );
	
	SetShapeColor( ShapeNameMoveX, decColor( 1.0f, 0.0f, 0.0f ) );
	SetShapeColor( ShapeNameMoveY, decColor( 0.0f, 1.0f, 0.0f ) );
	SetShapeColor( ShapeNameMoveZ, decColor( 0.0f, 0.0f, 1.0f ) );
	
	SetShapeColor( ShapeNameMoveYZ, decColor( 1.0f, 0.0f, 0.0f ) );
	SetShapeColor( ShapeNameMoveXZ, decColor( 0.0f, 1.0f, 0.0f ) );
	SetShapeColor( ShapeNameMoveXY, decColor( 0.0f, 0.0f, 1.0f ) );
	
	SetShapeColor( ShapeNameMoveView, decColor( 1.0f, 0.75f, 0.5f ) );
}

igdeGizmoMove::~igdeGizmoMove(){
}



// Management
///////////////

decQuaternion igdeGizmoMove::GetObjectOrientation(){
	return decQuaternion();
}

decVector igdeGizmoMove::GetObjectScale(){
	return decVector( 1.0f, 1.0f, 1.0f );
}

decDMatrix igdeGizmoMove::GetObjectMatrix(){
	return decDMatrix::CreateWorld( GetObjectPosition(),
		GetObjectOrientation(), decDVector( GetObjectScale() ) );
}

decDVector igdeGizmoMove::LimitPosition( const decDVector &position ){
	return position;
}

void igdeGizmoMove::OnObjectGeometryChanged(){
	SetGeometry( GetObjectPosition(), GetObjectOrientation() );
}



// Protected Functions
////////////////////////

igdeGizmoMove::eActions igdeGizmoMove::pShapeNameToAction( const decString &shapeName ) const{
	if( shapeName == ShapeNameMoveX ){
		return eaMoveX;
		
	}else if( shapeName == ShapeNameMoveY ){
		return eaMoveY;
		
	}else if( shapeName == ShapeNameMoveZ ){
		return eaMoveZ;
		
	}else if( shapeName == ShapeNameMoveXY ){
		return eaMoveXY;
		
	}else if( shapeName == ShapeNameMoveYZ ){
		return eaMoveYZ;
		
	}else if( shapeName == ShapeNameMoveXZ ){
		return eaMoveXZ;
		
	}else if( shapeName == ShapeNameMoveView ){
		return eaMoveView;
		
	}else{
		return eaNone;
	}
}

bool igdeGizmoMove::OnStartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, const decDVector &hitPoint, const decString &shapeName, int modifiers ){
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
const decDVector &rayDirection, const decDMatrix &viewMatrix, int modifiers ){
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

void igdeGizmoMove::OnStopEditing( bool cancel ){
	if( cancel ){
		SetObjectPosition( pMoveOrigin );
	}
	pAction = eaNone;
}

void igdeGizmoMove::OnAddToWorld(){
	OnObjectGeometryChanged();
}
