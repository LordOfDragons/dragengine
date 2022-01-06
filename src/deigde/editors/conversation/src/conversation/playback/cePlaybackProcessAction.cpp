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

#include "cePlayback.h"
#include "cePlaybackActor.h"
#include "cePlaybackCamera.h"
#include "cePlaybackActionStack.h"
#include "cePlaybackActionStackEntry.h"
#include "cePlaybackProcessAction.h"
#include "cePlaybackEvaluateCondition.h"
#include "variable/cePlaybackVariable.h"
#include "variable/cePlaybackVariableList.h"
#include "../ceConversation.h"
#include "../action/ceCACameraShot.h"
#include "../action/ceCAActorSpeak.h"
#include "../action/ceCASnippet.h"
#include "../action/ceCAStopConversation.h"
#include "../action/ceCAIfElse.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../action/ceConversationAction.h"
#include "../action/ceCASetVariable.h"
#include "../action/ceCASetActorParameter.h"
#include "../action/ceCAActorCommand.h"
#include "../action/ceCAGameCommand.h"
#include "../action/ceCAWait.h"
#include "../action/ceCATrigger.h"
#include "../action/ceCAActorAdd.h"
#include "../action/ceCAActorRemove.h"
#include "../action/ceCACoordSystemAdd.h"
#include "../action/ceCACoordSystemRemove.h"
#include "../action/ceCAIfElseCase.h"
#include "../actor/ceConversationActor.h"
#include "../actor/parameters/ceActorParameter.h"
#include "../actor/speechAnimation/ceSpeechAnimation.h"
#include "../actor/speechAnimation/ceSAWord.h"
#include "../actor/speechAnimation/ceSAPhoneme.h"
#include "../camerashot/ceCameraShot.h"
#include "../gesture/ceGesture.h"
#include "../facepose/ceFacePose.h"
#include "../file/ceConversationFile.h"
#include "../playerChoiceBox/cePlayerChoiceBox.h"
#include "../playerChoiceBox/cePCBOption.h"
#include "../target/ceTarget.h"
#include "../textbox/ceTextBox.h"
#include "../textbox/ceTextBoxText.h"
#include "../topic/ceConversationTopic.h"
#include "../infobox/ceConversationInfoBox.h"
#include "../strip/ceStrip.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSound.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class cePlaybackProcessAction
//////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackProcessAction::cePlaybackProcessAction(){
}

cePlaybackProcessAction::~cePlaybackProcessAction(){
}



// Management
///////////////

void cePlaybackProcessAction::ProcessAction( ceConversation &conversation, ceConversationAction *action ){
	const int actionType = action->GetType();
	
	if( actionType == ceConversationAction::eatCameraShot ){
		ProcessCameraShot( conversation, *( ( ceCACameraShot* )action ) );
		
	}else if( actionType == ceConversationAction::eatActorSpeak ){
		ProcessActorSpeak( conversation, *( ( ceCAActorSpeak* )action ) );
		
	}else if( actionType == ceConversationAction::eatIfElse ){
		ProcessIfElse( conversation, ( ceCAIfElse* )action );
		
	}else if( actionType == ceConversationAction::eatPlayerChoice ){
		ProcessPlayerChoice( conversation, ( ceCAPlayerChoice* )action );
		
	}else if( actionType == ceConversationAction::eatStopConversation ){
		ProcessStopConversation( conversation );
		
	}else if( actionType == ceConversationAction::eatStopTopic ){
		ProcessStopTopic( conversation );
		
	}else if( actionType == ceConversationAction::eatSnippet ){
		ProcessSnippet( conversation, ( ceCASnippet* )action );
		
	}else if( actionType == ceConversationAction::eatSetVariable ){
		ProcessSetVariable( conversation, *( ( ceCASetVariable* )action ) );
		
	}else if( actionType == ceConversationAction::eatSetActorParameter ){
		ProcessSetActorParameter( conversation, *( ( ceCASetActorParameter* )action ) );
		
	}else if( actionType == ceConversationAction::eatActorCommand ){
		ProcessActorCommand( conversation, *( ( ceCAActorCommand* )action ) );
		
	}else if( actionType == ceConversationAction::eatGameCommand ){
		ProcessGameCommand( conversation, *( ( ceCAGameCommand* )action ) );
		
	}else if( actionType == ceConversationAction::eatWait ){
		ProcessWait( conversation, ( ceCAWait* )action );
		
	}else if( actionType == ceConversationAction::eatTrigger ){
		ProcessTrigger( conversation, *( ( ceCATrigger* )action ) );
		
	}else if( actionType == ceConversationAction::eatActorAdd ){
		ProcessActorAdd( conversation, *( ( ceCAActorAdd* )action ) );
		
	}else if( actionType == ceConversationAction::eatActorRemove ){
		ProcessActorRemove( conversation, *( ( ceCAActorRemove* )action ) );
		
	}else if( actionType == ceConversationAction::eatCoordSystemAdd ){
		ProcessCoordSystemAdd( conversation, *( ( ceCACoordSystemAdd* )action ) );
		
	}else if( actionType == ceConversationAction::eatCoordSystemRemove ){
		ProcessCoordSystemRemove( conversation, *( ( ceCACoordSystemRemove* )action ) );
		
	}else if( actionType == ceConversationAction::eatComment ){
		conversation.GetPlayback()->AdvanceToNextAction();
		
	}else{
		conversation.GetPlayback()->SetRunning( false );
	}
}



void cePlaybackProcessAction::ProcessCameraShot( ceConversation &conversation, const ceCACameraShot &action ){
	ceCameraShot * const cameraShot = conversation.GetCameraShotNamed( action.GetName() );
	cePlayback &playback = *conversation.GetPlayback();
	
	if( cameraShot ){
		const int actorCount = playback.GetActorCount();
		cePlaybackCamera &camera = *playback.GetCamera();
		ceTarget *cameraTarget = NULL;
		ceTarget *lookAtTarget = NULL;
		int cameraActor = -1;
		int lookAtActor = -1;
		
		// determine targets
		if( ! action.GetCameraTarget().IsEmpty() ){
			cameraTarget = conversation.GetTargetNamed( action.GetCameraTarget() );
			
			if( cameraTarget ){
				cameraActor = conversation.GetActorList().IndexWithIDOrAliasID( cameraTarget->GetActor() );
				if( cameraActor == -1 ){
					ErrorNoActorWithID( playback, cameraTarget->GetActor() );
					return;
				}
				
			}else{
				ErrorTargetNotFound( playback, action.GetCameraTarget() );
				return;
			}
		}
		
		if( ! action.GetLookAtTarget().IsEmpty() ){
			lookAtTarget = conversation.GetTargetNamed( action.GetLookAtTarget() );
			
			if( lookAtTarget ){
				lookAtActor = conversation.GetActorList().IndexWithIDOrAliasID( lookAtTarget->GetActor() );
				if( lookAtActor == -1 ){
					ErrorNoActorWithID( playback, lookAtTarget->GetActor() );
					return;
				}
				
			}else{
				ErrorTargetNotFound( playback, action.GetLookAtTarget() );
				return;
			}
		}
		
		// set the coordinate system (temporary solution)
		const ceConversationActorList &list = conversation.GetActorList();
		
		if( actorCount == 1 ){
			camera.SetCoordSys1Actor( *list.GetAt( 0 ) );
			
		}else if( actorCount > 1 ){
			if( cameraShot->GetActorCount() > 1 ){
				camera.SetCoordSys2Actors( *list.GetAt( 0 ), *list.GetAt( 1 ) );
				
			}else{
				camera.SetCoordSys1Actor( *list.GetAt( 0 ) );
			}
		}
		
		// set the other parameters straight into the camera
		camera.SetCameraTarget( cameraTarget );
		camera.SetLookAtTarget( lookAtTarget );
		camera.SetOffsetCameraFrom( cameraShot->GetOffsetCameraFrom() );
		camera.SetOffsetCameraTo( cameraShot->GetOffsetCameraTo() );
		camera.SetCameraOrbitFrom( cameraShot->GetCameraOrbitFrom() );
		camera.SetCameraOrbitTo( cameraShot->GetCameraOrbitTo() );
		camera.SetCameraDistanceFrom( cameraShot->GetCameraDistanceFrom() );
		camera.SetCameraDistanceTo( cameraShot->GetCameraDistanceTo() );
		camera.SetLookAtFrom( cameraShot->GetOffsetLookAtFrom() );
		camera.SetLookAtTo( cameraShot->GetOffsetLookAtTo() );
		camera.SetPositionFrom( cameraShot->GetPositionFrom() );
		camera.SetPositionTo( cameraShot->GetPositionTo() );
		camera.SetRotationFrom( cameraShot->GetRotationFrom() );
		camera.SetRotationTo( cameraShot->GetRotationTo() );
		camera.SetTiltFrom( cameraShot->GetTiltFrom() );
		camera.SetTiltTo( cameraShot->GetTiltTo() );
		camera.SetFovFrom( cameraShot->GetFovFrom() );
		camera.SetFovTo( cameraShot->GetFovTo() );
		camera.SetLockUpAxis( cameraShot->GetLockUpAxis() );
		camera.SetAlignTargets( cameraShot->GetAlignTargets() );
		camera.SetRelativeToLookAt( cameraShot->GetRelativeToLookAt() );
		camera.SetLockCameraTarget( cameraShot->GetLockCameraTarget() );
		camera.SetLockLookAtTarget( cameraShot->GetLockLookAtTarget() );
		/*
		camera.SetParameterCurve( cePlaybackCamera::epPositionX, cameraShot->GetParameterCurve( ceCameraShot::epPositionX ) );
		camera.SetParameterCurve( cePlaybackCamera::epPositionY, cameraShot->GetParameterCurve( ceCameraShot::epPositionY ) );
		camera.SetParameterCurve( cePlaybackCamera::epPositionZ, cameraShot->GetParameterCurve( ceCameraShot::epPositionZ ) );
		camera.SetParameterCurve( cePlaybackCamera::epLookAtX, cameraShot->GetParameterCurve( ceCameraShot::epLookAtX ) );
		camera.SetParameterCurve( cePlaybackCamera::epLookAtY, cameraShot->GetParameterCurve( ceCameraShot::epLookAtY ) );
		camera.SetParameterCurve( cePlaybackCamera::epLookAtZ, cameraShot->GetParameterCurve( ceCameraShot::epLookAtZ ) );
		camera.SetParameterCurve( cePlaybackCamera::epTilt, cameraShot->GetParameterCurve( ceCameraShot::epTilt ) );
		camera.SetParameterCurve( cePlaybackCamera::epFov, cameraShot->GetParameterCurve( ceCameraShot::epFov ) );
		*/
		
		camera.SetDuration( action.GetDuration() );
		camera.Rewind();
	}
	
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessActorSpeak( ceConversation &conversation, ceCAActorSpeak &action ){
	const bool useSpeechAnimation = action.GetUseSpeechAnimation();
	const ceStripList &words = action.GetWordList();
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = action.GetActor();
	const int wordCount = words.GetCount();
	deSound *engSound = NULL;
	
	float speechLength = action.GetMinSpeechTime();
	
	// determine the actor to use
	const int actorIndex = conversation.GetActorList().IndexWithIDOrAliasID( actorID );
	if( actorIndex == -1 ){
		ErrorNoActorWithID( playback, actorID );
		return;
	}
	
	ceConversationActor &conversationActor = *conversation.GetActorList().GetAt( actorIndex );
	cePlaybackActor &playbackActor = playback.GetActorAt( actorIndex );
	
	// determine the length of the sound file if existing
	engSound = action.GetEngineSound();
	
	if( engSound ){
		conversationActor.PlayVoiceAudio( engSound );
		
		if( engSound->GetPlayTime() > speechLength ){
			speechLength = engSound->GetPlayTime();
		}
	}
	
	// prepare the speech
	if( wordCount > 0 ){
		if( conversationActor.GetSpeechAnimation() ){
			ceSpeechAnimation &speechAnimation = *conversationActor.GetSpeechAnimation();
			const ceSAPhonemeList &saPhonemeList = speechAnimation.GetPhonemeList();
			const ceSAWordList &saWordList = speechAnimation.GetWordList();
			int saPhonemeIndex;
			int i, j, length2;
			ceSAWord *saWord;
			decString word;
			float scaling;
			
			speechAnimation.RemoveAllSpeakPhonemes();
			
			for( i=0; i<wordCount; i++ ){
				const ceStrip &word = *words.GetAt( i );
				const float pauseLen = word.GetPause();
				
				if( pauseLen > FLOAT_SAFE_EPSILON ){
					// add pause. if the pause is added as single empty phoneme a long pause
					// leads to strange gaping motion. this is due to blending from phoneme
					// to phoneme. this is counteded by adding short empty phoneme at the
					// appropriate end to protect the long pause
					if( i == 0 ){
						if( pauseLen > 0.15f ){
							// protection at end of pause
							speechAnimation.AddSpeakPhoneme( -1, pauseLen - 0.1f );
							speechAnimation.AddSpeakPhoneme( -1, 0.1f );
							
						}else{
							speechAnimation.AddSpeakPhoneme( -1, pauseLen );
						}
						
					}else if( i == wordCount - 1 ){
						if( pauseLen > 0.15f ){
							// protection at beginning of pause
							speechAnimation.AddSpeakPhoneme( -1, 0.1f );
							speechAnimation.AddSpeakPhoneme( -1, pauseLen - 0.1f );
							
						}else{
							speechAnimation.AddSpeakPhoneme( -1, pauseLen );
						}
						
					}else{
						if( pauseLen > 0.25f ){
							// protection at both ends of pause
							speechAnimation.AddSpeakPhoneme( -1, 0.1f );
							speechAnimation.AddSpeakPhoneme( -1, pauseLen - 0.2f );
							speechAnimation.AddSpeakPhoneme( -1, 0.1f );
							
						}else{
							speechAnimation.AddSpeakPhoneme( -1, pauseLen );
						}
					}
				}
				
				if( word.GetID().IsEmpty() ){
					saWord = NULL;
					
				}else{
					saWord = saWordList.GetNamed( word.GetID() );
				}
				
				if( saWord ){
					const decUnicodeString &phonetics = saWord->GetPhonetics();
					
					length2 = phonetics.GetLength();
					
					scaling = 0.0f;
					for( j=0; j<length2; j++ ){
						saPhonemeIndex = saPhonemeList.IndexOfIPA( phonetics.GetAt( j ) );
						
						if( saPhonemeIndex == -1 ){
							scaling += 0.1f;
							
						}else{
							scaling += saPhonemeList.GetAt( saPhonemeIndex )->GetLength();
						}
					}
					if( scaling > 1e-5f ){
						scaling = word.GetDuration() / scaling;
					}
					
					for( j=0; j<length2; j++ ){
						saPhonemeIndex = saPhonemeList.IndexOfIPA( phonetics.GetAt( j ) );
						
						if( saPhonemeIndex == -1 ){
							speechAnimation.AddSpeakPhoneme( -1, 0.1f * scaling );
							
						}else{
							const ceSAPhoneme &phoneme = *saPhonemeList.GetAt( saPhonemeIndex );
							
							speechAnimation.AddSpeakPhoneme( saPhonemeIndex, phoneme.GetLength() * scaling );
						}
					}
					
				}else{
					if( useSpeechAnimation && ! word.GetID().IsEmpty() ){
						playback.GetMissingWords().Add( word.GetID() );
						conversation.NotifyPlaybackMissingWordsChanged();
					}
					
					speechAnimation.AddSpeakPhoneme( -1, word.GetDuration() );
				}
			}
			
			if( speechAnimation.GetSpeakLength() + 0.1f > speechLength ){
				speechLength = speechAnimation.GetSpeakLength() + 0.1f;
			}
			
			// remove speech animation if requested. this is done instead of avoiding adding the speech
			// animation in the first place since we need the speech animation to be set to know how
			// long the speaking takes. thus the actual speech animation is cleared afterwards
			if( ! useSpeechAnimation ){
				speechAnimation.RemoveAllSpeakPhonemes();
			}
			
		}else{
			float duration = 0.0f;
			int i;
			
			for( i=0; i<wordCount; i++ ){
				duration += words.GetAt( i )->GetPause() + words.GetAt( i )->GetDuration();
			}
			
			if( duration > speechLength ){
				speechLength = duration;
			}
		}
	}
	
	// update the sub title
	if( action.GetTextBoxText().GetLength() > 0 ){
		ceTextBoxText *text = NULL;
		
		try{
			text = new ceTextBoxText;
			text->SetName( conversationActor.GetTextBoxName() );
			text->SetText( action.GetTextBoxText() );
			// TODO text style 
			playbackActor.SetTextBoxText( text );
			text->FreeReference();
			
		}catch( const deException & ){
			if( text ){
				text->FreeReference();
			}
			throw;
		}
	}
	
	// update the gesture to play
	if( action.GetGestureList().GetCount() > 0 ){
		const ceStripList &caGestureList = action.GetGestureList();
		const int count = caGestureList.GetCount();
		int i;
		
		conversationActor.RemoveAllPlayGestures();
		
		for( i=0; i<count; i++ ){
			const ceStrip &caGesture = *caGestureList.GetAt( i );
			
			conversationActor.AddPlayGesture( conversation.GetGestureNamed( caGesture.GetID() ),
				caGesture.GetPause(), caGesture.GetDuration() );
		}
	}
	
	// update the face pose to play
	if( action.GetFacePoseList().GetCount() > 0 ){
		const ceStripList &caFacePoseList = action.GetFacePoseList();
		const int count = caFacePoseList.GetCount();
		int i;
		
		conversationActor.RemoveAllPlayFacePoses();
		
		for( i=0; i<count; i++ ){
			const ceStrip &caFacePose = *caFacePoseList.GetAt( i );
			
			conversationActor.AddPlayFacePose( conversation.GetFacePoseNamed( caFacePose.GetID() ),
				caFacePose.GetPause(), caFacePose.GetDuration() );
		}
	}
	
	// update the head look-at to play
	if( action.GetHeadLookAtList().GetCount() > 0 ){
		const ceStripList &caHeadLAList = action.GetHeadLookAtList();
		const int count = caHeadLAList.GetCount();
		int i;
		
		conversationActor.RemoveAllPlayHeadLookAts();
		
		for( i=0; i<count; i++ ){
			const ceStrip &caHeadLA = *caHeadLAList.GetAt( i );
			
			conversationActor.AddPlayHeadLookAt( conversation.GetTargetNamed( caHeadLA.GetID() ),
				caHeadLA.GetPause(), caHeadLA.GetDuration() );
		}
	}
	
	// update the eyes look-at to play. uses head look-ats if empty
	const ceStripList &caEyesLAList = action.GetEyesLookAtList().GetCount() > 0
		? action.GetEyesLookAtList() : action.GetHeadLookAtList();
	if( caEyesLAList.GetCount() > 0 ){
		const int count = caEyesLAList.GetCount();
		int i;
		
		conversationActor.RemoveAllPlayEyesLookAts();
		
		for( i=0; i<count; i++ ){
			const ceStrip &caEyesLA = *caEyesLAList.GetAt( i );
			
			conversationActor.AddPlayEyesLookAt( conversation.GetTargetNamed( caEyesLA.GetID() ),
				caEyesLA.GetPause(), caEyesLA.GetDuration() );
		}
	}
	
	// set the speech length if used
	if( speechLength > 0.0f ){
		playbackActor.SetSpeechLength( speechLength );
		playbackActor.SetElapsedTime( 0.0f );
	}
	
	// advance to the next action
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessSnippet( ceConversation &conversation, ceCASnippet *action ){
	ceConversationTopic * const topic = conversation.GetTopicWithID( action->GetFile(), action->GetTopic() );
	cePlayback &playback = *conversation.GetPlayback();
	
	if( topic ){
		playback.GetActionStack().Push( topic, action, &topic->GetActionList(), 0 );
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessStopConversation( ceConversation& conversation ){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActionStack();
	
	while( actionStack.GetCount() > 1 ){
		actionStack.Pop();
	}
	
	actionStack.GetTop().ForwardEnd();
	
	// show information
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	
	infobox.SetBackgroundColor( decColor( 1.0f, 0.0f, 0.0f, 0.5f ) );
	infobox.SetText( "Conversation Stop" );
}

void cePlaybackProcessAction::ProcessStopTopic( ceConversation& conversation ){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActionStack();
	
	while( actionStack.GetCount() > 0 && ! actionStack.GetTop().GetParentTopic() ){
		actionStack.Pop();
	}
	
	if( actionStack.GetCount() == 0 ){
		return;
	}
	
	actionStack.GetTop().ForwardLast();
	playback.AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessIfElse( ceConversation &conversation, ceCAIfElse *action ){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActionStack();
	cePlaybackEvaluateCondition evalCondition;
	
	// if cases
	ceCAIfElseCaseList &cases = action->GetCases();
	const int caseCount = cases.GetCount();
	int i;
	
	for( i=0; i<caseCount; i++ ){
		ceCAIfElseCase &ifcase = *cases.GetAt( i );
		const ceConversationCondition * const condition = ifcase.GetCondition();
		
		if( condition && evalCondition.EvaluateCondition( conversation, *condition ) ){
			if( ifcase.GetActions().GetCount() > 0 ){
				actionStack.Push( NULL, action, &ifcase.GetActions(), 0 );
				
			}else{
				playback.AdvanceToNextAction();
			}
			
			return;
		}
	}
	
	// else case
	if( action->GetElseActions().GetCount() > 0 ){
		actionStack.Push( NULL, action, &action->GetElseActions(), 0 );
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessPlayerChoice( ceConversation &conversation, ceCAPlayerChoice *action ){
	const ceCAPlayerChoiceOptionList &optionList = action->GetOptions();
	cePlayerChoiceBox &pcbox = conversation.GetPlayerChoiceBox();
	cePCBOptionList &pcbOptionList = pcbox.GetOptionList();
	const int optionCount = optionList.GetCount();
	cePlaybackEvaluateCondition evalCondition;
	ceCAPlayerChoiceOption *option;
	cePCBOption *pcbOption = NULL;
	int i;
	
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActionStack();
	actionStack.Push( NULL, action, &action->GetActions(), 0 );
	actionStack.GetTop().SetLooping( true );
	
	pcbox.SetPlaybackStackDepth( actionStack.GetCount() );
	
	pcbOptionList.RemoveAll();
	
	try{
		for( i=0; i<optionCount; i++ ){
			option = optionList.GetAt( i );
			
			if( option->GetCondition()
			&& ! evalCondition.EvaluateCondition( conversation, *option->GetCondition() ) ){
				continue;
			}
			
			pcbOption = new cePCBOption;
			pcbOption->SetText( option->GetText() );
			pcbOption->SetActionOption( action, option );
			
			pcbOptionList.Add( pcbOption );
			pcbOption->FreeReference();
			pcbOption = NULL;
		}
		
	}catch( const deException & ){
		if( pcbOption ){
			pcbOption->FreeReference();
		}
		throw;
	}
	
	pcbox.UpdateCanvas();
	
	if( pcbOptionList.GetCount() == 0 ){
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessSetVariable( ceConversation &conversation, const ceCASetVariable &action ){
	const decString &name = action.GetName();
	
	if( name.IsEmpty() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	cePlaybackVariableList &variableList = conversation.GetPlayback()->GetVariables();
	const ceCASetVariable::eOperators op = action.GetOperator();
	
	int value = action.GetValue();
	if( ! action.GetValueVariable().IsEmpty() ){
		cePlaybackVariable * const variable = variableList.GetNamed( action.GetValueVariable() );
		if( variable ){
			value = variable->GetValue();
		}
	}
	
	switch( op ){
	case ceCASetVariable::eopSet:
		variableList.Set( name, value );
		break;
		
	case ceCASetVariable::eopRandom:
		variableList.Set( name, decMath::random( 0, value ) );
		break;
		
	case ceCASetVariable::eopIncrement:
	case ceCASetVariable::eopDecrement:{
		cePlaybackVariable * const variable = variableList.GetNamed( name );
		int currentValue = 0;
		if( variable ){
			currentValue = variable->GetValue();
		}
		
		if( op == ceCASetVariable::eopIncrement ){
			variableList.Set( name, currentValue + value );
			
		}else{
			variableList.Set( name, currentValue - value );
		}
		}break;
		
	default:
		variableList.Set( name, value );
	}
	
	conversation.NotifyPlaybackVarListChanged();
	conversation.GetPlayback()->AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessSetActorParameter( ceConversation &conversation,
const ceCASetActorParameter &action ){
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = action.GetActor();
	
	ceConversationActor * const conversationActor = conversation.GetActorList().GetWithAliasID( actorID );
	const decString &name = action.GetName();
	if( name.IsEmpty() || ! conversationActor ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceActorParameterList &parameterList = conversationActor->GetParameters();
	const ceCASetActorParameter::eOperators op = action.GetOperator();
	
	int value = action.GetValue();
	if( ! action.GetValueVariable().IsEmpty() ){
		cePlaybackVariable * const variable = playback.GetVariables().GetNamed( action.GetValueVariable() );
		if( variable ){
			value = variable->GetValue();
		}
	}
	
	switch( op ){
	case ceCASetActorParameter::eopSet:
		parameterList.Set( name, value );
		break;
		
	case ceCASetActorParameter::eopRandom:
		parameterList.Set( name, decMath::random( 0, value ) );
		break;
		
	case ceCASetActorParameter::eopIncrement:
	case ceCASetActorParameter::eopDecrement:{
		ceActorParameter * const parameter = parameterList.GetNamed( name );
		int currentValue = 0;
		if( parameter ){
			currentValue = parameter->GetValue();
		}
		
		if( op == ceCASetActorParameter::eopIncrement ){
			parameterList.Set( name, currentValue + value );
			
		}else if( op == ceCASetActorParameter::eopDecrement ){
			parameterList.Set( name, currentValue - value );
			
		}
		}break;
		
	default:
		parameterList.Set( name, value );
	}
	
	conversation.NotifyActorParametersChanged( conversationActor );
	conversation.GetPlayback()->AdvanceToNextAction();
}

void cePlaybackProcessAction::ProcessActorCommand( ceConversation &conversation, const ceCAActorCommand &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	const decString &actorID = action.GetActor();
	
	// show information
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format( "Action Send Command to actor\n%s: %s", actorID.GetString(), action.GetCommand().GetString() );
	
	infobox.SetBackgroundColor( decColor( 1.0f, 1.0f, 0.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessGameCommand( ceConversation &conversation, const ceCAGameCommand &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format( "Action Send Command to game\n%s", action.GetCommand().GetString() );
	
	infobox.SetBackgroundColor( decColor( 1.0f, 1.0f, 0.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessWait( ceConversation &conversation, ceCAWait *action ){
	cePlayback &playback = *conversation.GetPlayback();
	cePlaybackActionStack &actionStack = playback.GetActionStack();
	
	if( action->GetCondition() ){
		actionStack.Push( NULL, action, &action->GetActions(), 0 );
		actionStack.GetTop().SetLoopCondition( action->GetCondition() );
		actionStack.GetTop().SetLooping( true );
		
	}else{
		playback.AdvanceToNextAction();
	}
}

void cePlaybackProcessAction::ProcessTrigger( ceConversation &conversation, const ceCATrigger &action ){
	cePlayback &playback = *conversation.GetPlayback();
	igdeTriggerTargetList &triggerTable = playback.GetTriggerTable();
	igdeTriggerTarget &target = *triggerTable.GetNamedAddIfMissing( action.GetName() );
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if( action.GetAction() == ceCATrigger::eaFire ){
		target.Fire();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format( "Action Trigger firing trigger '%s'", action.GetName().GetString() );
		
	}else if( action.GetAction() == ceCATrigger::eaReset ){
		target.Reset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format( "Action Trigger resetting trigger '%s'", action.GetName().GetString() );
		
	}else if( action.GetAction() == ceCATrigger::eaPulse ){
		target.Fire();
		target.Reset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format( "Action Trigger pulsing trigger '%s'", action.GetName().GetString() );
		
	}else{ // ceCATrigger::eaFullReset
		target.FullReset();
		conversation.NotifyPlaybackTriggerTableChanged();
		text.Format( "Action Trigger fully resetting trigger '%s'", action.GetName().GetString() );
	}
	
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	infobox.SetBackgroundColor( decColor( 1.0f, 0.5f, 0.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessActorAdd( ceConversation &conversation, const ceCAActorAdd &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if( action.GetAliasID().IsEmpty() ){
		text.Format( "Action Add Actor '%s'", action.GetID().GetString() );
		
	}else{
		text.Format( "Action Add Actor '%s' with alias '%s'",
			action.GetID().GetString(), action.GetAliasID().GetString() );
	}
	
	infobox.SetBackgroundColor( decColor( 0.5f, 1.0f, 0.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessActorRemove( ceConversation &conversation, const ceCAActorRemove &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format( "Action Remove Actor '%s'", action.GetActor().GetString() );
	
	infobox.SetBackgroundColor( decColor( 0.5f, 1.0f, 0.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessCoordSystemAdd( ceConversation &conversation, const ceCACoordSystemAdd &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	if( action.GetAliasID().IsEmpty() ){
		text.Format( "Action Add Coordinate System '%s'", action.GetCoordSystemID().GetString() );
		
	}else{
		text.Format( "Action Add Coordinate System '%s' with alias '%s'",
			action.GetCoordSystemID().GetString(), action.GetAliasID().GetString() );
	}
	
	infobox.SetBackgroundColor( decColor( 0.5f, 1.0f, 1.0f, 0.5f ) );
	infobox.SetText( text );
}

void cePlaybackProcessAction::ProcessCoordSystemRemove( ceConversation &conversation, const ceCACoordSystemRemove &action ){
	if( conversation.GetPlayback()->GetAutoAdvanceCommands() ){
		conversation.GetPlayback()->AdvanceToNextAction();
		return;
	}
	
	ceConversationInfoBox &infobox = conversation.GetInfoBox();
	decString text;
	
	text.Format( "Action Remove Coordinate System '%s'", action.GetCoordSystemID().GetString() );
	
	infobox.SetBackgroundColor( decColor( 0.5f, 1.0f, 1.0f, 0.5f ) );
	infobox.SetText( text );
}



int cePlaybackProcessAction::GetTargetActor( ceConversation &conversation, const decString &targetName ){
	int actor = -1;
	
	if( ! targetName.IsEmpty() ){
		const ceTarget * const target = conversation.GetTargetNamed( targetName );
		cePlayback &playback = *conversation.GetPlayback();
		
		if( target ){
			actor = conversation.GetActorList().IndexWithIDOrAliasID( target->GetActor() );
			if( actor == -1 ){
				ErrorNoActorWithID( playback, target->GetActor() );
			}
			
		}else{
			ErrorTargetNotFound( playback, targetName );
		}
	}
	
	return actor;
}



void cePlaybackProcessAction::ErrorNoActorWithID( cePlayback &playback, const char *id ){
	decString text;
	
	text.Format( "No actor with ID '%s' exists.", id );
	playback.SetTextBoxText( decUnicodeString::NewFromUTF8( text ) );
	
	playback.SetRunning( false );
}

void cePlaybackProcessAction::ErrorTargetNotFound( cePlayback &playback, const char *name ){
	decString text;
	
	text.Format( "No named target '%s' exists.", name );
	playback.SetTextBoxText( decUnicodeString::NewFromUTF8( text ) );
	
	playback.SetRunning( false );
}
