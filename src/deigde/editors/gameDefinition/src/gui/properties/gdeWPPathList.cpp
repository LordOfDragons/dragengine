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

#include "gdeWPPathList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
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
	gdeWPPathList &pPanel;
	igdeEditPathReference &pEditPath;
	igdeListBoxReference &pListBox;
	
public:
	cActionAdd( gdeWPPathList &panel, igdeEditPathReference &editPath, igdeListBoxReference &listBox ) : 
	igdeAction( "Add", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add path" ),
	pPanel( panel ), pEditPath( editPath ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPathList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( *pPanel.GetPathList() + pEditPath->GetPath() ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		pListBox->SetSelection( pListBox->IndexOfItem( pEditPath->GetPath() ) );
	}
};

class cActionRemove : public igdeAction {
	gdeWPPathList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionRemove( gdeWPPathList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove path" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPathList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decStringSet paths( *pPanel.GetPathList() );
		paths.Remove( pListBox->GetSelectedItem()->GetText() );
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( paths ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		if( pListBox->GetItemCount() > 0 ){
			pListBox->SetSelection( 0 );
		}
	}
};

class cActionClear : public igdeAction {
	gdeWPPathList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionClear( gdeWPPathList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Clear", NULL, "Clear path" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPathList() || ! pPanel.GetUndoSystem() || pListBox->GetItemCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSet( decStringSet() ) );
		pPanel.GetUndoSystem()->Add( undo );
	}
};

class cListPaths : public igdeListBoxListener {
	gdeWPPathList &pListBox;
	
public:
	cListPaths( gdeWPPathList &listBox ) : pListBox( listBox ){ }
	
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



// Class gdeWPPathList
///////////////////////

// Constructor, destructor
////////////////////////////

gdeWPPathList::gdeWPPathList( igdeUIHelper &helper, int rows, const char *description ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone ),
pPathList( NULL ),
pUndoSystem( NULL )
{
	pActionAdd.TakeOver( new cActionAdd( *this, pEditPath, pListBox ) );
	pActionRemove.TakeOver( new cActionRemove( *this, pListBox ) );
	pActionClear.TakeOver( new cActionClear( *this, pListBox ) );
	
	helper.EditPath( *this, description, igdeEnvironment::efpltAll, pEditPath, NULL );
	pEditPath->SetAutoValidatePath( false );
	
	helper.ListBox( *this, rows, description, pListBox, new cListPaths( *this ) );
	pListBox->SetDefaultSorter();
}

gdeWPPathList::~gdeWPPathList(){
}



// Management
///////////////

void gdeWPPathList::SetPathList( const decStringSet *tagList ){
	if( tagList == pPathList ){
		return;
	}
	
	pPathList = tagList;
	
	UpdateList();
}

void gdeWPPathList::SetUndoSystem( igdeUndoSystem *undoSystem ){
	pUndoSystem = undoSystem;
}



const decString &gdeWPPathList::GetSelectedPath() const{
	if( pListBox->GetSelectedItem() != NULL ){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void gdeWPPathList::UpdateList(){
	const decString selection( GetSelectedPath() );
	
	pListBox->RemoveAllItems();
	
	if( pPathList ){
		const int count = pPathList->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListBox->AddItem( pPathList->GetAt( i ) );
		}
		
		pListBox->SortItems();
	}
	
	SelectPath( selection );
}

void gdeWPPathList::SelectPath( const decString &tag ){
	if( pPathList ){
		pListBox->SetSelection( pListBox->IndexOfItem( tag ) );
	}
}
