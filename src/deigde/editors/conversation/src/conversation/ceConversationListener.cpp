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

void ceConversationListener::StateChanged( ceConversation* ){
}

void ceConversationListener::UndoChanged( ceConversation* ){
}

void ceConversationListener::ViewChanged( ceConversation* ){
}

void ceConversationListener::ConversationChanged( ceConversation* ){
}

void ceConversationListener::SkyChanged( ceConversation* ){
}

void ceConversationListener::EnvObjectChanged( ceConversation* ){
}

void ceConversationListener::CameraChanged( ceConversation* ){
}



void ceConversationListener::TargetStructureChanged( ceConversation* ){
}

void ceConversationListener::TargetChanged( ceConversation*, ceTarget* ){
}

void ceConversationListener::ActiveTargetChanged( ceConversation* ){
}



void ceConversationListener::CameraShotStructureChanged( ceConversation* ){
}

void ceConversationListener::CameraShotChanged( ceConversation*, ceCameraShot* ){
}

void ceConversationListener::ActiveCameraShotChanged( ceConversation* ){
}



void ceConversationListener::GestureStructureChanged( ceConversation* ){
}

void ceConversationListener::GestureChanged( ceConversation*, ceGesture* ){
}

void ceConversationListener::ActiveGestureChanged( ceConversation* ){
}



void ceConversationListener::FacePoseControllerNamesChanged( ceConversation* ){
}

void ceConversationListener::FacePoseStructureChanged( ceConversation* ){
}

void ceConversationListener::FacePoseChanged( ceConversation*, ceFacePose* ){
}

void ceConversationListener::ActiveFacePoseChanged( ceConversation* ){
}



void ceConversationListener::FileStructureChanged( ceConversation* ){
}

void ceConversationListener::FileChanged( ceConversation*, ceConversationFile* ){
}

void ceConversationListener::ActiveFileChanged( ceConversation* ){
}



void ceConversationListener::TopicStructureChanged( ceConversation*, ceConversationFile* ){
}

void ceConversationListener::TopicChanged( ceConversation*, ceConversationFile*, ceConversationTopic* ){
}

void ceConversationListener::ActiveTopicChanged( ceConversation*, ceConversationFile* ){
}



void ceConversationListener::ActionStructureChanged( ceConversation*, ceConversationFile*,
ceConversationTopic*, ceConversationAction* ){
}

void ceConversationListener::ActionChanged( ceConversation*, ceConversationFile*,
ceConversationTopic*, ceConversationAction* ){
}

void ceConversationListener::ConditionStructureChanged( ceConversation*,
ceConversationFile*, ceConversationTopic*, ceConversationAction* ){
}

void ceConversationListener::ConditionChanged( ceConversation*, ceConversationFile*,
ceConversationTopic*, ceConversationAction*, ceConversationCondition* ){
}

void ceConversationListener::ActiveChanged( ceConversation*, ceConversationFile*, ceConversationTopic* ){
}



void ceConversationListener::ActorStructureChanged( ceConversation* ){
}

void ceConversationListener::ActorChanged( ceConversation*, ceConversationActor* ){
}

void ceConversationListener::ActorActivePoseChanged( ceConversation*, ceConversationActor* ){
}

void ceConversationListener::ActorPosesChanged( ceConversation *conversation,
ceConversationActor *actor ){
	ActorChanged( conversation, actor );
}

void ceConversationListener::ActorPoseGesturesChanged( ceConversation *conversation,
ceConversationActor *actor, ceActorPose* ){
	ActorPosesChanged( conversation, actor );
}

void ceConversationListener::ActorCommandsChanged( ceConversation*, ceConversationActor* ){
}

void ceConversationListener::ActorParametersChanged( ceConversation*, ceConversationActor* ){
}

void ceConversationListener::ActiveActorChanged( ceConversation* ){
}



void ceConversationListener::CoordSystemStructureChanged( ceConversation* ){
}

void ceConversationListener::CoordSystemChanged( ceConversation*, ceCoordSystem* ){
}

void ceConversationListener::ActiveCoordSystemChanged( ceConversation* ){
}



void ceConversationListener::PropStructureChanged( ceConversation* ){
}

void ceConversationListener::PropChanged( ceConversation*, ceProp* ){
}

void ceConversationListener::ActivePropChanged( ceConversation* ){
}



void ceConversationListener::PlaybackChanged( ceConversation* ){
}

void ceConversationListener::PlaybackActionStackChanged( ceConversation* ){
}

void ceConversationListener::PlaybackCommandListChanged( ceConversation* ){
}

void ceConversationListener::PlaybackVarListChanged( ceConversation* ){
}

void ceConversationListener::PlaybackTriggerTableChanged( ceConversation* ){
}

void ceConversationListener::PlaybackMissingWordsChanged( ceConversation* ){
}



void ceConversationListener::LanguagePackChanged ( ceConversation* ) {
}
