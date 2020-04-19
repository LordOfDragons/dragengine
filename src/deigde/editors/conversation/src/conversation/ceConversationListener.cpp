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

#include "ceConversationListener.h"



// Class ceConversationListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationListener::ceConversationListener(){
}

ceConversationListener::~ceConversationListener(){
}



// Notifications
//////////////////

void ceConversationListener::StateChanged( ceConversation *conversation ){
}

void ceConversationListener::UndoChanged( ceConversation *conversation ){
}

void ceConversationListener::ViewChanged( ceConversation *conversation ){
}

void ceConversationListener::ConversationChanged( ceConversation *conversation ){
}

void ceConversationListener::SkyChanged( ceConversation *conversation ){
}

void ceConversationListener::EnvObjectChanged( ceConversation *conversation ){
}

void ceConversationListener::CameraChanged( ceConversation *conversation ){
}



void ceConversationListener::TargetStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::TargetChanged( ceConversation *conversation, ceTarget *target ){
}

void ceConversationListener::ActiveTargetChanged( ceConversation *conversation ){
}



void ceConversationListener::CameraShotStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot ){
}

void ceConversationListener::ActiveCameraShotChanged( ceConversation *conversation ){
}



void ceConversationListener::PoseStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::PoseChanged( ceConversation *conversation, cePose *pose ){
}

void ceConversationListener::ActivePoseChanged( ceConversation *conversation ){
}



void ceConversationListener::GestureStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::GestureChanged( ceConversation *conversation, ceGesture *gesture ){
}

void ceConversationListener::ActiveGestureChanged( ceConversation *conversation ){
}



void ceConversationListener::FacePoseControllerNamesChanged( ceConversation *conversation ){
}

void ceConversationListener::FacePoseStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::FacePoseChanged( ceConversation *conversation, ceFacePose *facePose ){
}

void ceConversationListener::ActiveFacePoseChanged( ceConversation *conversation ){
}



void ceConversationListener::LookAtStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::LookAtChanged( ceConversation *conversation, ceLookAt *lookat ){
}

void ceConversationListener::ActiveLookAtChanged( ceConversation *conversation ){
}



void ceConversationListener::FileStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::FileChanged( ceConversation *conversation, ceConversationFile *file ){
}

void ceConversationListener::ActiveFileChanged( ceConversation *conversation ){
}



void ceConversationListener::TopicStructureChanged( ceConversation *conversation, ceConversationFile *file ){
}

void ceConversationListener::TopicChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic ){
}

void ceConversationListener::ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file ){
}



void ceConversationListener::ActionStructureChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction *action ){
}

void ceConversationListener::ActionChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction *action ){
}

void ceConversationListener::ActiveActionChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic *topic ){
}



void ceConversationListener::ActorStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::ActorChanged( ceConversation *conversation,
ceConversationActor *actor ){
}

void ceConversationListener::ActorActivePoseChanged( ceConversation *conversation,
ceConversationActor *actor ){
}

void ceConversationListener::ActorPosesChanged( ceConversation *conversation,
ceConversationActor *actor ){
	ActorChanged( conversation, actor );
}

void ceConversationListener::ActorPoseGesturesChanged( ceConversation *conversation,
ceConversationActor *actor, ceActorPose *pose ){
	ActorPosesChanged( conversation, actor );
}

void ceConversationListener::ActorCommandsChanged( ceConversation *conversation, ceConversationActor *actor ){
}

void ceConversationListener::ActorParametersChanged( ceConversation *conversation, ceConversationActor *actor ){
}

void ceConversationListener::ActiveActorChanged( ceConversation *conversation ){
}



void ceConversationListener::CoordSystemStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::CoordSystemChanged( ceConversation *conversation, ceCoordSystem *coordSystem ){
}

void ceConversationListener::ActiveCoordSystemChanged( ceConversation *conversation ){
}



void ceConversationListener::PropStructureChanged( ceConversation *conversation ){
}

void ceConversationListener::PropChanged( ceConversation *conversation, ceProp *prop ){
}

void ceConversationListener::ActivePropChanged( ceConversation *conversation ){
}



void ceConversationListener::PlaybackChanged( ceConversation *conversation ){
}

void ceConversationListener::PlaybackActionStackChanged( ceConversation *conversation ){
}

void ceConversationListener::PlaybackCommandListChanged( ceConversation *conversation ){
}

void ceConversationListener::PlaybackVarListChanged( ceConversation *conversation ){
}

void ceConversationListener::PlaybackTriggerTableChanged( ceConversation *conversation ){
}

void ceConversationListener::PlaybackMissingWordsChanged( ceConversation *conversation ){
}
