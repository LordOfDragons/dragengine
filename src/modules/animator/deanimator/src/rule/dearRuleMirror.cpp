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
	if( pBonePairs ){
		delete [] pBonePairs;
	}
}



// Management
///////////////

void dearRuleMirror::Apply( dearBoneStateList &stalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() || pBonePairCount == 0 ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	// prepare transformation matrix
	decMatrix transformMatrix( pMirrorMatrix );
	
	if( pMirrorBone != -1 ){
		dearBoneState &bstate = *stalist.GetStateAt( pMirrorBone );
		bstate.UpdateMatrices();
		transformMatrix = bstate.GetInverseGlobalMatrix().QuickMultiply( transformMatrix )
			.QuickMultiply( bstate.GetGlobalMatrix() );
	}
	
	// step through all bone pairs and apply transformation
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	int i;
	
	for( i=0; i<pBonePairCount; i++ ){
		const sBonePair &pair = pBonePairs[ i ];
		
		// bone pair
		if( pair.first != pair.second ){
			// store first bone global matrix
			dearBoneState &bstate1 = *stalist.GetStateAt( pair.first );
			bstate1.UpdateMatrices();
			const decMatrix orgMatrix1( bstate1.GetGlobalMatrix() );
			
			// store second bone global matrix
			dearBoneState &bstate2 = *stalist.GetStateAt( pair.second );
			bstate2.UpdateMatrices();
			const decMatrix orgMatrix2( bstate2.GetGlobalMatrix() );
			
			// mirror first bone
			decMatrix matrix1( orgMatrix2.QuickMultiply( transformMatrix ).Normalized() );
			if( bstate2.GetParentState() ){
				matrix1 = matrix1.QuickMultiply( bstate2.GetParentState()->GetGlobalMatrix()
					.QuickMultiply( transformMatrix ).Normalized().QuickInvert() );
			}
			matrix1 = matrix1.QuickMultiply( bstate1.GetInverseRigLocalMatrix() );
			
			bstate1.BlendWith( matrix1.GetPosition(), matrix1.ToQuaternion(), matrix1.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			
			// mirror second bone
			decMatrix matrix2( orgMatrix1.QuickMultiply( transformMatrix ).Normalized() );
			if( bstate1.GetParentState() ){
				matrix2 = matrix2.QuickMultiply( bstate1.GetParentState()->GetGlobalMatrix()
					.QuickMultiply( transformMatrix ).Normalized().QuickInvert() );
			}
			matrix2 = matrix2.QuickMultiply( bstate2.GetInverseRigLocalMatrix() );
			
			bstate2.BlendWith( matrix2.GetPosition(), matrix2.ToQuaternion(), matrix2.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
			
		// single bone
		}else{
			dearBoneState &bstate = *stalist.GetStateAt( pair.first );
			bstate.UpdateMatrices();
			
			decMatrix matrix( bstate.GetGlobalMatrix().QuickMultiply( transformMatrix ).Normalized() );
			if( bstate.GetParentState() ){
				matrix = matrix.QuickMultiply( bstate.GetParentState()->GetGlobalMatrix()
					.QuickMultiply( transformMatrix ).Normalized().QuickInvert() );
			}
			matrix = matrix.QuickMultiply( bstate.GetInverseRigLocalMatrix() );
			
			bstate.BlendWith( matrix.GetPosition(), matrix.ToQuaternion(), matrix.GetScale(),
				blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
		}
	}
DEBUG_PRINT_TIMER;
}



// #include "../deDEAnimator.h"
void dearRuleMirror::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateBones();
	
	switch( pMirrorAxis ){
	case deAnimatorRuleMirror::emaX:
		pMirrorMatrix.SetScale( -1.0f, 1.0f, 1.0f );
		break;
		
	case deAnimatorRuleMirror::emaY:
		pMirrorMatrix.SetScale( 1.0f, -1.0f, 1.0f );
		break;
		
	case deAnimatorRuleMirror::emaZ:
		pMirrorMatrix.SetScale( 1.0f, 1.0f, -1.0f );
		break;
		
	default:
		pMirrorMatrix.SetIdentity();
	}
	
	// DEBUG
	/*
	const dearBoneStateList &l = GetInstance().GetBoneStateList();
	deDEAnimator &m = GetInstance().GetModule();
	m.LogInfoFormat("Mirror: %d", pBonePairCount);
	for(int i=0; i<pBonePairCount; i++){
		m.LogInfoFormat("- '%s' <-> '%s'", l.GetStateAt(pBonePairs[i].first)->GetRigBoneName(),
			l.GetStateAt(pBonePairs[i].second)->GetRigBoneName());
	}
	*/
}



// Private Functions
//////////////////////

void dearRuleMirror::pUpdateBones(){
	pMirrorBone = GetInstance().GetBoneStateList().IndexOfStateNamed( pMirror.GetMirrorBone() );
	
	if( pBonePairs ){
		delete [] pBonePairs;
	}
	pBonePairs = nullptr;
	pBonePairCount = 0;
	
	const int mappingCount = GetBoneMappingCount();
	if( mappingCount == 0 ){
		return;
	}
	
	const int count = pMirror.GetMatchNameCount();
	if( count == 0 ){
		return;
	}
	
	struct sBone{
		int index;
		decString name;
		bool paired;
	};
	sBone * const bones = new sBone[ mappingCount ];
	
	int i;
	for( i=0; i<mappingCount; i++ ){
		bones[ i ].index = GetBoneMappingFor( i );
		bones[ i ].name = GetInstance().GetBoneStateList().GetStateAt( bones[ i ].index )->GetRigBoneName();
		bones[ i ].paired = false;
	}
	
	struct sMatch{
		sBone *bone;
		const char *before;
		const char *after;
		int lenBefore;
		int lenAfter;
		
		void Set( sBone *bone, const char *before, int lenBefore, const char *after, int lenAfter ){
			this->bone = bone;
			this->before = before;
			this->lenBefore = lenBefore;
			this->after = after;
			this->lenAfter = lenAfter;
		}
	};
	sMatch * const matchesFirst = new sMatch[ mappingCount ];
	sMatch * const matchesSecond = new sMatch[ mappingCount ];
	int matchFirstCount, matchSecondCount;
	
	pBonePairs = new sBonePair[ mappingCount ];
	
	int j, k, index;
	
	for( i=0; i<count; i++ ){
		const deAnimatorRuleMirror::sMatchName &matchName = pMirror.GetMatchNameAt( i );
		const int lenFirst = matchName.first.GetLength();
		const int lenSecond = matchName.second.GetLength();
		
		// find matching bones for first and second string
		matchSecondCount = 0;
		matchFirstCount = 0;
		
		switch( matchName.type ){
		case deAnimatorRuleMirror::emntFirst:
			for( j=0; j<mappingCount; j++ ){
				if( bones[ j ].paired ){
					continue;
				}
				
				if( bones[ j ].name.BeginsWith( matchName.first ) ){
					matchesFirst[ matchFirstCount++ ].Set( bones + j, "", 0,
						bones[ j ].name.GetString() + lenFirst,
						bones[ j ].name.GetLength() - lenFirst );
					
				}else if( bones[ j ].name.BeginsWith( matchName.second ) ){
					matchesSecond[ matchSecondCount++ ].Set( bones + j, "", 0,
						bones[ j ].name.GetString() + lenSecond,
						bones[ j ].name.GetLength() - lenSecond );
				}
			}
			break;
			
		case deAnimatorRuleMirror::emntLast:
			for( j=0; j<mappingCount; j++ ){
				if( bones[ j ].paired ){
					continue;
				}
				
				if( bones[ j ].name.EndsWith( matchName.first ) ){
					matchesFirst[ matchFirstCount++ ].Set( bones + j,
						bones[ j ].name.GetString(), bones[ j ].name.GetLength() - lenFirst, "", 0 );
					
				}else if( bones[ j ].name.EndsWith( matchName.second ) ){
					matchesSecond[ matchSecondCount++ ].Set( bones + j,
						bones[ j ].name.GetString(), bones[ j ].name.GetLength() - lenSecond, "", 0 );
				}
			}
			break;
			
		case deAnimatorRuleMirror::emntMiddle:
			for( j=0; j<mappingCount; j++ ){
				if( bones[ j ].paired ){
					continue;
				}
				
				index = bones[ j ].name.FindString( matchName.first );
				if( index != -1 ){
					matchesFirst[ matchFirstCount++ ].Set( bones + j,
						bones[ j ].name.GetString(), index,
						bones[ j ].name.GetString() + index + lenFirst,
						bones[ j ].name.GetLength() - lenFirst - index );
					
				}else{
					index = bones[ j ].name.FindString( matchName.second );
					if( index != -1 ){
						matchesSecond[ matchSecondCount++ ].Set( bones + j,
							bones[ j ].name.GetString(), index,
							bones[ j ].name.GetString() + index + lenSecond,
							bones[ j ].name.GetLength() - lenSecond - index );
					}
				}
			}
			break;
			
		default:
			break;
		}
		
		if( matchFirstCount == 0 || matchSecondCount == 0 ){
			continue;
		}
		
		// find pairs
		for( j=0; j<matchFirstCount; j++ ){
			const sMatch &first = matchesFirst[ j ];
			
			for( k=0; k<matchSecondCount; k++ ){
				const sMatch &second = matchesSecond[ k ];
				if( second.bone->paired ){
					continue;
				}
				
				if( first.lenAfter == second.lenAfter && first.lenBefore == second.lenBefore
				&& strncmp( first.before, second.before, first.lenBefore ) == 0
				&& strncmp( first.after, second.after, first.lenAfter ) == 0 ){
					// pair found
					pBonePairs[ pBonePairCount ].first = first.bone->index;
					pBonePairs[ pBonePairCount ].second = second.bone->index;
					pBonePairCount++;
					
					first.bone->paired = true;
					second.bone->paired = true;
					break;
				}
			}
		}
	}
	
	// add all non-paired bones
	for( i=0; i<mappingCount; i++ ){
		if( bones[ i ].paired ){
			continue;
		}
		
		pBonePairs[ pBonePairCount ].first = bones[ i ].index;
		pBonePairs[ pBonePairCount ].second = bones[ i ].index;
		pBonePairCount++;
	}
	
	// we have to sort the pairs to ensure parents follow after their children
	const dearBoneStateList &slist = GetInstance().GetBoneStateList();
	
	for( i=1; i<pBonePairCount; i++ ){
		const sBonePair &prevPair = pBonePairs[ i - 1 ];
		const sBonePair &pair = pBonePairs[ i ];
		
		const dearBoneState * const prevState = slist.GetStateAt( prevPair.first );
		const dearBoneState *state = slist.GetStateAt( pair.first );
		while( state ){
			if( state == prevState ){
				const sBonePair exchange( pBonePairs[ i ] );
				pBonePairs[ i ] = pBonePairs[ i - 1 ];
				pBonePairs[ i - 1 ] = exchange;
				
				if( i > 1 ){
					i -= 2;
				}
				break;
			}
			
			state = state->GetParentState();
		}
	}
	
	delete [] bones;
	delete [] matchesFirst;
	delete [] matchesSecond;
}
