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

#include "saeWPSAnim.h"
#include "saeWPSAnimListener.h"
#include "saeWindowProperties.h"
#include "../saeWindowMain.h"
#include "../../sanimation/saeSAnimation.h"
#include "../../sanimation/phoneme/saePhoneme.h"
#include "../../sanimation/dictionary/saeWord.h"
#include "../../configuration/saeConfiguration.h"
#include "../../undosys/sanimation/saeUSAnimSetRigPath.h"
#include "../../undosys/sanimation/saeUSAnimSetAnimPath.h"
#include "../../undosys/sanimation/saeUSAnimSetNeutralMoveName.h"
#include "../../undosys/sanimation/saeUSAnimSetNeutralVertexPositionSets.h"
#include "../../undosys/phoneme/saeUPhonemeSetIPA.h"
#include "../../undosys/phoneme/saeUPhonemeSetSampleText.h"
#include "../../undosys/phoneme/saeUPhonemeSetLength.h"
#include "../../undosys/phoneme/saeUPhonemeSetMoveName.h"
#include "../../undosys/phoneme/saeUPhonemeSetVertexPositionSet.h"
#include "../../undosys/dictionary/word/saeUWordSetName.h"
#include "../../undosys/dictionary/word/saeUWordSetPhonetics.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseTextFieldListener(saeWPSAnim &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(!sanimation){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, sanimation));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, saeSAnimation *sanimation) = 0;
};

class cBaseAction : public igdeAction{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseAction(saeWPSAnim &panel, const char *text, const char *description) :
	igdeAction(text, description), pPanel(panel){}
	
	cBaseAction(saeWPSAnim &panel, igdeIcon *icon, const char *description) :
	igdeAction("", icon, description), pPanel(panel){}
	
	cBaseAction(saeWPSAnim &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description), pPanel(panel){}
	
	virtual void OnAction(){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(!sanimation){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(sanimation));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(saeSAnimation *sanimation) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseComboBoxListener(saeWPSAnim &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(!sanimation){
			return;
		}
		
		igdeUndo::Ref undo(OnTextChanged(comboBox, sanimation));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseEditPathListener(saeWPSAnim &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if(!sanimation){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editPath->GetPath(), sanimation));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decString &path, saeSAnimation *sanimation) = 0;
};



class cEditRigPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditRigPath> Ref;
	cEditRigPath(saeWPSAnim &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decString &path, saeSAnimation *sanimation){
		if(path == sanimation->GetRigPath()){
			return {};
		}
		return saeUSAnimSetRigPath::Ref::New(sanimation, path);
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditAnimationPath> Ref;
	cEditAnimationPath(saeWPSAnim &panel) : cBaseEditPathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, saeSAnimation *sanimation) override{
		if(path == sanimation->GetAnimationPath()){
			return {};
		}
		return saeUSAnimSetAnimPath::Ref::New(sanimation, path);
	}
};

class cComboNeutralMove : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboNeutralMove> Ref;
	cComboNeutralMove(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const decString &name = comboBox->GetText();
		if(name == sanimation->GetNeutralMoveName()){
			return {};
		}
		return saeUSAnimSetNeutralMoveName::Ref::New(sanimation, name);
	}
};


class cActionNeutralVertexPositionSetsAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionNeutralVertexPositionSetsAdd> Ref;
	cActionNeutralVertexPositionSetsAdd(saeWPSAnim &panel) : cBaseAction(panel, "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetAdd",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetAdd.Description"){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		if(name.IsEmpty() || sanimation->GetNeutralVertexPositionSets().Has(name)){
			return {};
		}
		
		decStringSet sets(sanimation->GetNeutralVertexPositionSets());
		sets.Add(name);
		return saeUSAnimSetNeutralVertexPositionSets::Ref::New(sanimation, sets);
	}
	
	void Update() override{
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		SetEnabled(sanimation && !name.IsEmpty() && !sanimation->GetNeutralVertexPositionSets().Has(name));
	}
};

class cActionNeutralVertexPositionSetsRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionNeutralVertexPositionSetsRemove> Ref;
	cActionNeutralVertexPositionSetsRemove(saeWPSAnim &panel) : cBaseAction(panel, "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetRemove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetRemove.Description"){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		if(name.IsEmpty() || !sanimation->GetNeutralVertexPositionSets().Has(name)){
			return {};
		}
		
		decStringSet sets(sanimation->GetNeutralVertexPositionSets());
		sets.Remove(name);
		return saeUSAnimSetNeutralVertexPositionSets::Ref::New(sanimation, sets);
	}
	
	void Update() override{
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		SetEnabled(sanimation && !name.IsEmpty() && sanimation->GetNeutralVertexPositionSets().Has(name));
	}
};

class cActionNeutralVertexPositionSetsClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionNeutralVertexPositionSetsClear> Ref;
	cActionNeutralVertexPositionSetsClear(saeWPSAnim &panel) : cBaseAction(panel, "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetClear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@SpeechAnimation.WPSAnim.NeutralVertexPositionSetClear.Description"){}
	
	igdeUndo::Ref OnAction(saeSAnimation *sanimation) override{
		return sanimation->GetNeutralVertexPositionSets().GetCount() > 0
			? saeUSAnimSetNeutralVertexPositionSets::Ref::New(sanimation, decStringSet()) : igdeUndo::Ref();
	}
	
	void Update() override{
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		SetEnabled(sanimation && sanimation->GetNeutralVertexPositionSets().GetCount() > 0);
	}
};

class cListNeutralVertexPositionSets : public igdeListBoxListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	typedef deTObjectReference<cListNeutralVertexPositionSets> Ref;
	cListNeutralVertexPositionSets(saeWPSAnim &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		if(pPanel.GetSAnimation() && listBox->GetSelectedItem()){
			pPanel.SetCBNeutralVertexPositionSetText(listBox->GetSelectedItem()->GetText());
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetSAnimation()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionNeutralVertexPositionSetsAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionNeutralVertexPositionSetsRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionNeutralVertexPositionSetsClear::Ref::New(pPanel));
	}
};



class cComboPhoneme : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPhoneme> Ref;
	cComboPhoneme(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			sanimation->SetActivePhoneme((saePhoneme*)selection->GetData());
			
		}else{
			sanimation->SetActivePhoneme(nullptr);
		}
		return {};
	}
};

class cBasePhonemeTextFieldListener : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBasePhonemeTextFieldListener> Ref;
	cBasePhonemeTextFieldListener(saeWPSAnim &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, saeSAnimation *sanimation) override{
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return {};
		}
		return OnChanged(textField, sanimation, phoneme);
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField,
		saeSAnimation *sanimation, saePhoneme * phoneme) = 0;
};

class cTextPhonemeIpa : public cBasePhonemeTextFieldListener{
public:
	typedef deTObjectReference<cTextPhonemeIpa> Ref;
	cTextPhonemeIpa(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme){
		decUnicodeString ipaText;
		try{
			ipaText = decUnicodeString::NewFromUTF8(textField->GetText());
			
		}catch(const deException &){
			ResetTextField(*textField, *phoneme, "@SpeechAnimation.WPSAnim.Error.IPASingleCharacter");
			return {};
		}
		
		if(ipaText.GetLength() != 1){
			ResetTextField(*textField, *phoneme, "@SpeechAnimation.WPSAnim.Error.IPASingleCharacter");
			return {};
		}
		const int ipa = ipaText.GetAt(0);
		if(ipa == phoneme->GetIPA()){
			return {};
		}
		
		const saePhoneme::List &phonemeList = sanimation->GetPhonemes();
		if(phonemeList.HasMatching([&](const saePhoneme &p){
			return p.GetIPA() == ipa;
		})){
			ResetTextField(*textField, *phoneme, "@SpeechAnimation.WPSAnim.Error.PhonemeIPAExists");
			return {};
		}
		
		return saeUPhonemeSetIPA::Ref::New(phoneme, ipa);
	}
	
	void ResetTextField(igdeTextField &textField, const saePhoneme &phoneme, const char *error){
		igdeCommonDialogs::Error(pPanel, "@SpeechAnimation.WPSAnim.Error.ChangePhonemeIPA", error);
		textField.SetText(decUnicodeString(phoneme.GetIPA()).ToUTF8());
	}
};

class cTextPhonemeSampleText : public cBasePhonemeTextFieldListener{
public:
	typedef deTObjectReference<cTextPhonemeSampleText> Ref;
	cTextPhonemeSampleText(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme){
		if(textField->GetText() == phoneme->GetSampleText()){
			return {};
		}
		return saeUPhonemeSetSampleText::Ref::New(phoneme, textField->GetText());
	}
};

class cTextPhonemeLength : public cBasePhonemeTextFieldListener{
public:
	typedef deTObjectReference<cTextPhonemeLength> Ref;
	cTextPhonemeLength(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, saeSAnimation*, saePhoneme *phoneme) override{
		const float value = textField->GetFloat();
		if(fabsf(value - phoneme->GetLength()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return saeUPhonemeSetLength::Ref::New(phoneme, value);
	}
};

class cComboPhonemeMove : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPhonemeMove> Ref;
	cComboPhonemeMove(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation*){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return {};
		}
		
		const decString &text = comboBox->GetText();
		if(text == phoneme->GetMoveName()){
			return {};
		}
		return saeUPhonemeSetMoveName::Ref::New(phoneme, text);
	}
};

class cComboPhonemeVertexPositionSet : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPhonemeVertexPositionSet> Ref;
	cComboPhonemeVertexPositionSet(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation*){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return {};
		}
		
		const decString &text = comboBox->GetText();
		if(text == phoneme->GetVertexPositionSet()){
			return {};
		}
		return saeUPhonemeSetVertexPositionSet::Ref::New(phoneme, text);
	}
};



class cComboWord : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboWord> Ref;
	cComboWord(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			sanimation->SetActiveWord((saeWord*)selection->GetData());
			
		}else{
			sanimation->SetActiveWord(nullptr);
		}
		return {};
	}
};

class cBaseWordTextFieldListener : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBaseWordTextFieldListener> Ref;
	cBaseWordTextFieldListener(saeWPSAnim &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, saeSAnimation *sanimation) override{
		saeWord * const word = pPanel.GetActiveWord();
		if(!word){
			return {};
		}
		return OnChanged(textField, sanimation, word);
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField,
		saeSAnimation *sanimation, saeWord * word) = 0;
};

class cTextWordName : public cBaseWordTextFieldListener{
public:
	typedef deTObjectReference<cTextWordName> Ref;
	cTextWordName(saeWPSAnim &panel) : cBaseWordTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saeWord * word){
		const decString &name = textField->GetText();
		if(name == word->GetName()){
			return {};
		}
		
		if(name.IsEmpty()){
			ResetTextField(*textField, *word, "@SpeechAnimation.WPSAnim.Error.WordNameEmpty");
			return {};
		}
		
		if(sanimation->GetWords().HasNamed(name)){
			ResetTextField(*textField, *word, "@SpeechAnimation.WPSAnim.Error.WordNameExists");
			return {};
		}
		
		return saeUWordSetName::Ref::New(word, name);
	}
	
	void ResetTextField(igdeTextField &textField, const saeWord &word, const char *error){
		igdeCommonDialogs::Error(pPanel, "@SpeechAnimation.WPSAnim.Error.ChangeWordName", error);
		textField.SetText(word.GetName());
	}
};

class cTextWordPhonetics : public cBaseWordTextFieldListener{
public:
	typedef deTObjectReference<cTextWordPhonetics> Ref;
	cTextWordPhonetics(saeWPSAnim &panel) : cBaseWordTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, saeSAnimation*, saeWord * word) override{
		decUnicodeString phonetics;
		try{
			phonetics = decUnicodeString::NewFromUTF8(textField->GetText());
			
		}catch(const deException &){
			ResetTextField(*textField, *word, "@SpeechAnimation.WPSAnim.Error.InvalidPhonetics");
			return {};
		}
		
		return phonetics != word->GetPhonetics() ? saeUWordSetPhonetics::Ref::New(word, phonetics) : saeUWordSetPhonetics::Ref();
	}
	
	void ResetTextField(igdeTextField &textField, const saeWord &word, const char *error){
		igdeCommonDialogs::Error(pPanel, "@SpeechAnimation.WPSAnim.Error.ChangeWordPhonetics", error);
		textField.SetText(word.GetPhonetics().ToUTF8());
	}
};

class cActionWordAddIpa : public cBaseAction {
private:
	igdeTextField &pTextFieldPhonetics;
	
public:
	typedef deTObjectReference<cActionWordAddIpa> Ref;
	cActionWordAddIpa(saeWPSAnim &panel, igdeTextField &textFieldPhonetics) :
	cBaseAction(panel, "@SpeechAnimation.WPSAnim.WordAddIPA", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"@SpeechAnimation.WPSAnim.WordAddIPA.Description"),
	pTextFieldPhonetics(textFieldPhonetics){}
	
	igdeUndo::Ref OnAction(saeSAnimation*) override{
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		saeWord * const word = pPanel.GetActiveWord();
		if(!phoneme || !word){
			return {};
		}
		
		const decString &oldPhoneticsUtf8 = pTextFieldPhonetics.GetText();
		const int position = pTextFieldPhonetics.GetCursorPosition();
		
		decUnicodeString phonetics(decUnicodeString::NewFromUTF8(oldPhoneticsUtf8.GetLeft(position)));
		
		phonetics.AppendCharacter(phoneme->GetIPA());
		
		phonetics += decUnicodeString::NewFromUTF8(
			oldPhoneticsUtf8.GetRight(oldPhoneticsUtf8.GetLength() - position));
		
		return phonetics != word->GetPhonetics() ? saeUWordSetPhonetics::Ref::New(word, phonetics) : saeUWordSetPhonetics::Ref();
	}
};

}



// Class saeWPSAnim
/////////////////////

// Constructor, destructor
////////////////////////////

saeWPSAnim::saeWPSAnim(saeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = saeWPSAnimListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// speech animation
	helper.GroupBox(content, groupBox, "@SpeechAnimation.WPSAnim.GroupSpeechAnimation");
	
	helper.EditPath(groupBox, "@SpeechAnimation.WPSAnim.Rig", "@SpeechAnimation.WPSAnim.Rig.Description",
		igdeEnvironment::efpltRig, pEditRigPath, cEditRigPath::Ref::New(*this));
	helper.EditPath(groupBox, "@SpeechAnimation.WPSAnim.Animation", "@SpeechAnimation.WPSAnim.Animation.Description",
		igdeEnvironment::efpltAnimation, pEditAnimPath, cEditAnimationPath::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "@SpeechAnimation.WPSAnim.NeutralMove", true,
		"@SpeechAnimation.WPSAnim.NeutralMove.Description",
		pCBNeutralMove, cComboNeutralMove::Ref::New(*this));
	pCBNeutralMove->SetDefaultSorter();
	
	helper.GroupBoxFlow(content, groupBox, "@SpeechAnimation.WPSAnim.GroupNeutralVertexPositionSets");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "@SpeechAnimation.WPSAnim.NeutralVertexPositionSet.Description", pCBNeutralVertexPositionSets, {});
	helper.Button(formLine, pBtnNeutralVertexPositionSetAdd, cActionNeutralVertexPositionSetsAdd::Ref::New(*this));
	helper.Button(formLine, pBtnNeutralVertexPositionSetDel, cActionNeutralVertexPositionSetsRemove::Ref::New(*this));
	
	helper.ListBox(groupBox, 5, "@SpeechAnimation.WPSAnim.NeutralVertexPositionSets.Description", pListNeutralVertexPositionSets, cListNeutralVertexPositionSets::Ref::New(*this));
	pListNeutralVertexPositionSets->SetDefaultSorter();
	
	
	// phoneme
	helper.GroupBox(content, groupBox, "@SpeechAnimation.WPSAnim.GroupPhoneme");
	
	helper.ComboBox(groupBox, "@SpeechAnimation.WPSAnim.Phoneme", "@SpeechAnimation.WPSAnim.Phoneme.Description", pCBPhoneme, cComboPhoneme::Ref::New(*this));
	pCBPhoneme->SetDefaultSorter();
	
	helper.EditString(groupBox, "@SpeechAnimation.WPSAnim.PhonemeIPA", "@SpeechAnimation.WPSAnim.PhonemeIPA.Description",
		pEditPhonemeIPA, cTextPhonemeIpa::Ref::New(*this));
	helper.EditString(groupBox, "@SpeechAnimation.WPSAnim.PhonemeSampleText", "@SpeechAnimation.WPSAnim.PhonemeSampleText.Description",
		pEditPhonemeSampleText, cTextPhonemeSampleText::Ref::New(*this));
	helper.EditFloat(groupBox, "@SpeechAnimation.WPSAnim.PhonemeLength", "@SpeechAnimation.WPSAnim.PhonemeLength.Description",
		pEditPhonemeLength, cTextPhonemeLength::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "@SpeechAnimation.WPSAnim.PhonemeMove", true, "@SpeechAnimation.WPSAnim.PhonemeMove.Description",
		pCBPhonemeMove, cComboPhonemeMove::Ref::New(*this));
	pCBPhonemeMove->SetDefaultSorter();
	
	helper.ComboBoxFilter(groupBox, "@SpeechAnimation.WPSAnim.PhonemeVertexPositionSet", true,
		"@SpeechAnimation.WPSAnim.PhonemeVertexPositionSet.Description",
		pCBPhonemeVertexPositionSet, cComboPhonemeVertexPositionSet::Ref::New(*this));
	pCBPhonemeVertexPositionSet->SetDefaultSorter();
	
	
	// word
	helper.GroupBox(content, groupBox, "@SpeechAnimation.WPSAnim.GroupWord");
	
	helper.ComboBoxFilter(groupBox, "@SpeechAnimation.WPSAnim.Word", "@SpeechAnimation.WPSAnim.Word.Description", pCBWord, cComboWord::Ref::New(*this));
	pCBWord->SetDefaultSorter();
	
	helper.EditString(groupBox, "@SpeechAnimation.WPSAnim.WordName", "@SpeechAnimation.WPSAnim.WordName.Description",
		pEditWordName, cTextWordName::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "@SpeechAnimation.WPSAnim.WordPhonetics", "@SpeechAnimation.WPSAnim.WordPhonetics.Description", formLine);
	helper.EditString(formLine, "@SpeechAnimation.WPSAnim.WordPhonetics.Description",
		pEditWordPhonetics, cTextWordPhonetics::Ref::New(*this));
	pActionWordAddIpa = cActionWordAddIpa::Ref::New(*this, pEditWordPhonetics);
	helper.Button(formLine, pBtnWordAddIPA, pActionWordAddIpa);
}

saeWPSAnim::~saeWPSAnim(){
	SetSAnimation(nullptr);
}



// Management
///////////////

void saeWPSAnim::SetSAnimation(saeSAnimation *sanimation){
	if(sanimation == pSAnimation){
		return;
	}
	
	if(pSAnimation){
		pSAnimation->RemoveListener(pListener);
	}
	
	pSAnimation = sanimation;
	
	if(sanimation){
		sanimation->AddListener(pListener);
	}
	
	UpdateNeutralMoveList();
	UpdateNeutralVertexPositionSetList();
	UpdateSAnimation();
	OnSAnimationPathChanged();
}

void saeWPSAnim::OnSAnimationPathChanged(){
	if(pSAnimation){
		pEditRigPath->SetBasePath(pSAnimation->GetDirectoryPath());
		pEditAnimPath->SetBasePath(pSAnimation->GetDirectoryPath());
		
	}else{
		pEditRigPath->SetBasePath("");
		pEditAnimPath->SetBasePath("");
	}
}



void saeWPSAnim::UpdateSAnimation(){
	UpdateNeutralMoveList();
	
	if(pSAnimation){
		pEditRigPath->SetPath(pSAnimation->GetRigPath());
		pEditAnimPath->SetPath(pSAnimation->GetAnimationPath());
		pCBNeutralMove->SetText(pSAnimation->GetNeutralMoveName());
		
		// vertex position sets
		const decStringSet &vpsList = pSAnimation->GetNeutralVertexPositionSets();
		const int vpsCount = vpsList.GetCount();
		const decString vpsSelection(pListNeutralVertexPositionSets->GetSelectedItem()
			? pListNeutralVertexPositionSets->GetSelectedItem()->GetText() : decString());
		
		pListNeutralVertexPositionSets->RemoveAllItems();
		int i;
		for(i=0; i<vpsCount; i++){
			pListNeutralVertexPositionSets->AddItem(vpsList.GetAt(i));
		}
		pListNeutralVertexPositionSets->SortItems();
		pListNeutralVertexPositionSets->SetSelection(pListNeutralVertexPositionSets->IndexOfItem(vpsSelection));
		if(!pListNeutralVertexPositionSets->GetSelectedItem() && pListNeutralVertexPositionSets->GetItems().IsNotEmpty()){
			pListNeutralVertexPositionSets->SetSelection(0);
		}
		
	}else{
		pEditRigPath->ClearPath();
		pEditAnimPath->ClearPath();
		pCBNeutralMove->ClearText();
		pListNeutralVertexPositionSets->RemoveAllItems();
	}
	
	const bool enabled = pSAnimation != nullptr;
	pEditRigPath->SetEnabled(enabled);
	pEditAnimPath->SetEnabled(enabled);
	pCBNeutralMove->SetEnabled(enabled);
	pCBNeutralVertexPositionSets->SetEnabled(enabled);
	pListNeutralVertexPositionSets->SetEnabled(enabled);
	
	UpdatePhonemeMoveList();
	UpdatePhonemeVertexPositionSetList();
	UpdatePhonemeList();
	
	UpdateWordList();
	
	pBtnNeutralVertexPositionSetAdd->GetAction()->Update();
	pBtnNeutralVertexPositionSetDel->GetAction()->Update();
}

void saeWPSAnim::UpdateNeutralMoveList(){
	pCBNeutralMove->RemoveAllItems();
	
	if(pSAnimation){
		const deAnimator * const engAnimator = pSAnimation->GetEngineAnimator();
		
		if(engAnimator){
			const deAnimation * const engAnimation = engAnimator->GetAnimation();
			
			if(engAnimation){
				const int count = engAnimation->GetMoveCount();
				int i;
				
				for(i=0; i<count; i++){
					pCBNeutralMove->AddItem(engAnimation->GetMove(i)->GetName());
				}
			}
		}
		
		pCBNeutralMove->SortItems();
	}
	
	pCBNeutralMove->StoreFilterItems();
	pCBNeutralMove->FilterItems();
}

void saeWPSAnim::UpdateNeutralVertexPositionSetList(){
	const decString selection(GetCBNeutralVertexPositionSetText());
	
	pCBNeutralVertexPositionSets->RemoveAllItems();
	
	if(pSAnimation){
		const deComponent * const component = pSAnimation->GetEngineComponent();
		const deModel * const model = component ? component->GetModel().Pointer() : nullptr;
		
		if(model){
			const int count = model->GetVertexPositionSetCount();
			int i;
			for(i=0; i<count; i++){
				pCBNeutralVertexPositionSets->AddItem(model->GetVertexPositionSetAt(i)->GetName());
			}
		}
		
		pCBNeutralVertexPositionSets->SortItems();
		pCBNeutralVertexPositionSets->StoreFilterItems();
	}
	
	pCBNeutralVertexPositionSets->SetText(selection);
}

const decString &saeWPSAnim::GetCBNeutralVertexPositionSetText() const{
	return pCBNeutralVertexPositionSets->GetText();
}

void saeWPSAnim::SetCBNeutralVertexPositionSetText(const char *text){
	pCBNeutralVertexPositionSets->SetText(text);
}



saePhoneme *saeWPSAnim::GetActivePhoneme() const{
	if(pSAnimation){
		return pSAnimation->GetActivePhoneme();
	}
	
	return nullptr;
}

void saeWPSAnim::UpdatePhonemeList(){
	pCBPhoneme->UpdateRestoreSelection([&](){
		pCBPhoneme->RemoveAllItems();
		
		if(pSAnimation){
			pSAnimation->GetPhonemes().Visit([&](saePhoneme *p){
				decUnicodeString text(p->GetIPA());
				text.AppendFormat(" (0x%.4x): %s", p->GetIPA(), p->GetSampleText().GetString());
				pCBPhoneme->AddItem(text.ToUTF8(), nullptr, p);
			});
			
			pCBPhoneme->SortItems();
		}
	}, 0);
	
	UpdatePhoneme();
}

void saeWPSAnim::SelectActivePhoneme(){
	pCBPhoneme->SetSelection(pCBPhoneme->IndexOfItemWithData(GetActivePhoneme()));
	UpdatePhoneme();
}

void saeWPSAnim::UpdatePhoneme(){
	saePhoneme * const phoneme = GetActivePhoneme();
	
	if(phoneme){
		pEditPhonemeIPA->SetText(decUnicodeString(phoneme->GetIPA()).ToUTF8());
		pEditPhonemeSampleText->SetText(phoneme->GetSampleText());
		pEditPhonemeLength->SetFloat(phoneme->GetLength());
		pCBPhonemeMove->SetText(phoneme->GetMoveName());
		pCBPhonemeVertexPositionSet->SetText(phoneme->GetVertexPositionSet());
		
	}else{
		pEditPhonemeIPA->ClearText();
		pEditPhonemeSampleText->ClearText();
		pEditPhonemeLength->ClearText();
		pCBPhonemeMove->ClearText();
		pCBPhonemeVertexPositionSet->ClearText();
	}
	
	const bool enabled = phoneme != nullptr;
	pEditPhonemeIPA->SetEnabled(enabled);
	pEditPhonemeSampleText->SetEnabled(enabled);
	pEditPhonemeLength->SetEnabled(enabled);
	pCBPhonemeMove->SetEnabled(enabled);
	pCBPhonemeVertexPositionSet->SetEnabled(enabled);
}

void saeWPSAnim::UpdatePhonemeMoveList(){
	saePhoneme * const phoneme = GetActivePhoneme();
	
	pCBPhonemeMove->RemoveAllItems();
	
	if(pSAnimation){
		const deAnimator * const engAnimator = pSAnimation->GetEngineAnimator();
		
		if(engAnimator){
			const deAnimation * const engAnimation = engAnimator->GetAnimation();
			
			if(engAnimation){
				const int count = engAnimation->GetMoveCount();
				int i;
				
				for(i=0; i<count; i++){
					pCBPhonemeMove->AddItem(engAnimation->GetMove(i)->GetName());
				}
			}
		}
		
		pCBPhonemeMove->SortItems();
	}
	
	pCBPhonemeMove->StoreFilterItems();
	pCBPhonemeMove->FilterItems();
	
	if(pSAnimation){
		pSAnimation->SetActivePhoneme(phoneme);
	}
}

void saeWPSAnim::UpdatePhonemeVertexPositionSetList(){
	saePhoneme * const phoneme = GetActivePhoneme();
	
	pCBPhonemeVertexPositionSet->RemoveAllItems();
	
	if(pSAnimation){
		const deComponent * const engComponent = pSAnimation->GetEngineComponent();
		const deModel * const engModel = engComponent ? engComponent->GetModel().Pointer() : nullptr;
		
		if(engModel){
			const int count = engModel->GetVertexPositionSetCount();
			int i;
			
			for(i=0; i<count; i++){
				pCBPhonemeVertexPositionSet->AddItem(engModel->GetVertexPositionSetAt(i)->GetName());
			}
		}
		
		pCBPhonemeVertexPositionSet->SortItems();
	}
	
	pCBPhonemeVertexPositionSet->StoreFilterItems();
	pCBPhonemeVertexPositionSet->FilterItems();
	
	if(pSAnimation){
		pSAnimation->SetActivePhoneme(phoneme);
	}
}



saeWord *saeWPSAnim::GetActiveWord() const{
	if(pSAnimation){
		return pSAnimation->GetActiveWord();
	}
	
	return nullptr;
}

void saeWPSAnim::UpdateWordList(){
	pCBWord->UpdateRestoreSelection([&](){
		pCBWord->RemoveAllItems();
		
		if(pSAnimation){
			pSAnimation->GetWords().Visit([&](saeWord *word){
				pCBWord->AddItem(word->GetName(), nullptr, word);
			});
			pCBWord->SortItems();
		}
		
		pCBWord->StoreFilterItems();
		pCBWord->FilterItems();
	}, 0);
	
	UpdateWord();
}

void saeWPSAnim::SelectActiveWord(){
	pCBWord->SetSelection(pCBWord->IndexOfItemWithData(GetActiveWord()));
	UpdateWord();
}

void saeWPSAnim::UpdateWord(){
	saeWord * const word = GetActiveWord();
	
	if(word){
		pEditWordName->SetText(word->GetName());
		pEditWordPhonetics->SetText(word->GetPhonetics().ToUTF8());
		
	}else{
		pEditWordName->ClearText();
		pEditWordPhonetics->ClearText();
	}
	
	const bool enabled = word != nullptr;
	pEditWordName->SetEnabled(enabled);
	pEditWordPhonetics->SetEnabled(enabled);
	pActionWordAddIpa->SetEnabled(enabled);
}
