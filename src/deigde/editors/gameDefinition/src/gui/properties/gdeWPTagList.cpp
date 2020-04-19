/* 
 * Drag[en]gine IGDE Game Definition Editor
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
#include <deigde/gui/event/igdeListBoxListenerReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cActionAdd : public igdeAction {
	gdeWPTagList &pPanel;
	igdeComboBoxFilterReference &pComboBox;
	igdeListBoxReference &pListBox;
	
public:
	cActionAdd( gdeWPTagList &panel, igdeComboBoxFilterReference &comboBox, igdeListBoxReference &listBox ) : 
	igdeAction( "Add...", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add tag" ),
	pPanel( panel ), pComboBox( comboBox ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( *pPanel.GetTagList() + pComboBox->GetText() ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		pListBox->SetSelection( pListBox->IndexOfItem( pComboBox->GetText() ) );
	}
};

class cActionRemove : public igdeAction {
	gdeWPTagList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionRemove( gdeWPTagList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove tag" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decStringSet tags( *pPanel.GetTagList() );
		tags.Remove( pListBox->GetSelectedItem()->GetText() );
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( tags ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		if( pListBox->GetItemCount() > 0 ){
			pListBox->SetSelection( 0 );
		}
	}
};

class cActionClear : public igdeAction {
	gdeWPTagList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionClear( gdeWPTagList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Clear", NULL, "Clear tag" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetTagList() || ! pPanel.GetUndoSystem() || pListBox->GetItemCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( decStringSet() ) );
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
	pActionAdd.TakeOver( new cActionAdd( *this, pComboBox, pListBox ) );
	pActionRemove.TakeOver( new cActionRemove( *this, pListBox ) );
	pActionClear.TakeOver( new cActionClear( *this, pListBox ) );
	
	igdeContainerReference comboLine;
	comboLine.TakeOver( new igdeContainerFlow( GetEnvironment(),
		igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
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
