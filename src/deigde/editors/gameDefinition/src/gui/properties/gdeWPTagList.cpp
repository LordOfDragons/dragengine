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

#include "gdeWPTagList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cActionAppend : public igdeAction {
	gdeWPTagList &pPanel;
	igdeComboBoxFilter::Ref &pComboBox;
	igdeListBox::Ref &pListBox;
	
public:
	cActionAppend ( gdeWPTagList &panel, igdeComboBoxFilter::Ref &comboBox, igdeListBox::Ref &listBox ) : 
	igdeAction( "Add...", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add tag" ),
	pPanel( panel ), pComboBox( comboBox ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( pPanel.UndoSet( *pPanel.GetTagList() + pComboBox->GetText() ) ));
		pPanel.GetUndoSystem()->Add( undo );
		
		pListBox->SetSelection( pListBox->IndexOfItem( pComboBox->GetText() ) );
	}
};

class cActionRemove : public igdeAction {
	gdeWPTagList &pPanel;
	igdeListBox::Ref &pListBox;
	
public:
	cActionRemove( gdeWPTagList &panel, igdeListBox::Ref &listBox ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove tag" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decStringSet tags( *pPanel.GetTagList() );
		tags.Remove( pListBox->GetSelectedItem()->GetText() );
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( pPanel.UndoSet( tags ) ));
		pPanel.GetUndoSystem()->Add( undo );
		
		if( pListBox->GetItemCount() > 0 ){
			pListBox->SetSelection( 0 );
		}
	}
};

class cActionClear : public igdeAction {
	gdeWPTagList &pPanel;
	igdeListBox::Ref &pListBox;
	
public:
	cActionClear( gdeWPTagList &panel, igdeListBox::Ref &listBox ) :
	igdeAction( "Clear", NULL, "Clear tag" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() || pListBox->GetItemCount() == 0 ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( pPanel.UndoSet( decStringSet() ) ));
		pPanel.GetUndoSystem()->Add( undo );
	}
};

class cListTags : public igdeListBoxListener {
	gdeWPTagList &pListBox;
	
public:
	cListTags( gdeWPTagList &listBox ) : pListBox( listBox ){ }
	
	virtual void OnSelectionChanged( igdeListBox* ){
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = pListBox.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pListBox.GetActionAdd() );
		helper.MenuCommand( menu, pListBox.GetActionRemove() );
		helper.MenuCommand( menu, pListBox.GetActionClear() );
	}
};

}



// Class gdeWPTagList
///////////////////////

// Constructor, destructor
////////////////////////////

gdeWPTagList::gdeWPTagList( igdeUIHelper &helper, int rows, const char *description ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone ),
pTagList( NULL ),
pUndoSystem( NULL )
{
	pActionAdd.TakeOver( new cActionAppend ( *this, pComboBox, pListBox ) );
	pActionRemove.TakeOver( new cActionRemove( *this, pListBox ) );
	pActionClear.TakeOver( new cActionClear( *this, pListBox ) );
	
	igdeContainerFlow::Ref comboLine(igdeContainerFlow::Ref::New(new igdeContainerFlow( GetEnvironment(),
		igdeContainerFlow::eaX, igdeContainerFlow::esFirst )));
	helper.ComboBoxFilter( comboLine, true, description, pComboBox, NULL );
	pComboBox->SetDefaultSorter();
	pComboBox->SetFilterCaseInsentive( true );
	helper.Button( comboLine, pActionAdd );
	AddChild( comboLine );
	
	helper.ListBox( *this, rows, description, pListBox, new cListTags( *this ) );
	pListBox->SetDefaultSorter();
}

gdeWPTagList::~gdeWPTagList(){
}



// Management
///////////////

void gdeWPTagList::SetTagList( const decStringSet *tagList ){
	if( tagList == pTagList ){
		return;
	}
	
	pTagList = tagList;
	
	UpdateList();
}

void gdeWPTagList::SetUndoSystem( igdeUndoSystem *undoSystem ){
	pUndoSystem = undoSystem;
}



const decString &gdeWPTagList::GetSelectedTag() const{
	if( pListBox->GetSelectedItem() != NULL ){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void gdeWPTagList::UpdateList(){
	const decString selection( GetSelectedTag() );
	
	pListBox->RemoveAllItems();
	
	if( pTagList ){
		const int count = pTagList->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListBox->AddItem( pTagList->GetAt( i ) );
		}
		
		pListBox->SortItems();
	}
	
	SelectTag( selection );
}

void gdeWPTagList::SelectTag( const decString &tag ){
	if( pTagList ){
		pListBox->SetSelection( pListBox->IndexOfItem( tag ) );
	}
}

void gdeWPTagList::UpdateUsedTagList( const decStringSet &usedTags ){
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
