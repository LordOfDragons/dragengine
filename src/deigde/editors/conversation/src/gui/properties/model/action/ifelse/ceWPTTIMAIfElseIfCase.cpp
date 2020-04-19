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
#include "ceWPTTIMAIfElseIfCaseCondition.h"
#include "ceWPTTIMAIfElseIfCaseActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseRemove.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseMove.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseIfCase::ceWPTTIMAIfElseIfCase( ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse &ifElse, ceCAIfElseCase *ifCase, int index ) :
ceWPTTreeItemModel( windowMain, conversation, etActionIfElseCase ),
pIfCase( NULL ),
pIndex( index ),
pCondition( NULL ),
pActions( NULL )
{
	if( ! ifCase ){
		DETHROW( deeInvalidParam );
	}
	
	SetIcon( windowMain.GetIconActionIfElseCaseIf() );
	
	try{
		pCondition = new ceWPTTIMAIfElseIfCaseCondition(
			windowMain, conversation, ifElse, ifCase );
		AddChild( pCondition );
		
		pActions = new ceWPTTIMAIfElseIfCaseActions(
			windowMain, conversation, ifCase->GetActions() );
		AddChild( pActions );
		
	}catch( const deException & ){
		if( pCondition ){
			pCondition->FreeReference();
		}
		if( pActions ){
			pActions->FreeReference();
		}
		throw;
	}
	
	pCondition->FreeReference(); // held by super-class child list
	pActions->FreeReference(); // held by super-class child list
	
	pIfCase = ifCase;
	ifCase->AddReference();
	
	pUpdateText();
}

ceWPTTIMAIfElseIfCase::~ceWPTTIMAIfElseIfCase(){
	if( pIfCase ){
		pIfCase->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCase::GetModelIfElse() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionIfElse ){
		return ( ceWPTTIMAIfElse* )parent;
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAIfElseIfCase::SetIndex( int index ){
	if( index == pIndex ){
		return;
	}
	
	pIndex = index;
	pUpdateText();
}

void ceWPTTIMAIfElseIfCase::Update(){
	SetExpanded( pIfCase->GetTIMExpanded() );
	
	pCondition->Update();
	pActions->Update();
}

void ceWPTTIMAIfElseIfCase::UpdateActionLists(){
	pActions->UpdateActions();
}

void ceWPTTIMAIfElseIfCase::OnExpandedChanged(){
	pIfCase->SetTIMExpanded( GetTreeItem()->GetExpanded() );
}

void ceWPTTIMAIfElseIfCase::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseMove( windowMain, conversation,
		*topic, ifElse, pIfCase, pIndex - 1, "Move If-Case Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseMove( windowMain, conversation,
		*topic, ifElse, pIfCase, pIndex + 1, "Move If-Case Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseRemove(
		windowMain, conversation, *topic, ifElse, pIfCase ), true );
}



// Private Functions
//////////////////////

void ceWPTTIMAIfElseIfCase::pUpdateText(){
	decString text;
	text.Format( "If-Case %d", pIndex + 1 );
	SetText( text );
}
