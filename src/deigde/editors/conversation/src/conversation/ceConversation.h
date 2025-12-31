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

#include "actor/ceConversationActor.h"
#include "actor/ceConversationActorList.h"
#include "camerashot/ceCameraShot.h"
#include "file/ceConversationFile.h"
#include "facepose/ceFacePose.h"
#include "gesture/ceGesture.h"
#include "target/ceTarget.h"
#include "textbox/ceTextBox.h"
#include "coordsystem/ceCoordSystem.h"
#include "coordsystem/ceCoordSystemList.h"
#include "infobox/ceConversationInfoBox.h"
#include "prop/ceProp.h"
#include "playerChoiceBox/cePlayerChoiceBox.h"
#include "playback/cePlayback.h"
#include "../langpack/ceLangPack.h"

#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>


class ceConversationAction;
class ceConversationCondition;
class ceConversationListener;
class ceActorPose;
class ceLoadSaveSystem;

class igdeWSky;
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
	typedef decTObjectOrderedSet<ceConversation> List;
	typedef decTObjectOrderedSet<ceCameraShot> CameraShotList;
	
	
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
	igdeWObject::Ref pEnvObject;
	
	deMicrophone::Ref pEngMicrophone;
	deSpeaker::Ref pEngSpeakerVAPreview;
	
	igdeCamera *pCamera;
	igdeCamera *pCameraFree;
	ceTextBox::Ref pTextBox;
	cePlayerChoiceBox::Ref pPlayerChoiceBox;
	ceConversationInfoBox::Ref pInfoBox;
	cePlayback::Ref pPlayback;
	
	float pScreenRatio;
	bool pShowRuleOfThirdsAid;
	
	decStringList pImportConversationPath;
	List pImportedConversations;
	
	ceTarget::List pTargets;
	ceTarget::Ref pActiveTarget;
	
	CameraShotList pCameraShots;
	ceCameraShot::Ref pActiveCameraShot;
	
	ceGesture::List pGestures;
	ceGesture::Ref pActiveGesture;
	
	decStringList pFacePoseControllerNames;
	ceFacePose::List pFacePoses;
	ceFacePose::Ref pActiveFacePose;
	
	ceConversationFile::List pFiles;
	ceConversationFile::Ref pActiveFile;
	
	ceConversationActorList pActorList;
	ceConversationActor::Ref pActiveActor;
	
	ceCoordSystemList pCoordSystemList;
	ceCoordSystem::Ref pActiveCoordSystem;
	
	ceProp::List pProps;
	ceProp::Ref pActiveProp;
	
	decString pCTSPath;
	decString pCTAPath;
	decString pCTFIPath;
	decString pLangPackPath;
	decString pLangPackEntryName;
	sPasteSnippetDialogParams pPasteSnippetDialogParams;
	
	ceLangPack::Ref pLangPack;
	
	decTObjectOrderedSet<ceConversationListener> pListeners;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation. */
	ceConversation(igdeEnvironment *environment);
	
	ceConversation(const ceConversation &) = delete;
	
	/** Cleans up the conversation. */
protected:
	virtual ~ceConversation();
public:
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
	inline const igdeWObject::Ref &GetEnvObject() const{ return pEnvObject; }
	
	/** Retrieves the camera. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** Free roaming camera. */
	inline igdeCamera *GetCameraFree() const{ return pCameraFree; }
	
	/** Retrieves the text box. */
	inline const ceTextBox::Ref &GetTextBox() const{ return pTextBox; }
	/** Retrieves the player choice box. */
	inline const cePlayerChoiceBox::Ref &GetPlayerChoiceBox() const{ return pPlayerChoiceBox; }
	/** Retrieves the info box. */
	inline const ceConversationInfoBox::Ref &GetInfoBox() const{ return pInfoBox; }
	/** Retrieves the playback object. */
	inline const cePlayback::Ref &GetPlayback() const{ return pPlayback; }
	
	/** Retrieves the ration of the screen. */
	inline float GetScreenRatio() const{ return pScreenRatio; }
	/** Sets the ratio of the screen. */
	void SetScreenRatio(float ratio);
	/** Determines if the rule of thirds aid is displayed. */
	inline bool GetShowRuleOfThirdsAid() const{ return pShowRuleOfThirdsAid; }
	/** Sets if the rule of thirds aid is displayed. */
	void SetShowRuleOfThirdsAid(bool showRuleOfThirdsAid);
	
	/** Dispose of all resources. */
	void Dispose();
	/** Updates the sky. */
	void Update(float elapsed);
	/** Resets the sky. */
	void Reset();
	
	/** Activate microphone. */
	void ActivateMicrophone();
	
	
	
	/** CTS path. */
	inline const decString &GetCTSPath() const{ return pCTSPath; }
	void SetCTSPath(const char *path);
	
	/** CTA path. */
	inline const decString &GetCTAPath() const{ return pCTAPath; }
	void SetCTAPath(const char *path);
	
	/** CTFI path. */
	inline const decString &GetCTFIPath() const{ return pCTFIPath; }
	void SetCTFIPath(const char *path);
	
	/** Language pack path. */
	inline const decString &GetLangPackPath() const{ return pLangPackPath; }
	void SetLangPackPath(const char *path);
	
	/** Language pack entry name. */
	inline const decString &GetLangPackEntryName() const{ return pLangPackEntryName; }
	void SetLangPackEntryName(const char *name);
	
	/** Paste snippet dialog parameters. */
	inline sPasteSnippetDialogParams &GetPasteSnippetDialogParams(){ return pPasteSnippetDialogParams; }
	
	/** Language pack or nullptr. */
	inline const ceLangPack::Ref &GetLanguagePack() const{ return pLangPack; }
	void SetLanguagePack(ceLangPack *langpack);
	/*@}*/
	
	
	
	/** \name Imported conversations */
	/*@{*/
	/** List of import conversation path. */
	inline const decStringList &GetImportConversationPath() const{ return pImportConversationPath; }
	
	/** Set list of import conversation path. */
	void SetImportConversationPath(const decStringList &list);
	
	/** Update imported conversations from path. */
	void UpdateImportedConversations(ceLoadSaveSystem &lssystem);
	
	/** Imported conversations. */
	inline const List &GetImportedConversations() const{ return pImportedConversations; }
	/*@}*/
	
	
	
	/** \name Targets */
	/*@{*/
	/** Retrieves the target list read-only. */
	inline const ceTarget::List &GetTargets() const{ return pTargets; }
	
	/** Adds a target. */
	void AddTarget(ceTarget *target);
	
	/** Removes a target. */
	void RemoveTarget(ceTarget *target);
	
	/** Remove all targets. */
	void RemoveAllTargets();
	
	/** Retrieves the active target or nullptr if none is active. */
	inline const ceTarget::Ref &GetActiveTarget() const{ return pActiveTarget; }
	
	/** Sets the active target or nullptr if none is active. */
	void SetActiveTarget(ceTarget *target);
	
	/** Named target including imported conversations. */
	ceTarget *GetTargetNamed(const char *name) const;
	
	/** All targets including imported conversations. */
	ceTarget::List AllTargets() const;
	void AllTargets(ceTarget::List &list) const;
	/*@}*/
	
	/** \name Camera Shots */
	/*@{*/
	/** Retrieves the camera shot list read-only. */
	inline const CameraShotList &GetCameraShotList() const{ return pCameraShots; }
	
	/** Adds a camera shot. */
	void AddCameraShot(ceCameraShot *cameraShot);
	
	/** Removes a camera shot. */
	void RemoveCameraShot(ceCameraShot *cameraShot);
	
	/** Remove all camera shots. */
	void RemoveAllCameraShots();
	
	/** Retrieves the active camera shot or nullptr if none is active. */
	inline const ceCameraShot::Ref &GetActiveCameraShot() const{ return pActiveCameraShot; }
	
	/** Sets the active camera shot or nullptr if none is active. */
	void SetActiveCameraShot(ceCameraShot *cameraShot);
	
	/** Named camera shot including imported conversations. */
	ceCameraShot *GetCameraShotNamed(const char *name) const;
	
	/** All camera shots including imported conversations. */
	CameraShotList GetAllCameraShots() const;
	void GetAllCameraShots(CameraShotList &list) const;
	/*@}*/
	
	
	
	/** \name Gestures */
	/*@{*/
	/** Retrieves the gesture list read-only. */
	inline const ceGesture::List &GetGestures() const{ return pGestures; }
	
	/** Adds a gesture. */
	void AddGesture(ceGesture *gesture);
	
	/** Removes a gesture. */
	void RemoveGesture(ceGesture *gesture);
	
	/** Remove all gestures. */
	void RemoveAllGestures();
	
	/** Retrieves the active gesture or nullptr if none is active. */
	inline const ceGesture::Ref &GetActiveGesture() const{ return pActiveGesture; }
	
	/** Sets the active gesture or nullptr if none is active. */
	void SetActiveGesture(ceGesture *gesture);
	
	/** Named gesture including imported conversations. */
	ceGesture *GetGestureNamed(const char *name) const;
	
	/** All gestures including imported conversations. */
	ceGesture::List AllGestures() const;
	void AllGestures(ceGesture::List &list) const;
	/*@}*/
	
	
	
	/** \name FacePoses */
	/*@{*/
	/** Retrieves the list of face pose controller names. */
	inline decStringList &GetFacePoseControllerNameList(){ return pFacePoseControllerNames; }
	inline const decStringList &GetFacePoseControllerNameList() const{ return pFacePoseControllerNames; }
	
	/** Retrieves the face pose list read-only. */
	inline const ceFacePose::List &GetFacePoseList() const{ return pFacePoses; }
	
	/** Adds a face pose. */
	void AddFacePose(ceFacePose *facePose);
	
	/** Removes a face pose. */
	void RemoveFacePose(ceFacePose *facePose);
	
	/** Remove all face poses. */
	void RemoveAllFacePoses();
	
	/** Retrieves the active face pose or nullptr if none is active. */
	inline const ceFacePose::Ref &GetActiveFacePose() const{ return pActiveFacePose; }
	
	/** Sets the active face pose or nullptr if none is active. */
	void SetActiveFacePose(ceFacePose *facePose);
	
	/** Named face pose including imported conversations. */
	ceFacePose *GetFacePoseNamed(const char *name) const;
	
	/** All face poses including imported conversations. */
	ceFacePose::List AllFacePoses() const;
	void AllFacePoses(ceFacePose::List &list) const;
	/*@}*/
	
	
	
	/** \name Files */
	/*@{*/
	/** Retrieves the file list read-only. */
	inline const ceConversationFile::List &GetFiles() const{ return pFiles; }
	
	/** Adds an file. */
	void AddFile(ceConversationFile *file);
	
	/** Removes an file. */
	void RemoveFile(ceConversationFile *file);
	
	/** Remove all files. */
	void RemoveAllFiles();
	
	/** Retrieves the active texture or nullptr if none is active. */
	inline const ceConversationFile::Ref &GetActiveFile() const{ return pActiveFile; }
	
	/** Sets the active file or nullptr if none is active. */
	void SetActiveFile(ceConversationFile *file);
	
	/** Named file including imported conversations. */
	ceConversationFile *GetFileWithID(const char *name) const;
	
	/** All files including imported conversations. */
	ceConversationFile::List AllFiles() const;
	void AllFiles(ceConversationFile::List &list) const;
	
	/** Named topic including imported conversations. */
	ceConversationTopic *GetTopicWithID(const char *fileName, const char *topicName) const;
	
	/** All files including imported conversations. */
	ceConversationTopic::List AllTopics(const char *fileName) const;
	void AllTopics(const char *fileName, ceConversationTopic::List &list) const;
	/*@}*/
	
	
	
	/** \name Actors */
	/*@{*/
	/** Retrieves the actor list read-only. */
	inline const ceConversationActorList &GetActorList() const{ return pActorList; }
	/** Adds a new actor. */
	void AddActor(ceConversationActor *actor);
	/** Removes a actor. */
	void RemoveActor(ceConversationActor *actor);
	/** Removes all actors. */
	void RemoveAllActors();
	/** Retrieves the active texture or nullptr if none is active. */
	inline const ceConversationActor::Ref &GetActiveActor() const{ return pActiveActor; }
	/** Determines if there is an active actor or not. */
	bool HasActiveActor() const;
	/** Sets the active actor or nullptr if none is active. */
	void SetActiveActor(ceConversationActor *actor);
	/*@}*/
	
	
	
	/** \name Coordinate Systems */
	/*@{*/
	/** Retrieves the coordinate system list read-only. */
	inline const ceCoordSystemList &GetCoordSystemList() const{ return pCoordSystemList; }
	/** Adds a new coordinate system. */
	void AddCoordSystem(ceCoordSystem *coordSystem);
	/** Removes a coordinate system. */
	void RemoveCoordSystem(ceCoordSystem *coordSystem);
	/** Removes all coordinate system. */
	void RemoveAllCoordSystems();
	/** Retrieves the active coordinate system or nullptr if none is active. */
	inline const ceCoordSystem::Ref &GetActiveCoordSystem() const{ return pActiveCoordSystem; }
	/** Determines if there is an active coordinate system or not. */
	bool HasActiveCoordSystem() const;
	/** Sets the active coordinate system or nullptr if none is active. */
	void SetActiveCoordSystem(ceCoordSystem *coordSystem);
	/*@}*/
	
	
	
	/** \name Props */
	/*@{*/
	/** Retrieves the prop list read-only. */
	inline const ceProp::List &GetProps() const{ return pProps; }
	
	/** Adds a prop. */
	void AddProp(ceProp *prop);
	
	/** Removes a prop. */
	void RemoveProp(ceProp *prop);
	
	/** Removes all props. */
	void RemoveAllProps();
	
	/** Retrieves the active prop or nullptr if none is active. */
	inline const ceProp::Ref &GetActiveProp() const{ return pActiveProp; }
	
	/** Determines if there is an active prop or not. */
	bool HasActiveProp() const;
	
	/** Sets the active prop or nullptr if none is active. */
	void SetActiveProp(ceProp *prop);
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** Adds a listener. */
	void AddListener(ceConversationListener *listener);
	/** Removes a listener. */
	void RemoveListener(ceConversationListener *listener);
	
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
	void NotifyTargetChanged(ceTarget *target);
	/** Notifies all that the active target changed. */
	void NotifyActiveTargetChanged();
	
	/** Notifies all that the camera shot count or order changed. */
	void NotifyCameraShotStructureChanged();
	/** Notifies all that a camera shot changed. */
	void NotifyCameraShotChanged(ceCameraShot *cameraShot);
	/** Notifies all that the active camera shot changed. */
	void NotifyActiveCameraShotChanged();
	
	/** Notifies all that the gesture count or order changed. */
	void NotifyGestureStructureChanged();
	/** Notifies all that a gesture changed. */
	void NotifyGestureChanged(ceGesture *gesture);
	/** Notifies all that the active gesture changed. */
	void NotifyActiveGestureChanged();
	
	/** Notifies all that the face pose controller names changed. */
	void NotifyFacePoseControllerNamesChanged();
	/** Notifies all that the face pose count or order changed. */
	void NotifyFacePoseStructureChanged();
	/** Notifies all that a face pose changed. */
	void NotifyFacePoseChanged(ceFacePose *facePose);
	/** Notifies all that the active face pose changed. */
	void NotifyActiveFacePoseChanged();
	
	/** Notifies all that the file count or order changed. */
	void NotifyFileStructureChanged();
	/** Notifies all that a file changed. */
	void NotifyFileChanged(ceConversationFile *file);
	/** Notifies all that the file actor changed. */
	void NotifyActiveFileChanged();
	
	/** Notifies all that the topic count or order changed. */
	void NotifyTopicStructureChanged(ceConversationFile *file);
	/** Notifies all that a topic changed. */
	void NotifyTopicChanged(ceConversationFile *file, ceConversationTopic *topic);
	/** Notifies all that the active topic changed. */
	void NotifyActiveTopicChanged(ceConversationFile *file);
	
	/** Notifies all that the action structure changed. */
	void NotifyActionStructureChanged(ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action);
	/** Notifies all that an action changed. */
	void NotifyActionChanged(ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action);
	
	/** Notifies all that the condition structure changed. */
	void NotifyConditionStructureChanged(ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action);
	
	/** Notifies all that an condition changed. */
	void NotifyConditionChanged(ceConversationFile *file, ceConversationTopic *topic,
		ceConversationAction *action, ceConversationCondition *condition);
	
	/** Notifies all that the active element changed. */
	void NotifyActiveChanged(ceConversationFile *file, ceConversationTopic *topic);
	
	
	
	/** Notifies all that the actor count or order changed. */
	void NotifyActorStructureChanged();
	
	/** Notifies all that an actor changed. */
	void NotifyActorChanged(ceConversationActor *actor);
	
	/** Notify all actor active pose changed. */
	void NotifyActorActivePoseChanged(ceConversationActor *actor);
	
	/** Notify listeners actor commands changed. */
	void NotifyActorCommandsChanged(ceConversationActor *actor);
	
	/** Notify listeners actor poses changed. */
	void NotifyActorPosesChanged(ceConversationActor *actor);
	
	/** Notify listeners actor pose gestures changed. */
	void NotifyActorPoseGesturesChanged(ceConversationActor *actor, ceActorPose *pose);
	
	/** Notify all actor parameters changed. */
	void NotifyActorParametersChanged(ceConversationActor *actor);
	
	/** Notify all active actor changed. */
	void NotifyActiveActorChanged();
	
	
	
	/** Notifies all that the coordinate system count or order changed. */
	void NotifyCoordSystemStructureChanged();
	/** Notifies all that an coordinate system changed. */
	void NotifyCoordSystemChanged(ceCoordSystem *coordSystem);
	/** Notifies all that the active coordinate system changed. */
	void NotifyActiveCoordSystemChanged();
	
	/** Notifies all that the prop count or order changed. */
	void NotifyPropStructureChanged();
	/** Notifies all that a prop changed. */
	void NotifyPropChanged(ceProp *prop);
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
