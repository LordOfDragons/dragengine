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

#include "ceWPTTIMAIfElse.h"
#include "ceWPTTIMAIfElseElse.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseAddAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseCopyActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElsePasteActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElsePasteSnippet.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseRemoveAllActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseRemoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseMoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseElseCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseElse::ceWPTTIMAIfElseElse( ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions ) :
ceWPTTIMActions( windowMain, conversation, etActionIfElseElse, actions )
{
	SetIcon( windowMain.GetIconActionIfElseCaseElse() );
	SetText( "Condition is false" );
}

ceWPTTIMAIfElseElse::~ceWPTTIMAIfElseElse(){
}



// Management
///////////////

ceWPTTIMAIfElse *ceWPTTIMAIfElseElse::GetModelIfElse() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionIfElse ){
		return ( ceWPTTIMAIfElse* )parent;
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAIfElseElse::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
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
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	
	// add action to end of action list
	const int indexAppend = ifElse.GetElseActions().GetCount();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	igdeMenuCascade::Ref subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Action",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseElseAddAction( windowMain, conversation, *topic,
			ifElse, ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseCopyActions( windowMain, ifElse ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElsePasteActions(
		windowMain, conversation, *topic, ifElse, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElsePasteSnippet(
		windowMain, conversation, *topic, ifElse, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseRemoveAllActions(
		windowMain, conversation, *topic, ifElse ), true );
}

void ceWPTTIMAIfElseElse::ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
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
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	const ceConversationActionList &actions = ifElse.GetElseActions();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseMoveAction( windowMain, conversation,
		*topic, ifElse, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseMoveAction( windowMain, conversation,
		*topic, ifElse, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( windowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseCutAction(
		windowMain, conversation, *topic, ifElse, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElsePasteActions( windowMain, conversation,
		*topic, ifElse, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElsePasteActions( windowMain, conversation,
		*topic, ifElse, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseElseRemoveAction(
		windowMain, conversation, *topic, ifElse, action ), true );
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascade::Ref subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action Before",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseElseAddAction( windowMain, conversation, *topic,
			ifElse, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseElseAddAction( windowMain, conversation, *topic,
			ifElse, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
}

void ceWPTTIMAIfElseElse::Update(){
	ceWPTTIMAIfElse * const ifElse = GetModelIfElse();
	if( ifElse ){
		SetExpanded( ifElse->GetActionIfElse()->GetTIMElseExpanded() );
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAIfElseElse::OnExpandedChanged(){
	ceWPTTIMAIfElse * const ifElse = GetModelIfElse();
	if( ifElse ){
		ifElse->GetActionIfElse()->SetTIMElseExpanded( GetExpanded() );
	}
}
