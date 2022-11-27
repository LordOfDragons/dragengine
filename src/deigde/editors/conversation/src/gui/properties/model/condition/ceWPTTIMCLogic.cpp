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

#include "ceWPTTIMCLogic.h"
#include "../ceWPTTreeItem.h"
#include "../condition/ceWPTTIMCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicAddCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicClearCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicCutCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicPasteCondition.h"
#include "../menuactions/condition/logic/ceWPTMACLogicRemoveCondition.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCAIfElse.h"
#include "../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../conversation/condition/ceCConditionLogic.h"
#include "../../../../conversation/file/ceConversationFile.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMCLogic::ceWPTTIMCLogic( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceCConditionLogic *condition ) :
ceWPTTIMConditions( windowMain, etConditionLogic, conversation, action,
	condition, condition->GetConditions() )
{
	SetIcon( windowMain.GetIconConditionLogic() );
	Update();
}

ceWPTTIMCLogic::~ceWPTTIMCLogic(){
}



// Management
///////////////

const char *ceWPTTIMCLogic::GetOperatorText( ceCConditionLogic::eOperators anOperator ){
	switch( anOperator ){
	case ceCConditionLogic::eopNone:
		return "none";
		
	case ceCConditionLogic::eopAny:
		return "any";
		
	case ceCConditionLogic::eopAll:
		return "all";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void ceWPTTIMCLogic::Update(){
	const ceCConditionLogic &condition = *GetConditionLogic();
	
	decString text;
	text.Format( "Logic (%s)", GetOperatorText( condition.GetOperator() ) );
	SetText( text );
	
	ceWPTTIMConditions::Update();
	
	SetExpanded( condition.GetTIMExpanded() );
}

void ceWPTTIMCLogic::OnContextMenu( igdeMenuCascade &contextMenu ){
	ceWPTTIMConditions::OnContextMenu( contextMenu );
	
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
	
	ceCConditionLogic &logic = *GetConditionLogic();
	ceConversationAction &action = GetAction();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	helper.MenuSeparator( contextMenu );
	
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Logic: Add Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMACLogicAddCondition( windowMain, conversation, *topic,
			action, logic, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMACLogicClearCondition( windowMain, conversation,
		*topic, action, logic ), true );
	helper.MenuCommand( contextMenu, new ceWPTMACLogicPasteCondition( windowMain, conversation,
		*topic, action, logic ), true );
}

void ceWPTTIMCLogic::ContextMenuCondition( igdeMenuCascade &contextMenu, ceConversationCondition *condition ){
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
	
	ceCConditionLogic &logic = *GetConditionLogic();
	ceConversationAction &action = GetAction();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	igdeMenuCascadeReference subMenu;
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Condition",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMACLogicAddCondition( windowMain, conversation, *topic,
			action, logic, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyCondition( windowMain, condition ), true );
	helper.MenuCommand( contextMenu, new ceWPTMACLogicCutCondition( windowMain, conversation,
		*topic, action, logic, condition ), true );
	helper.MenuCommand( contextMenu, new ceWPTMACLogicPasteCondition( windowMain, conversation,
		*topic, action, logic ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMACLogicRemoveCondition( windowMain, conversation,
		*topic, action, logic, condition ), true );
}

void ceWPTTIMCLogic::OnExpandedChanged(){
	GetConditionLogic()->SetTIMExpanded( GetExpanded() );
}
