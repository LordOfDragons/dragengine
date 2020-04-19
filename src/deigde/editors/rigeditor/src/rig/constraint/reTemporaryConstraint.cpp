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

#include <stdio.h>
#include <stdlib.h>

#include "reTemporaryConstraint.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>

#include <dragengine/common/exceptions.h>



// Class reTemporaryConstraint
////////////////////////////////

// Constructor, destructor
////////////////////////////

reTemporaryConstraint::reTemporaryConstraint( reRig *rig, reRigBone *bone,
const decDVector &position, const decQuaternion &orientation ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	decDMatrix matrix( decDMatrix::CreateWorld( position, orientation ) );
	deColliderComponent *simcol = rig->GetEngineSimulationCollider();
	
	pPosition = position;
	pOrientation = orientation;
	
	pEngSimCollider = NULL;
	pEngConstraint = NULL;
	
	if( simcol ){
		pEngConstraint = new deColliderConstraint;
		
		try{
			pEngConstraint->SetPosition2( position.ToVector() );
			pEngConstraint->SetOrientation2( orientation );
			
			pEngConstraint->SetToBallJoint();
			
			if( bone ){
				pEngConstraint->SetBone( bone->GetOrder() );
				
				matrix *= decDMatrix( bone->GetInversePoseMatrix() );
				
			}else{
				pEngConstraint->SetBone( -1 );
				
				matrix *= decDMatrix( rig->GetPoseMatrix() ).Invert();
			}
			
			pEngConstraint->SetPosition1( matrix.GetPosition().ToVector() );
			pEngConstraint->SetOrientation1( matrix.ToQuaternion() );
			
			simcol->AddConstraint( pEngConstraint );
			
		}catch( const deException & ){
			if( pEngConstraint ) delete pEngConstraint;
			throw;
		}
	}
	
	pEngSimCollider = simcol;
	if( simcol ) simcol->AddReference();
}

reTemporaryConstraint::~reTemporaryConstraint(){
	if( pEngSimCollider ){
		if( pEngConstraint ){
			pEngSimCollider->RemoveConstraint( pEngConstraint );
		}
		
		pEngSimCollider->FreeReference();
	}
}



// Management
///////////////

void reTemporaryConstraint::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pEngConstraint ){
			pEngConstraint->SetPosition2( pPosition.ToVector() );
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		
		if( pEngConstraint ){
			pEngConstraint->SetOrientation2( orientation );
		}
		
		pNotifyConstraintChanged();
	}
}



void reTemporaryConstraint::SetLinearLowerLimits( const decVector &lowerLimits ){
	if( ! lowerLimits.IsEqualTo( pLinearLowerLimits ) ){
		pLinearLowerLimits = lowerLimits;
		
		if( pEngConstraint ){
			pEngConstraint->GetDofLinearX().SetLowerLimit( pLinearLowerLimits.x );
			pEngConstraint->GetDofLinearY().SetLowerLimit( pLinearLowerLimits.y );
			pEngConstraint->GetDofLinearZ().SetLowerLimit( pLinearLowerLimits.z );
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetLinearUpperLimits( const decVector &upperLimits ){
	if( ! upperLimits.IsEqualTo( pLinearUpperLimits ) ){
		pLinearUpperLimits = upperLimits;
		
		if( pEngConstraint ){
			pEngConstraint->GetDofLinearX().SetUpperLimit( pLinearUpperLimits.x );
			pEngConstraint->GetDofLinearY().SetUpperLimit( pLinearUpperLimits.y );
			pEngConstraint->GetDofLinearZ().SetUpperLimit( pLinearUpperLimits.z );
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetAngularLowerLimits( const decVector &lowerLimits ){
	if( ! lowerLimits.IsEqualTo( pAngularLowerLimits ) ){
		pAngularLowerLimits = lowerLimits;
		
		if( pEngConstraint ){
			pEngConstraint->GetDofAngularX().SetLowerLimit( pAngularLowerLimits.x * DEG2RAD );
			pEngConstraint->GetDofAngularY().SetLowerLimit( pAngularLowerLimits.y * DEG2RAD );
			pEngConstraint->GetDofAngularZ().SetLowerLimit( pAngularLowerLimits.z * DEG2RAD );
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetAngularUpperLimits( const decVector &upperLimits ){
	if( ! upperLimits.IsEqualTo( pAngularUpperLimits ) ){
		pAngularUpperLimits = upperLimits;
		
		if( pEngConstraint ){
			pEngConstraint->GetDofAngularX().SetUpperLimit( pAngularUpperLimits.x * DEG2RAD );
			pEngConstraint->GetDofAngularY().SetUpperLimit( pAngularUpperLimits.y * DEG2RAD );
			pEngConstraint->GetDofAngularZ().SetUpperLimit( pAngularUpperLimits.z * DEG2RAD );
		}
		
		pNotifyConstraintChanged();
	}
}



// Private Functions
//////////////////////

void reTemporaryConstraint::pNotifyConstraintChanged(){
	if( pEngSimCollider && pEngConstraint ){
		pEngSimCollider->NotifyConstraintChanged( pEngSimCollider->IndexOfConstraint( pEngConstraint ) );
	}
}
