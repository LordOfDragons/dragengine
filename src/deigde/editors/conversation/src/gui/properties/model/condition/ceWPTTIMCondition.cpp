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

#include "ceWPTTIMCondition.h"
#include "../ceWPTTreeItem.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceConversationCondition.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Static parameters
//////////////////////

const ceConversationCondition::eConditionTypes ceWPTTIMCondition::ListAddMenuConditions[ 8 ] = {
	ceConversationCondition::ectActorCommand,
	ceConversationCondition::ectActorInConversation,
	ceConversationCondition::ectActorParameter,
	ceConversationCondition::ectGameCommand,
	ceConversationCondition::ectHasActor,
	ceConversationCondition::ectLogic,
	ceConversationCondition::ectTrigger,
	ceConversationCondition::ectVariable
};

const int ceWPTTIMCondition::ListAddMenuConditionsCount = 8;



// Constructor, destructor
////////////////////////////

ceWPTTIMCondition::ceWPTTIMCondition( ceWindowMain &windowMain, eTypes type,
ceConversation &conversation, ceConversationAction &action, ceConversationCondition *condition ) :
ceWPTTreeItemModel( windowMain, conversation, type ),
pAction( &action ),
pCondition( condition )
{
	if( ! condition ){
		DETHROW( deeInvalidParam );
	}
}

ceWPTTIMCondition::~ceWPTTIMCondition(){
}



// Management
///////////////

ceWPTTIMCondition *ceWPTTIMCondition::DeepFindCondition( ceConversationCondition *condition ){
	if( pCondition == condition ){
		return this;
		
	}else{
		return ceWPTTreeItemModel::DeepFindCondition( condition );
	}
}

void ceWPTTIMCondition::Update(){
}

void ceWPTTIMCondition::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTreeItemModel * const parent = GetParent();
	if( parent ){
		parent->ContextMenuCondition( contextMenu, GetCondition() );
	}
}

ceConversationCondition *ceWPTTIMCondition::GetOwnerCondition() const{
	return pCondition;
}
