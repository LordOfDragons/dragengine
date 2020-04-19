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
#include "ceWPTTIMConditions.h"
#include "ceWPTTIMCLogic.h"
#include "ceWPTTIMCHasActor.h"
#include "ceWPTTIMCActorInConversation.h"
#include "ceWPTTIMCVariable.h"
#include "ceWPTTIMCActorParameter.h"
#include "ceWPTTIMCActorCommand.h"
#include "ceWPTTIMCGameCommand.h"
#include "ceWPTTIMCTrigger.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceConversationCondition.h"
#include "../../../../conversation/condition/ceConversationConditionList.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMConditions::ceWPTTIMConditions( ceWindowMain &windowMain, eTypes type,
ceConversation &conversation, ceConversationAction &action, ceConversationCondition *condition,
const ceConversationConditionList &conditions ) :
ceWPTTIMCondition( windowMain, type, conversation, action, condition ),
pConditions( conditions ){
}

ceWPTTIMConditions::~ceWPTTIMConditions(){
}



// Management
///////////////

ceWPTTIMCondition *ceWPTTIMConditions::GetChildWith( ceConversationCondition *condition ) const{
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTIMCondition * const child = ( ceWPTTIMCondition* )GetChildAt( i );
		if( child->GetCondition() == condition ){
			return child;
		}
	}
	
	return NULL;
}

void ceWPTTIMConditions::StructureChanged(){
	Update();
}



void ceWPTTIMConditions::Update(){
	UpdateConditions();
}

void ceWPTTIMConditions::UpdateConditions(){
	const int count = pConditions.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		ceConversationCondition * const condition = pConditions.GetAt( i );
		
		// find item matching condition if present
		ceWPTTIMCondition *model = NULL;
		const int childCount = GetChildCount();
		for( j=i; j<childCount; j++ ){
			ceWPTTIMCondition * const child = ( ceWPTTIMCondition* )GetChildAt( j );
			if( child->GetCondition() == condition ){
				model = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if( model ){
			if( j != i ){
				MoveChild( j, i );
			}
			model->Update();
			
		}else{
			model = CreateConditionModel( GetWindowMain(), GetConversation(),
				GetAction(), condition );
			
			try{
				InsertChild( model, i );
				model->Update();
				
			}catch( const deException & ){
				model->FreeReference();
				throw;
			}
			
			model->FreeReference();
		}
	}
	
	// remove non-matching nodes
	while( GetChildCount() > count ){
		RemoveChild( GetChildAt( GetChildCount() - 1 ) );
	}
}

void ceWPTTIMConditions::OnContextMenu( igdeMenuCascade &contextMenu ){
	ceWPTTIMCondition::OnContextMenu( contextMenu );
}



ceWPTTIMCondition *ceWPTTIMConditions::CreateConditionModel( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationAction &action, ceConversationCondition *condition ){
	if( ! condition ){
		DETHROW( deeInvalidParam );
	}
	
	switch( condition->GetType() ){
	case ceConversationCondition::ectLogic:
		return new ceWPTTIMCLogic( windowMain, conversation, action,
			( ceCConditionLogic* )condition );
		
	case ceConversationCondition::ectHasActor:
		return new ceWPTTIMCHasActor( windowMain, conversation, action,
			( ceCConditionHasActor* )condition );
		
	case ceConversationCondition::ectActorInConversation:
		return new ceWPTTIMCActorInConversation( windowMain, conversation, action,
			( ceCConditionActorInConversation* )condition );
		
	case ceConversationCondition::ectVariable:
		return new ceWPTTIMCVariable( windowMain, conversation, action,
			( ceCConditionVariable* )condition );
		
	case ceConversationCondition::ectActorParameter:
		return new ceWPTTIMCActorParameter( windowMain, conversation, action,
			( ceCConditionActorParameter* )condition );
		
	case ceConversationCondition::ectActorCommand:
		return new ceWPTTIMCActorCommand( windowMain, conversation, action,
			( ceCConditionActorCommand* )condition );
		
	case ceConversationCondition::ectGameCommand:
		return new ceWPTTIMCGameCommand( windowMain, conversation, action,
			( ceCConditionGameCommand* )condition );
		
	case ceConversationCondition::ectTrigger:
		return new ceWPTTIMCTrigger( windowMain, conversation, action,
			( ceCConditionTrigger* )condition );
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	DETHROW( deeInvalidParam );
}
