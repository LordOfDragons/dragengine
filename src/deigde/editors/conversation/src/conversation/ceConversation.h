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

#include <deigde/editableentity/igdeEditableEntity.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/string/decStringList.h>

#include <dragengine/common/math/decMath.h>


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
class deWorld;
class deComponent;
class deAnimator;
class deAnimatorInstance;
class deMicrophone;
class deSpeaker;



/**
 * \brief Conversation.
 */
class ceConversation : public igdeEditableEntity{
public:
	/** \brief Paste snippet dialog parameters. */
	struct sPasteSnippetDialogParams{
		decString cameraShot1, cameraShot2;
		decString target1, target2;
		decString snippet;
	};
	
	/** \brief Layer masks. */
	enum eLayerMasks {
		/** \brief Camera. */
		elmCamera,
		
		/** \brief Environment map probes. */
		elmEnvMapProbes,
		
		/** \brief Audio. */
		elmAudio
	};
	
	
	
private:
	deWorld *pEngWorld;
	
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	
	deMicrophone *pEngMicrophone;
	deSpeaker *pEngSpeakerVAPreview;
	
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
	sPasteSnippetDialogParams pPasteSnippetDialogParams;
	
	decObjectSet pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation. */
	ceConversation( igdeEnvironment *environment );
	/** \brief Cleans up the conversation. */
	virtual ~ceConversation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	/** \brief Retrieves the engine microphone. */
	inline deMicrophone *GetEngineMicrophone() const{ return pEngMicrophone; }
	/** \brief Retrieves the voice audio preview speaker. */
	inline deSpeaker *GetEngineSpeakerVAPreview() const{ return pEngSpeakerVAPreview; }
	
	/** \brief Retrieves the sky wrapper. */
	inline igdeWSky *GetSky() const{ return pSky; }
	/** \brief Retrieves the environment wrapper object. */
	inline igdeWObject *GetEnvObject() const{ return pEnvObject; }
	
	/** \brief Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Free roaming camera. */
	inline igdeCamera *GetCameraFree() const{ return pCameraFree; }
	
	/** \brief Retrieves the text box. */
	inline ceTextBox *GetTextBox() const{ return pTextBox; }
	/** \brief Retrieves the player choice box. */
	inline cePlayerChoiceBox &GetPlayerChoiceBox() const{ return *pPlayerChoiceBox; }
	/** \brief Retrieves the info box. */
	inline ceConversationInfoBox &GetInfoBox() const{ return *pInfoBox; }
	/** \brief Retrieves the playback object. */
	inline cePlayback *GetPlayback() const{ return pPlayback; }
	
	/** \brief Retrieves the ration of the screen. */
	inline float GetScreenRatio() const{ return pScreenRatio; }
	/** \brief Sets the ratio of the screen. */
	void SetScreenRatio( float ratio );
	/** \brief Determines if the rule of thirds aid is displayed. */
	inline bool GetShowRuleOfThirdsAid() const{ return pShowRuleOfThirdsAid; }
	/** \brief Sets if the rule of thirds aid is displayed. */
	void SetShowRuleOfThirdsAid( bool showRuleOfThirdsAid );
	
	/** \brief Dispose of all resources. */
	void Dispose();
	/** \brief Updates the sky. */
	void Update( float elapsed );
	/** \brief Resets the sky. */
	void Reset();
	
	/** \brief Activate microphone. */
	void ActivateMicrophone();
	
	
	
	/** \brief CTS path. */
	inline const decString &GetCTSPath() const{ return pCTSPath; }
	
	/** \brief Set CTS path. */
	void SetCTSPath( const char *path );
	
	/** \brief CTA path. */
	inline const decString &GetCTAPath() const{ return pCTAPath; }
	
	/** \brief Set CTA path. */
	void SetCTAPath( const char *path );
	
	/** \brief CTFI path. */
	inline const decString &GetCTFIPath() const{ return pCTFIPath; }
	
	/** \brief Set CTFI path. */
	void SetCTFIPath( const char *path );
	
	/** \brief Paste snippet dialog parameters. */
	inline sPasteSnippetDialogParams &GetPasteSnippetDialogParams(){ return pPasteSnippetDialogParams; }
	/*@}*/
	
	
	
	/** \name Imported conversations */
	/*@{*/
	/** \brief List of import conversation path. */
	inline const decStringList &GetImportConversationPath() const{ return pImportConversationPath; }
	
	/** \brief Set list of import conversation path. */
	void SetImportConversationPath( const decStringList &list );
	
	/** \brief Update imported conversations from path. */
	void UpdateImportedConversations( ceLoadSaveSystem &lssystem );
	
	/** \brief Imported conversations. */
	inline const ceConversationList &GetImportedConversations() const{ return pImportedConversations; }
	/*@}*/
	
	
	
	/** \name Targets */
	/*@{*/
	/** \brief Retrieves the target list read-only. */
	inline const ceTargetList &GetTargetList() const{ return pTargetList; }
	/** \brief Adds a target. */
	void AddTarget( ceTarget *target );
	/** \brief Removes a target. */
	void RemoveTarget( ceTarget *target );
	/** \brief Remove all targets. */
	void RemoveAllTargets();
	/** \brief Retrieves the active target or NULL if none is active. */
	inline ceTarget *GetActiveTarget() const{ return pActiveTarget; }
	/** \brief Sets the active target or NULL if none is active. */
	void SetActiveTarget( ceTarget *target );
	
	/** \brief Named target including imported conversations. */
	ceTarget *GetTargetNamed( const char *name ) const;
	
	/** \brief All targets including imported conversations. */
	ceTargetList AllTargets() const;
	void AllTargets( ceTargetList &list ) const;
	/*@}*/
	
	/** \name Camera Shots */
	/*@{*/
	/** \brief Retrieves the camera shot list read-only. */
	inline const ceCameraShotList &GetCameraShotList() const{ return pCameraShotList; }
	/** \brief Adds a camera shot. */
	void AddCameraShot( ceCameraShot *cameraShot );
	/** \brief Removes a camera shot. */
	void RemoveCameraShot( ceCameraShot *cameraShot );
	/** \brief Remove all camera shots. */
	void RemoveAllCameraShots();
	/** \brief Retrieves the active camera shot or NULL if none is active. */
	inline ceCameraShot *GetActiveCameraShot() const{ return pActiveCameraShot; }
	/** \brief Sets the active camera shot or NULL if none is active. */
	void SetActiveCameraShot( ceCameraShot *cameraShot );
	
	/** \brief Named camera shot including imported conversations. */
	ceCameraShot *GetCameraShotNamed( const char *name ) const;
	
	/** \brief All camera shots including imported conversations. */
	ceCameraShotList AllCameraShots() const;
	void AllCameraShots( ceCameraShotList &list ) const;
	/*@}*/
	
	/** \name Gestures */
	/*@{*/
	/** \brief Retrieves the gesture list read-only. */
	inline const ceGestureList &GetGestureList() const{ return pGestureList; }
	/** \brief Adds a gesture. */
	void AddGesture( ceGesture *gesture );
	/** \brief Removes a gesture. */
	void RemoveGesture( ceGesture *gesture );
	/** \brief Remove all gestures. */
	void RemoveAllGestures();
	/** \brief Retrieves the active gesture or NULL if none is active. */
	inline ceGesture *GetActiveGesture() const{ return pActiveGesture; }
	/** \brief Sets the active gesture or NULL if none is active. */
	void SetActiveGesture( ceGesture *gesture );
	
	/** \brief Named gesture including imported conversations. */
	ceGesture *GetGestureNamed( const char *name ) const;
	
	/** \brief All gestures including imported conversations. */
	ceGestureList AllGestures() const;
	void AllGestures( ceGestureList &list ) const;
	/*@}*/
	
	/** \name FacePoses */
	/*@{*/
	/** \brief Retrieves the list of face pose controller names. */
	inline decStringList &GetFacePoseControllerNameList(){ return pFacePoseControllerNames; }
	inline const decStringList &GetFacePoseControllerNameList() const{ return pFacePoseControllerNames; }
	/** \brief Retrieves the face pose list read-only. */
	inline const ceFacePoseList &GetFacePoseList() const{ return pFacePoseList; }
	/** \brief Adds a face pose. */
	void AddFacePose( ceFacePose *facePose );
	/** \brief Removes a face pose. */
	void RemoveFacePose( ceFacePose *facePose );
	/** \brief Remove all face poses. */
	void RemoveAllFacePoses();
	/** \brief Retrieves the active face pose or NULL if none is active. */
	inline ceFacePose *GetActiveFacePose() const{ return pActiveFacePose; }
	/** \brief Sets the active face pose or NULL if none is active. */
	void SetActiveFacePose( ceFacePose *facePose );
	
	/** \brief Named face pose including imported conversations. */
	ceFacePose *GetFacePoseNamed( const char *name ) const;
	
	/** \brief All face poses including imported conversations. */
	ceFacePoseList AllFacePoses() const;
	void AllFacePoses( ceFacePoseList &list ) const;
	/*@}*/
	
	/** \name Files */
	/*@{*/
	/** \brief Retrieves the file list read-only. */
	inline const ceConversationFileList &GetFileList() const{ return pFileList; }
	/** \brief Adds an file. */
	void AddFile( ceConversationFile *file );
	/** \brief Removes an file. */
	void RemoveFile( ceConversationFile *file );
	/** \brief Remove all files. */
	void RemoveAllFiles();
	/** \brief Retrieves the active texture or NULL if none is active. */
	inline ceConversationFile *GetActiveFile() const{ return pActiveFile; }
	/** \brief Sets the active file or NULL if none is active. */
	void SetActiveFile( ceConversationFile *file );
	
	/** \brief Named file including imported conversations. */
	ceConversationFile *GetFileWithID( const char *name ) const;
	
	/** \brief All files including imported conversations. */
	ceConversationFileList AllFiles() const;
	void AllFiles( ceConversationFileList &list ) const;
	
	/** \brief Named topic including imported conversations. */
	ceConversationTopic *GetTopicWithID( const char *fileName, const char *topicName ) const;
	
	/** \brief All files including imported conversations. */
	ceConversationTopicList AllTopics( const char *fileName ) const;
	void AllTopics( const char *fileName, ceConversationTopicList &list ) const;
	/*@}*/
	
	/** \name Actors */
	/*@{*/
	/** \brief Retrieves the actor list read-only. */
	inline const ceConversationActorList &GetActorList() const{ return pActorList; }
	/** \brief Adds a new actor. */
	void AddActor( ceConversationActor *actor );
	/** \brief Removes a actor. */
	void RemoveActor( ceConversationActor *actor );
	/** \brief Removes all actors. */
	void RemoveAllActors();
	/** \brief Retrieves the active texture or NULL if none is active. */
	inline ceConversationActor *GetActiveActor() const{ return pActiveActor; }
	/** \brief Determines if there is an active actor or not. */
	bool HasActiveActor() const;
	/** \brief Sets the active actor or NULL if none is active. */
	void SetActiveActor( ceConversationActor *actor );
	/*@}*/
	
	/** \name Coordinate Systems */
	/*@{*/
	/** \brief Retrieves the coordinate system list read-only. */
	inline const ceCoordSystemList &GetCoordSystemList() const{ return pCoordSystemList; }
	/** \brief Adds a new coordinate system. */
	void AddCoordSystem( ceCoordSystem *coordSystem );
	/** \brief Removes a coordinate system. */
	void RemoveCoordSystem( ceCoordSystem *coordSystem );
	/** \brief Removes all coordinate system. */
	void RemoveAllCoordSystems();
	/** \brief Retrieves the active coordinate system or NULL if none is active. */
	inline ceCoordSystem *GetActiveCoordSystem() const{ return pActiveCoordSystem; }
	/** \brief Determines if there is an active coordinate system or not. */
	bool HasActiveCoordSystem() const;
	/** \brief Sets the active coordinate system or NULL if none is active. */
	void SetActiveCoordSystem( ceCoordSystem *coordSystem );
	/*@}*/
	
	/** \name Props */
	/*@{*/
	/** \brief Retrieves the prop list read-only. */
	inline const cePropList &GetPropList() const{ return pPropList; }
	/** \brief Adds a prop. */
	void AddProp( ceProp *prop );
	/** \brief Removes a prop. */
	void RemoveProp( ceProp *prop );
	/** \brief Removes all props. */
	void RemoveAllProps();
	/** \brief Retrieves the active prop or NULL if none is active. */
	inline ceProp *GetActiveProp() const{ return pActiveProp; }
	/** \brief Determines if there is an active prop or not. */
	bool HasActiveProp() const;
	/** \brief Sets the active prop or NULL if none is active. */
	void SetActiveProp( ceProp *prop );
	/*@}*/
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Adds a listener. */
	void AddListener( ceConversationListener *listener );
	/** \brief Removes a listener. */
	void RemoveListener( ceConversationListener *listener );
	
	/** \brief Notifies all listeners that the changed or saved state changed. */
	virtual void NotifyStateChanged();
	/** \brief Notifies all listeners that the undo system changed. */
	virtual void NotifyUndoChanged();
	/** \brief Notifies all that view properties changed. */
	void NotifyViewChanged();
	/** \brief Notifies all that the conversation changed. */
	void NotifyConversationChanged();
	/** \brief Notifies all that the sky changed. */
	void NotifySkyChanged();
	/** \brief Notifies all that the environment object changed. */
	void NotifyEnvObjectChanged();
	/** \brief Notifies all that the camera changed. */
	void NotifyCameraChanged();
	
	/** \brief Notifies all that the target count or order changed. */
	void NotifyTargetStructureChanged();
	/** \brief Notifies all that a target changed. */
	void NotifyTargetChanged( ceTarget *target );
	/** \brief Notifies all that the active target changed. */
	void NotifyActiveTargetChanged();
	
	/** \brief Notifies all that the camera shot count or order changed. */
	void NotifyCameraShotStructureChanged();
	/** \brief Notifies all that a camera shot changed. */
	void NotifyCameraShotChanged( ceCameraShot *cameraShot );
	/** \brief Notifies all that the active camera shot changed. */
	void NotifyActiveCameraShotChanged();
	
	/** \brief Notifies all that the gesture count or order changed. */
	void NotifyGestureStructureChanged();
	/** \brief Notifies all that a gesture changed. */
	void NotifyGestureChanged( ceGesture *gesture );
	/** \brief Notifies all that the active gesture changed. */
	void NotifyActiveGestureChanged();
	
	/** \brief Notifies all that the face pose controller names changed. */
	void NotifyFacePoseControllerNamesChanged();
	/** \brief Notifies all that the face pose count or order changed. */
	void NotifyFacePoseStructureChanged();
	/** \brief Notifies all that a face pose changed. */
	void NotifyFacePoseChanged( ceFacePose *facePose );
	/** \brief Notifies all that the active face pose changed. */
	void NotifyActiveFacePoseChanged();
	
	/** \brief Notifies all that the file count or order changed. */
	void NotifyFileStructureChanged();
	/** \brief Notifies all that a file changed. */
	void NotifyFileChanged( ceConversationFile *file );
	/** \brief Notifies all that the file actor changed. */
	void NotifyActiveFileChanged();
	
	/** \brief Notifies all that the topic count or order changed. */
	void NotifyTopicStructureChanged( ceConversationFile *file );
	/** \brief Notifies all that a topic changed. */
	void NotifyTopicChanged( ceConversationFile *file, ceConversationTopic *topic );
	/** \brief Notifies all that the active topic changed. */
	void NotifyActiveTopicChanged( ceConversationFile *file );
	
	/** \brief Notifies all that the action structure changed. */
	void NotifyActionStructureChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	/** \brief Notifies all that an action changed. */
	void NotifyActionChanged( ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Notifies all that the condition structure changed. */
	void NotifyConditionStructureChanged( ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Notifies all that an condition changed. */
	void NotifyConditionChanged( ceConversationFile *file, ceConversationTopic *topic,
		ceConversationAction *action, ceConversationCondition *condition );
	
	/** \brief Notifies all that the active element changed. */
	void NotifyActiveChanged( ceConversationFile *file, ceConversationTopic *topic );
	
	
	
	/** \brief Notifies all that the actor count or order changed. */
	void NotifyActorStructureChanged();
	
	/** \brief Notifies all that an actor changed. */
	void NotifyActorChanged( ceConversationActor *actor );
	
	/** \brief Notify all actor active pose changed. */
	void NotifyActorActivePoseChanged( ceConversationActor *actor );
	
	/** \brief Notify listeners actor commands changed. */
	void NotifyActorCommandsChanged( ceConversationActor *actor );
	
	/** \brief Notify listeners actor poses changed. */
	void NotifyActorPosesChanged( ceConversationActor *actor );
	
	/** \brief Notify listeners actor pose gestures changed. */
	void NotifyActorPoseGesturesChanged( ceConversationActor *actor, ceActorPose *pose );
	
	/** \brief Notify all actor parameters changed. */
	void NotifyActorParametersChanged( ceConversationActor *actor );
	
	/** \brief Notify all active actor changed. */
	void NotifyActiveActorChanged();
	
	
	
	/** \brief Notifies all that the coordinate system count or order changed. */
	void NotifyCoordSystemStructureChanged();
	/** \brief Notifies all that an coordinate system changed. */
	void NotifyCoordSystemChanged( ceCoordSystem *coordSystem );
	/** \brief Notifies all that the active coordinate system changed. */
	void NotifyActiveCoordSystemChanged();
	
	/** \brief Notifies all that the prop count or order changed. */
	void NotifyPropStructureChanged();
	/** \brief Notifies all that a prop changed. */
	void NotifyPropChanged( ceProp *prop );
	/** \brief Notifies all that the active prop changed. */
	void NotifyActivePropChanged();
	
	/** \brief Notifies all that playback parameters changed. */
	void NotifyPlaybackChanged();
	/** \brief Notifies all that playback command list changed. */
	void NotifyPlaybackCommandListChanged();
	/** \brief Notifies all that playback variable list changed. */
	void NotifyPlaybackVarListChanged();
	/** \brief Notifies all that playback the trigger table changed. */
	void NotifyPlaybackTriggerTableChanged();
	/** \brief Notifies all that playback mising words changed. */
	void NotifyPlaybackMissingWordsChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
