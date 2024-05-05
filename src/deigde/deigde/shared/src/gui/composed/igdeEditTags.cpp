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

#include <stdio.h>
#include <stdlib.h>

#include "igdeEditTags.h"

#include "../igdeComboBoxFilter.h"
#include "../igdeCommonDialogs.h"
#include "../igdeListBox.h"
#include "../igdeUIHelper.h"
#include "../igdeContainerReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeListBoxListenerReference.h"
#include "../menu/igdeMenuCascade.h"
#include "../model/igdeListItem.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndo.h"
#include "../../undo/igdeUndoReference.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

igdeEditTags::cActionAdd::cActionAdd( igdeEditTags &widget ) :
igdeAction( "Add...", widget.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add tag" ),
pWidget( widget ){
}

void igdeEditTags::cActionAdd::OnAction(){
	const decString tag( pWidget.GetComboBox().GetText() );
	pWidget.SetTags( pWidget.GetTags() + tag );
	pWidget.SelectTag( tag );
	pWidget.OnAction();
}


igdeEditTags::cActionRemove::cActionRemove( igdeEditTags &widget ) :
igdeAction( "Remove", widget.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove tag" ),
pWidget( widget ){
}

void igdeEditTags::cActionRemove::OnAction(){
	if( ! pWidget.GetListBox().GetSelectedItem() ){
		return;
	}
	
	decStringSet tags( pWidget.GetTags() );
	tags.Remove( pWidget.GetListBox().GetSelectedItem()->GetText() );
	pWidget.SetTags( tags );
	if( pWidget.GetListBox().GetItemCount() > 0 ){
		pWidget.GetListBox().SetSelection( 0 );
	}
	pWidget.OnAction();
}


igdeEditTags::cActionClear::cActionClear( igdeEditTags &widget ) :
igdeAction( "Clear", NULL, "Clear tag" ),
pWidget( widget ){
}

void igdeEditTags::cActionClear::OnAction(){
	if( pWidget.GetListBox().GetItemCount() == 0 ){
		return;
	}
	pWidget.SetTags( decStringSet() );
	pWidget.OnAction();
}


class igdeEditTags_ListTags : public igdeListBoxListener {
	igdeEditTags &pWidget;
	
public:
	igdeEditTags_ListTags( igdeEditTags &widget ) : pWidget( widget ){
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = pWidget.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pWidget.GetActionAdd() );
		helper.MenuCommand( menu, pWidget.GetActionRemove() );
		helper.MenuCommand( menu, pWidget.GetActionClear() );
	}
};



// Class igdeEditTags
///////////////////////

// Constructor, destructor
////////////////////////////

igdeEditTags::igdeEditTags( igdeUIHelper &helper, int rows ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone )
{
	pCreateContent( helper, rows );
}

igdeEditTags::igdeEditTags( igdeUIHelper &helper, int rows, igdeAction *action ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone )
{
	SetAction( action );
	pCreateContent( helper, rows );
}

igdeEditTags::~igdeEditTags(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeEditTags::SetTags( const decStringSet &tags ){
	if( tags == pTags ){
		return;
	}
	
	pTags = tags;
	
	UpdateList();
}

const decString &igdeEditTags::GetSelectedTag() const{
	if( pListBox->GetSelectedItem() != NULL ){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void igdeEditTags::UpdateList(){
	const decString selection( GetSelectedTag() );
	
	pListBox->RemoveAllItems();
	
	const int count = pTags.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pListBox->AddItem( pTags.GetAt( i ) );
	}
	
	pListBox->SortItems();
	
	SelectTag( selection );
}

void igdeEditTags::SelectTag( const decString &tag ){
	pListBox->SetSelection( pListBox->IndexOfItem( tag ) );
}

void igdeEditTags::UpdateUsedTagList( const decStringSet &usedTags ){
	const decString selectedTag( pComboBox->GetText() );
	
	pComboBox->RemoveAllItems();
	
	const int count = usedTags.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decString& tag = usedTags.GetAt( i );
		pComboBox->AddItem( tag );
	}
	
	pComboBox->SortItems();
	pComboBox->StoreFilterItems();
	
	pComboBox->SetText( selectedTag );
}

void igdeEditTags::Focus(){
	pComboBox->Focus();
}



void igdeEditTags::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeEditTags::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeEditTags::OnParameterChanged( igdeAction *action ){
	pComboBox->SetDescription( action->GetDescription() );
	pListBox->SetDescription( action->GetDescription() );
}

void igdeEditTags::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeEditTags::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeEditTags::pCreateContent( igdeUIHelper &helper, int rows ){
	pActionAdd.TakeOver( new cActionAdd( *this ) );
	pActionRemove.TakeOver( new cActionRemove( *this ) );
	pActionClear.TakeOver( new cActionClear( *this ) );
	
	igdeContainerReference comboLine;
	comboLine.TakeOver( new igdeContainerFlow( GetEnvironment(),
		igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.ComboBoxFilter( comboLine, true, "", pComboBox, NULL );
	pComboBox->SetDefaultSorter();
	pComboBox->SetFilterCaseInsentive( true );
	helper.Button( comboLine, pActionAdd );
	AddChild( comboLine );
	
	helper.ListBox( *this, rows, "", pListBox, new igdeEditTags_ListTags( *this ) );
	pListBox->SetDefaultSorter();
}
