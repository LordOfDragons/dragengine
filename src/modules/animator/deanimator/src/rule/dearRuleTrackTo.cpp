/* 
 * Drag[en]gine Animator Module
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
#include <string.h>
#include <unistd.h>

#include "dearRuleTrackTo.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../component/dearComponent.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/component/deComponent.h>



// Class dearRuleTrackTo
//////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Track To = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleTrackTo::dearRuleTrackTo( dearAnimatorInstance &instance, int firstLink, const deAnimatorRuleTrackTo &rule ) :
dearRule( instance, firstLink, rule ),
pTrackTo( rule ),

pTrackBone( -1 ),

pTargetPosition( rule.GetTargetPosition(), firstLink ),
pTargetUp( rule.GetTargetUp(), firstLink ),

pLockedAxis( rule.GetLockedAxis() ),
pUpTarget( rule.GetUpTarget() ),
pTrackAxis( rule.GetTrackAxis() ),
pUpAxis( rule.GetUpAxis() )
{
	RuleChanged();
}

dearRuleTrackTo::~dearRuleTrackTo(){
}



// Management
///////////////

void dearRuleTrackTo::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const dearAnimatorInstance &instance = GetInstance();
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	// controller affected values
	decVector position;
	pTargetPosition.GetVector( instance, position );
	
	decVector up;
	pTargetUp.GetVector( instance, up );
	
	
	// determine the position to track to
	decVector trackPosition;
	
	if( pTrackBone == -1 ){
		trackPosition = position;
		
	}else{
		dearBoneState &bstate = *stalist.GetStateAt( pTrackBone );
		bstate.UpdateMatrices();
		trackPosition = bstate.GetGlobalMatrix().GetPosition();
	}
	
	// if the track axis is a negative axis this is the same as using the respective positive axis
	// but using the negated track vector. removes a bunch of lines of code
	deAnimatorRuleTrackTo::eTrackAxis trackAxis = pTrackAxis;
	bool negativeTrackAxis = false;;
	
	if( trackAxis == deAnimatorRuleTrackTo::etaNegX ){
		trackAxis = deAnimatorRuleTrackTo::etaPosX;
		negativeTrackAxis = true;
		
	}else if( trackAxis == deAnimatorRuleTrackTo::etaNegY ){
		trackAxis = deAnimatorRuleTrackTo::etaPosY;
		negativeTrackAxis = true;
		
	}else if( trackAxis == deAnimatorRuleTrackTo::etaNegZ ){
		trackAxis = deAnimatorRuleTrackTo::etaPosZ;
		negativeTrackAxis = true;
	}
	
	// determine the up vector to align the bone rotations to
	decVector alignUpVector;
	
	switch( pUpTarget ){
	case deAnimatorRuleTrackTo::eutWorldX:
		if( GetInstance().GetComponent() ){
			alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformRight();
			
		}else{
			alignUpVector.Set( 1.0f, 0.0f, 0.0f );
		}
		break;
		
	case deAnimatorRuleTrackTo::eutWorldY:
		if( GetInstance().GetComponent() ){
			alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformUp();
			
		}else{
			alignUpVector.Set( 0.0f, 1.0f, 0.0f );
		}
		break;
		
	case deAnimatorRuleTrackTo::eutWorldZ:
		if( GetInstance().GetComponent() ){
			alignUpVector = GetInstance().GetComponent()->GetMatrix().TransformView();
			
		}else{
			alignUpVector.Set( 0.0f, 0.0f, 1.0f );
		}
		break;
		
	case deAnimatorRuleTrackTo::eutComponentX:
		alignUpVector.Set( 1.0f, 0.0f, 0.0f );
		break;
		
	case deAnimatorRuleTrackTo::eutComponentY:
		alignUpVector.Set( 0.0f, 1.0f, 0.0f );
		break;
		
	case deAnimatorRuleTrackTo::eutComponentZ:
		alignUpVector.Set( 0.0f, 0.0f, 1.0f );
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneX:
		if( pTrackBone == -1 ){
			alignUpVector.Set( 1.0f, 0.0f, 0.0f );
			
		}else{
			alignUpVector = stalist.GetStateAt( pTrackBone )->GetGlobalMatrix().TransformRight();
		}
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneY:
		if( pTrackBone == -1 ){
			alignUpVector.Set( 0.0f, 1.0f, 0.0f );
			
		}else{
			alignUpVector = stalist.GetStateAt( pTrackBone )->GetGlobalMatrix().TransformUp();
		}
		break;
		
	case deAnimatorRuleTrackTo::eutTrackBoneZ:
		if( pTrackBone == -1 ){
			alignUpVector.Set( 0.0f, 0.0f, 1.0f );
			
		}else{
			alignUpVector = stalist.GetStateAt( pTrackBone )->GetGlobalMatrix().TransformView();
		}
		break;
		
	case deAnimatorRuleTrackTo::eutController:{
		alignUpVector = up;
		
		const float vlen = alignUpVector.Length();
		if( vlen < 1e-5f ){
			alignUpVector.Set( 0.0f, 1.0f, 0.0f );
			
		}else{
			alignUpVector /= vlen;
		}
		}break;
	}
	
	// if the up axis is a negative axis this is the same as using the respective positive axis
	// but using the negated align up vector. removes a bunch of lines of code
	deAnimatorRuleTrackTo::eTrackAxis upAxis = pUpAxis;
	
	if( upAxis == deAnimatorRuleTrackTo::etaNegX ){
		upAxis = deAnimatorRuleTrackTo::etaPosX;
		alignUpVector = -alignUpVector;
		
	}else if( upAxis == deAnimatorRuleTrackTo::etaNegY ){
		upAxis = deAnimatorRuleTrackTo::etaPosY;
		alignUpVector = -alignUpVector;
		
	}else if( upAxis == deAnimatorRuleTrackTo::etaNegZ ){
		upAxis = deAnimatorRuleTrackTo::etaPosZ;
		alignUpVector = -alignUpVector;
	}
	
	// rotate bones
	for( i=0; i<boneCount; i++ ){
		const int boneIndex = GetBoneMappingFor( i );
		if( boneIndex == -1 ){
			continue;
		}
		
		dearBoneState &bstate = *stalist.GetStateAt( boneIndex );
		
		bstate.UpdateMatrices();
		const decVector bonePosition( bstate.GetGlobalMatrix().GetPosition() );
		
		// determine the track normal
		decVector axisTrack( trackPosition - bonePosition );
		const float vlen = axisTrack.Length();
		if( vlen < 1e-5f ){
			continue; // track position equals to bone position, no tracking possible
		}
		axisTrack /= vlen;
		
		if( negativeTrackAxis ){
			axisTrack = -axisTrack;
		}
		
		// calculate the new bone coordinate system
		decVector axisX;
		decVector axisY;
		decVector axisZ;
		
		if( trackAxis == deAnimatorRuleTrackTo::etaPosX ){
			axisX = axisTrack;
			
			if( upAxis == deAnimatorRuleTrackTo::etaPosX ){
				continue; // invalid axis combination, no tracking possible
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosY ){
				axisZ = axisX % alignUpVector;
				if( axisZ.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisY = axisZ % axisX;
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosZ ){
				axisY = alignUpVector % axisX;
				if( axisY.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisZ = axisX % axisY;
			}
			
		}else if( trackAxis == deAnimatorRuleTrackTo::etaPosY ){
			axisY = axisTrack;
			
			if( upAxis == deAnimatorRuleTrackTo::etaPosX ){
				axisZ = alignUpVector % axisY;
				if( axisZ.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisX = axisY % axisZ;
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosY ){
				continue; // invalid axis combination, no tracking possible
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosZ ){
				axisX = axisY % alignUpVector;
				if( axisX.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisZ = axisX % axisY;
			}
			
		}else if( trackAxis == deAnimatorRuleTrackTo::etaPosZ ){
			axisZ = axisTrack;
			
			if( upAxis == deAnimatorRuleTrackTo::etaPosX ){
				axisY = axisZ % alignUpVector;
				if( axisY.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisX = axisY % axisZ;
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosY ){
				axisX = alignUpVector % axisZ;
				if( axisX.Length() < 1e-5f ){
					continue; // pole situation, no tracking possible
				}
				axisY = axisZ % axisX;
				
			}else if( upAxis == deAnimatorRuleTrackTo::etaPosZ ){
				continue; // invalid axis combination, no tracking possible
			}
		}
		
		// calculate the bone parameters for the new coordinate system
		decMatrix globalMatrix;
		globalMatrix.a11 = axisX.x;
		globalMatrix.a21 = axisX.y;
		globalMatrix.a31 = axisX.z;
		globalMatrix.a12 = axisY.x;
		globalMatrix.a22 = axisY.y;
		globalMatrix.a32 = axisY.z;
		globalMatrix.a13 = axisZ.x;
		globalMatrix.a23 = axisZ.y;
		globalMatrix.a33 = axisZ.z;
		globalMatrix.a14 = bonePosition.x;
		globalMatrix.a24 = bonePosition.y;
		globalMatrix.a34 = bonePosition.z;
		
		if( bstate.GetParentState() ){
			const decMatrix m( globalMatrix
				.QuickMultiply( bstate.GetParentState()->GetInverseGlobalMatrix() )
				.QuickMultiply( bstate.GetInverseRigLocalMatrix() ) );
			
			bstate.BlendWith( decVector(), m.ToQuaternion(), blendMode, blendFactor, false, true );
			
		}else{
			const decMatrix m( globalMatrix.QuickMultiply( bstate.GetInverseRigLocalMatrix() ) );
			
			bstate.BlendWith( decVector(), m.ToQuaternion(), blendMode, blendFactor, false, true );
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleTrackTo::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateTrackBone();
}



// Private Functions
//////////////////////

void dearRuleTrackTo::pUpdateTrackBone(){
	pTrackBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pTrackTo.GetTrackBone() );
}
