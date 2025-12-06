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
	igdeEnvironment &environment, const char *windowTitle ) :
igdeDialog( environment, windowTitle )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerForm::Ref content(igdeContainerForm::Ref::NewWith(
		environment, igdeContainerForm::esLast));
	
	helper.ComboBox( content, "Type:", "How to match the name.", pCBType, nullptr );
	pCBType->AddItem( "Begin of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntFirst );
	pCBType->AddItem( "End of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntLast );
	pCBType->AddItem( "Middle of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntMiddle );
	pCBType->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleMirror::emntLast );
	
	helper.EditString( content, "First:", "First name string component to match.", 30, pEditFirst, nullptr );
	helper.EditString( content, "Second:", "Second name string component to match.", 30, pEditSecond, nullptr );
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Cancel" );
	
	AddContent( content, buttonBar );
}

aeDialogMirrorMatchName::~aeDialogMirrorMatchName(){
}



// Management
///////////////

void aeDialogMirrorMatchName::Set( const aeRuleMirror::cMatchName &matchName ){
	pCBType->SetSelectionWithData( ( void* )( intptr_t )matchName.GetType() );
	pEditFirst->SetText( matchName.GetFirst() );
	pEditSecond->SetText( matchName.GetSecond() );
}

aeRuleMirror::cMatchName::Ref aeDialogMirrorMatchName::CreateMatchName() const{
	return aeRuleMirror::cMatchName::Ref::NewWith(pEditFirst->GetText(), pEditSecond->GetText(),
		(deAnimatorRuleMirror::eMatchNameType)(intptr_t)pCBType->GetSelectedItem()->GetData());
}

void aeDialogMirrorMatchName::OnDialogShown(){
	pEditFirst->Focus();
}

bool aeDialogMirrorMatchName::Accept(){
	const decString &first = pEditFirst->GetText();
	const decString &second = pEditSecond->GetText();
	
	if( first.IsEmpty() ){
		igdeCommonDialogs::Error( this, GetTitle(), "First name string component to match can not be empty" );
		return false;
	}
	if( second.IsEmpty() ){
		igdeCommonDialogs::Error( this, GetTitle(), "Second name string component to match can not be empty" );
		return false;
	}
	
	return igdeDialog::Accept();
}
