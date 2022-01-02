/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "aeDialogMirrorMatchName.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
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
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerForm( environment, igdeContainerForm::esLast ) );
	
	helper.ComboBox( content, "Type:", "How to match the name.", pCBType, nullptr );
	pCBType->AddItem( "Begin of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntFirst );
	pCBType->AddItem( "End of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntLast );
	pCBType->AddItem( "Middle of name", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emntMiddle );
	pCBType->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleMirror::emntLast );
	
	helper.EditString( content, "First:", "First name string component to match.", 30, pEditFirst, nullptr );
	helper.EditString( content, "Second:", "Second name string component to match.", 30, pEditSecond, nullptr );
	
	igdeContainerReference buttonBar;
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
	return aeRuleMirror::cMatchName::Ref::New( new aeRuleMirror::cMatchName(
		pEditFirst->GetText(), pEditSecond->GetText(), ( deAnimatorRuleMirror::eMatchNameType )
			( intptr_t )pCBType->GetSelectedItem()->GetData() ) );
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
