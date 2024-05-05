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
#include "ceWPTTIMAIfElseIfCase.h"
#include "ceWPTTIMAIfElseIfCaseActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseAddAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseCopyActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteSnippet.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemoveAllActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseAddAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseCopyActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteSnippet.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemoveAllActions.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseMoveAction.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseCutAction.h"
#include "../../menuactions/action/ceWPTMACopyAction.h"
#include "../../ceWPTTreeItem.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseIfCaseActions::ceWPTTIMAIfElseIfCaseActions( ceWindowMain &windowMain,
ceConversation &conversation, const ceConversationActionList &actions ) :
ceWPTTIMActions( windowMain, conversation, etActionIfElseCaseActions, actions )
{
	SetText( "Actions" );
}

ceWPTTIMAIfElseIfCaseActions::~ceWPTTIMAIfElseIfCaseActions(){
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElseIfCaseActions::GetModelIfCase() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionIfElseCase ){
		return ( ceWPTTIMAIfElseIfCase* )parent;
		
	}else{
		return NULL;
	}
}

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCaseActions::GetModelIfElse() const{
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if( ifCase ){
		return ifCase->GetModelIfElse();
		
	}else{
		return NULL;
	}
}



void ceWPTTIMAIfElseIfCaseActions::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = GetModelIfCase();
	if( ! modelIfCase ){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = modelIfCase->GetModelIfElse();
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
	ceCAIfElseCase &ifCase = *modelIfCase->GetIfCase();
	
	// add action to end of action list
	const int indexAppend = ifCase.GetActions().GetCount();
	int i;
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Action",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseAddAction( windowMain, conversation, *topic,
			ifElse, ifCase, ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseCopyActions( windowMain, ifCase ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteActions(
		windowMain, conversation, *topic, ifElse, ifCase, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteSnippet(
		windowMain, conversation, *topic, ifElse, ifCase, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseRemoveAllActions(
		windowMain, conversation, *topic, ifElse, ifCase ), true );
}

void ceWPTTIMAIfElseIfCaseActions::ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAIfElseIfCase * const modelIfCase = GetModelIfCase();
	if( ! modelIfCase ){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = modelIfCase->GetModelIfElse();
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
	ceCAIfElseCase &ifCase = *modelIfCase->GetIfCase();
	const ceConversationActionList &actions = ifCase.GetActions();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseMoveAction( windowMain, conversation,
		*topic, ifElse, ifCase, action, indexAction - 1, "Move Action Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseMoveAction( windowMain, conversation,
		*topic, ifElse, ifCase, action, indexAction + 1, "Move Action Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( windowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseCutAction(
		windowMain, conversation, *topic, ifElse, ifCase, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteActions( windowMain, conversation,
		*topic, ifElse, ifCase, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteActions( windowMain, conversation,
		*topic, ifElse, ifCase, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseRemoveAction(
		windowMain, conversation, *topic, ifElse, ifCase, action ), true );
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action Before",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseAddAction( windowMain, conversation, *topic,
			ifElse, ifCase, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseAddAction( windowMain, conversation, *topic,
			ifElse, ifCase, ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
}

void ceWPTTIMAIfElseIfCaseActions::Update(){
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if( ifCase ){
		SetExpanded( ifCase->GetIfCase()->GetTIMActionsExpanded() );
	}
	
	ceWPTTIMActions::Update();
}

void ceWPTTIMAIfElseIfCaseActions::OnExpandedChanged(){
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if( ifCase ){
		ifCase->GetIfCase()->SetTIMActionsExpanded( GetExpanded() );
	}
}
