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

#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMACreateCondition::ceWPTMACreateCondition(ceWindowMain &windowMain,
ceConversation &conversation,
ceConversationCondition::eConditionTypes conditionType) :
ceWPTMenuAction(windowMain,
	ConditionTypeText(windowMain, conditionType),
	ConditionTypeIcon(windowMain, conditionType)),
pConversation(&conversation),
pConditionType(conditionType){
}



// Management
///////////////

void ceWPTMACreateCondition::OnAction(){
	GetConversation().GetUndoSystem()->Add(CreateUndo(CreateCondition()));
}

igdeUndo::Ref ceWPTMACreateCondition::CreateUndo(ceConversationCondition *condition){
	// only not pure-virtual because FOX toolkit requires final classes. if the system
	// moves over to the IGDE ToolKit this will become a pure virtual again
	DETHROW(deeInvalidParam);
}



ceConversationCondition::Ref ceWPTMACreateCondition::CreateCondition(){
	switch(pConditionType){
	case ceConversationCondition::ectLogic:
		return ceCConditionLogic::Ref::New();
		
	case ceConversationCondition::ectHasActor:
		return ceCConditionHasActor::Ref::New();
		
	case ceConversationCondition::ectActorInConversation:
		return ceCConditionActorInConversation::Ref::New();
		
	case ceConversationCondition::ectVariable:
		return ceCConditionVariable::Ref::New();
		
	case ceConversationCondition::ectActorParameter:
		return ceCConditionActorParameter::Ref::New();
		
	case ceConversationCondition::ectActorCommand:
		return ceCConditionActorCommand::Ref::New();
		
	case ceConversationCondition::ectGameCommand:
		return ceCConditionGameCommand::Ref::New();
		
	case ceConversationCondition::ectTrigger:
		return ceCConditionTrigger::Ref::New();
		
	default:
		DETHROW(deeInvalidParam);
	}
}

const char *ceWPTMACreateCondition::ConditionTypeText(ceWindowMain &windowMain,
ceConversationCondition::eConditionTypes conditionType){
	switch(conditionType){
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

igdeIcon *ceWPTMACreateCondition::ConditionTypeIcon(ceWindowMain &windowMain,
ceConversationCondition::eConditionTypes conditionType){
	switch(conditionType){
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
		return windowMain.GetIconConditionActorCommand();
		
	case ceConversationCondition::ectGameCommand:
		return windowMain.GetIconConditionCommand();
		
	case ceConversationCondition::ectTrigger:
		return windowMain.GetIconConditionTrigger();
		
	default:
		return nullptr;
	}
}
