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

#include "ceWPTTIMAIfElse.h"
#include "ceWPTTIMAIfElseIfCase.h"
#include "ceWPTTIMAIfElseIfCaseCondition.h"
#include "../../ceWPTTreeItem.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseSetCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseClearCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteCondition.h"
#include "../../menuactions/condition/ceWPTMACopyCondition.h"
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

ceWPTTIMAIfElseIfCaseCondition::ceWPTTIMAIfElseIfCaseCondition( ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse &ifElse, ceCAIfElseCase *ifCase ) :
ceWPTTIMConditionContainer( windowMain, conversation, ifElse, etActionIfElseCaseCondition ),
pIfCase( NULL )
{
	if( ! ifCase ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( "Condition" );
	
	pIfCase = ifCase;
	ifCase->AddReference();
}

ceWPTTIMAIfElseIfCaseCondition::~ceWPTTIMAIfElseIfCaseCondition(){
	if( pIfCase ){
		pIfCase->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElseIfCaseCondition::GetModelIfCase() const{
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

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCaseCondition::GetModelIfElse() const{
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if( ifCase ){
		return ifCase->GetModelIfElse();
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAIfElseIfCaseCondition::Update(){
	SetExpanded( pIfCase->GetTIMConditionExpanded() );
	
	SetOrUpdateCondition( pIfCase->GetCondition() );
}

void ceWPTTIMAIfElseIfCaseCondition::OnContextMenu( igdeMenuCascade &contextMenu ){
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
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
		return;
	}
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// condition
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Set Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseSetCondition( windowMain, conversation, *topic,
			ifElse, *pIfCase, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseClearCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
}

void ceWPTTIMAIfElseIfCaseCondition::ContextMenuCondition(
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
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
		return;
	}
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Replace Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseSetCondition( windowMain, conversation, *topic,
			ifElse, *pIfCase, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseClearCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMACopyCondition( windowMain, condition ), true );
}

void ceWPTTIMAIfElseIfCaseCondition::OnExpandedChanged(){
	pIfCase->SetTIMConditionExpanded( GetExpanded() );
}
