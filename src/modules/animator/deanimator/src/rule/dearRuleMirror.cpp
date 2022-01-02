/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "dearRuleMirror.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>
#include <dragengine/deEngine.h>



// Class dearRuleMirror
/////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Mirror = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleMirror::dearRuleMirror( dearAnimatorInstance &instance,
int firstLink, const deAnimatorRuleMirror &rule ) :
dearRule( instance, firstLink, rule ),

pMirror( rule ),
pMirrorBone( -1 ),
pBonePairs( nullptr ),
pBonePairCount( 0 ),
pMirrorAxis( rule.GetMirrorAxis() ),
pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() )
{
	RuleChanged();
}

dearRuleMirror::~dearRuleMirror(){
}



// Management
///////////////

void dearRuleMirror::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
// 	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
// 	const dearAnimatorInstance &instance = GetInstance();
// 	const int boneCount = GetBoneMappingCount();
// 	int i;
	
DEBUG_PRINT_TIMER;
}



void dearRuleMirror::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateBones();
}



// Private Functions
//////////////////////

void dearRuleMirror::pUpdateBones(){
	pMirrorBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pMirror.GetMirrorBone() );
	
	// TODO find bone pairs
}
