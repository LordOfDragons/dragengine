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
#include "ceWPTTIMAIfElseElse.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseAdd.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElse::ceWPTTIMAIfElse( ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse *action ) :
ceWPTTIMAction( windowMain, etActionIfElse, conversation, action ),
pElse( NULL )
{
	SetIcon( windowMain.GetIconActionIfElse() );
	SetText( "If-Else" );
	
	try{
		pElse = new ceWPTTIMAIfElseElse( windowMain, conversation, action->GetElseActions() );
		AddChild( pElse );
		
	}catch( const deException & ){
		if( pElse ){
			pElse->FreeReference();
		}
		throw;
	}
	
	pElse->FreeReference(); // held by super-class child list
}

ceWPTTIMAIfElse::~ceWPTTIMAIfElse(){
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElse::GetIfCaseChild( ceCAIfElseCase *ifCase ) const{
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTreeItemModel * const child = GetChildAt( i );
		if( child->GetType() != etActionIfElseCase ){
			continue;
		}
		
		ceWPTTIMAIfElseIfCase * const childIfCase = ( ceWPTTIMAIfElseIfCase* )child;
		if( childIfCase->GetIfCase() == ifCase ){
			return childIfCase;
		}
	}
	
	return NULL;
}

void ceWPTTIMAIfElse::Update(){
	const ceCAIfElse &ifElse = *GetActionIfElse();
	SetExpanded( ifElse.GetTIMExpanded() );
	
	pUpdateIfCases();
	
	const int countIfCases = ifElse.GetCases().GetCount();
	if( GetChildAt( countIfCases ) != pElse ){
		MoveChild( pElse, countIfCases );
	}
	pElse->Update();
	
	const int count = countIfCases + 1;
	while( GetChildCount() > count ){
		RemoveChild( GetChildAt( GetChildCount() - 1 ) );
	}
}

void ceWPTTIMAIfElse::UpdateActionLists(){
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTreeItemModel * const child = GetChildAt( i );
		
		switch( child->GetType() ){
		case etActionIfElseCase:
			( ( ceWPTTIMAIfElseIfCase* )child )->UpdateActionLists();
			break;
			
		case etActionIfElseElse:
			( ( ceWPTTIMAIfElseElse* )child )->UpdateActions();
			break;
			
		default:
			break;
		}
	}
}

void ceWPTTIMAIfElse::OnExpandedChanged(){
	GetActionIfElse()->SetTIMExpanded( GetExpanded() );
}

void ceWPTTIMAIfElse::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAIfElse &ifElse = *GetActionIfElse();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseAdd(
		windowMain, conversation, *topic, ifElse ), true );
	helper.MenuSeparator( contextMenu );
	
	ceWPTTIMAction::OnContextMenu( contextMenu );
}



// Private Functions
//////////////////////

void ceWPTTIMAIfElse::pUpdateIfCases(){
	ceCAIfElse &ifElse = *GetActionIfElse();
	ceWindowMain &windowMain = GetWindowMain();
	ceConversation &conversation = GetConversation();
	
	// update if-cases
	const ceCAIfElseCaseList &ifCases = ifElse.GetCases();
	const int ifCaseCount = ifCases.GetCount();
	int i, j;
	
	for( i=0; i<ifCaseCount; i++ ){
		ceCAIfElseCase * const ifCase = ifCases.GetAt( i );
		
		// find item matching ifCase if present
		ceWPTTIMAIfElseIfCase *model = NULL;
		const int childCount = GetChildCount();
		for( j=i; j<childCount; j++ ){
			ceWPTTreeItemModel * const child = GetChildAt( j );
			if( child->GetType() != etActionIfElseCase ){
				continue;
			}
			
			ceWPTTIMAIfElseIfCase * const childIfCase = ( ceWPTTIMAIfElseIfCase* )child;
			if( childIfCase->GetIfCase() == ifCase ){
				model = childIfCase;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if( model ){
			if( j != i ){
				MoveChild( j, i );
			}
			
			model->SetIndex( i );
			model->Update();
			
		}else{
			model = new ceWPTTIMAIfElseIfCase( windowMain, conversation, ifElse, ifCase, i );
			
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
