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
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>



// Actions
////////////

namespace {

class cComboActorID : public igdeComboBoxListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cComboActorID> Ref;
	cComboActorID(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action  || comboBox->GetText() == action->GetActor()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetActor::Ref::New(topic, action, comboBox->GetText()));
	}
};

class cTextTextBoxText : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cTextTextBoxText> Ref;
	cTextTextBoxText(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetTextBoxText().ToUTF8()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetTextBoxText::Ref::New(topic, action,
				decUnicodeString::NewFromUTF8(textField->GetText())));
	}
};

class cActionEditTextBoxText : public igdeAction {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cActionEditTextBoxText> Ref;
	cActionEditTextBoxText(ceWPAActorSpeak &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Edit command in larger dialog"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action){
			return;
		}
		
		decString text(action->GetTextBoxText().ToUTF8());
		if(!igdeCommonDialogs::GetMultilineString(
			&pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"Edit Text Box Text", "Text:", text)
		|| text == action->GetTextBoxText().ToUTF8()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetTextBoxText::Ref::New(topic, action,
				decUnicodeString::NewFromUTF8(text)));
	}
};

class cTextTextBoxTextTranslate : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cTextTextBoxTextTranslate> Ref;
	cTextTextBoxTextTranslate(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetTextBoxTextTranslate()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetTextBoxTextTranslate::Ref::New(topic, action, textField->GetText()));
	}
};

class cActionTbt2TranslationEntry : public igdeAction{
public:
	typedef deTObjectReference<cActionTbt2TranslationEntry> Ref;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbt2TranslationEntry(ceWPAActorSpeak &panel) : igdeAction("Text to translation entry...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongRight),
		"Move text box text to language pack translation entry"), pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !conversation || !action){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if(!langpack){
			return;
		}
		
		decString name(conversation->GetLangPackEntryName());
		if(!igdeCommonDialogs::GetString(pPanel.GetParentWindow(), "Move to translation entry",
		"Name:", name, langpack->GetEntries().GetKeys().GetSortedAscending())){
			return;
		}
		
		conversation->SetLangPackEntryName(name);
		
		const decUnicodeString *foundText = nullptr;
		if(langpack->GetEntries().GetAt(name, foundText)){
			if(igdeCommonDialogs::QuestionFormat(pPanel.GetParentWindow(), igdeCommonDialogs::ebsYesNo,
			"Move to translation entry", "Translation entry '%s' exists. Replace entry?",
			name.GetString()) == igdeCommonDialogs::ebNo){
				return;
			}
		}
		
		conversation->GetUndoSystem()->Add(ceUCAASpeakMoveTbt2Translation::Ref::New(
			topic, action, name, foundText, action->GetTextBoxText()));
	}
	
	void Update() override{
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled(conversation && conversation->GetLanguagePack());
	}
};

class cActionTbtFromTranslationEntry : public igdeAction{
public:
	typedef deTObjectReference<cActionTbtFromTranslationEntry> Ref;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbtFromTranslationEntry(ceWPAActorSpeak &panel) : igdeAction("Text from translation entry...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"Set text box text from language pack translation entry"), pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !conversation || !action){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if(!langpack){
			return;
		}
		
		const decUnicodeString *foundText = nullptr;
		if(!langpack->GetEntries().GetAt(action->GetTextBoxTextTranslate(), foundText)
		|| action->GetTextBoxText() == *foundText){
			return;
		}
		
		conversation->GetUndoSystem()->Add(ceUCAASpeakSetTextBoxText::Ref::New(topic, action, *foundText));
	}
	
	void Update() override{
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled(conversation && conversation->GetLanguagePack());
	}
};

class cActionShowTranslationEntry : public igdeAction{
public:
	typedef deTObjectReference<cActionShowTranslationEntry> Ref;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionShowTranslationEntry(ceWPAActorSpeak &panel) : igdeAction("Show translation entry...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"Show language pack translation entry"), pPanel(panel){}
	
	void OnAction() override{
		ceConversation * const conversation = pPanel.GetParentPanel().GetConversation();
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !conversation || !action){
			return;
		}
		
		ceLangPack * const langpack = conversation->GetLanguagePack();
		if(!langpack){
			return;
		}
		
		const decUnicodeString *foundText = nullptr;
		if(!langpack->GetEntries().GetAt(action->GetTextBoxTextTranslate(), foundText)){
			return;
		}
		
		igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Translation Entry", foundText->ToUTF8());
	}
	
	void Update() override{
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled(conversation && conversation->GetLanguagePack());
	}
};

class cActionTextBoxTextTranslateMenu : public igdeActionContextMenu{
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cActionTextBoxTextTranslateMenu> Ref;
	cActionTextBoxTextTranslateMenu(ceWPAActorSpeak &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Show Text Box Text Translate Menu"), pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(contextMenu, cActionTbt2TranslationEntry::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionTbtFromTranslationEntry::Ref::New(pPanel));
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, cActionShowTranslationEntry::Ref::New(pPanel));
	}
};

class cTextTextBoxStyle : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cTextTextBoxStyle> Ref;
	cTextTextBoxStyle(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetTextBoxTextStyle()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetTextBoxTextStyle::Ref::New(topic, action, textField->GetText()));
	}
};

class cTextMovement : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cTextMovement> Ref;
	cTextMovement(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action || textField->GetText() == action->GetMovement()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetMovement::Ref::New(topic, action, textField->GetText()));
	}
};

class cPathSound : public igdeEditPathListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cPathSound> Ref;
	cPathSound(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action || editPath->GetPath() == action->GetPathSound()){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetPathSound::Ref::New(topic, action, editPath->GetPath()));
	}
};

class cTextMinSpeechTime : public igdeTextFieldListener {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cTextMinSpeechTime> Ref;
	cTextMinSpeechTime(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		const float time = textField->GetFloat();
		if(!topic || !action || fabsf(time - action->GetMinSpeechTime()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakSetMinSpeechTime::Ref::New(topic, action, time));
	}
};

class cActionUseSpeechAnimation : public igdeAction {
	ceWPAActorSpeak &pPanel;
	
public:
	typedef deTObjectReference<cActionUseSpeechAnimation> Ref;
	cActionUseSpeechAnimation(ceWPAActorSpeak &panel) : igdeAction("Use Speech Animation",
		nullptr, "Speech animation is played back or not (for example thinking)"), pPanel(panel){ }
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakToggleUseSpeechAnimation::Ref::New(topic, action));
	}
};

}



// Class ceWPAActorSpeak
//////////////////////////

// Constructor, destructor
////////////////////////////

ceWPAActorSpeak::ceWPAActorSpeak(ceWPTopic &parentPanel) : ceWPAction(parentPanel){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	CreateGUICommon(*this);
	
	helper.ComboBox(*this, "Actor:", true, "ID of the actor speaking",
		pCBActorID, cComboActorID::Ref::New(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst(*this, "Text Box Text:", "Text to display in the text box", formLine);
	helper.EditString(formLine, "Text to display in the text box",
		pEditTextBoxText, cTextTextBoxText::Ref::New(*this));
	helper.Button(formLine, pBtnTextBoxText, cActionEditTextBoxText::Ref::New(*this));
	
	helper.FormLineStretchFirst(*this, "Translated:", "Translated text to display in the text box", formLine);
	helper.EditString(formLine, "Translation 'name' from active language pack to use as text box text."
		" Replaces static text if not empty.", pEditTextBoxTextTranslate, cTextTextBoxTextTranslate::Ref::New(*this));
	cActionTextBoxTextTranslateMenu::Ref actionTbtt = cActionTextBoxTextTranslateMenu::Ref::New(*this);
	helper.Button(formLine, pBtnTextBoxTextTranslate, actionTbtt);
	actionTbtt->SetWidget(pBtnTextBoxTextTranslate);
	
	helper.EditString(*this, "", "Language pack translation entry", pEditShowTranslation, {});
	pEditShowTranslation->SetEditable(false);
	
	helper.EditString(*this, "Text Style:", "Name of style to use for the text box text",
		pEditTextBoxTextStyle, cTextTextBoxStyle::Ref::New(*this));
	helper.EditString(*this, "Movement:", "Name of the actor movement to use or empty to not change",
		pEditMovement, cTextMovement::Ref::New(*this));
	helper.EditPath(*this, "Path Sound:", "Sound file to play or empty to not use a sound file",
		igdeEnvironment::efpltSound, pEditPathSound, cPathSound::Ref::New(*this));
	helper.EditFloat(*this, "Min Speech Time:", "Minimum time the actor is speaking",
		pEditMinSpeechTime, cTextMinSpeechTime::Ref::New(*this));
	helper.CheckBox(*this, pChkUseSpeechAnimation, cActionUseSpeechAnimation::Ref::New(*this));
}

ceWPAActorSpeak::~ceWPAActorSpeak(){
}



// Management
///////////////

ceCAActorSpeak *ceWPAActorSpeak::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if(action && action->GetType() == ceConversationAction::eatActorSpeak){
		return (ceCAActorSpeak*)action;
		
	}else{
		return nullptr;
	}
}

void ceWPAActorSpeak::OnConversationPathChanged(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	
	if(conversation){
		pEditPathSound->SetBasePath(conversation->GetDirectoryPath());
		
	}else{
		pEditPathSound->SetBasePath("");
	}
}

void ceWPAActorSpeak::UpdateAction(){
	const ceCAActorSpeak * const action = GetAction();
	
	UpdateCommonParams();
	
	if(action){
		pCBActorID->SetText(action->GetActor());
		pEditTextBoxText->SetText(action->GetTextBoxText().ToUTF8());
		pEditTextBoxTextTranslate->SetText(action->GetTextBoxTextTranslate());
		pEditTextBoxTextStyle->SetText(action->GetTextBoxTextStyle());
		pEditMovement->SetText(action->GetMovement());
		pEditPathSound->SetPath(action->GetPathSound());
		pEditMinSpeechTime->SetFloat(action->GetMinSpeechTime());
		pChkUseSpeechAnimation->SetChecked(action->GetUseSpeechAnimation());
		
		ceConversation * const conversation = GetParentPanel().GetConversation();
		const decUnicodeString *foundText = nullptr;
		
		if(conversation && action){
			const ceLangPack * const langpack = conversation->GetLanguagePack();
			if(langpack){
				langpack->GetEntries().GetAt(action->GetTextBoxTextTranslate(), foundText);
			}
		}
		
		if(foundText){
			pEditShowTranslation->SetText(foundText->ToUTF8());
			
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
		pChkUseSpeechAnimation->SetChecked(false);
	}
	
	pBtnTextBoxTextTranslate->GetAction()->Update();
}



void ceWPAActorSpeak::UpdateActorIDLists(){
	ceWPAction::UpdateActorIDLists();
	UpdateComboBoxWithActorIDList(pCBActorID);
}
