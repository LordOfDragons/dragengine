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

#include "dearRuleRetarget.h"
#include "../dearBoneState.h"
#include "../dearBoneStateList.h"

#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>
#include <dragengine/resources/animation/deAnimationKeyframeList.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleRetarget.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/deEngine.h>



// class dearRuleRetarget
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearRuleRetarget::dearRuleRetarget( dearAnimatorInstance &instance, int firstLink,
const deAnimatorRuleRetarget &rule ) :
dearRule( instance, firstLink, rule ),

//pRetarget( rule ),

pMatrices( NULL )
//,pMatrixCount( 0 )
{
	RuleChanged();
}

dearRuleRetarget::~dearRuleRetarget(){
	if( pMatrices ){
		delete [] pMatrices;
	}
}



// Management
///////////////

void dearRuleRetarget::Apply( dearBoneStateList &stalist ){
	if( ! GetEnabled() ){
		return;
	}
	
	/*
	if( pMoveIndex != -1 ){
		float sourceFactor = GetSourceFactor();
		float destFactor = GetDestinationFactor();
		dearAnimator *arAnimator = GetAnimator();
		deAnimator *animator = arAnimator->GetAnimator();
		deAnimation *animation = animator->GetAnimation();
		deAnimationMove *move = animation->GetMove( pMoveIndex );
		int boneCount = GetBoneMappingCount();
		deAnimationKeyframeList *kflist;
		deAnimationKeyframe *kf1, *kf2;
		dearBoneState *boneState;
		decVector position, size;
		decQuaternion orientation;
		float factor, moveTime = GetMoveTime();
		int i, animatorBone, animationBone;
		int leadkf;
		
		// step through all bones and set animation
		for( i=0; i<boneCount; i++ ){
			
			animatorBone = GetBoneMappingFor( i );
			if( animatorBone == -1 ) continue;
			boneState = stalist.GetStateAt( animatorBone );
			
			// determine animation state
			animationBone = boneState->GetAnimationBone();
			if( animationBone == -1 ){
				position.Set( 0.0f, 0.0f, 0.0f );
				orientation.SetZero();
				//size.Set( 1.0f, 1.0f, 1.0f );
				
			}else{
				// determine leading keyframe
				kflist = move->GetKeyframeList( animationBone );
				leadkf = kflist->GetLeadingKeyframe( 0, moveTime );
				// calculate bone data
				kf1 = kflist->GetKeyframe( leadkf );
				position = kf1->GetPosition();
				orientation = decMatrix::CreateRotation( kf1->GetRotation() ).ToQuaternion();
				// newSize = kf1->GetSize();
				if( kf1->GetTime() < moveTime && leadkf < kflist->GetKeyframeCount() - 1 ){
					kf2 = kflist->GetKeyframe( leadkf + 1 );
					factor = ( moveTime - kf1->GetTime() ) / ( kf2->GetTime() - kf1->GetTime() );
					position += ( kf2->GetPosition() - position ) * factor;
					orientation += ( decMatrix::CreateRotation( kf2->GetRotation() ).ToQuaternion() - orientation ) * factor;
					//size += ( kf2->GetSize() - size ) * factor;
				}
			}
			
			// blend with current state
			if( sourceFactor == 1.0 && destFactor == 0.0 ){
				boneState->SetPosition( position );
				boneState->SetOrientation( orientation );
				//boneState->SetSize( size );
			}else{
				boneState->SetPosition( boneState->GetPosition() * destFactor + position * sourceFactor );
				boneState->SetOrientation( boneState->GetOrientation() * destFactor + orientation * sourceFactor );
				//boneState->SetSize( boneState->GetSize() * destFactor + size * sourceFactor );
			}
		}
	}
	*/
}

void dearRuleRetarget::RuleChanged(){
	dearRule::RuleChanged();
	pUpdateMatrices();
}



// Private functions
//////////////////////

void dearRuleRetarget::pUpdateMatrices(){
#if 0
	if( pDirtyMatrices ){
		deRig *destRig = pRetarget.GetDestinationRig();
		deRig *srcRig = pRetarget.GetSourceRig();
		int boneCount = GetBoneMappingCount();
		decMatrix *matrices = NULL;
		
		if( boneCount > 0 ){
			matrices = new decMatrix[ boneCount ];
			if( ! matrices ) DETHROW( deeOutOfMemory );
		}
		
		if( pMatrices ) delete [] pMatrices;
		pMatrices = matrices;
		pMatrixCount = boneCount;
		
		if( srcRig ){
			if( ! destRig ){
				deComponent *component = GetAnimator().GetAnimator().GetComponent();
				
				if( component ){
					destRig = component->GetRig();
				}
			}
			
			if( destRig ){
				int i, j, rigBoneCount;
				deRigBone *destRigBone;
				deRigBone *srcRigBone;
				int destBoneIndex;
				int srcBoneIndex;
				
				for( i=0; i<boneCount; i++ ){
					destBoneIndex = GetBoneMappingFor( i );
					
					if( destBoneIndex != -1 ){
						destRigBone = destRig->GetBoneAt( destBoneIndex );
						
						srcBoneIndex = -1;
						rigBoneCount = srcRig->GetBoneCount();
						for( j=0; j<rigBoneCount; j++ ){
							srcRigBone = srcRig->GetBoneAt( j );
							if( strcmp( srcRigBone->GetName(), destRigBone->GetName() ) == 0 ){
								srcBoneIndex = j;
								break;
							}
						}
						
						if( srcBoneIndex != -1 ){
							// matrix = sourceBone.localMatrix * destBone.localMatrixInverse
							pMatrices[ i ] = decMatrix::CreateRotation( srcRigBone->GetRotation() )
								* decMatrix::CreateRotation( -destRigBone->GetRotation() );
							/*
							decMatrix::CreateWorld( srcRigBone->GetPosition(), srcRigBone->GetRotation() )
								* decMatrix::CreateWorld( destRigBone->GetPosition(), destRigBone->GetRotation() ).Invert();
							*/
						}
					}
				}
			}
		}
		
		pDirtyMatrices = false;
	}
#endif
}
