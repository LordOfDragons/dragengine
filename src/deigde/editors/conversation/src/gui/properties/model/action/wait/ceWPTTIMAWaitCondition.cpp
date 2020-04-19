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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPTTIMAWait.h"
#include "ceWPTTIMAWaitCondition.h"
#include "../../ceWPTTreeItem.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsSetCondition.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsClearCondition.h"
#include "../../menuactions/action/wait/ceWPTMAWaitActionsPasteCondition.h"
#include "../../menuactions/condition/ceWPTMACopyCondition.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAWait.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAWaitCondition::ceWPTTIMAWaitCondition(
ceWindowMain &windowMain, ceConversation &conversation, ceCAWait *action ) :
ceWPTTIMConditionContainer( windowMain, conversation, *action, etActionWaitCondition ),
pAction( NULL )
{
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( "Condition" );
	
	pAction = action;
	action->AddReference();
}

ceWPTTIMAWaitCondition::~ceWPTTIMAWaitCondition(){
	if( pAction ){
		pAction->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAWait *ceWPTTIMAWaitCondition::GetModelWait() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionWait ){
		return ( ceWPTTIMAWait* )parent;
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAWaitCondition::Update(){
	SetExpanded( pAction->GetTIMConditionExpanded() );
	
	SetOrUpdateCondition( pAction->GetCondition() );
}

void ceWPTTIMAWaitCondition::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// condition
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Set Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAWaitActionsSetCondition( windowMain, conversation, *topic,
			*pAction, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsClearCondition(
		windowMain, conversation, *topic, *pAction ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsPasteCondition(
		windowMain, conversation, *topic, *pAction ), true );
}

void ceWPTTIMAWaitCondition::ContextMenuCondition(
igdeMenuCascade &contextMenu, ceConversationCondition *condition ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Replace Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAWaitActionsSetCondition( windowMain, conversation, *topic,
			*pAction, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAWaitActionsClearCondition(
		windowMain, conversation, *topic, *pAction ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMACopyCondition( windowMain, condition ), true );
}

void ceWPTTIMAWaitCondition::OnExpandedChanged(){
	pAction->SetTIMConditionExpanded( GetExpanded() );
}
