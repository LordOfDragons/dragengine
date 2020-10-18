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

#include "ceWPTopic.h"
#include "ceWPTopicListener.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceWPTopicListener
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPTopicListener::ceWPTopicListener( ceWPTopic &panel ) :
pPanel( panel ){
}

ceWPTopicListener::~ceWPTopicListener(){
}



// Management
///////////////

void ceWPTopicListener::ConversationChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	// if imports changed
	pPanel.UpdateTargetLists();
	pPanel.UpdateCameraShotLists();
	pPanel.UpdateGestureLists();
	pPanel.UpdateFacePoseLists();
	pPanel.UpdateFileList();
	pPanel.UpdateTopicList();
}

void ceWPTopicListener::TargetStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateTargetLists();
}

void ceWPTopicListener::TargetChanged( ceConversation *conversation, ceTarget* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateTargetLists();
}



void ceWPTopicListener::CameraShotStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCameraShotLists();
}

void ceWPTopicListener::CameraShotChanged( ceConversation *conversation, ceCameraShot* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCameraShotLists();
}



void ceWPTopicListener::GestureStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateGestureLists();
}

void ceWPTopicListener::GestureChanged( ceConversation *conversation, ceGesture* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateGestureLists();
}



void ceWPTopicListener::FacePoseStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFacePoseLists();
}

void ceWPTopicListener::FacePoseChanged( ceConversation *conversation, ceFacePose* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFacePoseLists();
}



void ceWPTopicListener::FileStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFileList();
}

void ceWPTopicListener::FileChanged( ceConversation *conversation, ceConversationFile* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFileList();
}

void ceWPTopicListener::ActiveFileChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveFile();
}



void ceWPTopicListener::TopicStructureChanged( ceConversation *conversation, ceConversationFile *file ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile() ){
		return;
	}
	
	pPanel.UpdateTopicList();
}

void ceWPTopicListener::TopicChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic* ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile() ){
		return;
	}
	
	pPanel.UpdateTopicList();
}

void ceWPTopicListener::ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile() ){
		return;
	}
	
	pPanel.SelectActiveTopic();
}



void ceWPTopicListener::ActionStructureChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction* ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic() ){
		return;
	}
	
	pPanel.UpdateActive();
}

void ceWPTopicListener::ActionChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction* ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic() ){
		return;
	}
	
	pPanel.UpdateActive();
}



void ceWPTopicListener::ActorStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActorLists();
}

void ceWPTopicListener::ActorChanged( ceConversation *conversation, ceConversationActor* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActorLists();
}



void ceWPTopicListener::CoordSystemStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateConvoCoordSysLists();
}

void ceWPTopicListener::CoordSystemChanged( ceConversation *conversation, ceCoordSystem* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateConvoCoordSysLists();
}
