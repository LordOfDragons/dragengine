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
	igdeMenuCascade::Ref subMenu;
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
	igdeMenuCascade::Ref subMenu;
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
