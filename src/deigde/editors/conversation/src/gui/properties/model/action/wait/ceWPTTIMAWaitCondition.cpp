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
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Set Condition", environment.GetStockIcon( igdeEnvironment::esiPlus )));
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
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Replace Condition", environment.GetStockIcon( igdeEnvironment::esiPlus )));
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
