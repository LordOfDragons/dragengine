/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglTransformVolume.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionFrustum.h"
#include "../utils/collision/deoglDCollisionCylinder.h"
#include "../utils/collision/deoglDCollisionCapsule.h"

#include <dragengine/common/exceptions.h>



// class deoglTransformVolume
///////////////////////////////

// constructor, destructor
////////////////////////////

deoglTransformVolume::deoglTransformVolume(){
	pSphere = NULL;
	pCylinder = NULL;
	pCapsule = NULL;
	pBox = NULL;
	pTriangle = NULL;
	pFrustum = NULL;
	pVolume = NULL;
	
	pDirtyMatrix = false;
	pScaling.Set( 1.0f, 1.0f, 1.0f );
}

deoglTransformVolume::~deoglTransformVolume(){
	pCleanUp();
}



// Management
///////////////

void deoglTransformVolume::SetTranslation( const decDVector &translation ){
	pTranslation = translation;
	pDirtyMatrix = true;
}

void deoglTransformVolume::SetRotation( const decQuaternion &rotation ){
	pRotation = rotation;
	pDirtyMatrix = true;
}

void deoglTransformVolume::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}



// Convenience Function
/////////////////////////

deoglDCollisionVolume *deoglTransformVolume::GetVolumeFor( deoglDCollisionVolume *volume ){
	if( ! volume ) DETHROW( deeInvalidParam );
	volume->Visit( this );
	return pVolume;
}



// Visiting
/////////////

void deoglTransformVolume::VisitSphere( deoglDCollisionSphere *sphere ){
	if( ! pSphere ){
		pSphere = new deoglDCollisionSphere;
		if( ! pSphere ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	pSphere->SetCenter( pMatrix * sphere->GetCenter() );
	pSphere->SetRadius( sphere->GetRadius() );
	
	pVolume = pSphere;
}

void deoglTransformVolume::VisitCylinder( deoglDCollisionCylinder *cylinder ){
	if( ! pCylinder ){
		pCylinder = new deoglDCollisionCylinder;
		if( ! pCylinder ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	pCylinder->SetPosition( pMatrix * cylinder->GetPosition() );
	pCylinder->SetOrientation( ( decDMatrix::CreateFromQuaternion( cylinder->GetOrientation() ) * pMatrix ).ToQuaternion() );
	pCylinder->SetHalfHeight( cylinder->GetHalfHeight() );
	pCylinder->SetTopRadius( cylinder->GetTopRadius() );
	pCylinder->SetBottomRadius( cylinder->GetBottomRadius() );
	
	pVolume = pCylinder;
}

void deoglTransformVolume::VisitCapsule( deoglDCollisionCapsule *capsule ){
	if( ! pCapsule ){
		pCapsule = new deoglDCollisionCapsule;
		if( ! pCapsule ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	pCapsule->SetPosition( pMatrix * capsule->GetPosition() );
	pCapsule->SetOrientation( ( decDMatrix::CreateFromQuaternion( capsule->GetOrientation() ) * pMatrix ).ToQuaternion() );
	pCapsule->SetHalfHeight( capsule->GetHalfHeight() );
	pCapsule->SetTopRadius( capsule->GetTopRadius() );
	pCapsule->SetBottomRadius( capsule->GetBottomRadius() );
	
	pVolume = pCapsule;
}

void deoglTransformVolume::VisitBox( deoglDCollisionBox *box ){
	if( ! pBox ){
		pBox = new deoglDCollisionBox;
		if( ! pBox ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	pBox->SetCenter( pMatrix * box->GetCenter() );
	pBox->SetHalfSize( box->GetHalfSize() );
	pBox->SetOrientation( ( decDMatrix::CreateFromQuaternion( box->GetOrientation() ) * pMatrix ).ToQuaternion() );
	
	pVolume = pBox;
}

void deoglTransformVolume::VisitTriangle( deoglDCollisionTriangle *triangle ){
	if( ! pTriangle ){
		pTriangle = new deoglDCollisionTriangle;
		if( ! pTriangle ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	pTriangle->SetCorners( pMatrix * triangle->GetCorner1(), pMatrix * triangle->GetCorner2(),
			pMatrix * triangle->GetCorner3() );
	
	pVolume = pTriangle;
}

void deoglTransformVolume::VisitFrustum( deoglDCollisionFrustum *frustum ){
	if( ! pFrustum ){
		pFrustum = new deoglDCollisionFrustum;
		if( ! pFrustum ) DETHROW( deeOutOfMemory );
	}
	
	pUpdateMatrix();
	
	decDVector normal, position;
	
	position = pMatrix * ( frustum->GetLeftNormal() * -frustum->GetLeftDistance() );
	normal = pMatrix.TransformNormal( frustum->GetLeftNormal() );
	pFrustum->SetLeftPlane( normal, -( normal * position ) );
	
	position = pMatrix * ( frustum->GetRightNormal() * -frustum->GetRightDistance() );
	normal = pMatrix.TransformNormal( frustum->GetRightNormal() );
	pFrustum->SetRightPlane( normal, -( normal * position ) );
	
	position = pMatrix * ( frustum->GetTopNormal() * -frustum->GetTopDistance() );
	normal = pMatrix.TransformNormal( frustum->GetTopNormal() );
	pFrustum->SetTopPlane( normal, -( normal * position ) );
	
	position = pMatrix * ( frustum->GetBottomNormal() * -frustum->GetBottomDistance() );
	normal = pMatrix.TransformNormal( frustum->GetBottomNormal() );
	pFrustum->SetBottomPlane( normal, -( normal * position ) );
	
	position = pMatrix * ( frustum->GetNearNormal() * -frustum->GetNearDistance() );
	normal = pMatrix.TransformNormal( frustum->GetNearNormal() );
	pFrustum->SetNearPlane( normal, -( normal * position ) );
	
	position = pMatrix * ( frustum->GetFarNormal() * -frustum->GetFarDistance() );
	normal = pMatrix.TransformNormal( frustum->GetFarNormal() );
	pFrustum->SetFarPlane( normal, -( normal * position ) );
	
	pVolume = pFrustum;
}



// Private Functions
//////////////////////

void deoglTransformVolume::pCleanUp(){
	if( pFrustum ) delete pFrustum;
	if( pTriangle ) delete pTriangle;
	if( pBox ) delete pBox;
	if( pCapsule ) delete pCapsule;
	if( pCylinder ) delete pCylinder;
	if( pSphere ) delete pSphere;
}

void deoglTransformVolume::pUpdateMatrix(){
	if( pDirtyMatrix ){
		pMatrix.SetWorld( pTranslation, pRotation );
		pDirtyMatrix = false;
	}
}
