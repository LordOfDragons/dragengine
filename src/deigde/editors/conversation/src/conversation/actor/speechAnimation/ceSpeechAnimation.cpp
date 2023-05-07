/* 
 * Drag[en]gine IGDE Conversation Editor
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
#include <stdlib.h>
#include <string.h>

#include "ceSpeechAnimation.h"
#include "ceSAPhoneme.h"
#include "ceSAWord.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>



// Class ceSpeechAnimation
////////////////////////////

// Constructor, destructor
////////////////////////////

ceSpeechAnimation::ceSpeechAnimation( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	
	pEngAnimator = nullptr;
	pEngAnimatorInstance = nullptr;
	
	pSpeakPhonemes = nullptr;
	pSpeakPhonemeCount = 0;
	pSpeakPhonemeSize = 0;
	pSpeakLength = 0.0f;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = false;
	
	try{
		pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator( pEngAnimator );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

ceSpeechAnimation::~ceSpeechAnimation(){
	pCleanUp();
}



// Management
///////////////

void ceSpeechAnimation::SetNeutralMoveName( const char *name ){
	pNeutralMoveName = name;
}

void ceSpeechAnimation::RemoveAllSpeakPhonemes(){
	pSpeakPhonemeCount = 0;
	
	pSpeakLength = 0.0f;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = false;
}

void ceSpeechAnimation::AddSpeakPhoneme( int phoneme, float length ){
	DEASSERT_TRUE( phoneme >= -1 )
	DEASSERT_TRUE( phoneme < pPhonemeList.GetCount() )
	
	if( pSpeakPhonemeCount == pSpeakPhonemeSize ){
		int newSize = pSpeakPhonemeSize + 20;
		sSpeakPhoneme *newArray = new sSpeakPhoneme[ newSize ];
		if( pSpeakPhonemes ){
			memcpy( newArray, pSpeakPhonemes, sizeof( sSpeakPhoneme ) * pSpeakPhonemeCount );
			delete [] pSpeakPhonemes;
		}
		pSpeakPhonemes = newArray;
		pSpeakPhonemeSize = newSize;
	}
	
	pSpeakPhonemes[ pSpeakPhonemeCount ].phoneme = phoneme;
	pSpeakPhonemes[ pSpeakPhonemeCount ].length = length;
	pSpeakPhonemeCount++;
	
	pSpeakLength += length;
	pSpeakPos = -1;
	pSpeakElapsed = 0.0f;
	pSpeaking = true;
}



void ceSpeechAnimation::CreateAnimator(){
	const int phonemeCount = pPhonemeList.GetCount();
	deAnimatorLink *engLink = nullptr;
	int controllerIndex;
	int linkIndex;
	int i, j;
	
	// clear the engine controllers of all phonemes
	for( i=0; i<phonemeCount; i++ ){
		pPhonemeList.GetAt( i )->SetEngineController( -1 );
	}
	
	// remove the animator from the instance just for safety
	pEngAnimatorInstance->SetAnimator( nullptr );
	
	// rebuild the animator. for each animation move used in all phonemes
	// one controller and one animation rule is added. these are considered
	// the used visemes of all phonemes. the controller is used to set the
	// blend strength of each viseme. The matching controllers are then
	// assigned to each phoneme. Multiple phonemes can therefore use the
	// same controller.
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	
	// use the existing animation state
	const deAnimatorRuleStateSnapshot::Ref ruleReset(
		deAnimatorRuleStateSnapshot::Ref::New( new deAnimatorRuleStateSnapshot ) );
	ruleReset->SetUseLastState( true );
	pEngAnimator->AddRule( ruleReset );
	
	// add rule for neutral vertex position sets
	if( pNeutralVertexPositionSets.GetCount() > 0 ){
		const deAnimatorRuleStateManipulator::Ref rule(
			deAnimatorRuleStateManipulator::Ref::New( new deAnimatorRuleStateManipulator ) );
		rule->SetEnableRotation( false );
		rule->GetListVertexPositionSets() = pNeutralVertexPositionSets;
		pEngAnimator->AddRule( rule );
	}
	
	// add all unique visemes used by all phonemes
	for( i=0; i<phonemeCount; i++ ){
		ceSAPhoneme &phoneme = *pPhonemeList.GetAt( i );
		
		if( phoneme.GetEngineController() == -1 ){
			const decString &moveName = pPhonemeList.GetAt( i )->GetMoveName();
			
			// add a controller for the new viseme
			controllerIndex = pEngAnimator->GetControllerCount();
			pEngAnimator->AddController( new deAnimatorController );
			
			// add a link for the new viseme
			linkIndex = pEngAnimator->GetLinkCount();
			engLink = new deAnimatorLink;
			engLink->SetController( controllerIndex );
			pEngAnimator->AddLink( engLink );
			engLink = nullptr;
			
			// add an animation rule for the new viseme
			if( ! phoneme.GetVertexPositionSet().IsEmpty() ){
				const deAnimatorRuleStateManipulator::Ref rule(
					deAnimatorRuleStateManipulator::Ref::New( new deAnimatorRuleStateManipulator ) );
				rule->GetListVertexPositionSets().Add( phoneme.GetVertexPositionSet() );
				rule->SetEnableRotation( false );
				rule->GetTargetVertexPositionSet().AddLink( linkIndex );
				pEngAnimator->AddRule( rule );
				
			}else if( ! phoneme.GetMoveName().IsEmpty() ){
				const deAnimatorRuleAnimation::Ref rule(
					deAnimatorRuleAnimation::Ref::New( new deAnimatorRuleAnimation ) );
				rule->SetMoveName( phoneme.GetMoveName() );
				rule->SetBlendMode( deAnimatorRule::ebmOverlay );
				rule->GetTargetBlendFactor().AddLink( linkIndex );
				pEngAnimator->AddRule( rule );
			}
			
			// assign the controller to this phoneme and all other phonemes
			// using the same move name
			phoneme.SetEngineController( controllerIndex );
			
			for( j=i+1; j<phonemeCount; j++ ){
				ceSAPhoneme &phoneme2 = *pPhonemeList.GetAt( j );
				
				if( moveName.Equals( phoneme2.GetMoveName() ) ){
					phoneme2.SetEngineController( controllerIndex );
				}
			}
		}
	}
	
	// reassign the animator
	pEngAnimatorInstance->SetAnimator( pEngAnimator );
}

void ceSpeechAnimation::Update( float elapsed ){
	if( pSpeaking ){
		const int controllerCount = pEngAnimatorInstance->GetControllerCount();
		ceSAPhoneme *phoneme1 = nullptr;
		ceSAPhoneme *phoneme2 = nullptr;
		const float wordGapTime = 2.0f;
		const float windUpTime = 0.1f;
		float blendFactor = 1.0f;
		int controller1 = -1;
		int controller2 = -1;
		float phonemeLen;
		int i;
		
		// determine which phoneme to display and the blending
		pSpeakElapsed += elapsed;
		
		while( true ){
			phoneme1 = nullptr;
			phoneme2 = nullptr;
			
			if( pSpeakPos >= 0 && pSpeakPos < pSpeakPhonemeCount && pSpeakPhonemes[ pSpeakPos ].phoneme != -1 ){
				phoneme1 = pPhonemeList.GetAt( pSpeakPhonemes[ pSpeakPos ].phoneme );
			}
			
			if( pSpeakPos + 1 < pSpeakPhonemeCount && pSpeakPhonemes[ pSpeakPos + 1 ].phoneme != -1 ){
				phoneme2 = pPhonemeList.GetAt( pSpeakPhonemes[ pSpeakPos + 1 ].phoneme );
			}
			
			if( pSpeakPos == -1 ){
				phonemeLen = windUpTime;
				
			}else if( pSpeakPos == pSpeakPhonemeCount ){
				phonemeLen = wordGapTime;
				
			}else{
				phonemeLen = pSpeakPhonemes[ pSpeakPos ].length;
			}
			
			if( pSpeakElapsed < phonemeLen ){
				//if( pSpeakElapsed < phonemeLen - windUpTime ){
				//	blendFactor = 1.0f;
				//	
				//}else{
					//blendFactor = ( phonemeLen - pSpeakElapsed ) / windUpTime;
					//blendFactor = 1.0f - pSpeakElapsed / phonemeLen;
					blendFactor = 0.5f + cosf( ( pSpeakElapsed / phonemeLen ) * PI ) * 0.5f;
				//}
				
				if( phoneme1 ){
					controller1 = phoneme1->GetEngineController();
				}
				
				if( phoneme2 ){
					controller2 = phoneme2->GetEngineController();
				}
				
				if( controller1 == controller2 ){
					controller2 = -1;
					blendFactor = 1.0f;
				}
				
				break;
			}
			
			if( pSpeakPos < pSpeakPhonemeCount ){
				pSpeakPos++;
				
			}else{
				//pSpeakPos = -1;
				pSpeaking = false;
				controller1 = -1;
				controller2 = -1;
				break;
			}
			
			pSpeakElapsed -= phonemeLen;
		}
		
		// update the controllers
		if( controller1 != -1 && controller2 != -1 ){
			pEngAnimatorInstance->SetBlendFactor( 1.0f );
			
		}else if( controller1 != -1 ){
			pEngAnimatorInstance->SetBlendFactor( blendFactor );
			blendFactor = 1.0f;
			
		}else if( controller2 != -1 ){
			pEngAnimatorInstance->SetBlendFactor( 1.0f - blendFactor );
			blendFactor = 1.0f;
			
		}else{
			pEngAnimatorInstance->SetBlendFactor( 0.0f );
		}
		
		for( i=0; i<controllerCount; i++ ){
			if( i == controller1 ){
				pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( blendFactor );
				
			}else if( i == controller2 ){
				pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 1.0f - blendFactor );
				
			}else{
				pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 0.0f );
			}
			
			pEngAnimatorInstance->NotifyControllerChangedAt( i );
		}
	}
}

void ceSpeechAnimation::Clear(){
	pEngAnimatorInstance->SetAnimator( nullptr );
	pEngAnimator->RemoveAllRules();
	pEngAnimator->RemoveAllLinks();
	pEngAnimator->RemoveAllControllers();
	pEngAnimatorInstance->SetAnimator( pEngAnimator );
	
	RemoveAllSpeakPhonemes();
	pWordList.RemoveAll();
	pPhonemeList.RemoveAll();
	pNeutralMoveName = "";
	pNeutralVertexPositionSets.RemoveAll();
}



// Private Functions
//////////////////////

void ceSpeechAnimation::pCleanUp(){
	if( pSpeakPhonemes ){
		delete [] pSpeakPhonemes;
	}
	
	pWordList.RemoveAll();
	pPhonemeList.RemoveAll();
	
	if( pEngAnimatorInstance ){
		pEngAnimatorInstance->SetAnimator( nullptr );
		pEngAnimatorInstance->SetComponent( nullptr );
		pEngAnimatorInstance->FreeReference();
	}
	if( pEngAnimator ){
		pEngAnimator->SetRig( nullptr );
		pEngAnimator->FreeReference();
	}
}
