/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeSAnimation.h"
#include "saeSAnimationListener.h"
#include "phoneme/saePhoneme.h"
#include "dictionary/saeWord.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationReference.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleReference.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigReference.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>



// Definitions
////////////////

#define LOGSOURCE "Speech Animator Editor"



// Class saeSAnimation
////////////////////////

// Constructor, destructor
////////////////////////////

saeSAnimation::saeSAnimation( igdeEnvironment* environment ) :
igdeEditableEntity( environment ),
pSky( NULL ),
pCamera( NULL ),
pActivePhoneme( NULL ),
pActiveWord( NULL ),
pDisplayMode( edmWord ),
pDispWordPos( -1 ),
pDispWordElapsed( 0.0f ),
pDirtyAnimator( true )
{
	deEngine * const engine = GetEngine();
	
	try{
		SetFilePath( "new.desanim" );
		
		// create world
		pEngWorld.TakeOver( engine->GetWorldManager()->CreateWorld() );
		pEngWorld->SetGravity( decVector( 0.0f, 0.0f, 0.0f ) );
		pEngWorld->SetDisableLights( false );
		pEngWorld->SetAmbientLight( decColor( 0.0f, 0.0f, 0.0f ) );
		
		// create camera
		pCamera = new igdeCamera( engine );
		pCamera->SetEngineWorld( pEngWorld );
		pCamera->Reset();
		pCamera->SetFov( 30.0f );
		pCamera->SetHighestIntensity( 20.0f );
		pCamera->SetLowestIntensity( 20.0f );
		pCamera->SetAdaptionTime( 0.0f );
		pCamera->SetDistance( 0.6f ); // 0.4
		pCamera->SetOrientation( decVector( 0.0f, 180.0f, 0.0f ) );
		pCamera->SetPosition( decDVector( 0.0, 1.6, 0.0 ) );
		
		// create sky
		pSky = new igdeWSky( *environment );
		pSky->SetGDDefaultSky();
		pSky->SetWorld( pEngWorld );
		
		// create animator
		pEngAnimator.TakeOver( engine->GetAnimatorManager()->CreateAnimator() );
		
		// create animator instance
		pEngAnimatorInstance.TakeOver( engine->GetAnimatorInstanceManager()->CreateAnimatorInstance() );
		pEngAnimatorInstance->SetAnimator( pEngAnimator );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

saeSAnimation::~saeSAnimation(){
	pCleanUp();
}



// Management
///////////////

void saeSAnimation::Dispose(){
	RemoveAllWords();
	RemoveAllPhonemes();
	GetUndoSystem()->RemoveAll();
}



void saeSAnimation::SetDisplayModelPath( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pDisplayModelPath.Equals( path ) ){
		pDisplayModelPath = path;
		pUpdateComponent();
		NotifyModelChanged();
	}
}

void saeSAnimation::SetDisplaySkinPath( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pDisplaySkinPath.Equals( path ) ){
		pDisplaySkinPath = path;
		pUpdateComponent();
		NotifyModelChanged();
	}
}

void saeSAnimation::SetDisplayRigPath( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pDisplayRigPath.Equals( path ) ){
		pDisplayRigPath = path;
		pUpdateComponent();
		NotifyModelChanged();
	}
}

void saeSAnimation::SetRigPath( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pRigPath.Equals( path ) ){
		pRigPath = path;
		pUpdateAnimRig();
		NotifySAnimationChanged();
	}
}

void saeSAnimation::SetAnimationPath( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	if( ! pAnimationPath.Equals( path ) ){
		pAnimationPath = path;
		pUpdateAnimRig();
		NotifySAnimationChanged();
	}
}

void saeSAnimation::SetNeutralMoveName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	if( ! pNeutralMoveName.Equals( name ) ){
		pNeutralMoveName = name;
		pDirtyAnimator = true;
		NotifySAnimationChanged();
	}
}



void saeSAnimation::SetDisplayMode( eDisplayModes displayMode ){
	if( displayMode < 0 || displayMode > edmWord ) DETHROW( deeInvalidParam );
	
	if( displayMode != pDisplayMode ){
		pDisplayMode = displayMode;
		pDispWordPos = -1;
		pDispWordElapsed = 0.0f;
		NotifyViewChanged();
	}
}



void saeSAnimation::RebuildAnimator(){
	if( pDirtyAnimator ){
		const int phonemeCount = pPhonemeList.GetCount();
		int i, j;
		
		// remove dirty early to avoid race condition if something goes wrong
		pDirtyAnimator = false;
		
		// clear the engine controllers of all phonemes
		for( i=0; i<phonemeCount; i++ ){
			pPhonemeList.GetAt( i )->SetEngineController( -1 );
		}
		
		// remove the animator from the instance just for safety
		pEngAnimatorInstance->SetAnimator( NULL );
		
		// rebuild the animator. for each animation move used in all phonemes
		// one controller and one animation rule is added. these are considered
		// the used visemes of all phonemes. the controller is used to set the
		// blend strength of each viseme. The matching controllers are then
		// assigned to each phoneme. Multiple phonemes can therefore use the
		// same controller.
		pEngAnimator->RemoveAllRules();
		pEngAnimator->RemoveAllLinks();
		pEngAnimator->RemoveAllControllers();
		
		// reset the animation state to a well known state for testing purpose
		deAnimatorRuleReference engRule;
		engRule.TakeOver( new deAnimatorRuleStateManipulator );
		( ( deAnimatorRuleStateManipulator& )( deAnimatorRule& )engRule ).SetEnableRotation( true );
		( ( deAnimatorRuleStateManipulator& )( deAnimatorRule& )engRule ).SetEnablePosition( true );
		( ( deAnimatorRuleStateManipulator& )( deAnimatorRule& )engRule ).SetEnableSize( false );
		engRule->SetEnabled( true );
		engRule->SetBlendMode( deAnimatorRule::ebmBlend );
		engRule->SetBlendFactor( 1.0f );
		pEngAnimator->AddRule( engRule );
		
		// add rule for the neutral position
		engRule.TakeOver( new deAnimatorRuleAnimation );
		( ( deAnimatorRuleAnimation& )( deAnimatorRule& )engRule ).SetMoveName( pNeutralMoveName.GetString() );
		( ( deAnimatorRuleAnimation& )( deAnimatorRule& )engRule ).SetMoveTime( 0.0f );
		engRule->SetEnabled( true );
		engRule->SetBlendMode( deAnimatorRule::ebmBlend );
		engRule->SetBlendFactor( 1.0f );
		pEngAnimator->AddRule( engRule );
		
		// add all unique visemes used by all phonemes
		for( i=0; i<phonemeCount; i++ ){
			saePhoneme &phoneme = *pPhonemeList.GetAt( i );
			
			if( phoneme.GetEngineController() == -1 ){
				const decString &moveName = pPhonemeList.GetAt( i )->GetMoveName();
				
				// add a controller for the new viseme
				const int controllerIndex = pEngAnimator->GetControllerCount();
				
				deAnimatorController * const engController = new deAnimatorController;
				engController->SetValueRange( 0.0f, 1.0f );
				engController->SetClamp( true );
				engController->SetCurrentValue( 0.0f );
				pEngAnimator->AddController( engController );
				
				// add a link for the new viseme
				const int linkIndex = pEngAnimator->GetLinkCount();
				
				deAnimatorLink * const engLink = new deAnimatorLink;
				engLink->SetController( controllerIndex );
				decCurveBezier curve;
				curve.SetDefaultLinear();
				engLink->SetCurve( curve );
				pEngAnimator->AddLink( engLink );
				
				// add an animation rule for the new viseme
				engRule.TakeOver( new deAnimatorRuleAnimation );
				( ( deAnimatorRuleAnimation& )( deAnimatorRule& )engRule ).SetMoveName( phoneme.GetMoveName() );
				( ( deAnimatorRuleAnimation& )( deAnimatorRule& )engRule ).SetMoveTime( 0.0f );
				engRule->SetEnabled( true );
				engRule->SetBlendMode( deAnimatorRule::ebmOverlay );
				engRule->SetBlendFactor( 1.0f );
				engRule->GetTargetBlendFactor().AddLink( linkIndex );
				pEngAnimator->AddRule( engRule );
				
				// assign the controller to this phoneme and all other phonemes
				// using the same move name
				phoneme.SetEngineController( controllerIndex );
				
				for( j=i+1; j<phonemeCount; j++ ){
					saePhoneme &phoneme2 = *pPhonemeList.GetAt( j );
					
					if( moveName.Equals( phoneme2.GetMoveName() ) ){
						phoneme2.SetEngineController( controllerIndex );
					}
				}
			}
		}
		
		// reassign the animator
		pEngAnimatorInstance->SetAnimator( pEngAnimator );
	}
}



void saeSAnimation::Reset(){
	GetUndoSystem()->RemoveAll();
}

void saeSAnimation::Update( float elapsed ){
	RebuildAnimator();
	
	switch( pDisplayMode ){
	case edmPhoneme:{
		const int controllerCount = pEngAnimatorInstance->GetControllerCount();
		int i;
		
		if( pActivePhoneme ){
			const int controllerIndex = pActivePhoneme->GetEngineController();
			
			for( i=0; i<controllerCount; i++ ){
				if( i == controllerIndex ){
					pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 1.0f );
					
				}else{
					pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 0.0f );
				}
				
				pEngAnimatorInstance->NotifyControllerChangedAt( i );
			}
			
		}else{
			for( i=0; i<controllerCount; i++ ){
				pEngAnimatorInstance->GetControllerAt( i ).SetCurrentValue( 0.0f );
				pEngAnimatorInstance->NotifyControllerChangedAt( i );
			}
		}
		}break;
		
	case edmWord:{
		if( ! pActiveWord ){
			break;
		}
		
		const float talkSpeed = 1.0f;
		const int controllerCount = pEngAnimatorInstance->GetControllerCount();
		const decUnicodeString &phonetics = pActiveWord->GetPhonetics();
		const int phoneticsLen = phonetics.GetLength();
		const float wordGapTime = 2.0f * talkSpeed;
		const float windUpTime = 0.1f * talkSpeed;
		const float pauseTime = 0.1f * talkSpeed;
		saePhoneme *phoneme1 = NULL;
		saePhoneme *phoneme2 = NULL;
		float blendFactor = 1.0f;
		int controller1 = -1;
		int controller2 = -1;
		float phonemeLen;
		int i;
		
		// determine which phoneme to display and the blending
		pDispWordElapsed += elapsed;
		
		while( true ){
			phoneme1 = NULL;
			phoneme2 = NULL;
			
			if( pDispWordPos >= 0 && pDispWordPos < phoneticsLen ){
				phoneme1 = pPhonemeList.GetIPA( phonetics.GetAt( pDispWordPos ) );
			}
			
			if( pDispWordPos + 1 < phoneticsLen ){
				phoneme2 = pPhonemeList.GetIPA( phonetics.GetAt( pDispWordPos + 1 ) );
			}
			
			if( pDispWordPos == -1 ){
				phonemeLen = windUpTime;
				
			}else if( pDispWordPos == phoneticsLen ){
				phonemeLen = wordGapTime;
				
			}else if( phoneme1 ){
				phonemeLen = phoneme1->GetLength() * talkSpeed;
				
			}else{
				phonemeLen = pauseTime;
			}
			
			if( pDispWordElapsed < phonemeLen ){
				//if( pDispWordElapsed < phonemeLen - windUpTime ){
				//	blendFactor = 1.0f;
				//	
				//}else{
					//blendFactor = ( phonemeLen - pDispWordElapsed ) / windUpTime;
					//blendFactor = 1.0f - pDispWordElapsed / phonemeLen;
					blendFactor = 0.5f + cosf( ( pDispWordElapsed / phonemeLen ) * PI ) * 0.5f;
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
			
			if( pDispWordPos < phoneticsLen ){
				pDispWordPos++;
				
			}else{
				pDispWordPos = -1;
			}
			
			pDispWordElapsed -= phonemeLen;
		}
		
		// update the controllers
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
		}break;
		
	default:
		break;
	}
	
	pEngAnimatorInstance->Apply();
	pEngWorld->Update( elapsed );
}



// Phonemes
/////////////

void saeSAnimation::AddPhoneme( saePhoneme *phoneme ){
	pPhonemeList.Add( phoneme );
	phoneme->SetSAnimation( this );
	NotifyPhonemeStructureChanged();
	
	if( ! pActivePhoneme ){
		SetActivePhoneme( phoneme );
	}
}

void saeSAnimation::RemovePhoneme( saePhoneme *phoneme ){
	if( ! phoneme || phoneme->GetSAnimation() != this ) DETHROW( deeInvalidParam );
	
	if( phoneme->GetActive() ){
		if( pPhonemeList.GetCount() == 1 ){
			SetActivePhoneme( NULL );
			
		}else{
			if( pPhonemeList.GetAt( 0 ) == phoneme ){
				SetActivePhoneme( pPhonemeList.GetAt( 1 ) );
				
			}else{
				SetActivePhoneme( pPhonemeList.GetAt( 0 ) );
			}
		}
	}
	
	phoneme->SetSAnimation( NULL );
	pPhonemeList.Remove( phoneme );
	NotifyPhonemeStructureChanged();
}

void saeSAnimation::RemoveAllPhonemes(){
	const int count = pPhonemeList.GetCount();
	int i;
	
	SetActivePhoneme( NULL );
	
	for( i=0; i<count; i++ ){
		pPhonemeList.GetAt( i )->SetSAnimation( NULL );
	}
	pPhonemeList.RemoveAll();
	NotifyPhonemeStructureChanged();
}

bool saeSAnimation::HasActivePhoneme() const{
	return pActivePhoneme != NULL;
}

void saeSAnimation::SetActivePhoneme( saePhoneme *phoneme ){
	if( phoneme == pActivePhoneme ){
		return;
	}
	
	if( pActivePhoneme ){
		pActivePhoneme->SetActive( false );
		pActivePhoneme->FreeReference();
	}
	
	pActivePhoneme = phoneme;
	
	if( phoneme ){
		phoneme->AddReference();
		phoneme->SetActive( true );
	}
	
	NotifyActivePhonemeChanged();
}



// Words
/////////////

void saeSAnimation::AddWord( saeWord *word ){
	pWordList.Add( word );
	word->SetSAnimation( this );
	NotifyWordStructureChanged();
	
	if( ! pActiveWord ){
		SetActiveWord( word );
	}
}

void saeSAnimation::RemoveWord( saeWord *word ){
	if( ! word || word->GetSAnimation() != this ) DETHROW( deeInvalidParam );
	
	if( word->GetActive() ){
		if( pWordList.GetCount() == 1 ){
			SetActiveWord( NULL );
			
		}else{
			if( pWordList.GetAt( 0 ) == word ){
				SetActiveWord( pWordList.GetAt( 1 ) );
				
			}else{
				SetActiveWord( pWordList.GetAt( 0 ) );
			}
		}
	}
	
	word->SetSAnimation( NULL );
	pWordList.Remove( word );
	NotifyWordStructureChanged();
}

void saeSAnimation::RemoveAllWords(){
	const int count = pWordList.GetCount();
	int i;
	
	SetActiveWord( NULL );
	
	for( i=0; i<count; i++ ){
		pWordList.GetAt( i )->SetSAnimation( NULL );
	}
	pWordList.RemoveAll();
	NotifyWordStructureChanged();
}

bool saeSAnimation::HasActiveWord() const{
	return pActiveWord != NULL;
}

void saeSAnimation::SetActiveWord( saeWord *word ){
	if( word == pActiveWord ){
		return;
	}
	
	if( pActiveWord ){
		pActiveWord->SetActive( false );
		pActiveWord->FreeReference();
	}
	
	pActiveWord = word;
	
	if( word ){
		word->AddReference();
		word->SetActive( true );
	}
	
	pDispWordPos = -1;
	pDispWordElapsed = 0.0f;
	
	NotifyActiveWordChanged();
}



// Listeners
//////////////

void saeSAnimation::AddListener( saeSAnimationListener *listener ){
	if( ! listener ) DETHROW( deeInvalidParam );
	
	pListeners.Add( listener );
}

void saeSAnimation::RemoveListener( saeSAnimationListener *listener ){
	pListeners.RemoveIfPresent( listener );
}



void saeSAnimation::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->StateChanged( this );
	}
}

void saeSAnimation::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->UndoChanged( this );
	}
}

void saeSAnimation::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->ViewChanged( this );
	}
}

void saeSAnimation::NotifySAnimationChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->SAnimationChanged( this );
	}
	
	SetChanged( true );
}

void saeSAnimation::NotifyModelChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->ModelChanged( this );
	}
	
	SetChanged( true );
}

void saeSAnimation::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->SkyChanged( this );
	}
}

void saeSAnimation::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->CameraChanged( this );
	}
}



void saeSAnimation::NotifyPhonemeStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->PhonemeStructureChanged( this );
	}
	
	SetChanged( true );
	pDirtyAnimator = true;
}

void saeSAnimation::NotifyPhonemeChanged( saePhoneme *phoneme ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->PhonemeChanged( this, phoneme );
	}
	
	SetChanged( true );
	pDirtyAnimator = true;
}

void saeSAnimation::NotifyActivePhonemeChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->ActivePhonemeChanged( this );
	}
}



void saeSAnimation::NotifyWordStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->WordStructureChanged( this );
	}
	
	SetChanged( true );
}

void saeSAnimation::NotifyWordNameChanged( saeWord *word ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( i ) )->WordNameChanged( this, word );
	}
	
	SetChanged( true );
}

void saeSAnimation::NotifyWordChanged( saeWord *word ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->WordChanged( this, word );
	}
	
	SetChanged( true );
}

void saeSAnimation::NotifyActiveWordChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( saeSAnimationListener* )pListeners.GetAt( l ) )->ActiveWordChanged( this );
	}
}



// Private Functions
//////////////////////

void saeSAnimation::pCleanUp(){
	pListeners.RemoveAll();
	
	SetActiveWord( NULL );
	RemoveAllWords();
	
	SetActivePhoneme( NULL );
	RemoveAllPhonemes();
	
	if( pSky ){
		delete pSky;
	}
	if( pCamera ){
		delete pCamera;
	}
}



void saeSAnimation::pUpdateComponent(){
	const igdeGameDefinition * const gamedef = GetGameDefinition();
	deEngine &engine = *GetEngine();
	deModelReference displayModel;
	deSkinReference displaySkin;
	deRigReference displayRig;
	
	// try to load the resources if possible
	try{
		if( ! pDisplayModelPath.IsEmpty() ){
			displayModel.TakeOver( engine.GetModelManager()->LoadModel( pDisplayModelPath, GetDirectoryPath() ) );
		}
		if( ! pDisplaySkinPath.IsEmpty() ){
			displaySkin.TakeOver( engine.GetSkinManager()->LoadSkin( pDisplaySkinPath, GetDirectoryPath() ) );
		}
		if( ! pDisplayRigPath.IsEmpty() ){
			displayRig.TakeOver( engine.GetRigManager()->LoadRig( pDisplayRigPath, GetDirectoryPath() ) );
		}
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// if the skin is missing use the default one
	if( ! displaySkin && gamedef ){
		displaySkin = gamedef->GetDefaultSkin();
	}
	
	// reset the animator
	pEngAnimatorInstance->SetComponent( NULL ); // otherwise the animator is not reset
	
	// update the component with the model and skin
	if( displayModel && displaySkin ){
		if( pEngComponent ){
			pEngComponent->SetModelAndSkin( displayModel, displaySkin );
			
		}else{
			pEngComponent.TakeOver( engine.GetComponentManager()->CreateComponent( displayModel, displaySkin ) );
			pEngWorld->AddComponent( pEngComponent );
		}
		
	}else if( pEngComponent ){
		pEngWorld->RemoveComponent( pEngComponent );
		pEngComponent = NULL;
	}
	
	// set the rig if the component exists
	if( pEngComponent ){
		pEngComponent->SetRig( displayRig );
		pEngComponent->SetVisible( true );
		pEngComponent->SetPosition( decDVector() );
		pEngComponent->SetOrientation( decQuaternion() );
	}
	
	// update the animator
	pEngAnimatorInstance->SetComponent( pEngComponent );
}

void saeSAnimation::pUpdateAnimRig(){
	deEngine &engine = *GetEngine();
	deAnimationReference animation;
	deRigReference rig;
	
	try{
		if( ! pAnimationPath.IsEmpty() ){
			animation.TakeOver( engine.GetAnimationManager()->LoadAnimation(
				pAnimationPath, GetDirectoryPath() ) );
		}
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
	}
	pEngAnimator->SetAnimation( animation );
	
	try{
		if( ! pRigPath.IsEmpty() ){
			rig.TakeOver( engine.GetRigManager()->LoadRig( pRigPath, GetDirectoryPath() ) );
		}
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
	}
	pEngAnimator->SetRig( rig );
}
