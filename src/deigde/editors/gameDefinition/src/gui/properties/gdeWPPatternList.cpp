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

#include "gdeWPPatternList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cActionAppend : public igdeAction {
	gdeWPPatternList &pPanel;
	igdeEditPathReference &pEditPath;
	igdeListBoxReference &pListBox;
	
public:
	cActionAppend ( gdeWPPatternList &panel, igdeEditPathReference &editPath, igdeListBoxReference &listBox ) : 
	igdeAction( "Add", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add pattern" ),
	pPanel( panel ), pEditPath( editPath ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPatternList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( pPanel.UndoSet( *pPanel.GetPatternList() + pEditPath->GetPath() ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		pListBox->SetSelection( pListBox->IndexOfItem( pEditPath->GetPath() ) );
	}
};

class cActionRemove : public igdeAction {
	gdeWPPatternList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionRemove( gdeWPPatternList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove pattern" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPatternList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decStringSet patterns( *pPanel.GetPatternList() );
		patterns.Remove( pListBox->GetSelectedItem()->GetText() );
		
		igdeUndo::Ref undo;
		undo.TakeOver( pPanel.UndoSet( patterns ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		if( pListBox->GetItemCount() > 0 ){
			pListBox->SetSelection( 0 );
		}
	}
};

class cActionClear : public igdeAction {
	gdeWPPatternList &pPanel;
	igdeListBoxReference &pListBox;
	
public:
	cActionClear( gdeWPPatternList &panel, igdeListBoxReference &listBox ) :
	igdeAction( "Clear", NULL, "Clear pattern" ), pPanel( panel ), pListBox( listBox ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPatternList() || ! pPanel.GetUndoSystem() || pListBox->GetItemCount() == 0 ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( pPanel.UndoSet( decStringSet() ) );
		pPanel.GetUndoSystem()->Add( undo );
	}
};

class cListPatterns : public igdeListBoxListener {
	gdeWPPatternList &pListBox;
	
public:
	cListPatterns( gdeWPPatternList &listBox ) : pListBox( listBox ){ }
	
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



// Class gdeWPPatternList
///////////////////////

// Constructor, destructor
////////////////////////////

gdeWPPatternList::gdeWPPatternList( igdeUIHelper &helper, int rows, const char *description ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone ),
pPatternList( NULL ),
pUndoSystem( NULL )
{
	pActionAdd.TakeOver( new cActionAppend ( *this, pEditPath, pListBox ) );
	pActionRemove.TakeOver( new cActionRemove( *this, pListBox ) );
	pActionClear.TakeOver( new cActionClear( *this, pListBox ) );
	
	helper.EditPath( *this, description, igdeEnvironment::efpltAll, pEditPath, NULL );
	pEditPath->SetAutoValidatePath( false );
	
	helper.ListBox( *this, rows, description, pListBox, new cListPatterns( *this ) );
	pListBox->SetDefaultSorter();
}

gdeWPPatternList::~gdeWPPatternList(){
}



// Management
///////////////

void gdeWPPatternList::SetPatternList( const decStringSet *tagList ){
	if( tagList == pPatternList ){
		return;
	}
	
	pPatternList = tagList;
	
	UpdateList();
}

void gdeWPPatternList::SetUndoSystem( igdeUndoSystem *undoSystem ){
	pUndoSystem = undoSystem;
}



const decString &gdeWPPatternList::GetSelectedPattern() const{
	if( pListBox->GetSelectedItem() != NULL ){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void gdeWPPatternList::UpdateList(){
	const decString selection( GetSelectedPattern() );
	
	pListBox->RemoveAllItems();
	
	if( pPatternList ){
		const int count = pPatternList->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListBox->AddItem( pPatternList->GetAt( i ) );
		}
		
		pListBox->SortItems();
	}
	
	SelectPattern( selection );
}

void gdeWPPatternList::SelectPattern( const decString &tag ){
	if( pPatternList ){
		pListBox->SetSelection( pListBox->IndexOfItem( tag ) );
	}
}
