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



void ceWPConversationListener::PoseStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePoseList();
}

void ceWPConversationListener::PoseChanged( ceConversation *conversation, cePose* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdatePoseList();
}

void ceWPConversationListener::ActivePoseChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActivePose();
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



void ceWPConversationListener::LookAtStructureChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateLookAtList();
}

void ceWPConversationListener::LookAtChanged( ceConversation *conversation, ceLookAt* ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.UpdateLookAtList();
}

void ceWPConversationListener::ActiveLookAtChanged( ceConversation *conversation ){
	if( conversation != pPanel.GetConversation() ){
		return;
	}
	
	pPanel.SelectActiveLookAt();
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
