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
