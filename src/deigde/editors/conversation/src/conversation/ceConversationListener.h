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

#ifndef _CECONVERSATIONLISTENER_H_
#define _CECONVERSATIONLISTENER_H_

#include <dragengine/deObject.h>

class ceActorPose;
class ceProp;
class ceCoordSystem;
class ceConversation;
class ceConversationActor;
class ceConversationFile;
class ceConversationTopic;
class ceConversationAction;
class ceConversationCondition;
class ceCameraShot;
class ceGesture;
class ceFacePose;
class ceTarget;



/**
 * Conversation Listener.
 */
class ceConversationListener : public deObject{
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceConversationListener> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	ceConversationListener();
	/** Cleans up the listener. */
	virtual ~ceConversationListener();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged(ceConversation *conversation);
	/** Undo system changed. */
	virtual void UndoChanged(ceConversation *conversation);
	/** View properties changed. */
	virtual void ViewChanged(ceConversation *conversation);
	/** Conversation changed. */
	virtual void ConversationChanged(ceConversation *conversation);
	/** The sky changed. */
	virtual void SkyChanged(ceConversation *conversation);
	/** The environment object changed. */
	virtual void EnvObjectChanged(ceConversation *conversation);
	/** Camera changed. */
	virtual void CameraChanged(ceConversation *conversation);
	
	/** Target count or order changed. */
	virtual void TargetStructureChanged(ceConversation *conversation);
	/** Target changed. */
	virtual void TargetChanged(ceConversation *conversation, ceTarget *target);
	/** Active target changed. */
	virtual void ActiveTargetChanged(ceConversation *conversation);
	
	/** Camera shot count or order changed. */
	virtual void CameraShotStructureChanged(ceConversation *conversation);
	/** Camera shot changed. */
	virtual void CameraShotChanged(ceConversation *conversation, ceCameraShot *cameraShot);
	/** Active camera shot changed. */
	virtual void ActiveCameraShotChanged(ceConversation *conversation);
	
	/** Gesture count or order changed. */
	virtual void GestureStructureChanged(ceConversation *conversation);
	/** Gesture changed. */
	virtual void GestureChanged(ceConversation *conversation, ceGesture *gesture);
	/** Active gesture changed. */
	virtual void ActiveGestureChanged(ceConversation *conversation);
	
	/** Face pose controller names changed. */
	virtual void FacePoseControllerNamesChanged(ceConversation *conversation);
	/** Face pose count or order changed. */
	virtual void FacePoseStructureChanged(ceConversation *conversation);
	/** Face pose changed. */
	virtual void FacePoseChanged(ceConversation *conversation, ceFacePose *facePose);
	/** Active face pose changed. */
	virtual void ActiveFacePoseChanged(ceConversation *conversation);
	
	/** File count or order changed. */
	virtual void FileStructureChanged(ceConversation *conversation);
	/** File changed. */
	virtual void FileChanged(ceConversation *conversation, ceConversationFile *file);
	/** Active file changed. */
	virtual void ActiveFileChanged(ceConversation *conversation);
	
	/** Topic count or order changed. */
	virtual void TopicStructureChanged(ceConversation *conversation, ceConversationFile *file);
	/** Topic changed. */
	virtual void TopicChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic);
	/** Active topic changed. */
	virtual void ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file);
	
	/** Action count or order changed. */
	virtual void ActionStructureChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action);
	/** Action changed. */
	virtual void ActionChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action);
	
	/** Condition count or order changed. */
	virtual void ConditionStructureChanged(ceConversation *conversation,
		ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action);
	
	/** Condition changed. */
	virtual void ConditionChanged(ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition);
	
	/** Active element changed. */
	virtual void ActiveChanged(ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic);
	
	
	
	/** Actor count or order changed. */
	virtual void ActorStructureChanged(ceConversation *conversation);
	
	/** Actor changed. */
	virtual void ActorChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** Actor active pose changed. */
	virtual void ActorActivePoseChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** Actor poses changed. */
	virtual void ActorPosesChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** Actor active pose gestures changed. */
	virtual void ActorPoseGesturesChanged(ceConversation *conversation,
		ceConversationActor *actor, ceActorPose *pose);
	
	/** Actor commands changed. */
	virtual void ActorCommandsChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** Actor parameters changed. */
	virtual void ActorParametersChanged(ceConversation *conversation, ceConversationActor *actor);
	
	/** Active actor changed. */
	virtual void ActiveActorChanged(ceConversation *conversation);
	
	
	
	/** Coordinate system count or order changed. */
	virtual void CoordSystemStructureChanged(ceConversation *conversation);
	/** Coordinate system changed. */
	virtual void CoordSystemChanged(ceConversation *conversation, ceCoordSystem *coordSystem);
	/** Active coordinate system changed. */
	virtual void ActiveCoordSystemChanged(ceConversation *conversation);
	
	/** Prop count or order changed. */
	virtual void PropStructureChanged(ceConversation *conversation);
	/** Prop changed. */
	virtual void PropChanged(ceConversation *conversation, ceProp *prop);
	/** Active prop changed. */
	virtual void ActivePropChanged(ceConversation *conversation);
	
	/** Playback parameters changed. */
	virtual void PlaybackChanged(ceConversation *conversation);
	/** Playback action stack changed. */
	virtual void PlaybackActionStackChanged(ceConversation *conversation);
	/** Playback info command changed. */
	virtual void PlaybackCommandListChanged(ceConversation *conversation);
	/** Playback variable list changed. */
	virtual void PlaybackVarListChanged(ceConversation *conversation);
	/** Playback trigger table changed. */
	virtual void PlaybackTriggerTableChanged(ceConversation *conversation);
	/** Playback missing words changed. */
	virtual void PlaybackMissingWordsChanged(ceConversation *conversation);
	
	
	
	/** Language pack changed. */
	virtual void LanguagePackChanged(ceConversation *conversation);
	/*@}*/
};

#endif
