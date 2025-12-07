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

#include "ceWPTTIMAIfElse.h"
#include "ceWPTTIMAIfElseIfCase.h"
#include "ceWPTTIMAIfElseIfCaseCondition.h"
#include "../../ceWPTTreeItem.h"
#include "../../condition/ceWPTTIMCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseSetCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCaseClearCondition.h"
#include "../../menuactions/action/ifelse/ceWPTMAIfElseCasePasteCondition.h"
#include "../../menuactions/condition/ceWPTMACopyCondition.h"
#include "../../../../ceWindowMain.h"
#include "../../../../../conversation/ceConversation.h"
#include "../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../conversation/action/ceCAIfElseCase.h"
#include "../../../../../conversation/file/ceConversationFile.h"
#include "../../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMAIfElseIfCaseCondition::ceWPTTIMAIfElseIfCaseCondition( ceWindowMain &windowMain,
ceConversation &conversation, ceCAIfElse &ifElse, ceCAIfElseCase *ifCase ) :
ceWPTTIMConditionContainer( windowMain, conversation, ifElse, etActionIfElseCaseCondition ),
pIfCase( NULL )
{
	if( ! ifCase ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( "Condition" );
	
	pIfCase = ifCase;
	ifCase->AddReference();
}

ceWPTTIMAIfElseIfCaseCondition::~ceWPTTIMAIfElseIfCaseCondition(){
	if( pIfCase ){
		pIfCase->FreeReference();
	}
}



// Management
///////////////

ceWPTTIMAIfElseIfCase *ceWPTTIMAIfElseIfCaseCondition::GetModelIfCase() const{
	ceWPTTreeItemModel * const parent = GetParent();
	if( ! parent ){
		return NULL;
	}
	
	if( parent->GetType() == etActionIfElseCase ){
		return ( ceWPTTIMAIfElseIfCase* )parent;
		
	}else{
		return NULL;
	}
}

ceWPTTIMAIfElse *ceWPTTIMAIfElseIfCaseCondition::GetModelIfElse() const{
	ceWPTTIMAIfElseIfCase * const ifCase = GetModelIfCase();
	if( ifCase ){
		return ifCase->GetModelIfElse();
		
	}else{
		return NULL;
	}
}

void ceWPTTIMAIfElseIfCaseCondition::Update(){
	SetExpanded( pIfCase->GetTIMConditionExpanded() );
	
	SetOrUpdateCondition( pIfCase->GetCondition() );
}

void ceWPTTIMAIfElseIfCaseCondition::OnContextMenu( igdeMenuCascade &contextMenu ){
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
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
		return;
	}
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// condition
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Set Condition", environment.GetStockIcon( igdeEnvironment::esiPlus )));
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseSetCondition( windowMain, conversation, *topic,
			ifElse, *pIfCase, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseClearCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCasePasteCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
}

void ceWPTTIMAIfElseIfCaseCondition::ContextMenuCondition(
igdeMenuCascade &contextMenu, ceConversationCondition *condition ){
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
	
	ceWPTTIMAIfElse * const modelIfElse = GetModelIfElse();
	if( ! modelIfElse ){
		return;
	}
	ceCAIfElse &ifElse = *modelIfElse->GetActionIfElse();
	
	igdeEnvironment &environment = windowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	int i;
	
	// child action specific
	igdeMenuCascade::Ref subMenu(igdeMenuCascade::Ref::NewWith(
		environment, "Replace Condition", environment.GetStockIcon( igdeEnvironment::esiPlus )));
	contextMenu.AddChild( subMenu );
	
	for( i=0; i<ceWPTTIMCondition::ListAddMenuConditionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMAIfElseCaseSetCondition( windowMain, conversation, *topic,
			ifElse, *pIfCase, ceWPTTIMCondition::ListAddMenuConditions[ i ] ), true );
	}
	
	helper.MenuCommand( contextMenu, new ceWPTMAIfElseCaseClearCondition(
		windowMain, conversation, *topic, ifElse, *pIfCase ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMACopyCondition( windowMain, condition ), true );
}

void ceWPTTIMAIfElseIfCaseCondition::OnExpandedChanged(){
	pIfCase->SetTIMConditionExpanded( GetExpanded() );
}
