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

#include "ceConversation.h"
#include "ceConversationListener.h"
#include "action/ceConversationAction.h"
#include "actor/ceConversationActor.h"
#include "camerashot/ceCameraShot.h"
#include "facepose/ceFacePose.h"
#include "gesture/ceGesture.h"
#include "file/ceConversationFile.h"
#include "target/ceTarget.h"
#include "topic/ceConversationTopic.h"
#include "textbox/ceTextBox.h"
#include "playerChoiceBox/cePlayerChoiceBox.h"
#include "playback/cePlayback.h"
#include "infobox/ceConversationInfoBox.h"
#include "coordsystem/ceCoordSystem.h"
#include "prop/ceProp.h"
#include "../loadsave/ceLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
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
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deMicrophoneManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/sound/deSpeaker.h>



// Definitions
////////////////

#define LOGSOURCE "Speech Animator Editor"



// Class ceConversation
////////////////////////

// Constructor, destructor
////////////////////////////

ceConversation::ceConversation( igdeEnvironment *environment ) : igdeEditableEntity( environment ){
	deEngine * const engine = GetEngine();
	
	pSky = nullptr;
	pEnvObject = nullptr;
	
	pEngMicrophone = nullptr;
	pEngSpeakerVAPreview = nullptr;
	
	pActiveTarget = nullptr;
	pActiveCameraShot = nullptr;
	pActiveGesture = nullptr;
	pActiveFacePose = nullptr;
	pActiveFile = nullptr;
	pActiveActor = nullptr;
	pActiveCoordSystem = nullptr;
	pActiveProp = nullptr;
	
	pCamera = nullptr;
	pCameraFree = nullptr;
	pTextBox = nullptr;
	pPlayerChoiceBox = nullptr;
	pInfoBox = nullptr;
	pPlayback = nullptr;
	
	pScreenRatio = 1.6f; // 16:10 wide screen
	pShowRuleOfThirdsAid = false;
	
	pPasteSnippetDialogParams.delayCameraShot = 0.5f;
	pPasteSnippetDialogParams.delayActorSpeak = 0.5f;
	pPasteSnippetDialogParams.scaleActorSpeak = 1.0f;
	
	try{
		SetFilePath( "new.deconvo" );
		pCTSPath = "test.dects";
		pCTAPath = "test.decta";
		pCTFIPath = "test.dectfi";
		pLangPackPath = "/content/languages/english.delangpack";
		pLangPackEntryName = "convo.group.topic.s1";
		
		// create world
		pEngWorld.TakeOver( engine->GetWorldManager()->CreateWorld() );
		pEngWorld->SetGravity( decVector( 0.0f, 0.0f, 0.0f ) );
		pEngWorld->SetDisableLights( false );
		pEngWorld->SetAmbientLight( decColor( 0.0f, 0.0f, 0.0f ) );
		
		// create camera
		pCamera = new igdeCamera( engine );
		
		pCamera->SetEngineWorld( pEngWorld );
		
		pCamera->Reset();
		pCamera->SetFov( 90.0f );
		pCamera->SetHighestIntensity( 20.0f );
		pCamera->SetLowestIntensity( 20.0f );
		pCamera->SetAdaptionTime( 0.0f );
		pCamera->SetDistance( 0.0f );
		pCamera->SetOrientation( decVector( 0.0f, 180.0f, 0.0f ) );
		pCamera->SetPosition( decDVector( 0.0, 1.5, 5.0 ) );
		
		// create free camera
		pCameraFree = new igdeCamera( engine );
		pCameraFree->Reset();
		pCameraFree->SetFov( 90.0f );
		pCameraFree->SetHighestIntensity( 20.0f );
		pCameraFree->SetLowestIntensity( 20.0f );
		pCameraFree->SetAdaptionTime( 0.0f );
		pCameraFree->SetDistance( 0.0f );
		pCameraFree->SetOrientation( decVector( 0.0f, 180.0f, 0.0f ) );
		pCameraFree->SetPosition( decDVector( 0.0, 1.5, 5.0 ) );
		
		// create microphone
		decLayerMask layerMaskAudio;
		layerMaskAudio.SetBit( elmAudio );
		
		pEngMicrophone.TakeOver( engine->GetMicrophoneManager()->CreateMicrophone() );
		pEngMicrophone->SetMuted( false );
		pEngMicrophone->SetType( deMicrophone::emtPoint ); // directed in fact but that's for later
		pEngMicrophone->SetLayerMask( layerMaskAudio );
		pEngWorld->AddMicrophone( pEngMicrophone );
		
		// create voice audio preview speaker
		pEngSpeakerVAPreview.TakeOver( engine->GetSpeakerManager()->CreateSpeaker() );
		pEngSpeakerVAPreview->SetLooping( false );
		pEngSpeakerVAPreview->SetMuted( false );
		pEngSpeakerVAPreview->SetPlaySpeed( 1.0f );
		pEngSpeakerVAPreview->SetLayerMask( layerMaskAudio );
		pEngMicrophone->AddSpeaker( pEngSpeakerVAPreview );
		
		// create sky
		
		pSky = new igdeWSky( *environment );
		pSky->SetGDDefaultSky();
		pSky->SetWorld( pEngWorld );
		
		// create the environment wrapper object
		pEnvObject = new igdeWObject( *environment );
		pEnvObject->SetWorld( pEngWorld );
		pEnvObject->SetPosition( decDVector( 0.0, 0.0, 0.0 ) );
		
		decLayerMask layerMaskCollider;
		layerMaskCollider.SetBit( 0 );
		pEnvObject->SetCollisionFilter( decCollisionFilter( layerMaskCollider ) ); // just so something collides
		pEnvObject->SetCollisionFilterFallback( decCollisionFilter( layerMaskCollider ) ); // just so something collides
		
		pEnvObject->SetGDClassName( "IGDETestTerrain" );
		
		// create text box
		pTextBox = new ceTextBox( *engine, *GetLogger() );
		
		// create player choice box
		pPlayerChoiceBox = new cePlayerChoiceBox( *this );
		
		// create info box
		pInfoBox = new ceConversationInfoBox( *this );
		
		// Create playback
		pPlayback = new cePlayback( *this );
		
	}catch( const deException &e ){
		pCleanUp();
		e.PrintError();
		throw;
	}
}

ceConversation::~ceConversation(){
	pCleanUp();
}



// Management
///////////////

void ceConversation::Dispose(){
	RemoveAllProps();
	RemoveAllCoordSystems();
	RemoveAllActors();
	RemoveAllFiles();
	RemoveAllFacePoses();
	RemoveAllGestures();
	RemoveAllCameraShots();
	RemoveAllTargets();
	
	const int iccount = pImportedConversations.GetCount();
	int i;
	for( i=0; i<iccount; i++ ){
		pImportedConversations.GetAt( i )->Dispose();
	}
	pImportedConversations.RemoveAll();
	
	GetUndoSystem()->RemoveAll();
}



void ceConversation::SetScreenRatio( float ratio ){
	if( fabsf( ratio - pScreenRatio ) > 1e-5f ){
		pScreenRatio = ratio;
		NotifyConversationChanged();
	}
}

void ceConversation::SetShowRuleOfThirdsAid( bool showRuleOfThirdsAid ){
	if( showRuleOfThirdsAid != pShowRuleOfThirdsAid ){
		pShowRuleOfThirdsAid = showRuleOfThirdsAid;
		NotifyViewChanged();
	}
}



void ceConversation::Reset(){
	GetUndoSystem()->RemoveAll();
}

void ceConversation::ActivateMicrophone(){
	GetEngine()->GetAudioSystem()->SetActiveMicrophone( pEngMicrophone );
}

void ceConversation::Update( float elapsed ){
	pPlayback->Update( elapsed );
	
	const int count = pActorList.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pActorList.GetAt( i )->Update( *pPlayback, elapsed );
	}
	
	pEnvObject->Update( elapsed );
	
	pEngWorld->Update( elapsed );
}



void ceConversation::SetCTSPath( const char *path ){
	pCTSPath = path;
}

void ceConversation::SetCTAPath( const char *path ){
	pCTAPath = path;
}

void ceConversation::SetCTFIPath( const char *path ){
	pCTFIPath = path;
}

void ceConversation::SetLangPackPath( const char *path ){
	pLangPackPath = path;
}

void ceConversation::SetLangPackEntryName( const char *name ){
	pLangPackEntryName = name;
}

void ceConversation::SetLanguagePack( ceLangPack *langpack ){
	if( langpack == pLangPack ){
		return;
	}
	
	pLangPack = langpack;
	NotifyLanguagePackChanged();
}



// Imported conversations
///////////////////////////

void ceConversation::SetImportConversationPath( const decStringList &list ){
	pImportConversationPath = list;
}

void ceConversation::UpdateImportedConversations( ceLoadSaveSystem &lssystem ){
	// we reload all conversation to be on the save side. this is less efficient than keeping
	// alread loaded conversations around but avoids potential problems if files changed
	const int iccount = pImportedConversations.GetCount();
	int i;
	for( i=0; i<iccount; i++ ){
		pImportedConversations.GetAt( i )->Dispose();
	}
	pImportedConversations.RemoveAll();
	
	const int count = pImportConversationPath.GetCount();
	
	for( i=0; i<count; i++ ){
		try{
			pImportedConversations.Add( Ref::New( lssystem.LoadConversation(
				decPath::AbsolutePathUnix( pImportConversationPath.GetAt( i ),
					GetDirectoryPath() ).GetPathUnix() ) ) );
			
		}catch( const deException &e ){
			GetLogger()->LogException( LOGSOURCE, e );
			// ignore unusable files
		}
	}
}



// Targets
////////////

void ceConversation::AddTarget( ceTarget *target ){
	if( ! target || pTargetList.HasNamed( target->GetName().GetString() ) || target->GetConversation() ){
		DETHROW( deeInvalidParam );
	}
	
	pTargetList.Add( target );
	target->SetConversation( this );
	NotifyTargetStructureChanged();
	
	if( ! pActiveTarget ){
		SetActiveTarget( target );
	}
}

void ceConversation::RemoveTarget( ceTarget *target ){
	if( ! target || ! pTargetList.Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( target == pActiveTarget ){
		if( pTargetList.GetCount() == 1 ){
			SetActiveTarget( nullptr );
			
		}else{
			if( pTargetList.GetAt( 0 ) == target ){
				SetActiveTarget( pTargetList.GetAt( 1 ) );
				
			}else{
				SetActiveTarget( pTargetList.GetAt( 0 ) );
			}
		}
	}
	
	target->SetConversation( nullptr );
	pTargetList.Remove( target );
	NotifyTargetStructureChanged();
}

void ceConversation::RemoveAllTargets(){
	const int count = pTargetList.GetCount();
	int i;
	
	SetActiveTarget( nullptr );
	
	for( i=0; i<count; i++ ){
		pTargetList.GetAt( i )->SetConversation( nullptr );
	}
	pTargetList.RemoveAll();
	NotifyTargetStructureChanged();
}

void ceConversation::SetActiveTarget( ceTarget *target ){
	if( target != pActiveTarget ){
		if( pActiveTarget ){
			pActiveTarget->FreeReference();
		}
		
		pActiveTarget = target;
		
		if( target ){
			target->AddReference();
		}
		
		NotifyActiveTargetChanged();
	}
}

ceTarget *ceConversation::GetTargetNamed( const char *name ) const{
	ceTarget *target = pTargetList.GetNamed( name );
	if( target ){
		return target;
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		target = pImportedConversations.GetAt( i )->GetTargetNamed( name );
		if( target ){
			return target;
		}
	}
	
	return nullptr;
}

ceTargetList ceConversation::AllTargets() const{
	ceTargetList list;
	AllTargets( list );
	return list;
}

void ceConversation::AllTargets( ceTargetList &list ) const{
	list += pTargetList;
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllTargets( list );
	}
}



// Camera Shots
/////////////////

void ceConversation::AddCameraShot( ceCameraShot *cameraShot ){
	if( ! cameraShot || pCameraShotList.HasNamed( cameraShot->GetName().GetString() ) || cameraShot->GetConversation() ){
		DETHROW( deeInvalidParam );
	}
	
	pCameraShotList.Add( cameraShot );
	cameraShot->SetConversation( this );
	NotifyCameraShotStructureChanged();
	
	if( ! pActiveCameraShot ){
		SetActiveCameraShot( cameraShot );
	}
}

void ceConversation::RemoveCameraShot( ceCameraShot *cameraShot ){
	if( ! cameraShot || ! pCameraShotList.Has( cameraShot ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( cameraShot == pActiveCameraShot ){
		if( pCameraShotList.GetCount() == 1 ){
			SetActiveCameraShot( nullptr );
			
		}else{
			if( pCameraShotList.GetAt( 0 ) == cameraShot ){
				SetActiveCameraShot( pCameraShotList.GetAt( 1 ) );
				
			}else{
				SetActiveCameraShot( pCameraShotList.GetAt( 0 ) );
			}
		}
	}
	
	cameraShot->SetConversation( nullptr );
	pCameraShotList.Remove( cameraShot );
	NotifyCameraShotStructureChanged();
}

void ceConversation::RemoveAllCameraShots(){
	const int count = pCameraShotList.GetCount();
	int i;
	
	SetActiveCameraShot( nullptr );
	
	for( i=0; i<count; i++ ){
		pCameraShotList.GetAt( i )->SetConversation( nullptr );
	}
	pCameraShotList.RemoveAll();
	NotifyCameraShotStructureChanged();
}

void ceConversation::SetActiveCameraShot( ceCameraShot *cameraShot ){
	if( cameraShot != pActiveCameraShot ){
		if( pActiveCameraShot ){
			pActiveCameraShot->FreeReference();
		}
		
		pActiveCameraShot = cameraShot;
		
		if( cameraShot ){
			cameraShot->AddReference();
		}
		
		NotifyActiveCameraShotChanged();
	}
}

ceCameraShot *ceConversation::GetCameraShotNamed( const char *name ) const{
	ceCameraShot *cameraShot = pCameraShotList.GetNamed( name );
	if( cameraShot ){
		return cameraShot;
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		cameraShot = pImportedConversations.GetAt( i )->GetCameraShotNamed( name );
		if( cameraShot ){
			return cameraShot;
		}
	}
	
	return nullptr;
}

ceCameraShotList ceConversation::AllCameraShots() const{
	ceCameraShotList list;
	AllCameraShots( list );
	return list;
}

void ceConversation::AllCameraShots( ceCameraShotList &list ) const{
	list += pCameraShotList;
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllCameraShots( list );
	}
}



// Gestures
/////////////

void ceConversation::AddGesture( ceGesture *gesture ){
	if( ! gesture || pGestureList.HasNamed( gesture->GetName().GetString() ) || gesture->GetConversation() ){
		DETHROW( deeInvalidParam );
	}
	
	pGestureList.Add( gesture );
	gesture->SetConversation( this );
	NotifyGestureStructureChanged();
	
	if( ! pActiveGesture ){
		SetActiveGesture( gesture );
	}
}

void ceConversation::RemoveGesture( ceGesture *gesture ){
	if( ! gesture || ! pGestureList.Has( gesture ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( gesture == pActiveGesture ){
		if( pGestureList.GetCount() == 1 ){
			SetActiveGesture( nullptr );
			
		}else{
			if( pGestureList.GetAt( 0 ) == gesture ){
				SetActiveGesture( pGestureList.GetAt( 1 ) );
				
			}else{
				SetActiveGesture( pGestureList.GetAt( 0 ) );
			}
		}
	}
	
	gesture->SetConversation( nullptr );
	pGestureList.Remove( gesture );
	NotifyGestureStructureChanged();
}

void ceConversation::RemoveAllGestures(){
	const int count = pGestureList.GetCount();
	int i;
	
	SetActiveGesture( nullptr );
	
	for( i=0; i<count; i++ ){
		pGestureList.GetAt( i )->SetConversation( nullptr );
	}
	pGestureList.RemoveAll();
	NotifyGestureStructureChanged();
}

void ceConversation::SetActiveGesture( ceGesture *gesture ){
	if( gesture != pActiveGesture ){
		if( pActiveGesture ){
			pActiveGesture->FreeReference();
		}
		
		pActiveGesture = gesture;
		
		if( gesture ){
			gesture->AddReference();
		}
		
		NotifyActiveGestureChanged();
	}
}

ceGesture *ceConversation::GetGestureNamed( const char *name ) const{
	ceGesture *gesture = pGestureList.GetNamed( name );
	if( gesture ){
		return gesture;
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		gesture = pImportedConversations.GetAt( i )->GetGestureNamed( name );
		if( gesture ){
			return gesture;
		}
	}
	
	return nullptr;
}

ceGestureList ceConversation::AllGestures() const{
	ceGestureList list;
	AllGestures( list );
	return list;
}

void ceConversation::AllGestures( ceGestureList &list ) const{
	list += pGestureList;
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllGestures( list );
	}
}



// Face Poses
//////////////

void ceConversation::AddFacePose( ceFacePose *facePose ){
	if( ! facePose || pFacePoseList.HasNamed( facePose->GetName().GetString() ) || facePose->GetConversation() ){
		DETHROW( deeInvalidParam );
	}
	
	pFacePoseList.Add( facePose );
	facePose->SetConversation( this );
	NotifyFacePoseStructureChanged();
	
	if( ! pActiveFacePose ){
		SetActiveFacePose( facePose );
	}
}

void ceConversation::RemoveFacePose( ceFacePose *facePose ){
	if( ! facePose || ! pFacePoseList.Has( facePose ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( facePose == pActiveFacePose ){
		if( pFacePoseList.GetCount() == 1 ){
			SetActiveFacePose( nullptr );
			
		}else{
			if( pFacePoseList.GetAt( 0 ) == facePose ){
				SetActiveFacePose( pFacePoseList.GetAt( 1 ) );
				
			}else{
				SetActiveFacePose( pFacePoseList.GetAt( 0 ) );
			}
		}
	}
	
	facePose->SetConversation( nullptr );
	pFacePoseList.Remove( facePose );
	NotifyFacePoseStructureChanged();
}

void ceConversation::RemoveAllFacePoses(){
	const int count = pFacePoseList.GetCount();
	int i;
	
	SetActiveFacePose( nullptr );
	
	for( i=0; i<count; i++ ){
		pFacePoseList.GetAt( i )->SetConversation( nullptr );
	}
	pFacePoseList.RemoveAll();
	NotifyFacePoseStructureChanged();
}

void ceConversation::SetActiveFacePose( ceFacePose *facePose ){
	if( facePose != pActiveFacePose ){
		if( pActiveFacePose ){
			pActiveFacePose->FreeReference();
		}
		
		pActiveFacePose = facePose;
		
		if( facePose ){
			facePose->AddReference();
		}
		
		NotifyActiveFacePoseChanged();
	}
}

ceFacePose *ceConversation::GetFacePoseNamed( const char *name ) const{
	ceFacePose *facePose = pFacePoseList.GetNamed( name );
	if( facePose ){
		return facePose;
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		facePose = pImportedConversations.GetAt( i )->GetFacePoseNamed( name );
		if( facePose ){
			return facePose;
		}
	}
	
	return nullptr;
}

ceFacePoseList ceConversation::AllFacePoses() const{
	ceFacePoseList list;
	AllFacePoses( list );
	return list;
}

void ceConversation::AllFacePoses( ceFacePoseList &list ) const{
	list += pFacePoseList;
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllFacePoses( list );
	}
}



// Files
//////////

void ceConversation::AddFile( ceConversationFile *file ){
	if( ! file || pFileList.HasWithID( file->GetID().GetString() ) || file->GetConversation() ){
		DETHROW( deeInvalidParam );
	}
	
	pFileList.Add( file );
	file->SetConversation( this );
	NotifyFileStructureChanged();
	
	if( ! pActiveFile ){
		SetActiveFile( file );
	}
}

void ceConversation::RemoveFile( ceConversationFile *file ){
	if( ! file || ! pFileList.Has( file ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( file == pActiveFile ){
		if( pFileList.GetCount() == 1 ){
			SetActiveFile( nullptr );
			
		}else{
			if( pFileList.GetAt( 0 ) == file ){
				SetActiveFile( pFileList.GetAt( 1 ) );
				
			}else{
				SetActiveFile( pFileList.GetAt( 0 ) );
			}
		}
	}
	
	file->SetConversation( nullptr );
	pFileList.Remove( file );
	NotifyFileStructureChanged();
}

void ceConversation::RemoveAllFiles(){
	const int count = pFileList.GetCount();
	int i;
	
	SetActiveFile( nullptr );
	
	for( i=0; i<count; i++ ){
		pFileList.GetAt( i )->SetConversation( nullptr );
	}
	pFileList.RemoveAll();
	NotifyFileStructureChanged();
}

void ceConversation::SetActiveFile( ceConversationFile *file ){
	if( file == pActiveFile ){
		return;
	}
	
	if( pActiveFile ){
		pActiveFile->FreeReference();
	}
	
	pActiveFile = file;
	
	if( file ){
		file->AddReference();
	}
	
	NotifyActiveFileChanged();
}

ceConversationFile *ceConversation::GetFileWithID( const char *name ) const{
	ceConversationFile *file = pFileList.GetWithID( name );
	if( file ){
		return file;
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		file = pImportedConversations.GetAt( i )->GetFileWithID( name );
		if( file ){
			return file;
		}
	}
	
	return nullptr;
}

ceConversationFileList ceConversation::AllFiles() const{
	ceConversationFileList list;
	AllFiles( list );
	return list;
}

void ceConversation::AllFiles( ceConversationFileList &list ) const{
	list += pFileList;
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllFiles( list );
	}
}

ceConversationTopic *ceConversation::GetTopicWithID( const char * fileName, const char *topicName ) const{
	ceConversationFile *file = pFileList.GetWithID( fileName );
	if( file ){
		ceConversationTopic * const topic = file->GetTopicList().GetWithID( topicName );
		if( topic ){
			return topic;
		}
	}
	
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		ceConversationTopic * const topic = pImportedConversations.GetAt( i )->GetTopicWithID( fileName, topicName );
		if( topic ){
			return topic;
		}
	}
	
	return nullptr;
}

ceConversationTopicList ceConversation::AllTopics( const char *fileName ) const{
	ceConversationTopicList list;
	AllTopics( fileName, list );
	return list;
}

void ceConversation::AllTopics( const char *fileName, ceConversationTopicList &list ) const{
	ceConversationFile *file = pFileList.GetWithID( fileName );
	if( file ){
		list += file->GetTopicList();
	}
	const int count = pImportedConversations.GetCount();
	int i;
	for( i=count-1; i>=0; i-- ){
		pImportedConversations.GetAt( i )->AllTopics( fileName, list );
	}
}



// Actors
///////////

void ceConversation::AddActor( ceConversationActor *actor ){
	pActorList.Add( actor );
	actor->SetConversation( this );
	NotifyActorStructureChanged();
	
	if( ! pActiveActor ){
		SetActiveActor( actor );
	}
}

void ceConversation::RemoveActor( ceConversationActor *actor ){
	if( ! actor || actor->GetConversation() != this ) DETHROW( deeInvalidParam );
	
	if( actor == pActiveActor ){
		if( pActorList.GetCount() == 1 ){
			SetActiveActor( nullptr );
			
		}else{
			if( pActorList.GetAt( 0 ) == actor ){
				SetActiveActor( pActorList.GetAt( 1 ) );
				
			}else{
				SetActiveActor( pActorList.GetAt( 0 ) );
			}
		}
	}
	
	actor->SetConversation( nullptr );
	pActorList.Remove( actor );
	NotifyActorStructureChanged();
}

void ceConversation::RemoveAllActors(){
	const int count = pActorList.GetCount();
	int i;
	
	SetActiveActor( nullptr );
	
	for( i=0; i<count; i++ ){
		pActorList.GetAt( i )->SetConversation( nullptr );
	}
	pActorList.RemoveAll();
	NotifyActorStructureChanged();
}

bool ceConversation::HasActiveActor() const{
	return pActiveActor != nullptr;
}

void ceConversation::SetActiveActor( ceConversationActor *actor ){
	if( actor != pActiveActor ){
		if( pActiveActor ){
			pActiveActor->FreeReference();
		}
		
		pActiveActor = actor;
		
		if( actor ){
			actor->AddReference();
		}
		
		NotifyActiveActorChanged();
	}
}



// Coordinate systems
///////////////////////

void ceConversation::AddCoordSystem( ceCoordSystem *coordSystem ){
	pCoordSystemList.Add( coordSystem );
	coordSystem->SetConversation( this );
	NotifyCoordSystemStructureChanged();
	
	if( ! pActiveCoordSystem ){
		SetActiveCoordSystem( coordSystem );
	}
}

void ceConversation::RemoveCoordSystem( ceCoordSystem *coordSystem ){
	if( ! coordSystem || coordSystem->GetConversation() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( coordSystem == pActiveCoordSystem ){
		if( pCoordSystemList.GetCount() == 1 ){
			SetActiveCoordSystem( nullptr );
			
		}else{
			if( pCoordSystemList.GetAt( 0 ) == coordSystem ){
				SetActiveCoordSystem( pCoordSystemList.GetAt( 1 ) );
				
			}else{
				SetActiveCoordSystem( pCoordSystemList.GetAt( 0 ) );
			}
		}
	}
	
	coordSystem->SetConversation( nullptr );
	pCoordSystemList.Remove( coordSystem );
	NotifyCoordSystemStructureChanged();
}

void ceConversation::RemoveAllCoordSystems(){
	const int count = pCoordSystemList.GetCount();
	int i;
	
	SetActiveCoordSystem( nullptr );
	
	for( i=0; i<count; i++ ){
		pCoordSystemList.GetAt( i )->SetConversation( nullptr );
	}
	pCoordSystemList.RemoveAll();
	NotifyCoordSystemStructureChanged();
}

bool ceConversation::HasActiveCoordSystem() const{
	return pActiveCoordSystem != nullptr;
}

void ceConversation::SetActiveCoordSystem( ceCoordSystem *coordSystem ){
	if( coordSystem != pActiveCoordSystem ){
		if( pActiveCoordSystem ){
			pActiveCoordSystem->FreeReference();
		}
		
		pActiveCoordSystem = coordSystem;
		
		if( coordSystem ){
			coordSystem->AddReference();
		}
		
		NotifyActiveCoordSystemChanged();
	}
}



// Props
//////////

void ceConversation::AddProp( ceProp *prop ){
	pPropList.Add( prop );
	prop->SetConversation( this );
	NotifyPropStructureChanged();
	
	if( ! pActiveProp ){
		SetActiveProp( prop );
	}
}

void ceConversation::RemoveProp( ceProp *prop ){
	if( ! prop || prop->GetConversation() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( prop == pActiveProp ){
		if( pPropList.GetCount() == 1 ){
			SetActiveProp( nullptr );
			
		}else{
			if( pPropList.GetAt( 0 ) == prop ){
				SetActiveProp( pPropList.GetAt( 1 ) );
				
			}else{
				SetActiveProp( pPropList.GetAt( 0 ) );
			}
		}
	}
	
	prop->SetConversation( nullptr );
	pPropList.Remove( prop );
	NotifyPropStructureChanged();
}

void ceConversation::RemoveAllProps(){
	const int count = pPropList.GetCount();
	int i;
	
	SetActiveProp( nullptr );
	
	for( i=0; i<count; i++ ){
		pPropList.GetAt( i )->SetConversation( nullptr );
	}
	pPropList.RemoveAll();
	NotifyPropStructureChanged();
}

bool ceConversation::HasActiveProp() const{
	return pActiveProp != nullptr;
}

void ceConversation::SetActiveProp( ceProp *prop ){
	if( prop != pActiveProp ){
		if( pActiveProp ){
			pActiveProp->FreeReference();
		}
		
		pActiveProp = prop;
		
		if( prop ){
			prop->AddReference();
		}
		
		NotifyActivePropChanged();
	}
}



// Listeners
//////////////

void ceConversation::AddListener( ceConversationListener *listener ){
	if( ! listener ) DETHROW( deeInvalidParam );
	
	pListeners.Add( listener );
}

void ceConversation::RemoveListener( ceConversationListener *listener ){
	pListeners.RemoveIfPresent( listener );
}



void ceConversation::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->StateChanged( this );
	}
}

void ceConversation::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->UndoChanged( this );
	}
}

void ceConversation::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ViewChanged( this );
	}
}

void ceConversation::NotifyConversationChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ConversationChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->SkyChanged( this );
	}
}

void ceConversation::NotifyEnvObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->EnvObjectChanged( this );
	}
}

void ceConversation::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->CameraChanged( this );
	}
	
	pEngMicrophone->SetPosition( pCamera->GetPosition() );
	pEngMicrophone->SetOrientation( pCamera->GetViewMatrix().ToQuaternion() );
}



void ceConversation::NotifyTargetStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->TargetStructureChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyTargetChanged( ceTarget *target ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->TargetChanged( this, target );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveTargetChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveTargetChanged( this );
	}
}



void ceConversation::NotifyCameraShotStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->CameraShotStructureChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyCameraShotChanged( ceCameraShot *cameraShot ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->CameraShotChanged( this, cameraShot );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveCameraShotChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveCameraShotChanged( this );
	}
}



void ceConversation::NotifyGestureStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->GestureStructureChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyGestureChanged( ceGesture *gesture ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->GestureChanged( this, gesture );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveGestureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveGestureChanged( this );
	}
}



void ceConversation::NotifyFacePoseControllerNamesChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->FacePoseControllerNamesChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyFacePoseStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->FacePoseStructureChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyFacePoseChanged( ceFacePose *facePose ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->FacePoseChanged( this, facePose );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveFacePoseChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveFacePoseChanged( this );
	}
}



void ceConversation::NotifyFileStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->FileStructureChanged( this );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyFileChanged( ceConversationFile *file ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->FileChanged( this, file );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveFileChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveFileChanged( this );
	}
}



void ceConversation::NotifyTopicStructureChanged( ceConversationFile *file ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->TopicStructureChanged( this, file );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyTopicChanged( ceConversationFile *file, ceConversationTopic *topic ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->TopicChanged( this, file, topic );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveTopicChanged( ceConversationFile *file ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveTopicChanged( this, file );
	}
}



void ceConversation::NotifyActionStructureChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActionStructureChanged( this, file, topic, action );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActionChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActionChanged( this, file, topic, action );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyConditionStructureChanged( ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction *action ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->
			ConditionStructureChanged( this, file, topic, action );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyConditionChanged( ceConversationFile *file, ceConversationTopic *topic,
ceConversationAction *action, ceConversationCondition *condition ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->
			ConditionChanged( this, file, topic, action, condition );
	}
	
	SetChanged( true );
}

void ceConversation::NotifyActiveChanged( ceConversationFile *file, ceConversationTopic *topic ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActiveChanged( this, file, topic );
	}
}



void ceConversation::NotifyActorStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	pPlayback->SetActorCount( pActorList.GetCount() );
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActorStructureChanged( this );
	}
}

void ceConversation::NotifyActorChanged( ceConversationActor *actor ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActorChanged( this, actor );
	}
}

void ceConversation::NotifyActorActivePoseChanged( ceConversationActor *actor ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActorActivePoseChanged( this, actor );
	}
}

void ceConversation::NotifyActorCommandsChanged( ceConversationActor *actor ){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i=0; i<listenerCount; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActorCommandsChanged( this, actor );
	}
}

void ceConversation::NotifyActorPosesChanged( ceConversationActor *actor ){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i=0; i<listenerCount; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActorPosesChanged( this, actor );
	}
}

void ceConversation::NotifyActorPoseGesturesChanged( ceConversationActor *actor, ceActorPose *pose ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->
			ActorPoseGesturesChanged( this, actor, pose );
	}
}

void ceConversation::NotifyActorParametersChanged( ceConversationActor *actor ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActorParametersChanged( this, actor );
	}
}

void ceConversation::NotifyActiveActorChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->ActiveActorChanged( this );
	}
}



void ceConversation::NotifyCoordSystemStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->CoordSystemStructureChanged( this );
	}
}

void ceConversation::NotifyCoordSystemChanged( ceCoordSystem *coordSystem ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->CoordSystemChanged( this, coordSystem );
	}
}

void ceConversation::NotifyActiveCoordSystemChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActiveCoordSystemChanged( this );
	}
}



void ceConversation::NotifyPropStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->PropStructureChanged( this );
	}
}

void ceConversation::NotifyPropChanged( ceProp *prop ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->PropChanged( this, prop );
	}
}

void ceConversation::NotifyActivePropChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i =0; i < count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->ActivePropChanged( this );
	}
}



void ceConversation::NotifyPlaybackChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->PlaybackChanged( this );
	}
}

void ceConversation::NotifyPlaybackCommandListChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->PlaybackCommandListChanged( this );
	}
}

void ceConversation::NotifyPlaybackVarListChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->PlaybackVarListChanged( this );
	}
}

void ceConversation::NotifyPlaybackTriggerTableChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( ceConversationListener* )pListeners.GetAt( i ) )->PlaybackTriggerTableChanged( this );
	}
}

void ceConversation::NotifyPlaybackMissingWordsChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->PlaybackMissingWordsChanged( this );
	}
}

void ceConversation::NotifyLanguagePackChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( ceConversationListener* )pListeners.GetAt( l ) )->LanguagePackChanged( this );
	}
}



// Private Functions
//////////////////////

void ceConversation::pCleanUp(){
	pListeners.RemoveAll();
	
	if( pPlayback ){ // avoid a segf problem
		Dispose();
	}
	
	if( pPlayback ){
		delete pPlayback;
	}
	if( pInfoBox ){
		delete pInfoBox;
	}
	if( pPlayerChoiceBox ){
		delete pPlayerChoiceBox;
	}
	if( pTextBox ){
		delete pTextBox;
	}
	
	if( pSky ){
		delete pSky;
	}
	if( pEnvObject ){
		delete pEnvObject;
	}
	RemoveAllProps();
	
	if( pEngSpeakerVAPreview && pEngMicrophone ){
		pEngMicrophone->RemoveSpeaker( pEngSpeakerVAPreview );
	}
	if( pEngMicrophone ){
		if( GetEngine()->GetAudioSystem()->GetActiveMicrophone() == pEngMicrophone ){
			GetEngine()->GetAudioSystem()->SetActiveMicrophone( nullptr );
		}
		if( pEngWorld ){
			pEngWorld->RemoveMicrophone( pEngMicrophone );
		}
	}
	if( pCameraFree ){
		delete pCameraFree;
	}
	if( pCamera ){
		delete pCamera;
	}
}
