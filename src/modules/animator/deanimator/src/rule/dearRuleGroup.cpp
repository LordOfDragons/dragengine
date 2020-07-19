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
#include <unistd.h>

#include "dearRuleGroup.h"
#include "dearCreateRuleVisitor.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearAnimatorInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>



// class dearRuleGroup
////////////////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	#include "../deDEAnimator.h"
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS	timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER	GetModule().LogInfoFormat( "Rule Group = %iys", ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER
#endif



// Constructors and Destructors
/////////////////////////////////

dearRuleGroup::dearRuleGroup( dearAnimatorInstance &instance, int firstLink,
const deAnimatorRuleGroup &rule, const deAnimator &animator,
const decIntList &controllerMapping ) :
dearRule( instance, firstLink, rule ),

pGroup( rule ),
pStateList( NULL ),
pStateList2( NULL ),

pRules( NULL ),
pRuleCount( 0 ),

pTargetSelect( rule.GetTargetSelect(), firstLink ),

pApplicationType( rule.GetApplicationType() ),
pUseCurrentState( rule.GetUseCurrentState() ),
pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() )
{
	try{
		pCreateRules( firstLink, animator, controllerMapping );
		RuleChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dearRuleGroup::~dearRuleGroup(){
	pCleanUp();
}



// Management
///////////////

void dearRuleGroup::CaptureStateInto( int identifier ){
	int i;
	for( i=0; i<pRuleCount; i++ ){
		pRules[ i ]->CaptureStateInto( identifier );
	}
}

void dearRuleGroup::StoreFrameInto( int identifier, const char *moveName, float moveTime ){
	int i;
	for( i=0; i<pRuleCount; i++ ){
		pRules[ i ]->StoreFrameInto( identifier, moveName, moveTime );
	}
}

bool dearRuleGroup::RebuildInstance() const{
	int i;
	bool rebuild = false;
	for( i=0; i<pRuleCount; i++ ){
		rebuild |= pRules[ i ]->RebuildInstance();
	}
	
	return rebuild;
}

void dearRuleGroup::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( pRuleCount < 1 ){
		return;
	}
	
	if( ! GetEnabled() ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	dearAnimatorInstance &instance = GetInstance();
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	const int boneCount = GetBoneMappingCount();
	int i;
	
	// controller affected values
	float selectBlend = 0.0f;
	int selectIndex = 0;
	
	if( pApplicationType == deAnimatorRuleGroup::eatSelect && pRuleCount > 1 ){
		const float value = pTargetSelect.GetValue( instance, 0.0f );
		
		if( value <= 0.0f ){
			selectIndex = 0;
			selectBlend = 0.0f;
			
		}else if( value >= 1.0f ){
			selectIndex = pRuleCount - 1;
			selectBlend = 0.0f;
			
		}else{
			float intpart;
			selectBlend = modff( value * ( float )( pRuleCount - 1 ), &intpart );
			selectIndex = ( int )intpart;
		}
	}
	
	// apply all rules
	switch( pApplicationType ){
	case deAnimatorRuleGroup::eatAll:
		// use current state if required. some rule setups can not be done without
		if( pUseCurrentState ){
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				
				const dearBoneState &stateFrom = *stalist.GetStateAt( animatorBone );
				pStateList->GetStateAt( animatorBone )->SetFrom( stateFrom );
			}
		}
		
		// apply rules
		for( i=0; i<pRuleCount; i++ ){
			pRules[ i ]->Apply( *pStateList );
		}
		
		// apply the state
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				continue;
			}
			
			const dearBoneState &stateFrom = *pStateList->GetStateAt( animatorBone );
			dearBoneState &stateTo = *stalist.GetStateAt( animatorBone );
			
			stateTo.BlendWith( stateFrom, blendMode, blendFactor,
				pEnablePosition, pEnableOrientation, pEnableSize );
		}
		break;
		
	// apply a blend between two selected rules
	case deAnimatorRuleGroup::eatSelect:
		// if there are no rules apply the empty state
		if( pRuleCount == 0 ){
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				stalist.GetStateAt( animatorBone )->BlendWithDefault( blendMode,
					blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			}
		}
		
		// use current state if required. some rule setups can not be done without
		if( pUseCurrentState ){
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				
				const dearBoneState &stateFrom = *stalist.GetStateAt( animatorBone );
				pStateList->GetStateAt( animatorBone )->SetFrom( stateFrom );
				if( pStateList2 ){
					pStateList2->GetStateAt( animatorBone )->SetFrom( stateFrom );
				}
			}
		}
		
		// apply the blend between the two selected rules
		// TODO there is an optimization possible if pValueSelectBlend is 0.0 .
		//      in this case the rule could be applied directly to the current state.
		//      the problem is that this would apply the rule with it's own blending
		//      state if rule->Apply(instance,statelist) is called. it is though required
		//      to merge in our own blending. in case of blending being blend(1) this
		//      shortcut though would work
		pRules[ selectIndex ]->Apply( *pStateList );
		
		if( selectIndex < pRuleCount - 1 ){
			pRules[ selectIndex + 1 ]->Apply( *pStateList2 );
			
			for( i=0; i<boneCount; i++ ){
				const int animatorBone = GetBoneMappingFor( i );
				if( animatorBone == -1 ){
					continue;
				}
				
				const dearBoneState &stateFrom = *pStateList2->GetStateAt( animatorBone );
				dearBoneState &stateTo = *pStateList->GetStateAt( animatorBone );
				
				stateTo.BlendWith( stateFrom, deAnimatorRule::ebmBlend, selectBlend,
					pEnablePosition, pEnableOrientation, pEnableSize );
			}
		}
		
		// apply the state
		for( i=0; i<boneCount; i++ ){
			const int animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ){
				continue;
			}
			
			const dearBoneState &stateFrom = *pStateList->GetStateAt( animatorBone );
			dearBoneState &stateTo = *stalist.GetStateAt( animatorBone );
			
			stateTo.BlendWith( stateFrom, blendMode, blendFactor,
				pEnablePosition, pEnableOrientation, pEnableSize );
		}
		break;
	}
DEBUG_PRINT_TIMER;
}

void dearRuleGroup::ControllerChanged( int controller ){
	dearRule::ControllerChanged( controller );
	
	int i;
	
	for( i=0; i<pRuleCount; i++ ){
		pRules[ i ]->ControllerChanged( controller );
	}
}

void dearRuleGroup::RuleChanged(){
	dearRule::RuleChanged();
	
	dearAnimatorInstance &instance = GetInstance();
	
	// free old state lists. they are potentially out of date
	if( pStateList2 ){
		delete pStateList2;
		pStateList2 = NULL;
	}
	if( pStateList ){
		delete pStateList;
		pStateList = NULL;
	}
	
	// update all child rules
	int i;
	for( i=0; i<pRuleCount; i++ ){
		pRules[ i ]->RuleChanged();
	}
	
	// create copies of the current bone state list if required
	pStateList = instance.GetBoneStateList().CreateCopy();
	if( pApplicationType == deAnimatorRuleGroup::eatSelect && pRuleCount > 1 ){
		pStateList2 = instance.GetBoneStateList().CreateCopy();
	}
}



// Private Functions
//////////////////////

void dearRuleGroup::pCleanUp(){
	if( pRules ){
		while( pRuleCount > 0 ){
			delete pRules[ pRuleCount - 1 ];
			pRuleCount--;
		}
		
		delete [] pRules;
	}
	
	if( pStateList2 ){
		delete pStateList2;
	}
	if( pStateList ){
		delete pStateList;
	}
}

void dearRuleGroup::pCreateRules( int firstLink, const deAnimator &animator, const decIntList &controllerMapping ){
	const int ruleCount = pGroup.GetRuleCount();
	if( ruleCount == 0 ){
		return;
	}
	
	dearCreateRuleVisitor visitor( GetInstance(), animator, controllerMapping, firstLink );
	int i;
	
	pRules = new dearRule*[ ruleCount ];
	
	pRuleCount = 0;
	for( i=0; i<ruleCount; i++ ){
		pRules[ pRuleCount ] = visitor.CreateRuleFrom( *pGroup.GetRuleAt( i ) );
		
		if( pRules[ pRuleCount ] ){
			pRuleCount++;
		}
	}
}
