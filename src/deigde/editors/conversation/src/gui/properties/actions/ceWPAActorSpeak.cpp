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

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPAActorSpeak.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetActor.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetActor.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetTextBoxText.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetMovement.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetPathSound.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetTextBoxTextStyle.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakToggleUseSpeechAnimation.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetMinSpeechTime.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboActorID : public igdeComboBoxListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cComboActorID( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetActor( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTextBoxText : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cTextTextBoxText( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetTextBoxText().ToUTF8() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxText( topic, action,
			decUnicodeString::NewFromUTF8( textField->GetText() ) ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionEditTextBoxText : public igdeAction {
	ceWPAActorSpeak &pPanel;
	
public:
	cActionEditTextBoxText( ceWPAActorSpeak &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Edit command in larger dialog" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action ){
			return;
		}
		
		decString text( action->GetTextBoxText().ToUTF8() );
		if( ! igdeCommonDialogs::GetMultilineString( &pPanel, "Edit Text Box Text", "Text:", text )
		|| text == action->GetTextBoxText().ToUTF8() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxText( topic, action, decUnicodeString::NewFromUTF8( text ) ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTextBoxStyle : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cTextTextBoxStyle( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetTextBoxTextStyle() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxTextStyle( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextMovement : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cTextMovement( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetMovement() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetMovement( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cPathSound : public igdeEditPathListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cPathSound( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action || editPath->GetPath() == action->GetPathSound() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetPathSound( topic, action, editPath->GetPath() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextMinSpeechTime : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cTextMinSpeechTime( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		const float time = textField->GetFloat();
		if( ! topic || ! action || fabsf( time - action->GetMinSpeechTime() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetMinSpeechTime( topic, action, time ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionUseSpeechAnimation : public igdeAction {
	ceWPAActorSpeak &pPanel;
	
public:
	cActionUseSpeechAnimation( ceWPAActorSpeak &panel ) : igdeAction( "Use Speech Animation",
		NULL, "Speech animation is played back or not (for example thinking)" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakToggleUseSpeechAnimation( topic, action ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPAActorSpeak
//////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAActorSpeak::ceWPAActorSpeak( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	CreateGUICommon( *this );
	
	helper.ComboBox( *this, "Actor:", true, "ID of the actor speaking",
		pCBActorID, new cComboActorID( *this ) );
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst( *this, "Text Box Text:", "Text to display in the text box", formLine );
	helper.EditString( formLine, "Text to display in the text box",
		pEditTextBoxText, new cTextTextBoxText( *this ) );
	helper.Button( formLine, pBtnTextBoxText, new cActionEditTextBoxText( *this ), true );
	
	helper.EditString( *this, "Text Style:", "Name of style to use for the text box text",
		pEditTextBoxTextStyle, new cTextTextBoxStyle( *this ) );
	helper.EditString( *this, "Movement:", "Name of the actor movement to use or empty to not change",
		pEditMovement, new cTextMovement( *this ) );
	helper.EditPath( *this, "Path Sound:", "Sound file to play or empty to not use a sound file",
		igdeEnvironment::efpltSound, pEditPathSound, new cPathSound( *this ) );
	helper.EditFloat( *this, "Min Speech Time:", "Minimum time the actor is speaking",
		pEditMinSpeechTime, new cTextMinSpeechTime( *this ) );
	helper.CheckBox( *this, pChkUseSpeechAnimation, new cActionUseSpeechAnimation( *this ), true );
}

ceWPAActorSpeak::~ceWPAActorSpeak(){
}



// Management
///////////////

ceCAActorSpeak *ceWPAActorSpeak::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatActorSpeak ){
		return ( ceCAActorSpeak* )action;
		
	}else{
		return NULL;
	}
}

void ceWPAActorSpeak::UpdateAction(){
	const ceCAActorSpeak * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBActorID->SetText( action->GetActor() );
		pEditTextBoxText->SetText( action->GetTextBoxText().ToUTF8() );
		pEditTextBoxTextStyle->SetText( action->GetTextBoxTextStyle() );
		pEditMovement->SetText( action->GetMovement() );
		pEditPathSound->SetPath( action->GetPathSound() );
		pEditMinSpeechTime->SetFloat( action->GetMinSpeechTime() );
		pChkUseSpeechAnimation->SetChecked( action->GetUseSpeechAnimation() );
		
	}else{
		pCBActorID->ClearText();
		pEditTextBoxText->ClearText();
		pEditTextBoxTextStyle->ClearText();
		pEditMovement->ClearText();
		pEditPathSound->ClearPath();
		pEditMinSpeechTime->ClearText();
		pChkUseSpeechAnimation->SetChecked( false );
	}
}



void ceWPAActorSpeak::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList( pCBActorID );
}
