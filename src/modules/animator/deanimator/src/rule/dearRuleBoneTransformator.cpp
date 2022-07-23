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

#include "dearRuleBoneTransformator.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/deEngine.h>



// Class dearRuleBoneRotator
//////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Bone Transformator = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleBoneTransformator::dearRuleBoneTransformator( dearAnimatorInstance &instance,
const dearAnimator &animator, int firstLink, const deAnimatorRuleBoneTransformator &rule ) :
dearRule( instance, animator, firstLink, rule ),

pBoneTransformator( rule ),

pTargetBone( 0 ),

pTargetTranslation( rule.GetTargetTranslation(), firstLink ),
pTargetRotation( rule.GetTargetRotation(), firstLink ),
pTargetScaling( rule.GetTargetScaling(), firstLink ),

pCoordinateFrame( rule.GetCoordinateFrame() ),
pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() ),
pUseAxis( rule.GetUseAxis() ),

pMinTranslation( rule.GetMinimumTranslation() ),
pMaxTranslation( rule.GetMaximumTranslation() ),
pMinRotation( rule.GetMinimumRotation() ),
pMaxRotation( rule.GetMaximumRotation() ),
pMinScaling( rule.GetMinimumScaling() ),
pMaxScaling( rule.GetMaximumScaling() ),
pAxis( rule.GetAxis() ),
pMinAngle( rule.GetMinimumAngle() ),
pMaxAngle( rule.GetMaximumAngle() )
{
	RuleChanged();
}

dearRuleBoneTransformator::~dearRuleBoneTransformator(){
}



// Management
///////////////

void dearRuleBoneTransformator::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const dearAnimatorInstance &instance = GetInstance();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	// prepare transformation matrix
	const float valueTranslation = decMath::clamp( pTargetTranslation.GetValue( instance, 0.0f ), 0.0f, 1.0f );
	const decVector translation( pMinTranslation * ( 1.0f - valueTranslation ) + pMaxTranslation * valueTranslation );
	
	const float valueRotation = decMath::clamp( pTargetRotation.GetValue( instance, 0.0f ), 0.0f, 1.0f );
	
	const float valueScaling = decMath::clamp( pTargetScaling.GetValue( instance, 0.0f ), 0.0f, 1.0f );
	const decVector scaling( pMinScaling * ( 1.0f - valueScaling ) + pMaxScaling * valueScaling );
	
	decMatrix transformMatrix;
	
	if( pUseAxis ){
		const float angle = pMinAngle * ( 1.0f - valueRotation ) + pMaxAngle * valueRotation;
		
		transformMatrix = decMatrix::CreateScale( scaling ).
			QuickMultiply( decMatrix::CreateRotationAxis( pAxis, angle ) ).
			QuickMultiply( decMatrix::CreateTranslation( translation ) );
		
	}else{
		const decVector rotation( pMinRotation * ( 1.0f - valueRotation ) + pMaxRotation * valueRotation );
		
		transformMatrix.SetSRT( scaling, rotation, translation );
	}
	
	if( pCoordinateFrame == deAnimatorRuleBoneTransformator::ecfTargetBone && pTargetBone != -1 ){
		dearBoneState &bstate = *stalist.GetStateAt( pTargetBone );
		bstate.UpdateMatrices();
		transformMatrix = bstate.GetInverseGlobalMatrix().QuickMultiply( transformMatrix )
			.QuickMultiply( bstate.GetGlobalMatrix() );
	}
	
	// step through all bones and apply transformation
	for( i=0; i<boneCount; i++ ){
		const int animatorBone = GetBoneMappingFor( i );
		if( animatorBone == -1 ){
			continue;
		}
		
		dearBoneState &bstate = *stalist.GetStateAt( animatorBone );
		bstate.UpdateMatrices();
		
		if( pCoordinateFrame == deAnimatorRuleBoneTransformator::ecfBoneLocal ){
			const decMatrix m( bstate.GetLocalMatrix().QuickMultiply( transformMatrix ) );
			
			bstate.BlendWith( m.GetPosition(), m.ToQuaternion(), m.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			
		}else{
			const decMatrix m( bstate.GetGlobalMatrix().QuickMultiply( transformMatrix )
				.QuickMultiply( bstate.GetInverseGlobalMatrix() )
				.QuickMultiply( bstate.GetLocalMatrix() ) );
			
			bstate.BlendWith( m.GetPosition(), m.ToQuaternion(), m.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
		}
	}
DEBUG_PRINT_TIMER;
}

void dearRuleBoneTransformator::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateTargetBone();
}



// Private Functions
//////////////////////

void dearRuleBoneTransformator::pUpdateTargetBone(){
	pTargetBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pBoneTransformator.GetTargetBone() );
}
