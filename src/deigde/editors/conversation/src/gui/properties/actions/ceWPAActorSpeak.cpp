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

#include "ceWPAActorSpeak.h"
#include "../ceWindowProperties.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../langpack/ceLangPackEntry.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetActor.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetActor.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetTextBoxText.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetTextBoxTextTranslate.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetMovement.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetPathSound.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetTextBoxTextStyle.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakToggleUseSpeechAnimation.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakSetMinSpeechTime.h"
#include "../../../undosys/action/actorSpeak/ceUCAASpeakMoveTbt2Translation.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
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
		if( ! igdeCommonDialogs::GetMultilineString(
			&pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"Edit Text Box Text", "Text:", text )
		|| text == action->GetTextBoxText().ToUTF8() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxText( topic, action, decUnicodeString::NewFromUTF8( text ) ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextTextBoxTextTranslate : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	cTextTextBoxTextTranslate( ceWPAActorSpeak &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! action || textField->GetText() == action->GetTextBoxTextTranslate() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxTextTranslate( topic, action, textField->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionTbt2TranslationEntry : public igdeAction{
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbt2TranslationEntry( ceWPAActorSpeak &panel ) : igdeAction( "Text to translation entry...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongRight ),
		"Move text box text to language pack translation entry" ), pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! conversation || ! action ){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if( ! langpack ){
			return;
		}
		
		decStringList existingNames;
		langpack->GetEntryNames( existingNames );
		
		decString name( conversation->GetLangPackEntryName() );
		if( ! igdeCommonDialogs::GetString( pPanel.GetParentWindow(), "Move to translation entry",
		"Name:", name, existingNames ) ){
			return;
		}
		
		conversation->SetLangPackEntryName( name );
		
		igdeUndoReference undo;
		
		ceLangPackEntry::Ref entry( langpack->GetEntryNamed( name ) );
		if( entry ){
			if( igdeCommonDialogs::QuestionFormat( pPanel.GetParentWindow(), igdeCommonDialogs::ebsYesNo,
			"Move to translation entry", "Translation entry '%s' exists. Replace entry?",
			name.GetString() ) == igdeCommonDialogs::ebNo ){
				return;
			}
			
			undo.TakeOver( new ceUCAASpeakMoveTbt2Translation( topic, action, entry, false, action->GetTextBoxText() ) );
			
		}else{
			entry.TakeOver( new ceLangPackEntry );
			entry->SetName( name );
			entry->SetText( action->GetTextBoxText() );
			undo.TakeOver( new ceUCAASpeakMoveTbt2Translation( topic, action, entry, true, action->GetTextBoxText() ) );
		}
		
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled( conversation && conversation->GetLanguagePack() );
	}
};

class cActionTbtFromTranslationEntry : public igdeAction{
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbtFromTranslationEntry( ceWPAActorSpeak &panel ) : igdeAction( "Text from translation entry...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongLeft ),
		"Set text box text from language pack translation entry" ), pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! conversation || ! action ){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if( ! langpack ){
			return;
		}
		
		ceLangPackEntry::Ref entry( langpack->GetEntryNamed( action->GetTextBoxTextTranslate() ) );
		if( ! entry || action->GetTextBoxText() == entry->GetText() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCAASpeakSetTextBoxText( topic, action, entry->GetText() ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled( conversation && conversation->GetLanguagePack() );
	}
};

class cActionShowTranslationEntry : public igdeAction{
	ceWPAActorSpeak &pPanel;
	
public:
	cActionShowTranslationEntry( ceWPAActorSpeak &panel ) : igdeAction( "Show translation entry...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongLeft ),
		"Show language pack translation entry" ), pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if( ! topic || ! conversation || ! action ){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if( ! langpack ){
			return;
		}
		
		ceLangPackEntry::Ref entry( langpack->GetEntryNamed( action->GetTextBoxTextTranslate() ) );
		if( entry ){
			igdeCommonDialogs::Information( pPanel.GetParentWindow(),
				"Translation Entry", entry->GetText().ToUTF8().GetString() );
		}
	}
	
	virtual void Update(){
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled( conversation && conversation->GetLanguagePack() );
	}
};

class cActionTextBoxTextTranslateMenu : public igdeActionContextMenu{
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTextBoxTextTranslateMenu( ceWPAActorSpeak &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Show Text Box Text Translate Menu" ), pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( contextMenu, new cActionTbt2TranslationEntry( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTbtFromTranslationEntry( pPanel ), true );
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionShowTranslationEntry( pPanel ), true );
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
	
	helper.FormLineStretchFirst( *this, "Translated:", "Translated text to display in the text box", formLine );
	helper.EditString( formLine, "Translation 'name' from active language pack to use as text box text."
		" Replaces static text if not empty.", pEditTextBoxTextTranslate, new cTextTextBoxTextTranslate( *this ) );
	cActionTextBoxTextTranslateMenu * const actionTbtt = new cActionTextBoxTextTranslateMenu( *this );
	helper.Button( formLine, pBtnTextBoxTextTranslate, actionTbtt, true );
	actionTbtt->SetWidget( pBtnTextBoxTextTranslate );
	
	helper.EditString( *this, "", "Language pack translation entry", pEditShowTranslation, nullptr );
	pEditShowTranslation->SetEditable( false );
	
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

void ceWPAActorSpeak::OnConversationPathChanged(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	
	if( conversation ){
		pEditPathSound->SetBasePath( conversation->GetDirectoryPath() );
		
	}else{
		pEditPathSound->SetBasePath( "" );
	}
}

void ceWPAActorSpeak::UpdateAction(){
	const ceCAActorSpeak * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBActorID->SetText( action->GetActor() );
		pEditTextBoxText->SetText( action->GetTextBoxText().ToUTF8() );
		pEditTextBoxTextTranslate->SetText( action->GetTextBoxTextTranslate() );
		pEditTextBoxTextStyle->SetText( action->GetTextBoxTextStyle() );
		pEditMovement->SetText( action->GetMovement() );
		pEditPathSound->SetPath( action->GetPathSound() );
		pEditMinSpeechTime->SetFloat( action->GetMinSpeechTime() );
		pChkUseSpeechAnimation->SetChecked( action->GetUseSpeechAnimation() );
		
		ceConversation * const conversation = GetParentPanel().GetConversation();
		ceLangPackEntry::Ref entry;
		
		if( conversation && action ){
			const ceLangPack * const langpack = conversation->GetLanguagePack();
			if( langpack ){
				entry = langpack->GetEntryNamed( action->GetTextBoxTextTranslate() );
			}
		}
		
		if( entry ){
			pEditShowTranslation->SetText( entry->GetText().ToUTF8().GetString() );
			
		}else{
			pEditShowTranslation->ClearText();
		}
		
		
	}else{
		pCBActorID->ClearText();
		pEditTextBoxText->ClearText();
		pEditShowTranslation->ClearText();
		pEditTextBoxTextTranslate->ClearText();
		pEditTextBoxTextStyle->ClearText();
		pEditMovement->ClearText();
		pEditPathSound->ClearPath();
		pEditMinSpeechTime->ClearText();
		pChkUseSpeechAnimation->SetChecked( false );
	}
	
	pBtnTextBoxTextTranslate->GetAction()->Update();
}



void ceWPAActorSpeak::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList( pCBActorID );
}
