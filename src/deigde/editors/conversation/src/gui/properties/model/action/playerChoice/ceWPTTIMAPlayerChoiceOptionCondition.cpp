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

#include "ceWPTTIMAPlayerChoice.h"
#include "ceWPTTIMAPlayerChoiceOption.h"
#include "ceWPTTIMAPlayerChoiceOptionCondition.h"
#include "../../ceWPTTreeItem.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionSetCondition.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionClearCondition.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionPasteCondition.h"
#include "../../menuactions/condition/ceWPTMACopyCondition.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoiceOptionCondition::ceWPTTIMAPlayerChoiceOptionCondition(
ceWindowMain &windowMain, ceConversation &conversation, ceCAPlayerChoice &playerChoice,
ceCAPlayerChoiceOption *option ) :
ceWPTTIMConditionContainer( windowMain, conversation, playerChoice, etActionPlayerChoiceOptionCondition ),
pOption( NULL )
{
	if( ! option ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( "Condition" );
	
	pOption = option;
	option->AddReference();
}

ceWPTTIMAPlayerChoiceOptionCondition::~ceWPTTIMAPlayerChoiceOptionCondition(){
	if( pOption ){
		pOption->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAPlayerChoiceOption *ceWPTTIMAPlayerChoiceOptionCondition::GetModelOption() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionPlayerChoiceOption ){
		return ( ceWPTTIMAPlayerChoiceOption* )parent;
		
	}else{
		return NULL;
	}
}

ceWPTTIMAPlayerChoice *ceWPTTIMAPlayerChoiceOptionCondition::GetModelPlayerChoice() const{
	ceWPTTIMAPlayerChoiceOption * const option = GetModelOption();
	if( option ){
		return option->GetModelPlayerChoice();
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAPlayerChoiceOptionCondition::Update(){
	SetExpanded( pOption->GetTIMConditionExpanded() );
	
	SetOrUpdateCondition( pOption->GetCondition() );
}

void ceWPTTIMAPlayerChoiceOptionCondition::OnContextMenu( igdeMenuCascade &contextMenu ){
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
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if( ! modelPlayerChoice ){
		return;
	}
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// condition
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Set Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceOptionSetCondition( windowMain, conversation, *topic,
			playerChoice, *pOption, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionClearCondition(
		windowMain, conversation, *topic, playerChoice, *pOption ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionPasteCondition(
		windowMain, conversation, *topic, playerChoice, *pOption ), true );
}

void ceWPTTIMAPlayerChoiceOptionCondition::ContextMenuCondition(
igdeMenuCascade &contextMenu, ceConversationCondition *condition ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoiceOption * const modelOption = GetModelOption();
	if( ! modelOption ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = modelOption->GetModelPlayerChoice();
	if( ! modelPlayerChoice ){
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
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	int i;
	
	// child action specific
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Replace Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAPChoiceOptionSetCondition( windowMain, conversation, *topic,
			playerChoice, *pOption, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionClearCondition(
		windowMain, conversation, *topic, playerChoice, *pOption ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMACopyCondition( windowMain, condition ), true );
}

void ceWPTTIMAPlayerChoiceOptionCondition::OnExpandedChanged(){
	pOption->SetTIMConditionExpanded( GetExpanded() );
}
