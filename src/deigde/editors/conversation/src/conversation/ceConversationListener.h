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
class ceLookAt;
class ceGesture;
class ceFacePose;
class ceTarget;



/**
 * \brief Conversation Listener.
 */
class ceConversationListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new listener. */
	ceConversationListener();
	/** \brief Cleans up the listener. */
	virtual ~ceConversationListener();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( ceConversation *conversation );
	/** \brief Undo system changed. */
	virtual void UndoChanged( ceConversation *conversation );
	/** \brief View properties changed. */
	virtual void ViewChanged( ceConversation *conversation );
	/** \brief Conversation changed. */
	virtual void ConversationChanged( ceConversation *conversation );
	/** \brief The sky changed. */
	virtual void SkyChanged( ceConversation *conversation );
	/** \brief The environment object changed. */
	virtual void EnvObjectChanged( ceConversation *conversation );
	/** \brief Camera changed. */
	virtual void CameraChanged( ceConversation *conversation );
	
	/** \brief Target count or order changed. */
	virtual void TargetStructureChanged( ceConversation *conversation );
	/** \brief Target changed. */
	virtual void TargetChanged( ceConversation *conversation, ceTarget *target );
	/** \brief Active target changed. */
	virtual void ActiveTargetChanged( ceConversation *conversation );
	
	/** \brief Camera shot count or order changed. */
	virtual void CameraShotStructureChanged( ceConversation *conversation );
	/** \brief Camera shot changed. */
	virtual void CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot );
	/** \brief Active camera shot changed. */
	virtual void ActiveCameraShotChanged( ceConversation *conversation );
	
	/** \brief Gesture count or order changed. */
	virtual void GestureStructureChanged( ceConversation *conversation );
	/** \brief Gesture changed. */
	virtual void GestureChanged( ceConversation *conversation, ceGesture *gesture );
	/** \brief Active gesture changed. */
	virtual void ActiveGestureChanged( ceConversation *conversation );
	
	/** \brief Face pose controller names changed. */
	virtual void FacePoseControllerNamesChanged( ceConversation *conversation );
	/** \brief Face pose count or order changed. */
	virtual void FacePoseStructureChanged( ceConversation *conversation );
	/** \brief Face pose changed. */
	virtual void FacePoseChanged( ceConversation *conversation, ceFacePose *facePose );
	/** \brief Active face pose changed. */
	virtual void ActiveFacePoseChanged( ceConversation *conversation );
	
	/** \brief LookAt count or order changed. */
	virtual void LookAtStructureChanged( ceConversation *conversation );
	/** \brief LookAt changed. */
	virtual void LookAtChanged( ceConversation *conversation, ceLookAt *lookat );
	/** \brief Active lookat changed. */
	virtual void ActiveLookAtChanged( ceConversation *conversation );
	
	/** \brief File count or order changed. */
	virtual void FileStructureChanged( ceConversation *conversation );
	/** \brief File changed. */
	virtual void FileChanged( ceConversation *conversation, ceConversationFile *file );
	/** \brief Active file changed. */
	virtual void ActiveFileChanged( ceConversation *conversation );
	
	/** \brief Topic count or order changed. */
	virtual void TopicStructureChanged( ceConversation *conversation, ceConversationFile *file );
	/** \brief Topic changed. */
	virtual void TopicChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic );
	/** \brief Active topic changed. */
	virtual void ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file );
	
	/** \brief Action count or order changed. */
	virtual void ActionStructureChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	/** \brief Action changed. */
	virtual void ActionChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Condition count or order changed. */
	virtual void ConditionStructureChanged( ceConversation *conversation,
		ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action );
	
	/** \brief Condition changed. */
	virtual void ConditionChanged( ceConversation *conversation, ceConversationFile *file,
		ceConversationTopic *topic, ceConversationAction *action, ceConversationCondition *condition );
	
	/** \brief Active element changed. */
	virtual void ActiveChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic );
	
	
	
	/** \brief Actor count or order changed. */
	virtual void ActorStructureChanged( ceConversation *conversation );
	
	/** \brief Actor changed. */
	virtual void ActorChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor active pose changed. */
	virtual void ActorActivePoseChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor poses changed. */
	virtual void ActorPosesChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor active pose gestures changed. */
	virtual void ActorPoseGesturesChanged( ceConversation *conversation,
		ceConversationActor *actor, ceActorPose *pose );
	
	/** \brief Actor commands changed. */
	virtual void ActorCommandsChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Actor parameters changed. */
	virtual void ActorParametersChanged( ceConversation *conversation, ceConversationActor *actor );
	
	/** \brief Active actor changed. */
	virtual void ActiveActorChanged( ceConversation *conversation );
	
	
	
	/** \brief Coordinate system count or order changed. */
	virtual void CoordSystemStructureChanged( ceConversation *conversation );
	/** \brief Coordinate system changed. */
	virtual void CoordSystemChanged( ceConversation *conversation, ceCoordSystem *coordSystem );
	/** \brief Active coordinate system changed. */
	virtual void ActiveCoordSystemChanged( ceConversation *conversation );
	
	/** \brief Prop count or order changed. */
	virtual void PropStructureChanged( ceConversation *conversation );
	/** \brief Prop changed. */
	virtual void PropChanged( ceConversation *conversation, ceProp *prop );
	/** \brief Active prop changed. */
	virtual void ActivePropChanged( ceConversation *conversation );
	
	/** \brief Playback parameters changed. */
	virtual void PlaybackChanged( ceConversation *conversation );
	/** \brief Playback action stack changed. */
	virtual void PlaybackActionStackChanged( ceConversation *conversation );
	/** \brief Playback info command changed. */
	virtual void PlaybackCommandListChanged( ceConversation *conversation );
	/** \brief Playback variable list changed. */
	virtual void PlaybackVarListChanged( ceConversation *conversation );
	/** \brief Playback trigger table changed. */
	virtual void PlaybackTriggerTableChanged( ceConversation *conversation );
	/** \brief Playback missing words changed. */
	virtual void PlaybackMissingWordsChanged( ceConversation *conversation );
	/*@}*/
};

#endif
