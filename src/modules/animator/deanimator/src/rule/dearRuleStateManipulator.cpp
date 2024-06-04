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
#include <string.h>

#include "dearRuleStateManipulator.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
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
const dearAnimator &animator, int firstLink, const deAnimatorRuleStateManipulator &rule ) :
dearRule( instance, animator, firstLink, rule ),
//pStateManipulator( rule ),

pTargetPosition( rule.GetTargetPosition(), firstLink ),
pTargetRotation( rule.GetTargetRotation(), firstLink ),
pTargetSize( rule.GetTargetSize(), firstLink ),
pTargetVPS( rule.GetTargetVertexPositionSet(), firstLink ),

pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableRotation() ),
pEnableScale( rule.GetEnableSize() ),
pEnableVPS( rule.GetEnableVertexPositionSet() ),
pEnableBones( pEnablePosition || pEnableOrientation || pEnableScale ),

pMinPosition( rule.GetMinimumPosition() ),
pMaxPosition( rule.GetMaximumPosition() ),
pMinRotation( rule.GetMinimumRotation() ),
pMaxRotation( rule.GetMaximumRotation() ),
pMinSize( rule.GetMinimumSize() ),
pMaxSize( rule.GetMaximumSize() ),
pMinVPS( rule.GetMinimumVertexPositionSet() ),
pMaxVPS( rule.GetMaximumVertexPositionSet() )
{
	RuleChanged();
}

dearRuleStateManipulator::~dearRuleStateManipulator(){
}



// Management
///////////////

void dearRuleStateManipulator::Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist ){
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
	const int vpsCount = GetVPSMappingCount();
	int i;
	
	// retrieve the blend factors
	decVector scale( 1.0f, 1.0f, 1.0f );
	decQuaternion orientation;
	decVector position;
	float weight = 0.0f;
	
	if( pEnableBones ){
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
	}
	
	if( pEnableVPS ){
		const float valueWeight = decMath::clamp( pTargetVPS.GetValue( instance, 0.0f ), 0.0f, 1.0f );
		weight = pMinVPS * ( 1.0f - valueWeight ) + pMaxVPS * valueWeight;
	}
	
	// apply states
	if( pEnableBones ){
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				continue;
			}
		
			stalist.GetStateAt( animatorBone )->BlendWith( position, orientation, scale,
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableScale );
		}
	}
	
	if( pEnableVPS ){
		for( i=0; i<vpsCount; i++ ){
			const int animatorVps = GetVPSMappingFor( i );
			if( animatorVps == -1 ){
				continue;
			}
		
			vpsstalist.GetStateAt( animatorVps ).BlendWith( weight, blendMode, blendFactor, pEnableVPS );
		}
	}
DEBUG_PRINT_TIMER;
}
