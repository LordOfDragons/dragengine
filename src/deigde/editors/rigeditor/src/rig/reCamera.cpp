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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reCamera.h"
#include "reRig.h"
#include "bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class reCamera
///////////////////

// Constructor, destructor
////////////////////////////

reCamera::reCamera( reRig *rig, deEngine *engine ) : igdeCamera( engine ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	
	pFreeDistance = 0.0f;
	pBone = NULL;
	pDirty = false;
	pAttachToBone = false;
}

reCamera::~reCamera(){
}



// Management
///////////////

void reCamera::SetBone( reRigBone *bone ){
	if( bone != pBone ){
		if( pBone ) pBone->FreeReference();
		
		pBone = bone;
		
		if( bone ) bone->AddReference();
		
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreePosition( const decDVector &freePosition ){
	if( ! freePosition.IsEqualTo( pFreePosition ) ){
		pFreePosition = freePosition;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreeOrientation( const decVector &freeOrientation ){
	if( ! freeOrientation.IsEqualTo( pFreeOrientation ) ){
		pFreeOrientation = freeOrientation;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetFreeDistance( float freeDistance ){
	if( fabsf( freeDistance - pFreeDistance ) > 1e-5f ){
		pFreeDistance = freeDistance;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetRelativePosition( const decDVector &relativePosition ){
	if( ! relativePosition.IsEqualTo( pRelPosition ) ){
		pRelPosition = relativePosition;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetRelativeOrientation( const decVector &relativeOrientation ){
	if( ! relativeOrientation.IsEqualTo( pRelOrientation ) ){
		pRelOrientation = relativeOrientation;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::SetAttachToBone( bool attachToBone ){
	if( pAttachToBone != attachToBone ){
		pAttachToBone = attachToBone;
		pDirty = true;
		
		pRig->NotifyCameraChanged();
	}
}

void reCamera::Update(){
	if( pDirty || pAttachToBone ){
		if( pAttachToBone ){
			decDMatrix matrix = decDMatrix::CreateRT( decDVector( pRelOrientation * DEG2RAD ), pRelPosition );
			
			if( pBone ){
				matrix *= decDMatrix( pBone->GetPoseMatrix() );
			}
			
			SetPosition( matrix.GetPosition() );
			SetOrientation( matrix.GetEulerAngles().ToVector() / DEG2RAD );
			SetDistance( 0.0f );
			
		}else{
			SetPosition( pFreePosition );
			SetOrientation( pFreeOrientation );
			SetDistance( pFreeDistance );
		}
		
		pDirty = false;
		
		pRig->NotifyCameraViewChanged();
	}
}



void reCamera::Reset(){
	igdeCamera::Reset();
	
	pFreeDistance = 5.0f;
	if( pBone ){
		pBone->FreeReference();
		pBone = NULL;
	}
	pFreePosition.Set( 0.0, 1.0, 0.0 );
	pFreeOrientation.Set( 0.0f, 180.0f, 0.0f );
	pRelPosition.Set( 0.0, 0.0, 0.0 );
	pRelOrientation.Set( 0.0f, 0.0f, 0.0f );
	pAttachToBone = false;
	
	SetExposure( 1.0f );
	SetLowestIntensity( 20.0f );
	SetHighestIntensity( 20.0f );
	
	pDirty = true;
	
	pRig->NotifyCameraChanged();
}

void reCamera::ParameterChanged(){
	igdeCamera::ParameterChanged();
	
	pRig->NotifyCameraChanged();
}

void reCamera::AdaptionChanged(){
	igdeCamera::AdaptionChanged();
	
	pRig->NotifyCameraChanged();
}
