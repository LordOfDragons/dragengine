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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeLink.h"
#include "../aeAnimator.h"
#include "../controller/aeController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>



// Class aeLink
/////////////////

// Constructor, destructor
////////////////////////////

aeLink::aeLink( const char *name ) :
pAnimator( NULL ),
pEngLink( NULL ),
pName( name ),
pController( NULL ),
pRepeat( 1 ),
pBoneParameter( deAnimatorLink::ebpPositionZ ),
pBoneMinimum( 0.0f ),
pBoneMaximum( 1.0f ),
pWrapY( false )
{
	pCurve.SetDefaultLinear();
}

aeLink::aeLink( const aeLink &copy ) :
pAnimator( NULL ),
pEngLink( NULL ),
pName( copy.pName ),
pController( NULL ),
pRepeat( copy.pRepeat ),
pCurve( copy.pCurve ),
pBone( copy.pBone ),
pBoneParameter( copy.pBoneParameter ),
pBoneMinimum( copy.pBoneMinimum ),
pBoneMaximum( copy.pBoneMaximum ),
pWrapY( copy.pWrapY )
{
	pController = copy.pController;
	if( pController ){
		pController->AddReference();
	}
}

aeLink::~aeLink(){
	SetAnimator( NULL );
	
	if( pController ){
		pController->FreeReference();
	}
}



// Management
///////////////

void aeLink::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	if( pAnimator && pEngLink ){
		pAnimator->GetEngineAnimator()->RemoveLink( pEngLink );
		pEngLink = NULL;
	}
	
	pAnimator = animator;
	
	if( animator ){
		deAnimatorLink *link = NULL;
		
		try{
			link = new deAnimatorLink;
			animator->GetEngineAnimator()->AddLink( link );
			pEngLink = link;
			
		}catch( const deException & ){
			if( link ){
				delete link;
			}
			throw;
		}
		
		UpdateController();
		
		pEngLink->SetRepeat( pRepeat );
		pEngLink->SetBone( pBone );
		pEngLink->SetBoneParameter( pBoneParameter );
		pEngLink->SetWrapY( pWrapY );
		pUpdateBoneLimits();
		
		NotifyLinkChanged();
		
		pUpdateCurve();
	}
}

void aeLink::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	pName = name;
	
	if( pAnimator ){
		pAnimator->NotifyLinkNameChanged( this );
	}
}

void aeLink::SetController( aeController *controller, bool notify ){
	if( controller == pController ){
		return;
	}
	
	if( pController ){
		pController->FreeReference();
	}
	
	pController = controller;
	
	if( controller ){
		controller->AddReference();
	}
	
	UpdateController();
	
	if( pAnimator && notify ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetRepeat( int repeat ){
	if( repeat < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( repeat == pRepeat ){
		return;
	}
	
	pRepeat = repeat;
	
	if( pEngLink ){
		pEngLink->SetRepeat( pRepeat );
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetCurve( const decCurveBezier &curve ){
	if( curve == pCurve ){
		return;
	}
	
	pCurve = curve;
	
	pUpdateCurve();
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetBone( const char *bone ){
	if( pBone == bone ){
		return;
	}
	
	pBone = bone;
	
	if( pEngLink ){
		pEngLink->SetBone( pBone );
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetBoneParameter( deAnimatorLink::eBoneParameter parameter ){
	if( pBoneParameter == parameter ){
		return;
	}
	
	pBoneParameter = parameter;
	
	if( pEngLink ){
		pEngLink->SetBoneParameter( pBoneParameter );
		pUpdateBoneLimits();
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetBoneMinimum( float value ){
	if( fabsf( pBoneMinimum - value ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBoneMinimum = value;
	
	if( pEngLink ){
		pUpdateBoneLimits();
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetBoneMaximum( float value ){
	if( fabsf( pBoneMaximum - value ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBoneMaximum = value;
	
	if( pEngLink ){
		pUpdateBoneLimits();
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}

void aeLink::SetWrapY( bool wrap ){
	if( wrap == pWrapY ){
		return;
	}
	
	pWrapY = wrap;
	
	if( pEngLink ){
		pEngLink->SetWrapY( wrap );
		NotifyLinkChanged();
	}
	
	if( pAnimator ){
		pAnimator->NotifyLinkChanged( this );
	}
}



void aeLink::NotifyLinkChanged(){
	if( pEngLink && pAnimator ){
		deAnimator *engAnimator = pAnimator->GetEngineAnimator();
		
		engAnimator->NotifyLinkChangedAt( engAnimator->IndexOfLink( pEngLink ) );
	}
}



void aeLink::UpdateController(){
	if( ! pEngLink ){
		return;
	}
	
	deAnimator *engAnimator = pAnimator->GetEngineAnimator();
	int indexController = -1;
	
	if( pController && engAnimator ){
		indexController = pController->GetEngineControllerIndex();
	}
	
	pEngLink->SetController( indexController );
	
	NotifyLinkChanged();
}



// Operators
//////////////

aeLink &aeLink::operator=( const aeLink &copy ){
	SetName( copy.pName );
	SetController( copy.pController );
	SetRepeat( copy.pRepeat );
	pCurve = copy.pCurve;
	pBone = copy.pBone;
	pBoneParameter = copy.pBoneParameter;
	pBoneMinimum = copy.pBoneMinimum;
	pBoneMaximum = copy.pBoneMaximum;
	pWrapY = copy.pWrapY;
	return *this;
}

void aeLink::pUpdateCurve(){
	if( ! pEngLink ){
		return;
	}
	
	pEngLink->SetCurve( pCurve );
	
	NotifyLinkChanged();
}

void aeLink::pUpdateBoneLimits(){
	if( ! pEngLink ){
		return;
	}
	
	switch( pBoneParameter ){
	case deAnimatorLink::ebpRotationX:
	case deAnimatorLink::ebpRotationY:
	case deAnimatorLink::ebpRotationZ:
		pEngLink->SetBoneValueRange( pBoneMinimum * DEG2RAD, pBoneMaximum * DEG2RAD );
		break;
		
	default:
		pEngLink->SetBoneValueRange( pBoneMinimum, pBoneMaximum );
	}
}
