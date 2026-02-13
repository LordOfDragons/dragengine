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
	using Ref = deTObjectReference<cComboActorID>;
	cComboActorID(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
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
	using Ref = deTObjectReference<cTextTextBoxText>;
	cTextTextBoxText(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cActionEditTextBoxText>;
	cActionEditTextBoxText(ceWPAActorSpeak &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"@Conversation.Action.EditInDialog.ToolTip"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAActorSpeak * const action = pPanel.GetAction();
		if(!topic || !action){
			return;
		}
		
		decString text(action->GetTextBoxText().ToUTF8());
		if(!igdeCommonDialogs::GetMultilineString(
			pPanel.GetParentPanel().GetWindowProperties().GetWindowMain(),
			"@Conversation.Dialog.EditTextBoxText", "@Conversation.Dialog.Text", text)
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
	using Ref = deTObjectReference<cTextTextBoxTextTranslate>;
	cTextTextBoxTextTranslate(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cActionTbt2TranslationEntry>;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbt2TranslationEntry(ceWPAActorSpeak &panel) : igdeAction("@Conversation.WPActionActorSpeak.Texttotranslationentry",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongRight),
		"@Conversation.Action.ActorSpeakMoveTo.ToolTip"), pPanel(panel){}
	
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
		if(!igdeCommonDialogs::GetString(*pPanel.GetParentWindow(), "@Conversation.WPActionActorSpeak.Movetotranslationentry.Title",
			"@Conversation.Dialog.Name", name, langpack->GetEntries().GetKeys().GetSortedAscending())){
			return;
		}
		
		conversation->SetLangPackEntryName(name);
		
		const decUnicodeString *foundText = nullptr;
		if(langpack->GetEntries().GetAt(name, foundText)){
			if(igdeCommonDialogs::QuestionFormat(*pPanel.GetParentWindow(), igdeCommonDialogs::ebsYesNo,
			"@Conversation.WPActionActorSpeak.Movetotranslationentry.Title", "@Conversation.ToolTip.TranslationEntryExists",
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
	using Ref = deTObjectReference<cActionTbtFromTranslationEntry>;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionTbtFromTranslationEntry(ceWPAActorSpeak &panel) : igdeAction("@Conversation.WPActionActorSpeak.Textfromtranslationentry",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"@Conversation.Action.ActorSpeakMoveFrom.ToolTip"), pPanel(panel){}
	
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
	using Ref = deTObjectReference<cActionShowTranslationEntry>;
	
private:
	ceWPAActorSpeak &pPanel;
	
public:
	cActionShowTranslationEntry(ceWPAActorSpeak &panel) : igdeAction("@Conversation.WPActionActorSpeak.Showtranslationentry",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongLeft),
		"@Conversation.Action.ActorSpeakShowTranslation.ToolTip"), pPanel(panel){}
	
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
		
		igdeCommonDialogs::Information(*pPanel.GetParentWindow(), "@Conversation.WPActionActorSpeak.TranslationEntry.Title", foundText->ToUTF8());
	}
	
	void Update() override{
		const ceConversation *conversation = pPanel.GetParentPanel().GetConversation();
		SetEnabled(conversation && conversation->GetLanguagePack());
	}
};

class cActionTextBoxTextTranslateMenu : public igdeActionContextMenu{
	ceWPAActorSpeak &pPanel;
	
public:
	using Ref = deTObjectReference<cActionTextBoxTextTranslateMenu>;
	cActionTextBoxTextTranslateMenu(ceWPAActorSpeak &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"@Conversation.Action.ActorSpeakTextTranslateMenu.ToolTip"), pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
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
	using Ref = deTObjectReference<cTextTextBoxStyle>;
	cTextTextBoxStyle(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cTextMovement>;
	cTextMovement(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cPathSound>;
	cPathSound(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
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
	using Ref = deTObjectReference<cTextMinSpeechTime>;
	cTextMinSpeechTime(ceWPAActorSpeak &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
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
	using Ref = deTObjectReference<cActionUseSpeechAnimation>;
	cActionUseSpeechAnimation(ceWPAActorSpeak &panel) : igdeAction("@Conversation.WPActionActorSpeak.UseSpeechAnimation",
		nullptr, "@Conversation.Action.ActorSpeakUseSpeechAnimation.ToolTip"), pPanel(panel){ }
	
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
	
	helper.ComboBox(*this, "@Conversation.WPActionActorSpeak.Actor.Label", true, "@Conversation.ToolTip.ActorSpeaking",
		pCBActorID, cComboActorID::Ref::New(*this));
	pCBActorID->SetDefaultSorter();
	
	helper.FormLineStretchFirst(*this, "@Conversation.WPActionActorSpeak.TextBoxText.Label", "@Conversation.WPActionActorSpeak.TextBoxText.ToolTip", formLine);
	helper.EditString(formLine, "@Conversation.WPActionActorSpeak.Texttodisplayinthetextbox.Label",
		pEditTextBoxText, cTextTextBoxText::Ref::New(*this));
	helper.Button(formLine, pBtnTextBoxText, cActionEditTextBoxText::Ref::New(*this));
	
	helper.FormLineStretchFirst(*this, "@Conversation.WPActionActorSpeak.Translated.Label", "@Conversation.WPActionActorSpeak.Translated.ToolTip", formLine);
	helper.EditString(formLine, "@Conversation.WPActionActorSpeak.Translationnamefromactivelanguage.Label"
		" Replaces static text if not empty.", pEditTextBoxTextTranslate, cTextTextBoxTextTranslate::Ref::New(*this));
	cActionTextBoxTextTranslateMenu::Ref actionTbtt = cActionTextBoxTextTranslateMenu::Ref::New(*this);
	helper.Button(formLine, pBtnTextBoxTextTranslate, actionTbtt);
	actionTbtt->SetWidget(pBtnTextBoxTextTranslate);
	
	helper.EditString(*this, "", "@Conversation.ToolTip.LanguagePackEntry", pEditShowTranslation, {});
	pEditShowTranslation->SetEditable(false);
	
	helper.EditString(*this, "@Conversation.WPActionActorSpeak.TextStyle.Label", "@Conversation.ToolTip.TextBoxTextStyle",
		pEditTextBoxTextStyle, cTextTextBoxStyle::Ref::New(*this));
	helper.EditString(*this, "@Conversation.WPActionActorSpeak.Movement.Label", "@Conversation.ToolTip.ActorMovement",
		pEditMovement, cTextMovement::Ref::New(*this));
	helper.EditPath(*this, "@Conversation.WPActionActorSpeak.PathSound.Label", "@Conversation.ToolTip.SoundFile",
		igdeEnvironment::efpltSound, pEditPathSound, cPathSound::Ref::New(*this));
	helper.EditFloat(*this, "@Conversation.WPActionActorSpeak.MinSpeechTime.Label", "@Conversation.WPActionActorSpeak.MinSpeechTime.ToolTip",
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
