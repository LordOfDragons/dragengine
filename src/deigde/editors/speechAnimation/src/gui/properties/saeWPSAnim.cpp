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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, sanimation)));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, saeSAnimation *sanimation) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(sanimation)));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnTextChanged(comboBox, sanimation)));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation) = 0;
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
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editPath->GetPath(), sanimation)));
		if(undo){
			sanimation->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decString &path, saeSAnimation *sanimation) = 0;
};



class cEditRigPath : public cBaseEditPathListener{
public:
	cEditRigPath(saeWPSAnim &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, saeSAnimation *sanimation){
		if(path == sanimation->GetRigPath()){
			return nullptr;
		}
		return new saeUSAnimSetRigPath(sanimation, path);
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	cEditAnimationPath(saeWPSAnim &panel) : cBaseEditPathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, saeSAnimation *sanimation){
		if(path == sanimation->GetAnimationPath()){
			return nullptr;
		}
		return new saeUSAnimSetAnimPath(sanimation, path);
	}
};

class cComboNeutralMove : public cBaseComboBoxListener{
public:
	cComboNeutralMove(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const decString &name = comboBox->GetText();
		if(name == sanimation->GetNeutralMoveName()){
			return nullptr;
		}
		return new saeUSAnimSetNeutralMoveName(sanimation, name);
	}
};


class cActionNeutralVertexPositionSetsAdd : public cBaseAction{
public:
	cActionNeutralVertexPositionSetsAdd(saeWPSAnim &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add vertex position set"){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		if(name.IsEmpty() || sanimation->GetNeutralVertexPositionSets().Has(name)){
			return nullptr;
		}
		
		decStringSet sets(sanimation->GetNeutralVertexPositionSets());
		sets.Add(name);
		return new saeUSAnimSetNeutralVertexPositionSets(sanimation, sets);
	}
	
	virtual void Update(){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		SetEnabled(sanimation && !name.IsEmpty() && !sanimation->GetNeutralVertexPositionSets().Has(name));
	}
};

class cActionNeutralVertexPositionSetsRemove : public cBaseAction{
public:
	cActionNeutralVertexPositionSetsRemove(saeWPSAnim &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove vertex position set"){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		if(name.IsEmpty() || !sanimation->GetNeutralVertexPositionSets().Has(name)){
			return nullptr;
		}
		
		decStringSet sets(sanimation->GetNeutralVertexPositionSets());
		sets.Remove(name);
		return new saeUSAnimSetNeutralVertexPositionSets(sanimation, sets);
	}
	
	virtual void Update(){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		const decString &name = pPanel.GetCBNeutralVertexPositionSetText();
		SetEnabled(sanimation && !name.IsEmpty() && sanimation->GetNeutralVertexPositionSets().Has(name));
	}
};

class cActionNeutralVertexPositionSetsClear : public cBaseAction{
public:
	cActionNeutralVertexPositionSetsClear(saeWPSAnim &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all vertex position sets"){}
	
	virtual igdeUndo *OnAction(saeSAnimation *sanimation){
		return sanimation->GetNeutralVertexPositionSets().GetCount() > 0
			? new saeUSAnimSetNeutralVertexPositionSets(sanimation, decStringSet()) : nullptr;
	}
	
	virtual void Update(){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		SetEnabled(sanimation && sanimation->GetNeutralVertexPositionSets().GetCount() > 0);
	}
};

class cListNeutralVertexPositionSets : public igdeListBoxListener{
protected:
	saeWPSAnim &pPanel;
	
public:
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
		
		helper.MenuCommand(menu, new cActionNeutralVertexPositionSetsAdd(pPanel), true);
		helper.MenuCommand(menu, new cActionNeutralVertexPositionSetsRemove(pPanel), true);
		helper.MenuCommand(menu, new cActionNeutralVertexPositionSetsClear(pPanel), true);
	}
};



class cComboPhoneme : public cBaseComboBoxListener{
public:
	cComboPhoneme(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			sanimation->SetActivePhoneme((saePhoneme*)selection->GetData());
			
		}else{
			sanimation->SetActivePhoneme(nullptr);
		}
		return nullptr;
	}
};

class cBasePhonemeTextFieldListener : public cBaseTextFieldListener{
public:
	cBasePhonemeTextFieldListener(saeWPSAnim &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, saeSAnimation *sanimation){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return nullptr;
		}
		return OnChanged(textField, sanimation, phoneme);
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField,
		saeSAnimation *sanimation, saePhoneme * phoneme) = 0;
};

class cTextPhonemeIpa : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeIpa(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme){
		decUnicodeString ipaText;
		try{
			ipaText = decUnicodeString::NewFromUTF8(textField->GetText());
			
		}catch(const deException &){
			ResetTextField(*textField, *phoneme, "IPA symbol has to be 1 unicode character");
			return nullptr;
		}
		
		if(ipaText.GetLength() != 1){
			ResetTextField(*textField, *phoneme, "IPA symbol has to be 1 unicode character");
			return nullptr;
		}
		const int ipa = ipaText.GetAt(0);
		if(ipa == phoneme->GetIPA()){
			return nullptr;
		}
		
		const saePhonemeList &phonemeList = sanimation->GetPhonemeList();
		if(phonemeList.HasIPA(ipa)){
			ResetTextField(*textField, *phoneme, "There exists already a Phoneme with this IPA symbol");
			return nullptr;
		}
		
		return new saeUPhonemeSetIPA(phoneme, ipa);
	}
	
	void ResetTextField(igdeTextField &textField, const saePhoneme &phoneme, const char *error){
		igdeCommonDialogs::Error(&pPanel, "Change Phoneme IPA", error);
		textField.SetText(decUnicodeString(phoneme.GetIPA()).ToUTF8());
	}
};

class cTextPhonemeSampleText : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeSampleText(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme){
		if(textField->GetText() == phoneme->GetSampleText()){
			return nullptr;
		}
		return new saeUPhonemeSetSampleText(phoneme, textField->GetText());
	}
};

class cTextPhonemeLength : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeLength(saeWPSAnim &panel) : cBasePhonemeTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, saeSAnimation*, saePhoneme *phoneme){
		const float value = textField->GetFloat();
		if(fabsf(value - phoneme->GetLength()) < FLOAT_SAFE_EPSILON){
			return nullptr;
		}
		return new saeUPhonemeSetLength(phoneme, value);
	}
};

class cComboPhonemeMove : public cBaseComboBoxListener{
public:
	cComboPhonemeMove(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation*){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return nullptr;
		}
		
		const decString &text = comboBox->GetText();
		if(text == phoneme->GetMoveName()){
			return nullptr;
		}
		return new saeUPhonemeSetMoveName(phoneme, text);
	}
};

class cComboPhonemeVertexPositionSet : public cBaseComboBoxListener{
public:
	cComboPhonemeVertexPositionSet(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation*){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if(!phoneme){
			return nullptr;
		}
		
		const decString &text = comboBox->GetText();
		if(text == phoneme->GetVertexPositionSet()){
			return nullptr;
		}
		return new saeUPhonemeSetVertexPositionSet(phoneme, text);
	}
};



class cComboWord : public cBaseComboBoxListener{
public:
	cComboWord(saeWPSAnim &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnTextChanged(igdeComboBox *comboBox, saeSAnimation *sanimation){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(selection){
			sanimation->SetActiveWord((saeWord*)selection->GetData());
			
		}else{
			sanimation->SetActiveWord(nullptr);
		}
		return nullptr;
	}
};

class cBaseWordTextFieldListener : public cBaseTextFieldListener{
public:
	cBaseWordTextFieldListener(saeWPSAnim &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, saeSAnimation *sanimation){
		saeWord * const word = pPanel.GetActiveWord();
		if(!word){
			return nullptr;
		}
		return OnChanged(textField, sanimation, word);
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField,
		saeSAnimation *sanimation, saeWord * word) = 0;
};

class cTextWordName : public cBaseWordTextFieldListener{
public:
	cTextWordName(saeWPSAnim &panel) : cBaseWordTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField,
	saeSAnimation *sanimation, saeWord * word){
		const decString &name = textField->GetText();
		if(name == word->GetName()){
			return nullptr;
		}
		
		if(name.IsEmpty()){
			ResetTextField(*textField, *word, "Name can not be empty");
			return nullptr;
		}
		
		const saeWordList &wordList = sanimation->GetWordList();
		if(wordList.HasNamed(name)){
			ResetTextField(*textField, *word, "There exists already a Word with this name");
			return nullptr;
		}
		
		return new saeUWordSetName(word, name);
	}
	
	void ResetTextField(igdeTextField &textField, const saeWord &word, const char *error){
		igdeCommonDialogs::Error(&pPanel, "Change Word Name", error);
		textField.SetText(word.GetName());
	}
};

class cTextWordPhonetics : public cBaseWordTextFieldListener{
public:
	cTextWordPhonetics(saeWPSAnim &panel) : cBaseWordTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, saeSAnimation*, saeWord * word){
		decUnicodeString phonetics;
		try{
			phonetics = decUnicodeString::NewFromUTF8(textField->GetText());
			
		}catch(const deException &){
			ResetTextField(*textField, *word, "Invalid phonetics (wrong UTF8 encoding)");
			return nullptr;
		}
		
		return phonetics != word->GetPhonetics() ? new saeUWordSetPhonetics(word, phonetics) : nullptr;
	}
	
	void ResetTextField(igdeTextField &textField, const saeWord &word, const char *error){
		igdeCommonDialogs::Error(&pPanel, "Change Word Phonetics", error);
		textField.SetText(word.GetPhonetics().ToUTF8());
	}
};

class cActionWordAddIpa : public cBaseAction {
private:
	igdeTextField &pTextFieldPhonetics;
	
public:
	cActionWordAddIpa(saeWPSAnim &panel, igdeTextField &textFieldPhonetics) :
	cBaseAction(panel, panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus),
		"Insert selected phoneme IPA to selected word phonetics at cursor position"),
	pTextFieldPhonetics(textFieldPhonetics){}
	
	virtual igdeUndo *OnAction(saeSAnimation*){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		saeWord * const word = pPanel.GetActiveWord();
		if(!phoneme || !word){
			return nullptr;
		}
		
		const decString &oldPhoneticsUtf8 = pTextFieldPhonetics.GetText();
		const int position = pTextFieldPhonetics.GetCursorPosition();
		
		decUnicodeString phonetics(decUnicodeString::NewFromUTF8(oldPhoneticsUtf8.GetLeft(position)));
		
		phonetics.AppendCharacter(phoneme->GetIPA());
		
		phonetics += decUnicodeString::NewFromUTF8(
			oldPhoneticsUtf8.GetRight(oldPhoneticsUtf8.GetLength() - position));
		
		return phonetics != word->GetPhonetics() ? new saeUWordSetPhonetics(word, phonetics) : nullptr;
	}
};

}



// Class saeWPSAnim
/////////////////////

// Constructor, destructor
////////////////////////////

saeWPSAnim::saeWPSAnim(saeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(nullptr),
pSAnimation(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new saeWPSAnimListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// speech animation
	helper.GroupBox(content, groupBox, "Speech Animation:");
	
	helper.EditPath(groupBox, "Rig:", "Path to rig resource.",
		igdeEnvironment::efpltRig, pEditRigPath, new cEditRigPath(*this));
	helper.EditPath(groupBox, "Animation:", "Path to animation resource.",
		igdeEnvironment::efpltAnimation, pEditAnimPath, new cEditAnimationPath(*this));
	
	helper.ComboBoxFilter(groupBox, "Neutral Move:", true,
		"Animation move to use for the neutral mouth position.",
		pCBNeutralMove, new cComboNeutralMove(*this));
	pCBNeutralMove->SetDefaultSorter();
	
	helper.GroupBoxFlow(content, groupBox, "Neutral vertex position sets:");
	
	formLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Set name", pCBNeutralVertexPositionSets, nullptr);
	helper.Button(formLine, pBtnNeutralVertexPositionSetAdd, new cActionNeutralVertexPositionSetsAdd(*this), true);
	helper.Button(formLine, pBtnNeutralVertexPositionSetDel, new cActionNeutralVertexPositionSetsRemove(*this), true);
	
	helper.ListBox(groupBox, 5, "Neutral vertex positions sets. Will be reset to avoid problems."
		" Sets used by phonemes are automatically reset", pListNeutralVertexPositionSets,
		new cListNeutralVertexPositionSets(*this));
	pListNeutralVertexPositionSets->SetDefaultSorter();
	
	
	// phoneme
	helper.GroupBox(content, groupBox, "Phoneme:");
	
	helper.ComboBox(groupBox, "Phoneme:", "Phoneme to edit.", pCBPhoneme, new cComboPhoneme(*this));
	pCBPhoneme->SetDefaultSorter();
	
	helper.EditString(groupBox, "IPA Symbol:", "IPA Symbol (Unicode) representing the phoneme.",
		pEditPhonemeIPA, new cTextPhonemeIpa(*this));
	helper.EditString(groupBox, "Sample Text:", "Sample text containing the phoneme.",
		pEditPhonemeSampleText, new cTextPhonemeSampleText(*this));
	helper.EditFloat(groupBox, "Length:", "Length of the phoneme in seconds.",
		pEditPhonemeLength, new cTextPhonemeLength(*this));
	
	helper.ComboBoxFilter(groupBox, "Move:", true, "Animation move to use for this phoneme.",
		pCBPhonemeMove, new cComboPhonemeMove(*this));
	pCBPhonemeMove->SetDefaultSorter();
	
	helper.ComboBoxFilter(groupBox, "Vertex Position Set:", true,
		"Model vertex position set to use for this phoneme.",
		pCBPhonemeVertexPositionSet, new cComboPhonemeVertexPositionSet(*this));
	pCBPhonemeVertexPositionSet->SetDefaultSorter();
	
	
	// word
	helper.GroupBox(content, groupBox, "Word:");
	
	helper.ComboBoxFilter(groupBox, "Word:", "Word to edit.", pCBWord, new cComboWord(*this));
	pCBWord->SetDefaultSorter();
	
	helper.EditString(groupBox, "Name:", "Name of the word.",
		pEditWordName, new cTextWordName(*this));
	
	helper.FormLineStretchFirst(groupBox, "Phonetics", "Phonetics of the word.", formLine);
	helper.EditString(formLine, "Phonetics of the word.",
		pEditWordPhonetics, new cTextWordPhonetics(*this));
	pActionWordAddIpa.TakeOver(new cActionWordAddIpa(*this, pEditWordPhonetics));
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
		sanimation->AddReference();
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
		if(!pListNeutralVertexPositionSets->GetSelectedItem() && pListNeutralVertexPositionSets->GetItemCount() > 0){
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
		const deModel * const model = component ? component->GetModel() : nullptr;
		
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
	pCBPhoneme->RemoveAllItems();
	
	if(pSAnimation){
		const saePhonemeList &list = pSAnimation->GetPhonemeList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			saePhoneme * const phoneme = list.GetAt(i);
			decUnicodeString text(phoneme->GetIPA());
			text.AppendFormat(" (0x%.4x): %s", phoneme->GetIPA(),
				phoneme->GetSampleText().GetString());
			pCBPhoneme->AddItem(text.ToUTF8(), nullptr, phoneme);
		}
		
		pCBPhoneme->SortItems();
	}
	
	SelectActivePhoneme();
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
		const deModel * const engModel = engComponent ? engComponent->GetModel() : nullptr;
		
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
	saeWord * const selection = GetActiveWord();
	
	pCBWord->RemoveAllItems();
	
	if(pSAnimation){
		const saeWordList &list = pSAnimation->GetWordList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			saeWord * const word = list.GetAt(i);
			pCBWord->AddItem(word->GetName(), nullptr, word);
		}
		
		pCBWord->SortItems();
	}
	
	pCBWord->StoreFilterItems();
	pCBWord->FilterItems();
	
	if(pSAnimation){
		pSAnimation->SetActiveWord(selection);
	}
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
