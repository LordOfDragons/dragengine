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
