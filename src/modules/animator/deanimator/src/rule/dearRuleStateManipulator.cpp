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

#include <stdio.h>
#include <string.h>

#include "dearRuleStateManipulator.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/deEngine.h>



// Class dearRuleStateManipulator
///////////////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule State Manipulator = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleStateManipulator::dearRuleStateManipulator( dearAnimatorInstance &instance,
int firstLink, const deAnimatorRuleStateManipulator &rule ) :
dearRule( instance, firstLink, rule ),
//pStateManipulator( rule ),

pTargetPosition( rule.GetTargetPosition(), firstLink ),
pTargetRotation( rule.GetTargetRotation(), firstLink ),
pTargetSize( rule.GetTargetSize(), firstLink ),

pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableRotation() ),
pEnableScale( rule.GetEnableSize() ),

pMinPosition( rule.GetMinimumPosition() ),
pMaxPosition( rule.GetMaximumPosition() ),
pMinRotation( rule.GetMinimumRotation() ),
pMaxRotation( rule.GetMaximumRotation() ),
pMinSize( rule.GetMinimumSize() ),
pMaxSize( rule.GetMaximumSize() )
{
	RuleChanged();
}

dearRuleStateManipulator::~dearRuleStateManipulator(){
}



// Management
///////////////

void dearRuleStateManipulator::Apply( dearBoneStateList &stalist ){
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
	
	// retrieve the blend factors
	decVector scale( 1.0f, 1.0f, 1.0f );
	decQuaternion orientation;
	decVector position;
	
	if( pEnablePosition ){
		const float valuePosition = decMath::clamp( pTargetPosition.GetValue( instance, 0.0f ), 0.0f, 1.0f );
		position = pMinPosition * ( 1.0f - valuePosition ) + pMaxPosition * valuePosition;
	}
	
	if( pEnableOrientation ){
		const float valueRotation = decMath::clamp( pTargetRotation.GetValue( instance, 0.0f ), 0.0f, 1.0f );
		orientation.SetFromEuler( pMinRotation * ( 1.0f - valueRotation ) + pMaxRotation * valueRotation );
	}
	
	if( pEnableScale ){
		const float valueScale = decMath::clamp( pTargetSize.GetValue( instance, 0.0f ), 0.0f, 1.0f );
		scale = pMinSize * ( 1.0f - valueScale ) + pMaxSize * valueScale;
	}
	
	// apply states
	for( i=0; i<boneCount; i++ ){
		const int animatorBone = GetBoneMappingFor( i );
		if( animatorBone == -1 ){
			continue;
		}
		
		stalist.GetStateAt( animatorBone )->BlendWith( position, orientation, scale,
			blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableScale );
	}
DEBUG_PRINT_TIMER;
}
