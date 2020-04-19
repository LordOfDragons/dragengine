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

#include "ceWindowDopeSheet.h"
#include "ceWindowDopeSheetListener.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceWindowDopeSheetListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceWindowDopeSheetListener::ceWindowDopeSheetListener( ceWindowDopeSheet &window ) :
pWindow( window ){
}

ceWindowDopeSheetListener::~ceWindowDopeSheetListener(){
}



// Management
///////////////

void ceWindowDopeSheetListener::CameraShotChanged( ceConversation *conversation, ceCameraShot *cameraShot ){
	if( conversation != pWindow.GetConversation()
	|| cameraShot != conversation->GetActiveCameraShot() ){
		return;
	}
}

void ceWindowDopeSheetListener::ActiveCameraShotChanged( ceConversation *conversation ){
	if( conversation != pWindow.GetConversation() ){
		return;
	}
}



void ceWindowDopeSheetListener::FileStructureChanged( ceConversation *conversation ){
	if( conversation != pWindow.GetConversation() ){
		return;
	}
	
	pWindow.OnActionChanged();
}

void ceWindowDopeSheetListener::ActiveFileChanged( ceConversation *conversation ){
	if( conversation != pWindow.GetConversation() ){
		return;
	}
	
	pWindow.OnActionChanged();
}



void ceWindowDopeSheetListener::TopicStructureChanged( ceConversation *conversation, ceConversationFile *file ){
	if( conversation != pWindow.GetConversation()
	|| file != conversation->GetActiveFile() ){
		return;
	}
	
	pWindow.OnActionChanged();
}

void ceWindowDopeSheetListener::ActiveTopicChanged( ceConversation *conversation, ceConversationFile *file ){
	if( conversation != pWindow.GetConversation()
	|| file != conversation->GetActiveFile() ){
		return;
	}
	
	pWindow.OnActionChanged();
}



void ceWindowDopeSheetListener::ActionChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction *action ){
	if( conversation != pWindow.GetConversation()
	|| file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic()
	|| action != topic->GetActiveAction() ){
		return;
	}
	
	pWindow.OnActionChanged();
}

void ceWindowDopeSheetListener::ActiveActionChanged( ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic ){
	if( conversation != pWindow.GetConversation()
	|| file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic() ){
		return;
	}
	
	pWindow.OnActionChanged();
}



void ceWindowDopeSheetListener::PlaybackChanged( ceConversation *conversation ){
	if( conversation != pWindow.GetConversation() ){
		return;
	}
	
	pWindow.OnPlaybackChanged();
}

void ceWindowDopeSheetListener::PlaybackActionStackChanged( ceConversation *conversation ){
	if( conversation != pWindow.GetConversation() ){
		return;
	}
	
	pWindow.OnPlaybackChanged();
}
