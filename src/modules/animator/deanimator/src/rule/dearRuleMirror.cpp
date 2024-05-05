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

#include "dearRuleMirror.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"
#include "../dearVPSState.h"
#include "../dearVPSStateList.h"
#include "../dearAnimatorInstance.h"
#include "../deDEAnimator.h"

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

dearRuleMirror::dearRuleMirror( dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRuleMirror &rule ) :
dearRule( instance, animator, firstLink, rule ),

pMirror( rule ),
pMirrorBone( -1 ),

pBonePairs( nullptr ),
pBonePairCount( 0 ),

pVPSPairs( nullptr ),
pVPSPairCount( 0 ),

pMirrorAxis( rule.GetMirrorAxis() ),
pEnablePosition( rule.GetEnablePosition() ),
pEnableOrientation( rule.GetEnableOrientation() ),
pEnableSize( rule.GetEnableSize() ),
pEnableVPS( rule.GetEnableVertexPositionSet() )
{
	RuleChanged();
}

dearRuleMirror::~dearRuleMirror(){
	if( pVPSPairs ){
		delete [] pVPSPairs;
	}
	if( pBonePairs ){
		delete [] pBonePairs;
	}
}



// Management
///////////////

void dearRuleMirror::Apply( dearBoneStateList &stalist, dearVPSStateList &vpsstalist ){
DEBUG_RESET_TIMERS;
	if( ! GetEnabled() || pBonePairCount == 0 ){
		return;
	}
	
	const float blendFactor = GetBlendFactor();
	if( blendFactor < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	
	// update global matrices of all affected bones.
	const int mappingCount = GetBoneMappingCount();
	int i;
	
	for( i=0; i<mappingCount; i++ ){
		stalist.GetStateAt( GetBoneMappingFor( i ) )->UpdateMatrices();
	}
	
	
	// prepare transformation matrix
	decMatrix transformMatrix( pMirrorMatrix );
	
	if( pMirrorBone != -1 ){
		dearBoneState &bstate = *stalist.GetStateAt( pMirrorBone );
		bstate.UpdateMatrices();
		transformMatrix = bstate.GetInverseGlobalMatrix().QuickMultiply( transformMatrix )
			.QuickMultiply( bstate.GetGlobalMatrix() );
	}
	
	
	// mirror global matrices of all bone pairs and single bones
	for( i=0; i<pBonePairCount; i++ ){
		const sBonePair &pair = pBonePairs[ i ];
		
		// bone pair
		if( pair.first != pair.second ){
			// store first bone global matrix
			dearBoneState &bstate1 = *stalist.GetStateAt( pair.first );
			const decMatrix orgMatrix1( bstate1.GetGlobalMatrix() );
			
			// store second bone global matrix
			dearBoneState &bstate2 = *stalist.GetStateAt( pair.second );
			const decMatrix orgMatrix2( bstate2.GetGlobalMatrix() );
			
			// mirror first bone
			bstate1.SetGlobalMatrix( orgMatrix2.QuickMultiply( transformMatrix ).Normalized() );
			bstate1.SetInverseGlobalMatrix( bstate1.GetGlobalMatrix().QuickInvert() );
			
			// mirror second bone
			bstate2.SetGlobalMatrix( orgMatrix1.QuickMultiply( transformMatrix ).Normalized() );
			bstate2.SetInverseGlobalMatrix( bstate2.GetGlobalMatrix().QuickInvert() );
			
		// single bone
		}else{
			dearBoneState &bstate = *stalist.GetStateAt( pair.first );
			
			bstate.SetGlobalMatrix( bstate.GetGlobalMatrix().QuickMultiply( transformMatrix ).Normalized() );
			bstate.SetInverseGlobalMatrix( bstate.GetGlobalMatrix().QuickInvert() );
		}
	}
	
	
	// apply changes to state. by doing this in a separate step it is not necessary to sort
	// bones up front to process them in the correct order (from children to parents)
	const deAnimatorRule::eBlendModes blendMode = GetBlendMode();
	
	for( i=0; i<mappingCount; i++ ){
		dearBoneState &bstate = *stalist.GetStateAt( GetBoneMappingFor( i ) );
		
		decMatrix matrix( bstate.GetGlobalMatrix() );
		if( bstate.GetParentState() ){
			matrix = matrix.QuickMultiply( bstate.GetParentState()->GetInverseGlobalMatrix() );
		}
		matrix = matrix.QuickMultiply( bstate.GetInverseRigLocalMatrix() );
		
		bstate.BlendWith( matrix.GetPosition(), matrix.ToQuaternion(), matrix.GetScale(),
			blendMode, blendFactor, pEnablePosition, pEnableOrientation, pEnableSize );
	}
	
	
	// step through all vertex position set pairs and apply transformation
	for( i=0; i<pVPSPairCount; i++ ){
		const sVPSPair &pair = pVPSPairs[ i ];
		
		dearVPSState &vpsState1 = vpsstalist.GetStateAt( pair.first );
		const float weight1 = vpsState1.GetWeight();
		
		dearVPSState &vpsState2 = vpsstalist.GetStateAt( pair.second );
		const float weight2 = vpsState2.GetWeight();
		
		vpsState1.BlendWith( weight2, blendMode, blendFactor, pEnableVPS );
		vpsState2.BlendWith( weight1, blendMode, blendFactor, pEnableVPS );
	}
DEBUG_PRINT_TIMER;
}



void dearRuleMirror::RuleChanged(){
	dearRule::RuleChanged();
	
	pUpdateBones();
	pUpdateVPS();
	
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
}



// Private Functions
//////////////////////

void dearRuleMirror::pUpdateBones(){
	const dearBoneStateList &slist = GetInstance().GetBoneStateList();
	
	pMirrorBone = slist.IndexOfStateNamed( pMirror.GetMirrorBone() );
	
	if( pBonePairs ){
		delete [] pBonePairs;
		pBonePairs = nullptr;
	}
	pBonePairCount = 0;
	
	const int mappingCount = GetBoneMappingCount();
	if( mappingCount == 0 ){
		return;
	}
	
	
	// find affected bones
	struct sBone{
		int index;
		decString name;
		bool paired;
	};
	sBone * const bones = new sBone[ mappingCount ];
	
	int i;
	for( i=0; i<mappingCount; i++ ){
		bones[ i ].index = GetBoneMappingFor( i );
		bones[ i ].name = slist.GetStateAt( bones[ i ].index )->GetRigBoneName();
		bones[ i ].paired = false;
	}
	
	
	// find matching pairs
	struct sMatch{
		sBone *bone;
		const char *before;
		const char *after;
		int lenBefore;
		int lenAfter;
		
		void Set( sBone *pbone, const char *pbefore, int plenBefore, const char *pafter, int plenAfter ){
			bone = pbone;
			before = pbefore;
			lenBefore = plenBefore;
			after = pafter;
			lenAfter = plenAfter;
		}
	};
	sMatch * const matchesFirst = new sMatch[ mappingCount ];
	sMatch * const matchesSecond = new sMatch[ mappingCount ];
	int matchFirstCount, matchSecondCount;
	
	pBonePairs = new sBonePair[ mappingCount ];
	
	const int count = pMirror.GetMatchNameCount();
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
	
	
	delete [] bones;
	delete [] matchesFirst;
	delete [] matchesSecond;
	
	
#if 0
	// debug
	GetModule().LogInfoFormat( "Mirror: matchNames %d:", pMirror.GetMatchNameCount() );
	for( i=0; i<pRootParentCount; i++ ){
		const deAnimatorRuleMirror::sMatchName &m = pMirror.GetMatchNameAt( i );
		GetModule().LogInfoFormat( "- '%s' -> '%s' (%d)", m.first.GetString(), m.second.GetString(), m.type );
	}
	
	GetModule().LogInfoFormat( "Mirror: bonePairs %d:", pBonePairCount );
	for( i=0; i<pBonePairCount; i++ ){
		const dearBoneState &s1 = *slist.GetStateAt( pBonePairs[ i ].first );
		if( pBonePairs[ i ].first != pBonePairs[ i ].second ){
			const dearBoneState &s2 = *slist.GetStateAt( pBonePairs[ i ].second );
			GetModule().LogInfoFormat( "- %d (%s) -> %d (%s)", pBonePairs[ i ].first,
				s1.GetRigBoneName(), pBonePairs[ i ].second, s2.GetRigBoneName() );
			
		}else{
			GetModule().LogInfoFormat( "- %d (%s)", pBonePairs[ i ].first, s1.GetRigBoneName() );
		}
	}
	
	GetModule().LogInfoFormat( "Mirror: rootParents %d:", pRootParentCount );
	for( i=0; i<pRootParentCount; i++ ){
		const dearBoneState &s = *slist.GetStateAt( pRootParents[ i ] );
		GetModule().LogInfoFormat( "- %d (%s)", pRootParents[ i ], s.GetRigBoneName() );
	}
#endif
}

void dearRuleMirror::pUpdateVPS(){
	if( pVPSPairs ){
		delete [] pVPSPairs;
		pVPSPairs = nullptr;
	}
	pVPSPairCount = 0;
	
	const int mappingCount = GetVPSMappingCount();
	if( mappingCount == 0 ){
		return;
	}
	
	struct sVPS{
		int index;
		decString name;
		bool paired;
	};
	sVPS * const vpsList = new sVPS[ mappingCount ];
	
	int i;
	for( i=0; i<mappingCount; i++ ){
		vpsList[ i ].index = GetVPSMappingFor( i );
		vpsList[ i ].name = GetInstance().GetVPSStateList().GetStateAt( vpsList[ i ].index ).GetName();
		vpsList[ i ].paired = false;
	}
	
	struct sMatch{
		sVPS *vps;
		const char *before;
		const char *after;
		int lenBefore;
		int lenAfter;
		
		void Set( sVPS *pvps, const char *pbefore, int plenBefore, const char *pafter, int plenAfter ){
			vps = pvps;
			before = pbefore;
			lenBefore = plenBefore;
			after = pafter;
			lenAfter = plenAfter;
		}
	};
	sMatch * const matchesFirst = new sMatch[ mappingCount ];
	sMatch * const matchesSecond = new sMatch[ mappingCount ];
	int matchFirstCount, matchSecondCount;
	
	pVPSPairs = new sVPSPair[ mappingCount ];
	
	const int count = pMirror.GetMatchNameCount();
	int j, k, index;
	
	for( i=0; i<count; i++ ){
		const deAnimatorRuleMirror::sMatchName &matchName = pMirror.GetMatchNameAt( i );
		const int lenFirst = matchName.first.GetLength();
		const int lenSecond = matchName.second.GetLength();
		
		// find matching vertex position sets for first and second string
		matchSecondCount = 0;
		matchFirstCount = 0;
		
		switch( matchName.type ){
		case deAnimatorRuleMirror::emntFirst:
			for( j=0; j<mappingCount; j++ ){
				if( vpsList[ j ].paired ){
					continue;
				}
				
				if( vpsList[ j ].name.BeginsWith( matchName.first ) ){
					matchesFirst[ matchFirstCount++ ].Set( vpsList + j, "", 0,
						vpsList[ j ].name.GetString() + lenFirst,
						vpsList[ j ].name.GetLength() - lenFirst );
					
				}else if( vpsList[ j ].name.BeginsWith( matchName.second ) ){
					matchesSecond[ matchSecondCount++ ].Set( vpsList + j, "", 0,
						vpsList[ j ].name.GetString() + lenSecond,
						vpsList[ j ].name.GetLength() - lenSecond );
				}
			}
			break;
			
		case deAnimatorRuleMirror::emntLast:
			for( j=0; j<mappingCount; j++ ){
				if( vpsList[ j ].paired ){
					continue;
				}
				
				if( vpsList[ j ].name.EndsWith( matchName.first ) ){
					matchesFirst[ matchFirstCount++ ].Set( vpsList + j,
						vpsList[ j ].name.GetString(), vpsList[ j ].name.GetLength() - lenFirst, "", 0 );
					
				}else if( vpsList[ j ].name.EndsWith( matchName.second ) ){
					matchesSecond[ matchSecondCount++ ].Set( vpsList + j,
						vpsList[ j ].name.GetString(), vpsList[ j ].name.GetLength() - lenSecond, "", 0 );
				}
			}
			break;
			
		case deAnimatorRuleMirror::emntMiddle:
			for( j=0; j<mappingCount; j++ ){
				if( vpsList[ j ].paired ){
					continue;
				}
				
				index = vpsList[ j ].name.FindString( matchName.first );
				if( index != -1 ){
					matchesFirst[ matchFirstCount++ ].Set( vpsList + j,
						vpsList[ j ].name.GetString(), index,
						vpsList[ j ].name.GetString() + index + lenFirst,
						vpsList[ j ].name.GetLength() - lenFirst - index );
					
				}else{
					index = vpsList[ j ].name.FindString( matchName.second );
					if( index != -1 ){
						matchesSecond[ matchSecondCount++ ].Set( vpsList + j,
							vpsList[ j ].name.GetString(), index,
							vpsList[ j ].name.GetString() + index + lenSecond,
							vpsList[ j ].name.GetLength() - lenSecond - index );
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
				if( second.vps->paired ){
					continue;
				}
				
				if( first.lenAfter == second.lenAfter && first.lenBefore == second.lenBefore
				&& strncmp( first.before, second.before, first.lenBefore ) == 0
				&& strncmp( first.after, second.after, first.lenAfter ) == 0 ){
					// pair found
					pVPSPairs[ pVPSPairCount ].first = first.vps->index;
					pVPSPairs[ pVPSPairCount ].second = second.vps->index;
					pVPSPairCount++;
					
					first.vps->paired = true;
					second.vps->paired = true;
					break;
				}
			}
		}
	}
	
	// ignore non-paired
	
	// sorting is not required for vertex position sets
	
	delete [] vpsList;
	delete [] matchesFirst;
	delete [] matchesSecond;
}
