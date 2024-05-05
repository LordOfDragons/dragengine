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

#include <stdlib.h>
#include <string.h>

#include "ceWPView.h"
#include "ceWPViewListener.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/actor/ceConversationActor.h"

#include <dragengine/common/exceptions.h>



// Class ceWPViewListener
///////////////////////////

// Constructor, destructor
////////////////////////////

ceWPViewListener::ceWPViewListener( ceWPView &panel ) :
pPanel( panel ){
}

ceWPViewListener::~ceWPViewListener(){
}



// Management
///////////////

void ceWPViewListener::ConversationChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateView();
	
	// if imports changed
	pPanel.UpdatePlaybackFileList();
	pPanel.UpdatePlaybackTopicList();
}

void ceWPViewListener::SkyChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateSky();
}

void ceWPViewListener::EnvObjectChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateEnvObject();
}

void ceWPViewListener::CameraChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCamera();
}



void ceWPViewListener::FileStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackFileList();
}

void ceWPViewListener::FileChanged( ceConversation *conversation, ceConversationFile* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackFileList();
}



void ceWPViewListener::TopicStructureChanged( ceConversation *conversation, ceConversationFile *file ){
	if( conversation != pPanel.GetConversation() || file != pPanel.GetSelectedPlaybackFile() ){
		return;
	}
	
	pPanel.UpdatePlaybackTopicList();
}

void ceWPViewListener::TopicChanged( ceConversation *conversation, ceConversationFile *file, ceConversationTopic* ){
	if( conversation != pPanel.GetConversation() || file != pPanel.GetSelectedPlaybackFile() ){
		return;
	}
	
	pPanel.UpdatePlaybackTopicList();
}



void ceWPViewListener::ActorStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActors();
}

void ceWPViewListener::ActorChanged( ceConversation *conversation, ceConversationActor* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActors();
}

void ceWPViewListener::ActorActivePoseChanged( ceConversation *conversation, ceConversationActor *actor ){
	if( conversation != pPanel.GetConversation() || actor != pPanel.GetActor() ){
		return;
	}
	
	pPanel.SelectActiveActorPose();
}

void ceWPViewListener::ActorPosesChanged( ceConversation *conversation, ceConversationActor *actor ){
	if( conversation != pPanel.GetConversation() || actor != pPanel.GetActor() ){
		return;
	}
	
	pPanel.UpdateActorPoses();
}

void ceWPViewListener::ActorPoseGesturesChanged( ceConversation *conversation,
ceConversationActor *actor, ceActorPose *pose ){
	if( conversation != pPanel.GetConversation() || actor != pPanel.GetActor() || pose != actor->GetActivePose() ){
		return;
	}
	
	pPanel.UpdateActorGestures();
}

void ceWPViewListener::ActorCommandsChanged( ceConversation *conversation, ceConversationActor *actor ){
	if( conversation != pPanel.GetConversation() || actor == pPanel.GetActor() ){
		pPanel.UpdateActorCommands();
	}
}

void ceWPViewListener::ActorParametersChanged( ceConversation *conversation, ceConversationActor *actor ){
	if( conversation != pPanel.GetConversation() || actor == pPanel.GetActor() ){
		pPanel.UpdateActorParameters();
	}
}

void ceWPViewListener::ActiveActorChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActors();
}



void ceWPViewListener::CoordSystemStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCoordSystems();
}

void ceWPViewListener::CoordSystemChanged( ceConversation *conversation, ceCoordSystem* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCoordSystems();
}

void ceWPViewListener::ActiveCoordSystemChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCoordSystems();
}



void ceWPViewListener::PropStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePropList();
}

void ceWPViewListener::PropChanged( ceConversation *conversation, ceProp* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePropList();
}

void ceWPViewListener::ActivePropChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateProp();
}



void ceWPViewListener::PlaybackChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlayback();
}

void ceWPViewListener::PlaybackCommandListChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackCommands();
}

void ceWPViewListener::PlaybackVarListChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackVariables();
}

void ceWPViewListener::PlaybackTriggerTableChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackTriggerTable();
}

void ceWPViewListener::PlaybackMissingWordsChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePlaybackMissingWords();
}
