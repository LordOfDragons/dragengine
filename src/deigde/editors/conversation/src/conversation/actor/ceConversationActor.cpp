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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ceConversationActor.h"
#include "gesture/ceActorGesture.h"
#include "pose/ceActorPose.h"
#include "speechAnimation/ceSpeechAnimation.h"
#include "speechAnimation/ceSpeechAnimationXML.h"
#include "controller/ceActorController.h"
#include "../ceConversation.h"
#include "../facepose/ceFacePose.h"
#include "../gesture/ceGesture.h"
#include "../lookat/ceLookAt.h"
#include "../target/ceTarget.h"
#include "../../utils/ceControllerValue.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/loadsave/igdeLoadAnimator.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
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
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRigBone.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceConversationActor
////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActor::ceConversationActor( igdeEnvironment &environment ) :
pEnvironment( environment ),
pNameGestureProgress( "gesture.progress" ),
pNameGesturePlayback( "gesture.playback" ),
pNameGestureVariation( "gesture.variation" ),
pNameGestureBlink( "blinking" ),
pActivePose( NULL )
{
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	deAnimatorInstanceManager &animatorInstanceManager = *engine.GetAnimatorInstanceManager();
	
	pConversation = NULL;
	
	pEngComponent = NULL;
	pEngAnimatorInstance = NULL;
	pEngGestureAnimatorInstance = NULL;
	pEngFacePoseAnimator = NULL;
	pEngFacePoseAnimatorInstance = NULL;
	pEngEyesAnimator = NULL;
	pEngEyesAnimatorInstance = NULL;
	pEngSpeaker = NULL;
	
	pSpeechAnimation = NULL;
	
	pHeadLeftRight = 0.0f;
	pHeadUpDown = 0.0f;
	pEyesLeftRight = 0.0f;
	pEyesUpDown = 0.0f;
	
	pPlayGestures = NULL;
	pPlayGestureCount = 0;
	pPlayGestureSize = 0;
	pPlayGesturePos = 0;
	pPlayGestureElapsed = 0.0f;
	pPlayGestureRunning = false;
	
	pPlayFacePoses = NULL;
	pPlayFacePoseCount = 0;
	pPlayFacePoseSize = 0;
	pPlayFacePosePos = 0;
	pPlayFacePoseElapsed = 0.0f;
	pPlayFacePoseRunning = false;
	pPlayLastFacePose = NULL;
	pPlayCurFacePose = NULL;
	
	pPlayHeadLAs = NULL;
	pPlayHeadLACount = 0;
	pPlayHeadLASize = 0;
	pPlayHeadLAPos = 0;
	pPlayHeadLAElapsed = 0.0f;
	pPlayHeadLARunning = false;
	pPlayLastHeadLA = NULL;
	pPlayCurHeadLA = NULL;
	
	pPlayEyesLAs = NULL;
	pPlayEyesLACount = 0;
	pPlayEyesLASize = 0;
	pPlayEyesLAPos = 0;
	pPlayEyesLAElapsed = 0.0f;
	pPlayEyesLARunning = false;
	pPlayLastEyesLA = NULL;
	pPlayCurEyesLA = NULL;
	pPlayEyesBoneDisable = -1;
	
	pBlinkFreqMin = 2.0f; // according to wikipedia 2-10s
	pBlinkFreqMax = 6.0f;
	pBlinkNext = 1.0f;
	pBlinkElapsed = 1.0f;
	
	pWaiting = false;
	
	try{
		SetTextBoxName( decUnicodeString::NewFromUTF8( "Actor" ) );
		
		pEngAnimatorInstance = animatorInstanceManager.CreateAnimatorInstance();
		
		pSpeechAnimation = new ceSpeechAnimation( &engine );
		
		pEngGestureAnimatorInstance = animatorInstanceManager.CreateAnimatorInstance();
		pEngFacePoseAnimatorInstance = animatorInstanceManager.CreateAnimatorInstance();
		pEngEyesAnimatorInstance = animatorInstanceManager.CreateAnimatorInstance();
		
		pEngSpeaker = engine.GetSpeakerManager()->CreateSpeaker();
		pEngSpeaker->SetLooping( false );
		pEngSpeaker->SetMuted( false );
		pEngSpeaker->SetType( deSpeaker::estPoint ); // correctly directed but that's for later
		pEngSpeaker->SetVolume( 1.0f );
		pEngSpeaker->SetRange( 50.0f );
		pEngSpeaker->SetRollOff( 0.1f );
		
		decLayerMask layerMask;
		layerMask.SetBit( ceConversation::elmAudio );
		pEngSpeaker->SetLayerMask( layerMask );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

ceConversationActor::~ceConversationActor(){
	pCleanUp();
}



// Management
///////////////

void ceConversationActor::SetConversation( ceConversation *conversation ){
	if( conversation != pConversation ){
		if( pConversation ){
			if( pEngComponent ){
				pConversation->GetEngineWorld()->RemoveComponent( pEngComponent );
			}
			pConversation->GetEngineWorld()->RemoveSpeaker( pEngSpeaker );
		}
		
		pConversation = conversation;
		
		if( conversation ){
			if( pEngComponent ){
				conversation->GetEngineWorld()->AddComponent( pEngComponent );
			}
			conversation->GetEngineWorld()->AddSpeaker( pEngSpeaker );
		}
	}
}



void ceConversationActor::Update( cePlayback &playback, float elapsed ){
	// update head look-at first since it is used by gestures
	pUpdatePlayHeadLookAt( playback, elapsed );
	
	// update the other animators but do not apply them yet since gestures can capture state
	pUpdateAnimatorInstance( elapsed );
	pUpdatePlayGesture( elapsed );
	pUpdatePlayFacePose( elapsed );
	pUpdatePlayEyesLookAt( playback, elapsed );
	pSpeechAnimation->Update( elapsed );
	
	// not it is safe to apply the animators in the right order
	pEngAnimatorInstance->Apply();
	if( pEngGestureAnimatorInstance->GetAnimator() ){
		pEngGestureAnimatorInstance->Apply();
	}
	if( pEngFacePoseAnimatorInstance->GetAnimator() ){
		pEngFacePoseAnimatorInstance->Apply();
	}
	if( pEngEyesAnimatorInstance->GetAnimator() ){
		pEngEyesAnimatorInstance->Apply();
	}
	if( pSpeechAnimation->GetEngineAnimator() ){
		pSpeechAnimation->GetEngineAnimatorInstance()->Apply();
	}
}



void ceConversationActor::SetPathModel( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathModel.Equals( path ) ){
		pPathModel = path;
		pUpdateComponent();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetPathSkin( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathSkin.Equals( path ) ){
		pPathSkin = path;
		pUpdateComponent();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetPathRig( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathRig.Equals( path ) ){
		pPathRig = path;
		pUpdateComponent();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetPathSpeechAnimation( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathSpeechAnimation.Equals( path ) ){
		pPathSpeechAnimation = path;
		pUpdateSpeechAnimation();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetPathFacePoseAnimator( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathFacePoseAnimator.Equals( path ) ){
		pPathFacePoseAnimator = path;
		pUpdateFacePoseAnimator();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetPathEyesAnimator( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pPathEyesAnimator.Equals( path ) ){
		pPathEyesAnimator = path;
		pUpdateEyesAnimator();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pID.Equals( id ) ){
		pID = id;
		NotifyActorChanged();
	}
}

void ceConversationActor::SetAliasID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pAliasID.Equals( id ) ){
		pAliasID = id;
		NotifyActorChanged();
	}
}

void ceConversationActor::SetTextBoxName( const decUnicodeString &name ){
	if( pTextBoxName != name ){
		pTextBoxName = name;
		NotifyActorChanged();
	}
}



void ceConversationActor::SetPosition( const decVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		pRepositionComponent();
		pRepositionSpeaker();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetOrientation( const decVector &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		pRepositionComponent();
		pRepositionSpeaker();
		NotifyActorChanged();
	}
}

void ceConversationActor::SetBoneHeadRotator( const char *bone ){
	if( pBoneHeadRotator == bone ){
		return;
	}
	
	pBoneHeadRotator = bone;
	NotifyActorChanged();
}



void ceConversationActor::SetActivePose( ceActorPose *pose ){
	if( pose == pActivePose ){
		return;
	}
	
	if( pActivePose ){
		pActivePose->FreeReference();
	}
	
	pActivePose = pose;
	
	if( pose ){
		pose->AddReference();
		pEngAnimatorInstance->SetAnimator( pose->GetEngineAnimator() );
		
	}else{
		pEngAnimatorInstance->SetAnimator( NULL );
	}
	
	if( pConversation ){
		pConversation->NotifyActorActivePoseChanged( this );
	}
}

void ceConversationActor::NotifyPosesChanged(){
// 	RemoveAllPlayGestures();
	
	if( pActivePose ){
		pEngAnimatorInstance->SetAnimator( pActivePose->GetEngineAnimator() );
	}
	
	if( pConversation ){
		pConversation->NotifyActorPosesChanged( this );
	}
}

void ceConversationActor::NotifyPoseGesturesChanged( ceActorPose *pose ){
// 	RemoveAllPlayGestures();
	
	if( pConversation ){
		pConversation->NotifyActorPoseGesturesChanged( this, pose );
	}
}



void ceConversationActor::PlayVoiceAudio( deSound *sound ){
	pEngSpeaker->SetSound( sound );
	
	if( sound ){
		pEngSpeaker->Play();
	}
}



void ceConversationActor::SetHeadLeftRight( float angle ){
	pHeadLeftRight = decMath::clamp( angle, -90.0f, 90.0f );
}

void ceConversationActor::SetHeadUpDown( float angle ){
	pHeadUpDown = decMath::clamp( angle, -80.0f, 80.0f );
}

void ceConversationActor::SetEyesLeftRight( float angle ){
	pEyesLeftRight = decMath::clamp( angle, -90.0f, 90.0f );
}

void ceConversationActor::SetEyesUpDown( float angle ){
	pEyesUpDown = decMath::clamp( angle, -80.0f, 80.0f );
}




void ceConversationActor::RemoveAllPlayGestures(){
	pPlayGestureCount = 0;
	
	pPlayGesturePos = 0;
	pPlayGestureElapsed = 0.0f;
	pPlayGestureRunning = false;
}

void ceConversationActor::AddPlayGesture( ceGesture *gesture, float pause, float length ){
	if( pPlayGestureCount == pPlayGestureSize ){
		int newSize = pPlayGestureSize + 20;
		sGesture *newArray = new sGesture[ newSize ];
		if( pPlayGestures ){
			memcpy( newArray, pPlayGestures, sizeof( sGesture ) * pPlayGestureCount );
			delete [] pPlayGestures;
		}
		pPlayGestures = newArray;
		pPlayGestureSize = newSize;
	}
	
	pPlayGestures[ pPlayGestureCount ].gesture = gesture;
	pPlayGestures[ pPlayGestureCount ].pause = pause;
	pPlayGestures[ pPlayGestureCount ].length = length;
	pPlayGestureCount++;
	
	pPlayGesturePos = 0;
	pPlayGestureElapsed = 0.0f;
	pPlayGestureRunning = true;
}



void ceConversationActor::RemoveAllPlayFacePoses(){
	while( pPlayFacePoseCount > 0 ){
		pPlayFacePoseCount--;
		if( pPlayFacePoses[ pPlayFacePoseCount ].facePose ){
			pPlayFacePoses[ pPlayFacePoseCount ].facePose->FreeReference();
		}
	}
	
	pPlayFacePosePos = 0;
	pPlayFacePoseElapsed = 0.0f;
	pPlayFacePoseRunning = false;
}

void ceConversationActor::AddPlayFacePose( ceFacePose *facePose, float pause, float length ){
	if( pPlayFacePoseCount == pPlayFacePoseSize ){
		int newSize = pPlayFacePoseSize + 20;
		sFacePose *newArray = new sFacePose[ newSize ];
		if( pPlayFacePoses ){
			memcpy( newArray, pPlayFacePoses, sizeof( sFacePose ) * pPlayFacePoseCount );
			delete [] pPlayFacePoses;
		}
		pPlayFacePoses = newArray;
		pPlayFacePoseSize = newSize;
	}
	
	pPlayFacePoses[ pPlayFacePoseCount ].facePose = facePose;
	pPlayFacePoses[ pPlayFacePoseCount ].pause = pause;
	pPlayFacePoses[ pPlayFacePoseCount ].length = length;
	pPlayFacePoseCount++;
	if( facePose ){
		facePose->AddReference();
	}
	
	pPlayFacePosePos = 0;
	pPlayFacePoseElapsed = 0.0f;
	pPlayFacePoseRunning = true;
}



void ceConversationActor::RemoveAllPlayHeadLookAts(){
	while( pPlayHeadLACount > 0 ){
		pPlayHeadLACount--;
		if( pPlayHeadLAs[ pPlayHeadLACount ].lookAt ){
			pPlayHeadLAs[ pPlayHeadLACount ].lookAt->FreeReference();
		}
	}
	
	pPlayHeadLAPos = 0;
	pPlayHeadLAElapsed = 0.0f;
	pPlayHeadLARunning = false;
}

void ceConversationActor::AddPlayHeadLookAt( ceLookAt *lookAt, float pause, float duration ){
	if( pPlayHeadLACount == pPlayHeadLASize ){
		int newSize = pPlayFacePoseSize + 20;
		sLookAt *newArray = new sLookAt[ newSize ];
		if( pPlayHeadLAs ){
			memcpy( newArray, pPlayHeadLAs, sizeof( sLookAt ) * pPlayHeadLACount );
			delete [] pPlayHeadLAs;
		}
		pPlayHeadLAs = newArray;
		pPlayHeadLASize = newSize;
	}
	
	pPlayHeadLAs[ pPlayHeadLACount ].lookAt = lookAt;
	pPlayHeadLAs[ pPlayHeadLACount ].pause = pause;
	pPlayHeadLAs[ pPlayHeadLACount ].duration = duration;
	pPlayHeadLACount++;
	if( lookAt ){
		lookAt->AddReference();
	}
	
	pPlayHeadLAPos = 0;
	pPlayHeadLAElapsed = 0.0f;
	pPlayHeadLARunning = true;
}



void ceConversationActor::RemoveAllPlayEyesLookAts(){
	while( pPlayEyesLACount > 0 ){
		pPlayEyesLACount--;
		if( pPlayEyesLAs[ pPlayEyesLACount ].lookAt ){
			pPlayEyesLAs[ pPlayEyesLACount ].lookAt->FreeReference();
		}
	}
	
	pPlayEyesLAPos = 0;
	pPlayEyesLAElapsed = 0.0f;
	pPlayEyesLARunning = false;
}

void ceConversationActor::AddPlayEyesLookAt( ceLookAt *lookAt, float pause, float duration ){
	if( pPlayEyesLACount == pPlayEyesLASize ){
		int newSize = pPlayFacePoseSize + 20;
		sLookAt *newArray = new sLookAt[ newSize ];
		if( pPlayEyesLAs ){
			memcpy( newArray, pPlayEyesLAs, sizeof( sLookAt ) * pPlayEyesLACount );
			delete [] pPlayEyesLAs;
		}
		pPlayEyesLAs = newArray;
		pPlayEyesLASize = newSize;
	}
	
	pPlayEyesLAs[ pPlayEyesLACount ].lookAt = lookAt;
	pPlayEyesLAs[ pPlayEyesLACount ].pause = pause;
	pPlayEyesLAs[ pPlayEyesLACount ].duration = duration;
	pPlayEyesLACount++;
	if( lookAt ){
		lookAt->AddReference();
	}
	
	pPlayEyesLAPos = 0;
	pPlayEyesLAElapsed = 0.0f;
	pPlayEyesLARunning = true;
}



void ceConversationActor::SetWaiting( bool waiting ){
	if( waiting != pWaiting ){
		pWaiting = waiting;
		NotifyActorChanged();
	}
}



decMatrix ceConversationActor::GetBoneMatrix( const char *bone ) const{
	if( ! bone ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEngComponent && pEngComponent->GetRig() ){
		const int index = pEngComponent->GetRig()->IndexOfBoneNamed( bone );
		
		if( index != -1 ){
			pEngComponent->PrepareMatrix();
			return pEngComponent->GetBoneAt( index ).GetMatrix();
		}
	}
	
	return decMatrix();
}



void ceConversationActor::Reset(){
	RemoveAllPlayEyesLookAts();
	RemoveAllPlayHeadLookAts();
	RemoveAllPlayFacePoses();
	RemoveAllPlayGestures();
	
	pCommands.RemoveAll();
	pParameter.RemoveAll();
	
	SetActivePose( NULL );
	pPoses.RemoveAll();
	NotifyPosesChanged();
	
	SetPathSpeechAnimation( "" );
	SetPathEyesAnimator( "" );
	SetPathFacePoseAnimator( "" );
	
	pPathRig.Empty();
	pPathSkin.Empty();
	pPathModel.Empty();
	pUpdateComponent();
}



void ceConversationActor::NotifyActorChanged(){
	if( pConversation ){
		pConversation->NotifyActorChanged( this );
	}
}



// Private Functions
//////////////////////

void ceConversationActor::pCleanUp(){
	if( pPlayCurEyesLA ){
		pPlayCurEyesLA->FreeReference();
	}
	if( pPlayLastEyesLA ){
		pPlayLastEyesLA->FreeReference();
	}
	if( pPlayEyesLAs ){
		RemoveAllPlayEyesLookAts();
		delete [] pPlayEyesLAs;
	}
	
	if( pPlayCurHeadLA ){
		pPlayCurHeadLA->FreeReference();
	}
	if( pPlayLastHeadLA ){
		pPlayLastHeadLA->FreeReference();
	}
	if( pPlayHeadLAs ){
		RemoveAllPlayHeadLookAts();
		delete [] pPlayHeadLAs;
	}
	
	if( pPlayCurFacePose ){
		pPlayCurFacePose->FreeReference();
	}
	if( pPlayLastFacePose ){
		pPlayLastFacePose->FreeReference();
	}
	if( pPlayFacePoses ){
		RemoveAllPlayFacePoses();
		delete [] pPlayFacePoses;
	}
	
	if( pPlayGestures ){
		delete [] pPlayGestures;
	}
	
	if( pActivePose ){
		pActivePose->FreeReference();
	}
	pPoses.RemoveAll();
	
	SetConversation( NULL );
	
	if( pEngSpeaker ){
		pEngSpeaker->Stop();
		pEngSpeaker->SetSound( NULL );
		pEngSpeaker->FreeReference();
	}
	
	if( pEngEyesAnimatorInstance ){
		pEngEyesAnimatorInstance->SetAnimator( NULL );
		pEngEyesAnimatorInstance->SetComponent( NULL );
		pEngEyesAnimatorInstance->FreeReference();
	}
	if( pEngEyesAnimator ){
		pEngEyesAnimator->SetRig( NULL );
		pEngEyesAnimator->FreeReference();
	}
	
	if( pEngFacePoseAnimatorInstance ){
		pEngFacePoseAnimatorInstance->SetAnimator( NULL );
		pEngFacePoseAnimatorInstance->SetComponent( NULL );
		pEngFacePoseAnimatorInstance->FreeReference();
	}
	if( pEngFacePoseAnimator ){
		pEngFacePoseAnimator->SetRig( NULL );
		pEngFacePoseAnimator->FreeReference();
	}
	
	if( pEngGestureAnimatorInstance ){
		pEngGestureAnimatorInstance->SetAnimator( NULL );
		pEngGestureAnimatorInstance->SetComponent( NULL );
		pEngGestureAnimatorInstance->FreeReference();
	}
	
	if( pSpeechAnimation ){
		delete pSpeechAnimation;
	}
	
	if( pEngAnimatorInstance ){
		pEngAnimatorInstance->SetAnimator( NULL );
		pEngAnimatorInstance->SetComponent( NULL );
		pEngAnimatorInstance->FreeReference();
	}
	if( pEngComponent ){
		pEngComponent->FreeReference();
	}
}



void ceConversationActor::pUpdateComponent(){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	const igdeGameDefinition *gamedef = NULL;
	deModel *model = NULL;
	deSkin *skin = NULL;
	deRig *rig = NULL;
	
	if( pConversation ){
		gamedef = pConversation->GetGameDefinition();
	}
	
	// try to load the resources if possible
	try{
		if( ! pPathModel.IsEmpty() ){
			model = engine.GetModelManager()->LoadModel( pPathModel, "/" );
		}
		if( ! pPathSkin.IsEmpty() ){
			skin = engine.GetSkinManager()->LoadSkin( pPathSkin, "/" );
		}
		if( ! pPathRig.IsEmpty() ){
			rig = engine.GetRigManager()->LoadRig( pPathRig, "/" );
		}
		
	}catch( const deException &e ){
		if( model ){
			model->FreeReference();
		}
		if( skin ){
			skin->FreeReference();
		}
		if( rig ){
			rig->FreeReference();
		}
		pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// protect the loaded parts
	try{
		// if the skin is missing use the default one
		if( ! skin && gamedef ){
			skin = pEnvironment.GetErrorSkin();
			skin->AddReference();
		}
		
		// reset the animator
		pEngAnimatorInstance->SetComponent( NULL ); // otherwise the animator is not reset
		
		// update the component with the model and skin
		if( model && skin ){
			if( pEngComponent ){
				pEngComponent->SetModelAndSkin( model, skin );
				
			}else{
				pEngComponent = engine.GetComponentManager()->CreateComponent( model, skin );
				if( pConversation ){
					pConversation->GetEngineWorld()->AddComponent( pEngComponent );
				}
				pRepositionComponent();
			}
			
		}else if( pEngComponent ){
			if( pConversation ){
				pConversation->GetEngineWorld()->RemoveComponent( pEngComponent );
			}
			pEngComponent->FreeReference();
			pEngComponent = NULL;
		}
		
		// set the rig if the component exists
		if( pEngComponent ){
			pEngComponent->SetRig( rig );
			pEngComponent->SetVisible( true );
			pRepositionComponent();
		}
		
		// free the reference we hold
		if( rig ){
			rig->FreeReference();
			rig = NULL;
		}
		if( model ){
			model->FreeReference();
			model = NULL;
		}
		if( skin ){
			skin->FreeReference();
			skin = NULL;
		}
		
	}catch( const deException & ){
		if( model ){
			model->FreeReference();
		}
		if( skin ){
			skin->FreeReference();
		}
		if( rig ){
			rig->FreeReference();
		}
		throw;
	}
	
	// update the animator
	pEngAnimatorInstance->SetComponent( pEngComponent );
	pSpeechAnimation->GetEngineAnimatorInstance()->SetComponent( pEngComponent );
	pEngGestureAnimatorInstance->SetComponent( pEngComponent );
	pEngFacePoseAnimatorInstance->SetComponent( pEngComponent );
	pEngEyesAnimatorInstance->SetComponent( pEngComponent );
}

void ceConversationActor::pUpdateSpeechAnimation(){
	deVirtualFileSystem &vfs = *pEnvironment.GetEngineController()->GetEngine()->GetVirtualFileSystem();
	ceSpeechAnimationXML sanimationXML( pEnvironment.GetLogger(), LOGSOURCE );
	decBaseFileReader *reader = NULL;
	decPath pathFile;
	
	pSpeechAnimation->Clear();
	
	if( ! pPathSpeechAnimation.IsEmpty() ){
		try{
			pathFile.SetFromUnix( pPathSpeechAnimation );
			
			reader = vfs.OpenFileForReading( pathFile );
			sanimationXML.ReadFromFile( pPathSpeechAnimation, *reader, *pSpeechAnimation );
			
			reader->FreeReference();
			
		}catch( const deException & ){
			if( reader ){
				reader->FreeReference();
			}
		}
	}
	
	pSpeechAnimation->GetEngineAnimatorInstance()->SetComponent( pEngComponent );
	/*
	decUnicodeString text;
	text.AppendFromUTF8( "          " );
	text.AppendFromUTF8( "          " );
	text.AppendFromUTF8( "          " );
	text.AppendFromUTF8( "hǝləʊ" );
	text.AppendFromUTF8( "wɜrld" );
	text.AppendFromUTF8( "     " );
	text.AppendFromUTF8( "aɪm" );
	text.AppendFromUTF8( "ʤɒrʤɒ" );
	text.AppendFromUTF8( "vʌlentinɒ" );
	pSpeechAnimation->SetSpeakText( text );
	*/
}

void ceConversationActor::pUpdateFacePoseAnimator(){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	deVirtualFileSystem &vfs = *engine.GetVirtualFileSystem();
	igdeLoadAnimator animatorXML( pEnvironment, pEnvironment.GetLogger(), LOGSOURCE );
	decBaseFileReader *reader = NULL;
	deAnimator *animator = NULL;
	decPath pathFile;
	
	if( ! pPathFacePoseAnimator.IsEmpty() ){
		try{
			pathFile.SetFromUnix( pPathFacePoseAnimator );
			
			reader = vfs.OpenFileForReading( pathFile );
			animator = engine.GetAnimatorManager()->CreateAnimator();
			animatorXML.Load( pPathFacePoseAnimator, *animator, *reader );
			
			if( pEngFacePoseAnimator ){
				pEngFacePoseAnimator->FreeReference();
			}
			pEngFacePoseAnimator = animator;
			
			reader->FreeReference();
			
		}catch( const deException &e ){
			if( animator ){
				animator->FreeReference();
			}
			if( reader ){
				reader->FreeReference();
			}
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
		}
	}
	
	pEngFacePoseAnimatorInstance->SetAnimator( pEngFacePoseAnimator );
}

void ceConversationActor::pUpdateEyesAnimator(){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	deVirtualFileSystem &vfs = *engine.GetVirtualFileSystem();
	igdeLoadAnimator animatorXML( pEnvironment, pEnvironment.GetLogger(), LOGSOURCE );
	decBaseFileReader *reader = NULL;
	deAnimator *animator = NULL;
	decPath pathFile;
	
	if( ! pPathEyesAnimator.IsEmpty() ){
		try{
			pathFile.SetFromUnix( pPathEyesAnimator );
			
			reader = vfs.OpenFileForReading( pathFile );
			animator = engine.GetAnimatorManager()->CreateAnimator();
			animatorXML.Load( pPathEyesAnimator, *animator, *reader );
			
			if( pEngEyesAnimator ){
				pEngEyesAnimator->FreeReference();
			}
			pEngEyesAnimator = animator;
			
			reader->FreeReference();
			
		}catch( const deException &e ){
			if( animator ){
				animator->FreeReference();
			}
			if( reader ){
				reader->FreeReference();
			}
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
		}
	}
	
	pEngEyesAnimatorInstance->SetAnimator( pEngEyesAnimator );
	
	// update disabled eyes look-at bone index if present
	pPlayEyesBoneDisable = -1;
	if( pEngComponent->GetRig() ){
		pPlayEyesBoneDisable = pEngComponent->GetRig()->IndexOfBoneNamed( "disable eyes lookat" );
	}
}



void ceConversationActor::pRepositionComponent(){
	if( pEngComponent ){
		pEngComponent->SetPosition( decDVector( pPosition ) );
		pEngComponent->SetOrientation( decMatrix::CreateRotation( pOrientation * DEG2RAD ).ToQuaternion() );
	}
}

void ceConversationActor::pRepositionSpeaker(){
	// currently the speaker is placed 1.65 above the actor position. This is roughly where
	// the head is. A correct solution would be to track the head bone with an offset.
	// this will be done later on though. for the time being this solution is enough
	decMatrix matrix = decMatrix::CreateRotation( pOrientation * DEG2RAD );
	
	pEngSpeaker->SetPosition( decDVector( pPosition + matrix.TransformUp() * 1.65f ) );
	pEngSpeaker->SetOrientation( matrix.ToQuaternion() );
}



void ceConversationActor::pUpdateAnimatorInstance( float elapsed ){
	if( ! pActivePose ){
		return;
	}
	
	const ceActorControllerList &poseControllers = pActivePose->GetControllers();
	const int count = decMath::min( poseControllers.GetCount(), pEngAnimatorInstance->GetControllerCount() );
	int i;
	
	for( i=0; i<count; i++ ){
		deAnimatorController &animatorController = pEngAnimatorInstance->GetControllerAt( i );
		const ceActorController &poseController = *poseControllers.GetAt( i );
		
		switch( poseController.GetUpdateType() ){
		case ceActorController::eutConstant:
			animatorController.SetCurrentValue( poseController.GetValue() );
			break;
			
		case ceActorController::eutElapsedTime:
			animatorController.IncrementCurrentValue( elapsed );
			break;
			
		case ceActorController::eutHeadLeftRight:
			animatorController.SetCurrentValue( pHeadLeftRight );
			break;
			
		case ceActorController::eutHeadUpDown:
			animatorController.SetCurrentValue( pHeadUpDown );
			break;
			
		case ceActorController::eutEyesLeftRight:
			animatorController.SetCurrentValue( pEyesLeftRight );
			break;
			
		case ceActorController::eutEyesUpDown:
			animatorController.SetCurrentValue( pEyesUpDown );
			break;
			
		default:
			animatorController.SetCurrentValue( 0.0f );
		}
		
		animatorController.SetVector( poseController.GetVector() );
		
		pEngAnimatorInstance->NotifyControllerChangedAt( i );
	}
}

void ceConversationActor::pUpdatePlayGesture( float elapsed ){
	if( ! pPlayGestureRunning || pPlayGestureCount == 0 ){
		return;
	}
	
	ceActorGesture *actorGesture = NULL;
	deAnimator *engAnimator = NULL;
	bool resetAnimation = false;
	float startTime = 0.0f;
	float endTime = 0.0f;
	
	pPlayGestureElapsed += elapsed;
	
	while( pPlayGesturePos < pPlayGestureCount ){
		const float totalLength = pPlayGestures[ pPlayGesturePos ].pause
			+ pPlayGestures[ pPlayGesturePos ].length;
		bool hold = false;
		if( pPlayGestures[ pPlayGesturePos ].gesture ){
			hold = pPlayGestures[ pPlayGesturePos ].gesture->GetHold();
		}
		
		if( pPlayGestureElapsed < totalLength || hold ){
			startTime = pPlayGestures[ pPlayGesturePos ].pause;
			endTime = totalLength;
			
			if( pPlayGestureElapsed < startTime ){
				engAnimator = pEngGestureAnimatorInstance->GetAnimator();
				break; // use actorGesturePose = NULL during pause
			}
			
			if( ! pPlayGestures[ pPlayGesturePos ].gesture || ! pActivePose ){
				break;
			}
			
			actorGesture = pActivePose->GetGestures().GetNamed(
				pPlayGestures[ pPlayGesturePos ].gesture->GetAnimator() );
			
			if( ! actorGesture ){
				break;
			}
			
			engAnimator = actorGesture->GetEngineAnimator();
			break;
			
		}else{
			pPlayGestureElapsed -= totalLength;
			pPlayGesturePos++;
			resetAnimation = true;
		}
	}
	
	if( pPlayGesturePos >= pPlayGestureCount ){
		pPlayGestureRunning = false;
	}
	
	if( engAnimator != pEngGestureAnimatorInstance->GetAnimator() ){
		pEngGestureAnimatorInstance->SetAnimator( engAnimator );
		resetAnimation = true;
	}
	
	// update the gesture animator instance if there is an actor animator instance
	if( engAnimator ){
		const int poseControllerCount = pEngAnimatorInstance->GetControllerCount();
		int i;
		
		// copy actor pose controller values to the respective gesture controllers if existing
		for( i=0; i<poseControllerCount; i++ ){
			const deAnimatorController &controllerPose = pEngAnimatorInstance->GetControllerAt( i );
			const int index = pEngGestureAnimatorInstance->IndexOfControllerNamed( controllerPose.GetName() );
			if( index == -1 ){
				continue;
			}
			
			deAnimatorController &controllerGesture = pEngGestureAnimatorInstance->GetControllerAt( index );
			controllerGesture.SetCurrentValue( controllerPose.GetCurrentValue() );
			controllerGesture.SetVector( controllerPose.GetVector() );
			pEngGestureAnimatorInstance->NotifyControllerChangedAt( index );
		}
		
		// update gesture controllers if present
		const int indexControllerProgress = pEngGestureAnimatorInstance->IndexOfControllerNamed( pNameGestureProgress );
		const int indexControllerPlayback = pEngGestureAnimatorInstance->IndexOfControllerNamed( pNameGesturePlayback );
		const int indexControllerVariation = pEngGestureAnimatorInstance->IndexOfControllerNamed( pNameGestureVariation );
		
		if( resetAnimation ){
			if( indexControllerProgress != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerProgress ).SetValueRange( startTime, endTime );
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerProgress ).SetCurrentValue( startTime );
			}
			if( indexControllerPlayback != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerPlayback ).SetCurrentValue(
					pEngGestureAnimatorInstance->GetControllerAt( indexControllerPlayback ).GetMinimumValue() );
			}
			if( indexControllerVariation != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerVariation )
					.SetCurrentValue( decMath::random( 0.0f, 1.0f ) );
				pEngGestureAnimatorInstance->NotifyControllerChangedAt( indexControllerVariation );
			}
			
			pEngGestureAnimatorInstance->CaptureStateInto( 0 );
			
			if( indexControllerProgress != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerProgress ).SetCurrentValue( pPlayGestureElapsed );
				pEngGestureAnimatorInstance->NotifyControllerChangedAt( indexControllerProgress );
			}
			if( indexControllerPlayback != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerPlayback ).SetCurrentValue( pPlayGestureElapsed - startTime );
				pEngGestureAnimatorInstance->NotifyControllerChangedAt( indexControllerPlayback );
			}
			
		}else{
			if( indexControllerProgress != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerProgress ).IncrementCurrentValue( elapsed );
				pEngGestureAnimatorInstance->NotifyControllerChangedAt( indexControllerProgress );
			}
			if( indexControllerPlayback != -1 ){
				pEngGestureAnimatorInstance->GetControllerAt( indexControllerPlayback ).IncrementCurrentValue( elapsed );
				pEngGestureAnimatorInstance->NotifyControllerChangedAt( indexControllerPlayback );
			}
		}
	}
}

void ceConversationActor::pUpdatePlayFacePose( float elapsed ){
	const int controllerCount = pEngFacePoseAnimatorInstance->GetControllerCount();
	float blendFactor1 = 0.0f;
	float blendFactor2 = 1.0f;
	int i;
	
	if( pPlayFacePoseRunning && pPlayFacePoseCount > 0 ){
		pPlayFacePoseElapsed += elapsed;
		
		while( pPlayFacePosePos < pPlayFacePoseCount ){
			const float totalLength = pPlayFacePoses[ pPlayFacePosePos ].pause + pPlayFacePoses[ pPlayFacePosePos ].length;
			ceFacePose * const facePose = pPlayFacePoses[ pPlayFacePosePos ].facePose;
			
			if( pPlayFacePoseElapsed < totalLength ){
				if( pPlayCurFacePose != facePose ){
					if( pPlayLastFacePose ){
						pPlayLastFacePose->FreeReference();
					}
					pPlayLastFacePose = pPlayCurFacePose;
					pPlayCurFacePose = facePose;
					if( facePose ){
						facePose->AddReference();
					}
				}
				
				if( pPlayFacePoseElapsed < pPlayFacePoses[ pPlayFacePosePos ].pause ){
					blendFactor1 = 1.0f;
					blendFactor2 = 0.0f;
					
				}else{
					const float duration = pPlayFacePoses[ pPlayFacePosePos ].length;
					
					if( duration > 1e-5f ){
						blendFactor1 = ( pPlayFacePoseElapsed - pPlayFacePoses[ pPlayFacePosePos ].pause ) / duration;
						
						if( blendFactor1 < 1.0f ){
							//blendFactor1 = 0.5f + cosf( blendFactor1 * PI ) * 0.5f;
							//blendFactor2 = 1.0f - blendFactor1;
							blendFactor2 = blendFactor1 * blendFactor1 * blendFactor1 * ( blendFactor1 * ( blendFactor1 * 6.0f - 15.0f ) + 10.0f );
							blendFactor1 = 1.0f - blendFactor2;
							
						}else{
							blendFactor1 = 0.0f;
							blendFactor2 = 1.0f;
						}
					}
				}
				
				break;
				
			}else{
				if( pPlayLastFacePose ){
					pPlayLastFacePose->FreeReference();
				}
				pPlayLastFacePose = pPlayCurFacePose;
				pPlayCurFacePose = facePose;
				if( facePose ){
					facePose->AddReference();
				}
				
				pPlayFacePoseElapsed -= totalLength;
				pPlayFacePosePos++;
			}
		}
		
		if( pPlayFacePosePos >= pPlayFacePoseCount ){
			pPlayFacePoseRunning = false;
		}
		
	}
	
	// set the controller values. for this first all controllers are set to zero. then
	// the controller values of the last face pose (if existing) multiplied by the
	// first blend factor are set. then the controller values of the current face pose
	// (if existing) multiplied by the second blend factor are added. this way a proper
	// blending can be achieved no matter which combination of face poses (as well as
	// the lack thereof) is in effect
	for( i=0; i<controllerCount; i++ ){
		pEngFacePoseAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 0.0f );
	}
	
	if( pPlayLastFacePose ){
		const ceControllerValueList &list = pPlayLastFacePose->GetControllerList();
		const int entryCount = list.GetCount();
		
		for( i=0; i<entryCount; i++ ){
			const ceControllerValue &entry = *list.GetAt( i );
			const int controller = entry.GetController();
			
			if( controller >= 0 && controller < controllerCount ){
				pEngFacePoseAnimatorInstance->GetControllerAt( controller ).SetCurrentValue( entry.GetValue() * blendFactor1 );
			}
		}
	}
	
	if( pPlayCurFacePose ){
		const ceControllerValueList &list = pPlayCurFacePose->GetControllerList();
		const int entryCount = list.GetCount();
		
		for( i=0; i<entryCount; i++ ){
			const ceControllerValue &entry = *list.GetAt( i );
			const int controller = entry.GetController();
			
			if( controller >= 0 && controller < controllerCount ){
				pEngFacePoseAnimatorInstance->GetControllerAt( controller ).IncrementCurrentValue( entry.GetValue() * blendFactor2 );
			}
		}
	}
	
	for( i=0; i<controllerCount; i++ ){
		pEngFacePoseAnimatorInstance->NotifyControllerChangedAt( i );
	}
	
	// update blinking
	pBlinkNext -= elapsed;
	pBlinkElapsed += elapsed;
	
	if( pBlinkNext <= 0.0f ){
		pBlinkNext = decMath::random( pBlinkFreqMin, pBlinkFreqMax );
		pBlinkElapsed = 0.0f;
	}
	
	const int indexControllerBlink = pEngGestureAnimatorInstance->IndexOfControllerNamed( pNameGestureBlink );
	if( indexControllerBlink != -1 ){
		pEngFacePoseAnimatorInstance->GetControllerAt( indexControllerBlink ).SetCurrentValue( pBlinkElapsed );
		pEngFacePoseAnimatorInstance->NotifyControllerChangedAt( indexControllerBlink );
	}
	// end of hack
}

void ceConversationActor::pUpdatePlayHeadLookAt( cePlayback &playback, float elapsed ){
	decMatrix invActorMatrix;
	float blendFactor1 = 0.0f;
	float blendFactor2 = 1.0f;
	decVector lookAtPosition;
	float headLeftRight = 0.0f;
	float headUpDown = 0.0f;
	decMatrix targetMatrix;
	ceTarget *target;
	
	invActorMatrix = decMatrix::CreateRT( pOrientation * DEG2RAD, pPosition ).Invert();
	if( pEngComponent && pEngComponent->GetRig() && ! pBoneHeadRotator.IsEmpty() ){
		const int index = pEngComponent->GetRig()->IndexOfBoneNamed( pBoneHeadRotator );
		
		if( index != -1 ){
			pEngComponent->PrepareBones();
			invActorMatrix *= decDMatrix( pEngComponent->GetBoneAt( index ).GetInverseMatrix() );
// 			const decDVector p = invActorMatrix.Invert().GetPosition();
// 			printf( "%s: p(%f,%f,%f)\n", GetID().GetString(), p.x, p.y, p.z );
// 			const decVector b = pEngComponent->GetBoneAt( index ).GetMatrix().GetPosition();
// 			printf( "b(%f,%f,%f)\n", b.x, b.y, b.z );
		}
	}
	
	if( pPlayHeadLARunning && pPlayHeadLACount > 0 ){
		pPlayHeadLAElapsed += elapsed;
		
		while( pPlayHeadLAPos < pPlayHeadLACount ){
			const float totalLength = pPlayHeadLAs[ pPlayHeadLAPos ].pause + pPlayHeadLAs[ pPlayHeadLAPos ].duration;
			ceLookAt * const lookAt = pPlayHeadLAs[ pPlayHeadLAPos ].lookAt;
			
			if( pPlayHeadLAElapsed < totalLength ){
				if( pPlayCurHeadLA != lookAt ){
					if( pPlayLastHeadLA ){
						pPlayLastHeadLA->FreeReference();
					}
					pPlayLastHeadLA = pPlayCurHeadLA;
					pPlayCurHeadLA = lookAt;
					if( lookAt ){
						lookAt->AddReference();
					}
				}
				
				if( pPlayHeadLAElapsed < pPlayHeadLAs[ pPlayHeadLAPos ].pause ){
					blendFactor1 = 1.0f;
					blendFactor2 = 0.0f;
					
				}else{
					const float duration = pPlayHeadLAs[ pPlayHeadLAPos ].duration;
					
					if( duration > 1e-5f ){
						blendFactor1 = ( pPlayHeadLAElapsed - pPlayHeadLAs[ pPlayHeadLAPos ].pause ) / duration;
						
						if( blendFactor1 < 1.0f ){
							//blendFactor1 = 0.5f + cosf( blendFactor1 * PI ) * 0.5f;
							//blendFactor2 = 1.0f - blendFactor1;
							blendFactor2 = blendFactor1 * blendFactor1 * blendFactor1 * ( blendFactor1 * ( blendFactor1 * 6.0f - 15.0f ) + 10.0f );
							blendFactor1 = 1.0f - blendFactor2;
							
						}else{
							blendFactor1 = 0.0f;
							blendFactor2 = 1.0f;
						}
					}
				}
				
				break;
				
			}else{
				if( pPlayLastHeadLA ){
					pPlayLastHeadLA->FreeReference();
				}
				pPlayLastHeadLA = pPlayCurHeadLA;
				pPlayCurHeadLA = lookAt;
				if( lookAt ){
					lookAt->AddReference();
				}
				
				pPlayHeadLAElapsed -= totalLength;
				pPlayHeadLAPos++;
			}
		}
		
		if( pPlayHeadLAPos >= pPlayHeadLACount ){
			pPlayHeadLARunning = false;
		}
	}
	
	// determine the head left-right and up-down angles. for this the angles are calculated
	// for the last and current look-at and then weighted
	if( pPlayLastHeadLA ){
		target = NULL;
		if( ! pPlayLastHeadLA->GetTarget().IsEmpty() ){
			target = pConversation->GetTargetNamed( pPlayLastHeadLA->GetTarget() );
		}
		
		if( target ){
			target->GetCoordinateSystem( playback, targetMatrix );
			lookAtPosition = invActorMatrix * targetMatrix.GetPosition();
			
			float value = -atan2f( lookAtPosition.x, lookAtPosition.z ) * RAD2DEG;
			if( value < -90.0f ){
				value = -90.0f;
			}
			if( value > 90.0f ){
				value = 90.0f;
			}
			headLeftRight = value * blendFactor1;
			
			value = atan2f( lookAtPosition.y, decVector2( lookAtPosition.x, lookAtPosition.z ).Length() ) * RAD2DEG;
			headUpDown = value * blendFactor1;
		}
	}
	
	if( pPlayCurHeadLA ){
		target = NULL;
		if( ! pPlayCurHeadLA->GetTarget().IsEmpty() ){
			target = pConversation->GetTargetNamed( pPlayCurHeadLA->GetTarget() );
		}
		
		if( target ){
			target->GetCoordinateSystem( playback, targetMatrix );
			lookAtPosition = invActorMatrix * targetMatrix.GetPosition();
			
			float value = -atan2f( lookAtPosition.x, lookAtPosition.z ) * RAD2DEG;
			if( value < -90.0f ){
				value = -90.0f;
			}
			if( value > 90.0f ){
				value = 90.0f;
			}
			headLeftRight += value * blendFactor2;
			
			value = atan2f( lookAtPosition.y, decVector2( lookAtPosition.x, lookAtPosition.z ).Length() ) * RAD2DEG;
			headUpDown += value * blendFactor2;
		}
	}
	
	// apply the head rotation
	SetHeadLeftRight( headLeftRight );
	SetHeadUpDown( headUpDown );
}

void ceConversationActor::pUpdatePlayEyesLookAt( cePlayback &playback, float elapsed ){
	float blendFactor1 = 0.0f;
	float blendFactor2 = 1.0f;
	decVector lookAtPosition;
	float eyesLeftRight = 0.0f;
	float eyesUpDown = 0.0f;
	decMatrix targetMatrix;
	ceTarget *target;
	
	if( pPlayEyesLARunning && pPlayEyesLACount > 0 ){
		pPlayEyesLAElapsed += elapsed;
		
		while( pPlayEyesLAPos < pPlayEyesLACount ){
			const float totalLength = pPlayEyesLAs[ pPlayEyesLAPos ].pause + pPlayEyesLAs[ pPlayEyesLAPos ].duration;
			ceLookAt * const lookAt = pPlayEyesLAs[ pPlayEyesLAPos ].lookAt;
			
			if( pPlayEyesLAElapsed < totalLength ){
				if( pPlayCurEyesLA != lookAt ){
					if( pPlayLastEyesLA ){
						pPlayLastEyesLA->FreeReference();
					}
					pPlayLastEyesLA = pPlayCurEyesLA;
					pPlayCurEyesLA = lookAt;
					if( lookAt ){
						lookAt->AddReference();
					}
				}
				
				if( pPlayEyesLAElapsed < pPlayEyesLAs[ pPlayEyesLAPos ].pause ){
					blendFactor1 = 1.0f;
					blendFactor2 = 0.0f;
					
				}else{
					const float duration = pPlayEyesLAs[ pPlayEyesLAPos ].duration;
					
					if( duration > 1e-5f ){
						blendFactor1 = ( pPlayEyesLAElapsed - pPlayEyesLAs[ pPlayEyesLAPos ].pause ) / duration;
						
						if( blendFactor1 < 1.0f ){
							//blendFactor1 = 0.5f + cosf( blendFactor1 * PI ) * 0.5f;
							//blendFactor2 = 1.0f - blendFactor1;
							blendFactor2 = blendFactor1 * blendFactor1 * blendFactor1 * ( blendFactor1 * ( blendFactor1 * 6.0f - 15.0f ) + 10.0f );
							blendFactor1 = 1.0f - blendFactor2;
							
						}else{
							blendFactor1 = 0.0f;
							blendFactor2 = 1.0f;
						}
					}
				}
				
				break;
				
			}else{
				if( pPlayLastEyesLA ){
					pPlayLastEyesLA->FreeReference();
				}
				pPlayLastEyesLA = pPlayCurEyesLA;
				pPlayCurEyesLA = lookAt;
				if( lookAt ){
					lookAt->AddReference();
				}
				
				pPlayEyesLAElapsed -= totalLength;
				pPlayEyesLAPos++;
			}
		}
		
		if( pPlayEyesLAPos >= pPlayEyesLACount ){
			pPlayEyesLARunning = false;
		}
	}
	
	// determine the eyes left-right and up-down angles. for this the angles are calculated
	// for the last and current look-at and then weighted
	deComponentBone *boneHead = NULL;
	deRigBone *boneEyeRight = NULL;
	deRigBone *boneEyeLeft = NULL;
	
	if( pEngComponent ){
		pEngComponent->PrepareBones();
		
		if( pEngComponent->GetRig() ){
			const int indexEyeRight = pEngComponent->GetRig()->IndexOfBoneNamed( "eye.r" ); // has to be made better
			const int indexEyeLeft = pEngComponent->GetRig()->IndexOfBoneNamed( "eye.l" ); // has to be made better
			const int indexHead = pEngComponent->GetRig()->IndexOfBoneNamed( "head" ); // has to be made better
			
			if( indexHead != -1 ){
				boneHead = &pEngComponent->GetBoneAt( indexHead );
			}
			if( indexEyeLeft != -1 ){
				boneEyeLeft = &pEngComponent->GetRig()->GetBoneAt( indexEyeLeft );
			}
			if( indexEyeRight != -1 ){
				boneEyeRight = &pEngComponent->GetRig()->GetBoneAt( indexEyeRight );
			}
		}
	}
	
	if( boneHead && boneEyeLeft && boneEyeRight ){
		const decMatrix matrixEye = decMatrix::CreateTranslation( ( boneEyeLeft->GetPosition() + boneEyeRight->GetPosition() ) * 0.5f );
		const decMatrix invHeadMatrix = ( matrixEye * boneHead->GetMatrix() * decMatrix::CreateRT( pOrientation * DEG2RAD, pPosition ) ).Invert();
		
		if( pPlayLastEyesLA ){
			target = NULL;
			if( ! pPlayLastEyesLA->GetTarget().IsEmpty() ){
				target = pConversation->GetTargetNamed( pPlayLastEyesLA->GetTarget() );
			}
			
			if( target ){
				target->GetCoordinateSystem( playback, targetMatrix );
				lookAtPosition = invHeadMatrix * targetMatrix.GetPosition();
				
				float value = -atan2f( lookAtPosition.x, lookAtPosition.z ) * RAD2DEG;
				if( value < -90.0f ){
					value = -90.0f;
				}
				if( value > 90.0f ){
					value = 90.0f;
				}
				eyesLeftRight = value * blendFactor1;
				
				value = atan2f( lookAtPosition.y, decVector2( lookAtPosition.x, lookAtPosition.z ).Length() ) * RAD2DEG;
				eyesUpDown = value * blendFactor1;
			}
		}
		
		if( pPlayCurEyesLA ){
			target = NULL;
			if( ! pPlayCurEyesLA->GetTarget().IsEmpty() ){
				target = pConversation->GetTargetNamed( pPlayCurEyesLA->GetTarget() );
			}
			
			if( target ){
				target->GetCoordinateSystem( playback, targetMatrix );
				lookAtPosition = invHeadMatrix * targetMatrix.GetPosition();
				
				float value = -atan2f( lookAtPosition.x, lookAtPosition.z ) * RAD2DEG;
				if( value < -90.0f ){
					value = -90.0f;
				}
				if( value > 90.0f ){
					value = 90.0f;
				}
				eyesLeftRight += value * blendFactor2;
				
				value = atan2f( lookAtPosition.y, decVector2( lookAtPosition.x, lookAtPosition.z ).Length() ) * RAD2DEG;
				eyesUpDown += value * blendFactor2;
			}
		}
	}
	
	// use disable look-at bone if present
	if( pPlayEyesBoneDisable != -1 ){
		const float blend = decMath::linearStep( pEngComponent->GetBoneAt(
			pPlayEyesBoneDisable ).GetPosition().z, 0.0f, 0.1f, 1.0f, 0.0f );
		eyesLeftRight *= blend;
		eyesUpDown *= blend;
	}
	
	// apply the eyes rotation
	SetEyesLeftRight( eyesLeftRight );
	SetEyesUpDown( eyesUpDown );
	
	// set the controllers and apply the animator
	const int controllerCount = pEngEyesAnimatorInstance->GetControllerCount();
	
	if( controllerCount >= 1 ){
		pEngEyesAnimatorInstance->GetControllerAt( 0 ).SetCurrentValue( pEyesLeftRight );
		pEngEyesAnimatorInstance->NotifyControllerChangedAt( 0 );
	}
	if( controllerCount >= 2 ){
		pEngEyesAnimatorInstance->GetControllerAt( 1 ).SetCurrentValue( pEyesUpDown );
		pEngEyesAnimatorInstance->NotifyControllerChangedAt( 1 );
	}
}

void ceConversationActor::pUpdateBlinking( float elapsed ){
	pBlinkNext -= elapsed;
	pBlinkElapsed += elapsed;
	
	if( pBlinkNext < 0.0f ){
		const float factor = ( float )rand() / ( float )RAND_MAX;
		
		pBlinkNext = pBlinkFreqMin * ( 1.0f - factor ) + pBlinkFreqMax * factor;
		pBlinkElapsed = 0.0f;
	}
}
