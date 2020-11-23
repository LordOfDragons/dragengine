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
#include "ceWPTTIMAPlayerChoiceActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionAdd.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoice::ceWPTTIMAPlayerChoice( ceWindowMain &windowMain,
ceConversation &conversation, ceCAPlayerChoice *action ) :
ceWPTTIMAction( windowMain, etActionPlayerChoice, conversation, action ),
pActions( NULL )
{
	SetIcon( windowMain.GetIconActionPlayerChoice() );
	SetText( "Player Choice" );
	
	try{
		pActions = new ceWPTTIMAPlayerChoiceActions( windowMain, conversation, action->GetActions() );
		AddChild( pActions );
		
	}catch( const deException & ){
		if( pActions ){
			pActions->FreeReference();
		}
		throw;
	}
	
	pActions->FreeReference(); // held by super-class child list
}

ceWPTTIMAPlayerChoice::~ceWPTTIMAPlayerChoice(){
}



// Management
///////////////

ceWPTTIMAPlayerChoiceOption *ceWPTTIMAPlayerChoice::GetOptionChild( ceCAPlayerChoiceOption *option ) const{
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTreeItemModel * const child = GetChildAt( i );
		if( child->GetType() != etActionPlayerChoiceOption ){
			continue;
		}
		
		ceWPTTIMAPlayerChoiceOption * const childOption = ( ceWPTTIMAPlayerChoiceOption* )child;
		if( childOption->GetOption() == option ){
			return childOption;
		}
	}
	
	return NULL;
}

void ceWPTTIMAPlayerChoice::Update(){
	const ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	SetExpanded( playerChoice.GetTIMExpanded() );
	
	pUpdateOptions();
	
	const int countOptions = GetActionPlayerChoice()->GetOptions().GetCount();
	if( GetChildAt( countOptions ) != pActions ){
		MoveChild( pActions, countOptions );
	}
	pActions->Update();
	
	const int count = countOptions + 1;
	while( GetChildCount() > count ){
		RemoveChild( GetChildAt( GetChildCount() - 1 ) );
	}
}

void ceWPTTIMAPlayerChoice::UpdateActionLists(){
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTreeItemModel * const child = GetChildAt( i );
		
		switch( child->GetType() ){
		case etActionPlayerChoiceOption:
			( ( ceWPTTIMAPlayerChoiceOption* )child )->UpdateActionLists();
			break;
			
		case etActionPlayerChoiceActions:
			( ( ceWPTTIMAPlayerChoiceActions* )child )->UpdateActions();
			break;
			
		default:
			break;
		}
	}
}

void ceWPTTIMAPlayerChoice::OnExpandedChanged(){
	GetActionPlayerChoice()->SetTIMExpanded( GetExpanded() );
}

void ceWPTTIMAPlayerChoice::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionAdd(
		windowMain, conversation, *topic, playerChoice ), true );
	helper.MenuSeparator( contextMenu );
	
	ceWPTTIMAction::OnContextMenu( contextMenu );
}



// Private Functions
//////////////////////

void ceWPTTIMAPlayerChoice::pUpdateOptions(){
	ceCAPlayerChoice &playerChoice = *GetActionPlayerChoice();
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	
	// update if-cases
	const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
	const int optionCount = options.GetCount();
	int i, j;
	
	for( i=0; i<optionCount; i++ ){
		ceCAPlayerChoiceOption * const option = options.GetAt( i );
		
		// find item matching option if present
		ceWPTTIMAPlayerChoiceOption *model = NULL;
		const int childCount = GetChildCount();
		for( j=i; j<childCount; j++ ){
			ceWPTTreeItemModel * const child = GetChildAt( j );
			if( child->GetType() != etActionPlayerChoiceOption ){
				continue;
			}
			
			ceWPTTIMAPlayerChoiceOption * const childOption = ( ceWPTTIMAPlayerChoiceOption* )child;
			if( childOption->GetOption() == option ){
				model = childOption;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		// in both cases the index is 
		if( model ){
			if( j != i ){
				MoveChild( j, i );
			}
			model->SetIndex( i );
			model->Update();
			
		}else{
			model = new ceWPTTIMAPlayerChoiceOption( windowMain, conversation, playerChoice, option, i );
			
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
}
