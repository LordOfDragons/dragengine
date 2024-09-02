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

#include "ceWPConversation.h"
#include "ceWPConversationListener.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceWPConversationListener
///////////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPConversationListener::ceWPConversationListener( ceWPConversation &panel ) :
pPanel( panel ){
}

ceWPConversationListener::~ceWPConversationListener(){
}



// Management
///////////////

void ceWPConversationListener::ConversationChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateConversation();
}



void ceWPConversationListener::TargetStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateTargetList();
}

void ceWPConversationListener::TargetChanged( ceConversation *conversation, ceTarget* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateTargetList();
}

void ceWPConversationListener::ActiveTargetChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveTarget();
}



void ceWPConversationListener::CameraShotStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCameraShotList();
}

void ceWPConversationListener::CameraShotChanged( ceConversation *conversation, ceCameraShot* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateCameraShotList();
}

void ceWPConversationListener::ActiveCameraShotChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveCameraShot();
}



void ceWPConversationListener::GestureStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateGestureList();
}

void ceWPConversationListener::GestureChanged( ceConversation *conversation, ceGesture* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateGestureList();
}

void ceWPConversationListener::ActiveGestureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveGesture();
}



void ceWPConversationListener::FacePoseControllerNamesChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFPControllerList();
}

void ceWPConversationListener::FacePoseStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFacePoseList();
}

void ceWPConversationListener::FacePoseChanged( ceConversation *conversation, ceFacePose* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateFacePoseList();
}

void ceWPConversationListener::ActiveFacePoseChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveFacePose();
}



void ceWPConversationListener::ActorStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActorIDLists();
}

void ceWPConversationListener::ActorChanged( ceConversation *conversation, ceConversationActor* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateActorIDLists();
}
