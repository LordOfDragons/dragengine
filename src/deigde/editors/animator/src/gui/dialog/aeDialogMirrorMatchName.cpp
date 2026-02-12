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

#include "aeDialogMirrorMatchName.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/common/exceptions.h>



// Class aeDialogMirrorMatchName
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeDialogMirrorMatchName::aeDialogMirrorMatchName(
	igdeEnvironment &environment, const char *windowTitle) :
igdeDialog(environment, windowTitle)
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerForm::Ref content(igdeContainerForm::Ref::New(
		environment, igdeContainerForm::esLast));
	
	helper.ComboBox(content, "@Animator.DialogMirrorMatchName.Type.Label", "@Animator.DialogMirrorMatchName.Type.ToolTip", pCBType, {});
	pCBType->SetAutoTranslateItems(true);
	pCBType->AddItem("@Animator.DialogMirrorMatchName.Type.BeginOfName", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emntFirst);
	pCBType->AddItem("@Animator.DialogMirrorMatchName.Type.EndOfName", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emntLast);
	pCBType->AddItem("@Animator.DialogMirrorMatchName.Type.MiddleOfName", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emntMiddle);
	pCBType->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleMirror::emntLast);
	
	helper.EditString(content, "@Animator.DialogMirrorMatchName.First.Label", "@Animator.DialogMirrorMatchName.First.ToolTip", 30, pEditFirst, {});
	helper.EditString(content, "@Animator.DialogMirrorMatchName.Second.Label", "@Animator.DialogMirrorMatchName.Second.ToolTip", 30, pEditSecond, {});
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Igde.Accept", "@Igde.Cancel");
	
	AddContent(content, buttonBar);
}

aeDialogMirrorMatchName::~aeDialogMirrorMatchName(){
}



// Management
///////////////

void aeDialogMirrorMatchName::Set(const aeRuleMirror::MatchName &matchName){
	pCBType->SetSelectionWithData((void*)(intptr_t)matchName.type);
	pEditFirst->SetText(matchName.first);
	pEditSecond->SetText(matchName.second);
}

aeRuleMirror::MatchName::Ref aeDialogMirrorMatchName::CreateMatchName() const{
	return aeRuleMirror::MatchName::Ref::New(pEditFirst->GetText(), pEditSecond->GetText(),
		(deAnimatorRuleMirror::eMatchNameType)(intptr_t)pCBType->GetSelectedItem()->GetData());
}

void aeDialogMirrorMatchName::OnDialogShown(){
	pEditFirst->Focus();
}

bool aeDialogMirrorMatchName::Accept(){
	const decString &first = pEditFirst->GetText();
	const decString &second = pEditSecond->GetText();
	
	if(first.IsEmpty()){
		igdeCommonDialogs::Error(*this, GetTitle(), "@Animator.DialogMirrorMatchName.ErrorFirstEmpty");
		return false;
	}
	if(second.IsEmpty()){
		igdeCommonDialogs::Error(*this, GetTitle(), "@Animator.DialogMirrorMatchName.ErrorSecondEmpty");
		return false;
	}
	
	return igdeDialog::Accept();
}
