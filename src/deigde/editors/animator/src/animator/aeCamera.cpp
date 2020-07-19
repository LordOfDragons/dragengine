/* 
 * Drag[en]gine IGDE Animator Editor
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
#include "aeCamera.h"
#include "aeAnimator.h"
#include "locomotion/aeAnimatorLocomotion.h"

#include "dragengine/deEngine.h"
#include "dragengine/common/exceptions.h"
#include "dragengine/resources/component/deComponent.h"
#include "dragengine/resources/component/deComponentBone.h"
#include "dragengine/resources/rig/deRig.h"



// Class aeCamera
///////////////////

// Constructor, destructor
////////////////////////////

aeCamera::aeCamera( aeAnimator *animator, deEngine *engine ) : igdeCamera( engine ){
	if( ! animator ) DETHROW( deeInvalidParam );
	
	pAnimator = animator;
	
	pFreeDistance = 0.0f;
	pDirty = false;
	pAttachToBone = false;
}

aeCamera::~aeCamera(){
}



// Management
///////////////

void aeCamera::SetBone( const char *bone ){
	if( pBone == bone ){
		return;
	}
	
	pBone = bone;
	pDirty = true;
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::SetFreePosition( const decDVector &freePosition ){
	if( ! freePosition.IsEqualTo( pFreePosition ) ){
		pFreePosition = freePosition;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetFreeOrientation( const decVector &freeOrientation ){
	if( ! freeOrientation.IsEqualTo( pFreeOrientation ) ){
		pFreeOrientation = freeOrientation;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetFreeDistance( float freeDistance ){
	if( fabsf( freeDistance - pFreeDistance ) > 1e-5f ){
		pFreeDistance = freeDistance;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetRelativePosition( const decVector &relativePosition ){
	if( ! relativePosition.IsEqualTo( pRelPosition ) ){
		pRelPosition = relativePosition;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetRelativeOrientation( const decVector &relativeOrientation ){
	if( ! relativeOrientation.IsEqualTo( pRelOrientation ) ){
		pRelOrientation = relativeOrientation;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::SetAttachToBone( bool attachToBone ){
	if( pAttachToBone != attachToBone ){
		pAttachToBone = attachToBone;
		pDirty = true;
		
		pAnimator->NotifyCameraChanged();
	}
}

void aeCamera::Update(){
	aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	if( pDirty || pAttachToBone || locomotion.GetEnabled() ){
		if( pAttachToBone ){
			decMatrix matrix( decMatrix::CreateRT( pRelOrientation * DEG2RAD, pRelPosition ) );
			deComponent *engComponent = pAnimator->GetEngineComponent();
			
			if( engComponent ){
				decMatrix compMat;
				
				engComponent->PrepareBones();
				
				if( ! pBone.IsEmpty() && engComponent->GetRig() ){
					const int index = engComponent->GetRig()->IndexOfBoneNamed( pBone );
					if( index != -1 ){
						compMat = engComponent->GetBoneAt( index ).GetMatrix();
					}
				}
				
				compMat = compMat.QuickMultiply( engComponent->GetMatrix() );
				
				matrix = matrix.QuickMultiply( compMat.Normalized() );
			}
			
			SetPosition( matrix.GetPosition() );
			SetOrientation( matrix.GetEulerAngles() * RAD2DEG );
			SetDistance( 0.0f );
			
			pDirty = false;
			
		}else if( locomotion.GetEnabled() ){
			const float orientation = locomotion.GetOrientation().GetValue();
			const float lookLeftRight = locomotion.GetLookLeftRight().GetValue();
			decDMatrix matrix( decDMatrix::CreateRT( decDVector( pFreeOrientation * DEG2RAD ), pFreePosition )
				.QuickMultiply( decDMatrix::CreateRotationY( ( orientation + lookLeftRight ) * DEG2RAD ) )
				.QuickMultiply( decDMatrix::CreateTranslation( locomotion.GetPosition() ) ) );
			
			SetPosition( matrix.GetPosition() );
			SetOrientation( matrix.GetEulerAngles().ToVector() * RAD2DEG );
			SetDistance( pFreeDistance );
			
			pDirty = true; // so that when we stop the testing an update is done to recover
			
		}else{
			SetPosition( pFreePosition );
			SetOrientation( pFreeOrientation );
			SetDistance( pFreeDistance );
			
			pDirty = false;
		}
		
		pAnimator->NotifyCameraViewChanged();
	}
}



void aeCamera::Reset(){
	igdeCamera::Reset();
	
	pFreeDistance = 5.0f;
	pBone.Empty();
	pFreePosition.Set( 0.0, 1.5, 0.0 );
	pFreeOrientation.Set( 0.0f, 180.0f, 0.0f );
	pRelPosition.Set( 0.0f, 0.0f, 0.0f );
	pRelOrientation.Set( 0.0f, 0.0f, 0.0f );
	pAttachToBone = false;
	
	SetExposure( 1.0f ); // 2.0f
	SetLowestIntensity( 20.0f );
	SetHighestIntensity( 20.0f );
	
//	SetAdaptionTime( 0.8f );
//	SetExposure( 1.0f );
//	SetLowestIntensity( 0.1f );
//	SetHighestIntensity( 2500.0f );
	
	pDirty = true;
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::ParameterChanged(){
	igdeCamera::ParameterChanged();
	
	pAnimator->NotifyCameraChanged();
}

void aeCamera::AdaptionChanged(){
	igdeCamera::AdaptionChanged();
	
	pAnimator->NotifyCameraChanged();
}
