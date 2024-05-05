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
#include "ceWPTTIMAPlayerChoiceOptionActions.h"
#include "../../ceWPTTreeItem.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionRemove.h"
#include "../../menuactions/action/playerChoice/ceWPTMAPChoiceOptionMove.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAPlayerChoiceOption::ceWPTTIMAPlayerChoiceOption( ceWindowMain &windowMain,
ceConversation &conversation, ceCAPlayerChoice &playerChoice,
ceCAPlayerChoiceOption *option, int index ) :
ceWPTTreeItemModel( windowMain, conversation, etActionPlayerChoiceOption ),
pOption( NULL ),
pIndex( index ),
pCondition( NULL ),
pActions( NULL )
{
	if( ! option ){
		DETHROW( deeInvalidParam );
	}
	
	SetIcon( windowMain.GetIconActionOption() );
	
	try{
		pCondition = new ceWPTTIMAPlayerChoiceOptionCondition(
			windowMain, conversation, playerChoice, option );
		AddChild( pCondition );
		
		pActions = new ceWPTTIMAPlayerChoiceOptionActions(
			windowMain, conversation, option->GetActions() );
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
	
	pOption = option;
	option->AddReference();
	
	pUpdateText();
}

ceWPTTIMAPlayerChoiceOption::~ceWPTTIMAPlayerChoiceOption(){
	if( pOption ){
		pOption->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAPlayerChoice *ceWPTTIMAPlayerChoiceOption::GetModelPlayerChoice() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionPlayerChoice ){
		return ( ceWPTTIMAPlayerChoice* )parent;
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAPlayerChoiceOption::SetIndex( int index ){
	if( index == pIndex ){
		return;
	}
	
	pIndex = index;
	pUpdateText();
}

void ceWPTTIMAPlayerChoiceOption::Update(){
	SetExpanded( pOption->GetTIMExpanded() );
	
	pCondition->Update();
	pActions->Update();
}

void ceWPTTIMAPlayerChoiceOption::UpdateActionLists(){
	pActions->UpdateActions();
}

void ceWPTTIMAPlayerChoiceOption::OnExpandedChanged(){
	pOption->SetTIMExpanded( GetExpanded() );
}

void ceWPTTIMAPlayerChoiceOption::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( ! GetTreeItem() ){
		return;
	}
	
	ceWPTTIMAPlayerChoice * const modelPlayerChoice = GetModelPlayerChoice();
	if( ! modelPlayerChoice ){
		return;
	}
	
	ceWindowMain &windowMain = GetWindowMain();
	ceCAPlayerChoice &playerChoice = *modelPlayerChoice->GetActionPlayerChoice();
	ceConversation &conversation = GetConversation();
	ceConversationTopic * const topic = conversation.GetActiveFile()
		? conversation.GetActiveFile()->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionMove( windowMain, conversation,
		*topic, playerChoice, pOption, pIndex - 1, "Move Option Up",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionMove( windowMain, conversation,
		*topic, playerChoice, pOption, pIndex + 1, "Move Option Down",
		windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMAPChoiceOptionRemove(
		windowMain, conversation, *topic, playerChoice, pOption ), true );
}



// Private Functions
//////////////////////

void ceWPTTIMAPlayerChoiceOption::pUpdateText(){
	decString text;
	text.Format( "Option %d", pIndex + 1 );
	SetText( text );
}
