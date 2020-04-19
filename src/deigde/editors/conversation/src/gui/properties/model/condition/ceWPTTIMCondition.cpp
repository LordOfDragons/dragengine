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
