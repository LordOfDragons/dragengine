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

#include "ceWPTMACreateCondition.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../ceWPTTreeModel.h"
#include "../../../ceWPTopic.h"
#include "../../../ceWindowProperties.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/condition/ceCConditionLogic.h"
#include "../../../../../conversation/condition/ceCConditionHasActor.h"
#include "../../../../../conversation/condition/ceCConditionActorInConversation.h"
#include "../../../../../conversation/condition/ceCConditionVariable.h"
#include "../../../../../conversation/condition/ceCConditionActorParameter.h"
#include "../../../../../conversation/condition/ceCConditionActorCommand.h"
#include "../../../../../conversation/condition/ceCConditionGameCommand.h"
#include "../../../../../conversation/condition/ceCConditionTrigger.h"

#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMACreateCondition::ceWPTMACreateCondition( ceWindowMain &windowMain,
ceConversation &conversation,
ceConversationCondition::eConditionTypes conditionType ) :
ceWPTMenuAction( windowMain,
	ConditionTypeText( windowMain, conditionType ),
	ConditionTypeIcon( windowMain, conditionType ) ),
pConversation( &conversation ),
pConditionType( conditionType ){
}



// Management
///////////////

void ceWPTMACreateCondition::OnAction(){
	ceConversationConditionReference condition;
	condition.TakeOver( CreateCondition() );
	
	igdeUndoReference undo;
	undo.TakeOver( CreateUndo( condition ) );
	GetConversation().GetUndoSystem()->Add( undo );
}

igdeUndo *ceWPTMACreateCondition::CreateUndo( ceConversationCondition *condition ){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW( deeInvalidParam );
}



ceConversationCondition *ceWPTMACreateCondition::CreateCondition(){
	switch( pConditionType ){
	case ceConversationCondition::ectLogic:
		return new ceCConditionLogic;
		
	case ceConversationCondition::ectHasActor:
		return new ceCConditionHasActor;
		
	case ceConversationCondition::ectActorInConversation:
		return new ceCConditionActorInConversation;
		
	case ceConversationCondition::ectVariable:
		return new ceCConditionVariable;
		
	case ceConversationCondition::ectActorParameter:
		return new ceCConditionActorParameter;
		
	case ceConversationCondition::ectActorCommand:
		return new ceCConditionActorCommand;
		
	case ceConversationCondition::ectGameCommand:
		return new ceCConditionGameCommand;
		
	case ceConversationCondition::ectTrigger:
		return new ceCConditionTrigger;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

const char *ceWPTMACreateCondition::ConditionTypeText( ceWindowMain &windowMain,
ceConversationCondition::eConditionTypes conditionType ){
	switch( conditionType ){
	case ceConversationCondition::ectLogic:
		return "Logic";
		
	case ceConversationCondition::ectHasActor:
		return "Has Actor";
		
	case ceConversationCondition::ectActorInConversation:
		return "Actor In Conversation";
		
	case ceConversationCondition::ectVariable:
		return "Variable";
		
	case ceConversationCondition::ectActorParameter:
		return "Actor Parameter";
		
	case ceConversationCondition::ectActorCommand:
		return "Actor Command";
		
	case ceConversationCondition::ectGameCommand:
		return "Game Command";
		
	case ceConversationCondition::ectTrigger:
		return "Trigger";
		
	default:
		return "??";
	}
}

igdeIcon *ceWPTMACreateCondition::ConditionTypeIcon( ceWindowMain &windowMain,
ceConversationCondition::eConditionTypes conditionType ){
	switch( conditionType ){
	case ceConversationCondition::ectLogic:
		return windowMain.GetIconConditionLogic();
		
	case ceConversationCondition::ectHasActor:
		return windowMain.GetIconConditionHasActor();
		
	case ceConversationCondition::ectActorInConversation:
		return windowMain.GetIconConditionActorInConversation();
		
	case ceConversationCondition::ectVariable:
		return windowMain.GetIconConditionVariable();
		
	case ceConversationCondition::ectActorParameter:
		return windowMain.GetIconConditionVariable();
		
	case ceConversationCondition::ectActorCommand:
		return windowMain.GetIconConditionCommand();
		
	case ceConversationCondition::ectGameCommand:
		return windowMain.GetIconConditionCommand();
		
	case ceConversationCondition::ectTrigger:
		return windowMain.GetIconConditionTrigger();
		
	default:
		return NULL;
	}
}
