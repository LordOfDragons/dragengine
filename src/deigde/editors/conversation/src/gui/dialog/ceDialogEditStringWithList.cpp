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

#include "ceDialogEditStringWithList.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>

#include <dragengine/common/exceptions.h>



// Class ceDialogEditStringWithList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceDialogEditStringWithList::ceDialogEditStringWithList( igdeEnvironment &environment,
const char *windowTitle, const char *textLabel, const char *value, const decStringList &choices ) :
igdeDialog( environment, windowTitle )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerForm( environment, igdeContainerForm::esLast ) );
	helper.ComboBoxFilter( content, textLabel, true, "", pCBString, NULL );
	pCBString->SetDefaultSorter();
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Cancel" );
	
	AddContent( content, buttonBar );
	
	// init list
	const int count = choices.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pCBString->AddItem( choices.GetAt( i ) );
	}
	pCBString->SortItems();
	pCBString->StoreFilterItems();
	
	pCBString->SetText( value );
}

ceDialogEditStringWithList::~ceDialogEditStringWithList(){
}



// Management
///////////////

const decString &ceDialogEditStringWithList::GetString() const{
	return pCBString->GetText();
}



void ceDialogEditStringWithList::OnDialogShown(){
	pCBString->Focus();
}
