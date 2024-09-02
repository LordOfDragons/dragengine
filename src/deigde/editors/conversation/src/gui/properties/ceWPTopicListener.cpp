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

void ceWPTopicListener::ConditionStructureChanged( ceConversation *conversation,
ceConversationFile *file, ceConversationTopic *topic, ceConversationAction* ){
	if( conversation != pPanel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic() ){
		return;
	}
	
	pPanel.UpdateActive();
}

void ceWPTopicListener::ConditionChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction*, ceConversationCondition* ){
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



void ceWPTopicListener::LanguagePackChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActive();
}
