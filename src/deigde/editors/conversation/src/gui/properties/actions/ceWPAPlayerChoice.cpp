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

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPAPlayerChoice.h"
#include "../ceWindowProperties.h"
#include "../ceWPTopic.h"
#include "../model/ceWPTTreeItemModel.h"
#include "../model/action/playerChoice/ceWPTTIMAPlayerChoice.h"
#include "../model/action/playerChoice/ceWPTTIMAPlayerChoiceOption.h"
#include "../model/action/playerChoice/ceWPTTIMAPlayerChoiceOptionActions.h"
#include "../model/action/playerChoice/ceWPTTIMAPlayerChoiceOptionCondition.h"
#include "../model/action/playerChoice/ceWPTTIMAPlayerChoiceActions.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/playerChoice/ceUCAPChoiceOptionSetText.h"
#include "../../../undosys/action/playerChoice/ceUCAPChoiceSetVarName.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextVarName : public igdeTextFieldListener {
	ceWPAPlayerChoice &pPanel;
	
public:
	cTextVarName( ceWPAPlayerChoice &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAPlayerChoice * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetVariableName() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAPChoiceSetVarName( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextOptionText : public igdeTextFieldListener {
	ceWPAPlayerChoice &pPanel;
	
public:
	cTextOptionText( ceWPAPlayerChoice &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAPlayerChoice * const action = pPanel.GetAction();
		ceCAPlayerChoiceOption * const option = pPanel.GetActiveOption();
		if( ! topic || ! action || ! option || textField->GetText() == option->GetText().ToUTF8() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAPChoiceOptionSetText( topic, action, option,
			decUnicodeString::NewFromUTF8( textField->GetText() ) ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditOptionText : public igdeAction {
	ceWPAPlayerChoice &pPanel;
	
public:
	cActionEditOptionText( ceWPAPlayerChoice &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAPlayerChoice * const action = pPanel.GetAction();
		ceCAPlayerChoiceOption * const option = pPanel.GetActiveOption();
		if( ! topic || ! action || ! option ){
			return;
		}
		
		decString text( option->GetText().ToUTF8() );
		if( ! igdeCommonDialogs::GetMultilineString(
			&pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"Edit Option Text", "Text:", text )
		|| text == option->GetText().ToUTF8() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAPChoiceOptionSetText( topic, action, option,
			decUnicodeString::NewFromUTF8( text ) ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPAPlayerChoice
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAPlayerChoice::ceWPAPlayerChoice( ceWPTopic &parentPanel ) :
ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	CreateGUICommon( *this );
	
	helper.EditString( *this, "Variable:", "Name of the variable to store the choice in or empty to use none",
		pEditVarName, new cTextVarName( *this ) );
	
	helper.FormLineStretchFirst( *this, "Option Text:", "Text to display for option", formLine );
	helper.EditString( formLine, "Text to display for option", pEditOptionText, new cTextOptionText( *this ) );
	helper.Button( formLine, pBtnOptionText, new cActionEditOptionText( *this ), true );
}

ceWPAPlayerChoice::~ceWPAPlayerChoice(){
}



// Management
///////////////

ceCAPlayerChoice *ceWPAPlayerChoice::GetAction() const{
	ceWPTTreeItemModel * const selected = GetParentPanel().GetActionTreeItem();
	if( ! selected ){
		return NULL;
	}
	
	ceWPTTIMAPlayerChoice *modelPlayerChoice = NULL;
	
	switch( selected->GetType() ){
	case ceWPTTreeItemModel::etActionPlayerChoice:
		modelPlayerChoice = ( ceWPTTIMAPlayerChoice* )selected;
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceOption:
		modelPlayerChoice = ( ( ceWPTTIMAPlayerChoiceOption* )selected )->GetModelPlayerChoice();
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceOptionCondition:
		modelPlayerChoice = ( ( ceWPTTIMAPlayerChoiceOptionCondition* )selected )->GetModelPlayerChoice();
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceOptionActions:
		modelPlayerChoice = ( ( ceWPTTIMAPlayerChoiceOptionActions* )selected )->GetModelPlayerChoice();
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceActions:
		modelPlayerChoice = ( ( ceWPTTIMAPlayerChoiceActions* )selected )->GetModelPlayerChoice();
		break;
		
	default:
		break;
	}
	
	if( modelPlayerChoice ){
		return modelPlayerChoice->GetActionPlayerChoice();
		
	}else{
		return NULL;
	}
}

ceCAPlayerChoiceOption *ceWPAPlayerChoice::GetActiveOption() const{
	ceWPTTreeItemModel * const selected = GetParentPanel().GetActionTreeItem();
	if( ! selected ){
		return NULL;
	}
	
	ceWPTTIMAPlayerChoiceOption *modelOption = NULL;
	
	switch( selected->GetType() ){
	case ceWPTTreeItemModel::etActionPlayerChoiceOption:
		modelOption = ( ceWPTTIMAPlayerChoiceOption* )selected;
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceOptionCondition:
		modelOption = ( ( ceWPTTIMAPlayerChoiceOptionCondition* )selected )->GetModelOption();
		break;
		
	case ceWPTTreeItemModel::etActionPlayerChoiceOptionActions:
		modelOption = ( ( ceWPTTIMAPlayerChoiceOptionActions* )selected )->GetModelOption();
		break;
		
	default:
		break;
	}
	
	if( modelOption ){
		return modelOption->GetOption();
		
	}else{
		return NULL;
	}
}

void ceWPAPlayerChoice::UpdateAction(){
	const ceCAPlayerChoice * const action = GetAction();
	const ceCAPlayerChoiceOption * const option = GetActiveOption();
	
	UpdateCommonParams();
	
	if( action ){
		pEditVarName->SetText( action->GetVariableName() );
		
	}else{
		pEditVarName->ClearText();
	}
	
	if( option ){
		pEditOptionText->SetText( option->GetText().ToUTF8() );
		
	}else{
		pEditOptionText->ClearText();
	}
	
	pEditOptionText->SetEnabled( option );
}
