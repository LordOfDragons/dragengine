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

#ifndef _CECONVERSATION_H_
#define _CECONVERSATION_H_

#include "ceConversationList.h"
#include "actor/ceConversationActorList.h"
#include "camerashot/ceCameraShotList.h"
#include "file/ceConversationFileList.h"
#include "facepose/ceFacePoseList.h"
#include "gesture/ceGestureList.h"
#include "target/ceTargetList.h"
#include "coordsystem/ceCoordSystemList.h"
#include "prop/cePropList.h"
#include "../langpack/ceLangPack.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>


class ceConversationTopic;
class ceConversationTopicList;
class ceConversationAction;
class ceConversationCondition;
class ceConversationInfoBox;
class ceConversationListener;
class ceActorPose;
class ceTextBox;
class cePlayerChoiceBox;
class cePlayback;
class ceLoadSaveSystem;

class igdeWSky;
class igdeWObject;
class igdeCamera;
class deComponent;
class deAnimator;
class deAnimatorInstance;



/**
 * Conversation.
 */
class ceConversation : public igdeEditableEntity{
public:
	typedef deTObjectReference<ceConversation> Ref;
	
	/** Paste snippet dialog parameters. */
	struct sPasteSnippetDialogParams{
		decString cameraShot1, cameraShot2;
		decString target1, target2;
		decString snippet;
		float delayCameraShot, delayActorSpeak;
		float scaleActorSpeak;
	};
	
	/** Layer masks. */
	enum eLayerMasks {
		/** Camera. */
		elmCamera,
		
		/** Environment map probes. */
		elmEnvMapProbes,
		
		/** Audio. */
		elmAudio
	};
	
	
	
private:
	deWorld::Ref pEngWorld;
	
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	
	deMicrophone::Ref pEngMicrophone;
	deSpeaker::Ref pEngSpeakerVAPreview;
	
	igdeCamera *pCamera;
	igdeCamera *pCameraFree;
	ceTextBox *pTextBox;
	cePlayerChoiceBox *pPlayerChoiceBox;
	ceConversationInfoBox *pInfoBox;
	cePlayback *pPlayback;
	
	float pScreenRatio;
	bool pShowRuleOfThirdsAid;
	
	decStringList pImportConversationPath;
	ceConversationList pImportedConversations;
	
	ceTargetList pTargetList;
	ceTarget *pActiveTarget;
	
	ceCameraShotList pCameraShotList;
	ceCameraShot *pActiveCameraShot;
	
	ceGestureList pGestureList;
	ceGesture *pActiveGesture;
	
	decStringList pFacePoseControllerNames;
	ceFacePoseList pFacePoseList;
	ceFacePose *pActiveFacePose;
	
	ceConversationFileList pFileList;
	ceConversationFile *pActiveFile;
	
	ceConversationActorList pActorList;
	ceConversationActor *pActiveActor;
	
	ceCoordSystemList pCoordSystemList;
	ceCoordSystem *pActiveCoordSystem;
	
	cePropList pPropList;
	ceProp *pActiveProp;
	
	decString pCTSPath;
	decString pCTAPath;
	decString pCTFIPath;
	decString pLangPackPath;
	decString pLangPackEntryName;
	sPasteSnippetDialogParams pPasteSnippetDialogParams;
	
	ceLangPack::Ref pLangPack;
	
	decObjectSet pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation. */
	ceConversation( igdeEnvironment *environment );
	/** Cleans up the conversation. */
	virtual ~ceConversation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Engine world. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	
	/** Engine microphone. */
	inline const deMicrophone::Ref &GetEngineMicrophone() const{ return pEngMicrophone; }
	
	/** Voice audio preview speaker. */
	inline const deSpeaker::Ref &GetEngineSpeakerVAPreview() const{ return pEngSpeakerVAPreview; }
	
	/** Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** Retrieves the environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Free roaming camera. */
	inline igdeCamera *GetCameraFree() const{ return pCameraFree; }
	
	/** Retrieves the text box. */
	inline ceTextBox *GetTextBox() const{ return pTextBox; }
	/** Retrieves the player choice box. */
	inline cePlayerChoiceBox &GetPlayerChoiceBox() const{ return *pPlayerChoiceBox; }
	/** Retrieves the info box. */
	inline ceConversationInfoBox &GetInfoBox() const{ return *pInfoBox; }
	/** Retrieves the playback object. */
	inline cePlayback *GetPlayback() const{ return pPlayback; }
	
	/** Retrieves the ration of the screen. */
	inline float GetScreenRatio() const{ return pScreenRatio; }
	/** Sets the ratio of the screen. */
	void SetScreenRatio( float ratio );
	/** Determines if the rule of thirds aid is displayed. */
	inline bool GetShowRuleOfThirdsAid() const{ return pShowRuleOfThirdsAid; }
	/** Sets if the rule of thirds aid is displayed. */
	void SetShowRuleOfThirdsAid( bool showRuleOfThirdsAid );
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the sky. */
	void Update( float elapsed );
	/** Resets the sky. */
	void Reset();
	
	/** Activate microphone. */
	void ActivateMicrophone();
	
	
	
	/** CTS path. */
	inline const decString &GetCTSPath() const{ return pCTSPath; }
	void SetCTSPath( const char *path );
	
	/** CTA path. */
	inline const decString &GetCTAPath() const{ return pCTAPath; }
	void SetCTAPath( const char *path );
	
	/** CTFI path. */
	inline const decString &GetCTFIPath() const{ return pCTFIPath; }
	void SetCTFIPath( const char *path );
	
	/** Language pack path. */
	inline const decString &GetLangPackPath() const{ return pLangPackPath; }
	void SetLangPackPath( const char *path );
	
	/** Language pack entry name. */
	inline const decString &GetLangPackEntryName() const{ return pLangPackEntryName; }
	void SetLangPackEntryName( const char *name );
	
	/** Paste snippet dialog parameters. */
	inline sPasteSnippetDialogParams &GetPasteSnippetDialogParams(){ return pPasteSnippetDialogParams; }
	
	/** Language pack or nullptr. */
	inline const ceLangPack::Ref &GetLanguagePack() const{ return pLangPack; }
	void SetLanguagePack( ceLangPack *langpack );
	/*@}*/
	
	
	
	/** \name Imported conversations */
	/*@{*/
	/** List of import conversation path. */
	inline const decStringList &GetImportConversationPath() const{ return pImportConversationPath; }
	
	/** Set list of import conversation path. */
	void SetImportConversationPath( const decStringList &list );
	
	/** Update imported conversations from path. */
	void UpdateImportedConversations( ceLoadSaveSystem &lssystem );
	
	/** Imported conversations. */
	inline const ceConversationList &GetImportedConversations() const{ return pImportedConversations; }
	/*@}*/
	
	
	
	/** \name Targets */
	/*@{*/
	/** Retrieves the target list read-only. */
	inline const ceTargetList &GetTargetList() const{ return pTargetList; }
	/** Adds a target. */
	void AddTarget( ceTarget *target );
	/** Removes a target. */
	void RemoveTarget( ceTarget *target );
	/** Remove all targets. */
	void RemoveAllTargets();
	/** Retrieves the active target or NULL if none is active. */
	inline ceTarget *GetActiveTarget() const{ return pActiveTarget; }
	/** Sets the active target or NULL if none is active. */
	void SetActiveTarget( ceTarget *target );
	
	/** Named target including imported conversations. */
	ceTarget *GetTargetNamed( const char *name ) const;
	
	/** All targets including imported conversations. */
	ceTargetList AllTargets() const;
	void AllTargets( ceTargetList &list ) const;
	/*@}*/
	
	/** \name Camera Shots */
	/*@{*/
	/** Retrieves the camera shot list read-only. */
	inline const ceCameraShotList &GetCameraShotList() const{ return pCameraShotList; }
	/** Adds a camera shot. */
	void AddCameraShot( ceCameraShot *cameraShot );
	/** Removes a camera shot. */
	void RemoveCameraShot( ceCameraShot *cameraShot );
	/** Remove all camera shots. */
	void RemoveAllCameraShots();
	/** Retrieves the active camera shot or NULL if none is active. */
	inline ceCameraShot *GetActiveCameraShot() const{ return pActiveCameraShot; }
	/** Sets the active camera shot or NULL if none is active. */
	void SetActiveCameraShot( ceCameraShot *cameraShot );
	
	/** Named camera shot including imported conversations. */
	ceCameraShot *GetCameraShotNamed( const char *name ) const;
	
	/** All camera shots including imported conversations. */
	ceCameraShotList AllCameraShots() const;
	void AllCameraShots( ceCameraShotList &list ) const;
	/*@}*/
	
	
	
	/** \name Gestures */
	/*@{*/
	/** Retrieves the gesture list read-only. */
	inline const ceGestureList &GetGestureList() const{ return pGestureList; }
	/** Adds a gesture. */
	void AddGesture( ceGesture *gesture );
	/** Removes a gesture. */
	void RemoveGesture( ceGesture *gesture );
	/** Remove all gestures. */
	void RemoveAllGestures();
	/** Retrieves the active gesture or NULL if none is active. */
	inline ceGesture *GetActiveGesture() const{ return pActiveGesture; }
	/** Sets the active gesture or NULL if none is active. */
	void SetActiveGesture( ceGesture *gesture );
	
	/** Named gesture including imported conversations. */
	ceGesture *GetGestureNamed( const char *name ) const;
	
	/** All gestures including imported conversations. */
	ceGestureList AllGestures() const;
	void AllGestures( ceGestureList &list ) const;
	/*@}*/
	
	
	
	/** \name FacePoses */
	/*@{*/
	/** Retrieves the list of face pose controller names. */
	inline decStringList &GetFacePoseControllerNameList(){ return pFacePoseControllerNames; }
	inline const decStringList &GetFacePoseControllerNameList() const{ return pFacePoseControllerNames; }
	/** Retrieves the face pose list read-only. */
	inline const ceFacePoseList &GetFacePoseList() const{ return pFacePoseList; }
	/** Adds a face pose. */
	void AddFacePose( ceFacePose *facePose );
	/** Removes a face pose. */
	void RemoveFacePose( ceFacePose *facePose );
	/** Remove all face poses. */
	void RemoveAllFacePoses();
	/** Retrieves the active face pose or NULL if none is active. */
	inline ceFacePose *GetActiveFacePose() const{ return pActiveFacePose; }
	/** Sets the active face pose or NULL if none is active. */
	void SetActiveFacePose( ceFacePose *facePose );
	
	/** Named face pose including imported conversations. */
	ceFacePose *GetFacePoseNamed( const char *name ) const;
	
	/** All face poses including imported conversations. */
	ceFacePoseList AllFacePoses() const;
	void AllFacePoses( ceFacePoseList &list ) const;
	/*@}*/
	
	
	
	/** \name Files */
	/*@{*/
	/** Retrieves the file list read-only. */
	inline const ceConversationFileList &GetFileList() const{ return pFileList; }
	/** Adds an file. */
	void AddFile( ceConversationFile *file );
	/** Removes an file. */
	void RemoveFile( ceConversationFile *file );
	/** Remove all files. */
	void RemoveAllFiles();
	/** Retrieves the active texture or NULL if none is active. */
	inline ceConversationFile *GetActiveFile() const{ return pActiveFile; }
	/** Sets the active file or NULL if none is active. */
	void SetActiveFile( ceConversationFile *file );
	
	/** Named file including imported conversations. */
	ceConversationFile *GetFileWithID( const char *name ) const;
	
	/** All files including imported conversations. */
	ceConversationFileList AllFiles() const;
	void AllFiles( ceConversationFileList &list ) const;
	
	/** Named topic including imported conversations. */
	ceConversationTopic *GetTopicWithID( const char *fileName, const char *topicName ) const;
	
	/** All files including imported conversations. */
	ceConversationTopicList AllTopics( const char *fileName ) const;
	void AllTopics( const char *fileName, ceConversationTopicList &list ) const;
	/*@}*/
	
	
	
	/** \name Actors */
	/*@{*/
	/** Retrieves the actor list read-only. */
	inline const ceConversationActorList &GetActorList() const{ return pActorList; }
	/** Adds a new actor. */
	void AddActor( ceConversationActor *actor );
	/** Removes a actor. */
	void RemoveActor( ceConversationActor *actor );
	/** Removes all actors. */
	void RemoveAllActors();
	/** Retrieves the active texture or NULL if none is active. */
	inline ceConversationActor *GetActiveActor() const{ return pActiveActor; }
	/** Determines if there is an active actor or not. */
	bool HasActiveActor() const;
	/** Sets the active actor or NULL if none is active. */
	void SetActiveActor( ceConversationActor *actor );
	/*@}*/
	
	
	
	/** \name Coordinate Systems */
	/*@{*/
	/** Retrieves the coordinate system list read-only. */
	inline const ceCoordSystemList &GetCoordSystemList() const{ return pCoordSystemList; }
	/** Adds a new coordinate system. */
	void AddCoordSystem( ceCoordSystem *coordSystem );
	/** Removes a coordinate system. */
	void RemoveCoordSystem( ceCoordSystem *coordSystem );
	/** Removes all coordinate system. */
	void RemoveAllCoordSystems();
	/** Retrieves the active coordinate system or NULL if none is active. */
	inline ceCoordSystem *GetActiveCoordSystem() const{ return pActiveCoordSystem; }
	/** Determines if there is an active coordinate system or not. */
	bool HasActiveCoordSystem() const;
	/** Sets the active coordinate system or NULL if none is active. */
	void SetActiveCoordSystem( ceCoordSystem *coordSystem );
	/*@}*/
	
	
	
	/** \name Props */
	/*@{*/
	/** Retrieves the prop list read-only. */
	inline const cePropList &GetPropList() const{ return pPropList; }
	/** Adds a prop. */
	void AddProp( ceProp *prop );
	/** Removes a prop. */
	void RemoveProp( ceProp *prop );
	/** Removes all props. */
	void RemoveAllProps();
	/** Retrieves the active prop or NULL if none is active. */
	inline ceProp *GetActiveProp() const{ return pActiveProp; }
	/** Determines if there is an active prop or not. */
	bool HasActiveProp() const;
	/** Sets the active prop or NULL if none is active. */
	void SetActiveProp( ceProp *prop );
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener( ceConversationListener *listener );
	/** Removes a listener. */
	void RemoveListener( ceConversationListener *listener );
	
	/** Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	/** Notifies all that view properties changed. */
	void NotifyViewChanged();
	/** Notifies all that the conversation changed. */
	void NotifyConversationChanged();
	/** Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** Notifies all that the camera changed. */
	void NotifyCameraChanged();
	
	/** Notifies all that the target count or order changed. */
	void NotifyTargetStructureChanged();
	/** Notifies all that a target changed. */
	void NotifyTargetChanged( ceTarget *target );
	/** Notifies all that the active target changed. */
	void NotifyActiveTargetChanged();
	
	/** Notifies all that the camera shot count or order changed. */
	void NotifyCameraShotStructureChanged();
	/** Notifies all that a camera shot changed. */
	void NotifyCameraShotChanged( ceCameraShot *cameraShot );
	/** Notifies all that the active camera shot changed. */
	void NotifyActiveCameraShotChanged();
	
	/** Notifies all that the gesture count or order changed. */
	void NotifyGestureStructureChanged();
	/** Notifies all that a gesture changed. */
	void NotifyGestureChanged( ceGesture *gesture );
	/** Notifies all that the active gesture changed. */
	void NotifyActiveGestureChanged();
	
	/** Notifies all that the face pose controller names changed. */
	void NotifyFacePoseControllerNamesChanged();
	/** Notifies all that the face pose count or order changed. */
	void NotifyFacePoseStructureChanged();
	/** Notifies all that a face pose changed. */
	void NotifyFacePoseChanged( ceFacePose *facePose );
	/** Notifies all that the active face pose changed. */
	void NotifyActiveFacePoseChanged();
	
	/** Notifies all that the file count or order changed. */
	void NotifyFileStructureChanged();
	/** Notifies all that a file changed. */
	void NotifyFileChanged( ceConversationFile *file );
	/** Notifies all that the file actor changed. */
	void NotifyActiveFileChanged();
	
	/** Notifies all that the topic count or order changed. */
	void NotifyTopicStructureChanged( ceConversationFile *file );
	/** Notifies all that a topic changed. */
	void NotifyTopicChanged( ceConversationFile *file, ceConversationTopic *topic );
	/** Notifies all that the active topic changed. */
	void NotifyActiveTopicChanged( ceConversationFile *file );
	
	/** Notifies all that the action structure changed. */
	void NotifyActionStructureChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	/** Notifies all that an action changed. */
	void NotifyActionChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	
	/** Notifies all that the condition structure changed. */
	void NotifyConditionStructureChanged( ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** Notifies all that an condition changed. */
	void NotifyConditionChanged( ceConversationFile *file, ceConversationTopic *topic,
		ceConversationAction *action, ceConversationCondition *condition );
	
	/** Notifies all that the active element changed. */
	void NotifyActiveChanged( ceConversationFile *file, ceConversationTopic *topic );
	
	
	
	/** Notifies all that the actor count or order changed. */
	void NotifyActorStructureChanged();
	
	/** Notifies all that an actor changed. */
	void NotifyActorChanged( ceConversationActor *actor );
	
	/** Notify all actor active pose changed. */
	void NotifyActorActivePoseChanged( ceConversationActor *actor );
	
	/** Notify listeners actor commands changed. */
	void NotifyActorCommandsChanged( ceConversationActor *actor );
	
	/** Notify listeners actor poses changed. */
	void NotifyActorPosesChanged( ceConversationActor *actor );
	
	/** Notify listeners actor pose gestures changed. */
	void NotifyActorPoseGesturesChanged( ceConversationActor *actor, ceActorPose *pose );
	
	/** Notify all actor parameters changed. */
	void NotifyActorParametersChanged( ceConversationActor *actor );
	
	/** Notify all active actor changed. */
	void NotifyActiveActorChanged();
	
	
	
	/** Notifies all that the coordinate system count or order changed. */
	void NotifyCoordSystemStructureChanged();
	/** Notifies all that an coordinate system changed. */
	void NotifyCoordSystemChanged( ceCoordSystem *coordSystem );
	/** Notifies all that the active coordinate system changed. */
	void NotifyActiveCoordSystemChanged();
	
	/** Notifies all that the prop count or order changed. */
	void NotifyPropStructureChanged();
	/** Notifies all that a prop changed. */
	void NotifyPropChanged( ceProp *prop );
	/** Notifies all that the active prop changed. */
	void NotifyActivePropChanged();
	
	/** Notifies all that playback parameters changed. */
	void NotifyPlaybackChanged();
	/** Notifies all that playback command list changed. */
	void NotifyPlaybackCommandListChanged();
	/** Notifies all that playback variable list changed. */
	void NotifyPlaybackVarListChanged();
	/** Notifies all that playback the trigger table changed. */
	void NotifyPlaybackTriggerTableChanged();
	/** Notifies all that playback mising words changed. */
	void NotifyPlaybackMissingWordsChanged();
	
	/** Notify listeners language pack changed. */
	void NotifyLanguagePackChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
