/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "lpeViewLangPack.h"
#include "lpeViewLangPackListener.h"
#include "lpeWindowMain.h"
#include "../configuration/lpeConfiguration.h"
#include "../langpack/lpeLangPack.h"
#include "../langpack/entry/lpeLangPackEntry.h"
#include "../undosys/entry/lpeULangPackEntrySetText.h"
#include "../undosys/entry/lpeULangPackEntrySetName.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/layout/igdeContainerSplittedReference.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeIconListBoxListenerReference.h>
#include <deigde/gui/model/igdeListItemSorter.h>
#include <deigde/gui/model/igdeListItemSorterReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



namespace{

class cTextFilter : public igdeTextFieldListener{
protected:
	lpeViewLangPack &pView;
	
public:
	cTextFilter( lpeViewLangPack &view ) : pView( view ){}
	
	virtual void OnTextChanging( igdeTextField* ){
		pView.UpdateEntries();
	}
};


class cTextName : public igdeTextFieldListener{
protected:
	lpeViewLangPack &pView;
	
public:
	cTextName( lpeViewLangPack &view ) : pView( view ){}
	
	virtual void OnTextChanged( igdeTextField *textField ){
		lpeLangPackEntry * const entry = pView.GetActiveEntry();
		if( ! entry ){
			return;
		}
		
		const decString &name = textField->GetText();
		if( entry->GetName() == name ){
			return;
		}
		
		lpeLangPack &langpack = *pView.GetLangPack();
		if( langpack.GetEntryList().HasNamed( name ) ){
			igdeCommonDialogs::ErrorFormat( &pView, "Set Entry Identifier",
				"There exists already an entry with name '%s'", name.GetString() );
			textField->SetText( entry->GetName() );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new lpeULangPackEntrySetName( entry, name ) );
		if( undo ){
			langpack.GetUndoSystem()->Add( undo );
		}
	}
};


class cTextText : public igdeTextAreaListener{
protected:
	lpeViewLangPack &pView;
	
public:
	cTextText( lpeViewLangPack &view ) : pView( view ){}
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		lpeLangPackEntry * const entry = pView.GetActiveEntry();
		if( ! entry ){
			return;
		}
		
		const decUnicodeString text( decUnicodeString::NewFromUTF8( textArea->GetText() ) );
		if( entry->GetText() == text ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new lpeULangPackEntrySetText( entry, text ) );
		if( undo ){
			pView.GetLangPack()->GetUndoSystem()->Add( undo );
		}
	}
	
	// SEL_FOCUSOUT ?
};


class cListEntries : public igdeIconListBoxListener{
protected:
	enum eSorting{
		esNameAscending,
		esNameDescending,
	};
	
	lpeViewLangPack &pView;
	eSorting pSorting;
	
public:
	cListEntries( lpeViewLangPack &view ) :
	pView( view ),
	pSorting( esNameAscending ){}
	
	virtual void OnSelectionChanged( igdeIconListBox *listBox ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		
		langpack->GetEntrySelection().SetActive( selection
			? ( lpeLangPackEntry* )selection->GetData() : NULL );
		
		langpack->NotifyActiveEntryChanged();
	}
	
	// TODO add
	// - SEL_RIGHTBUTTONPRESS, SEL_RIGHTBUTTONRELEASE
	//   something like ListBoxClicked, ItemClicked
	virtual void OnItemSelected( igdeIconListBox *listBox, int index ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		langpack->GetEntrySelection().Add( ( lpeLangPackEntry* )listBox->GetItemAt( index )->GetData() );
		langpack->NotifyEntrySelectionChanged();
	}
	
	virtual void OnItemDeselected( igdeIconListBox *listBox, int index ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		langpack->GetEntrySelection().Remove( ( lpeLangPackEntry* )listBox->GetItemAt( index )->GetData() );
		langpack->NotifyEntrySelectionChanged();
	}
	
	virtual void OnHeaderClicked( igdeIconListBox *listBox, int index ){
		if( index == 0 ){
			pSorting = pSorting == esNameAscending ? esNameDescending : esNameAscending;
			UpdateSorter( *listBox );
		}
	}
	
	void UpdateSorter( igdeIconListBox &listBox ){
		igdeListItemSorterReference sorter;
		
		switch( pSorting ){
		case esNameDescending:
			sorter.TakeOver( new igdeListItemSorter( false ) );
			break;
			
		case esNameAscending:
		default:
			sorter.TakeOver( new igdeListItemSorter( true ) );
			break;
		}
		
		listBox.SetSorter( sorter );
	}
	
	static void AddToListBox( lpeViewLangPack &window, igdeIconListBox &listBox ){
		igdeIconListBoxListenerReference listener;
		listener.TakeOver( new cListEntries( window ) );
		listBox.AddListener( listener );
		
		( ( cListEntries& )( igdeIconListBoxListener& )listener ).UpdateSorter( listBox );
	}
};

}



// Class lpeViewLangPack
//////////////////////////

// Constructor, destructor
////////////////////////////

lpeViewLangPack::lpeViewLangPack( lpeWindowMain &windowMain ) :
igdeContainerBorder( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pListener( new lpeViewLangPackListener( *this ) ),
pLangPack( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	// filter line on top
	igdeContainerReference topLine;
	topLine.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( topLine, "Filter:", "Filter entries by identifier.",
		pEditFilter, new cTextFilter( *this ) );
	AddChild( topLine, igdeContainerBorder::eaTop );
	
	// content split between list and bottom lin
	igdeContainerSplittedReference splitted;
	splitted.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espBottom, 100 ) );
	AddChild( splitted, igdeContainerBorder::eaCenter );
	
	igdeContainerReference sidePanel;
	sidePanel.TakeOver( new igdeContainerForm( env, igdeContainerForm::esLast ) );
	helper.EditString( sidePanel, "Identifier:", "Unique identifier name of the entry.",
		pEditEntryName, new cTextName( *this ) );
	helper.EditString( sidePanel, "Text:", "Text of the selected entry.",
		pEditEntryText, 5, new cTextText( *this ) );
	splitted->AddChild( sidePanel, igdeContainerSplitted::eaSide );
	
	const igdeUIHelper::sColumnHeader listHeaders[ 2 ] = {
		{ "Identifier", NULL, 200 },
		{ "Text", NULL, 800 }
	};
	helper.IconListBox( decPoint( 100, 150 ), listHeaders, 2, "Entries", pListEntries, NULL );
	pListEntries->SetSelectionMode( igdeIconListBox::esmMultiple );
	cListEntries::AddToListBox( *this, pListEntries );
	splitted->AddChild( pListEntries, igdeContainerSplitted::eaCenter );
}

lpeViewLangPack::~lpeViewLangPack(){
	SetLangPack( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void lpeViewLangPack::SetLangPack( lpeLangPack *langpack ){
	if( langpack == pLangPack ){
		return;
	}
	
	if( pLangPack ){
		pLangPack->RemoveListener( pListener );
		pLangPack->FreeReference();
	}
	
	pLangPack = langpack;
	
	if( langpack ){
		langpack->AddReference();
		langpack->AddListener( pListener );
	}
	
	UpdateEntries();
	SelectActiveEntry();
}



lpeLangPackEntry *lpeViewLangPack::GetActiveEntry() const{
	if( pLangPack ){
		return pLangPack->GetEntrySelection().GetActive();
	}
	return NULL;
}



void lpeViewLangPack::UpdateEntries(){
	pListEntries->RemoveAllItems();
	
	if( pLangPack ){
		const decString filter( pEditFilter->GetText().GetLower() );
		const lpeLangPackEntryList &list = pLangPack->GetEntryList();
		const int entryCount = list.GetCount();
		int i;
		
		for( i=0; i<entryCount; i++ ){
			lpeLangPackEntry * const entry = list.GetAt( i );
			
			if( filter.IsEmpty() || entry->GetName().GetLower().FindString( filter ) != -1 ){
			//if( filter.IsEmpty() || entry->GetName().MatchesPattern( filter ) ){
				decStringList details;
				details.Add( entry->GetText().ToUTF8() );
				pListEntries->AddItem( entry->GetName(), details, NULL, entry );
			}
		}
	}
	
	SortEntries();
	UpdateEntrySelection();
	SelectActiveEntry();
}

void lpeViewLangPack::SortEntries() {
	pListEntries->SortItems();
}

void lpeViewLangPack::SelectActiveEntry(){
	pListEntries->SetSelection( pListEntries->IndexOfItemWithData( GetActiveEntry() ) );
	pListEntries->EnsureSelectedItemVisible();
	UpdateActiveEntry();
}

void lpeViewLangPack::UpdateActiveEntry(){
	lpeLangPackEntry * const entry = GetActiveEntry();
	
	if( entry ){
		UpdateEntry( entry );
		pEditEntryName->SetText( entry->GetName() );
		pEditEntryText->SetText( entry->GetText().ToUTF8() );
		
	}else{
		pEditEntryName->SetText( "" );
		pEditEntryText->SetText( "" );
	}
	
	const bool enabled = entry != NULL;
	pEditEntryName->SetEnabled( enabled );
	pEditEntryText->SetEnabled( enabled );
}

void lpeViewLangPack::UpdateEntrySelection(){
	if( ! pLangPack ){
		return;
	}
	
	//const lpeLangPackEntryList &selection = pLangPack->GetEntrySelection().GetSelected();
	igdeIconListBox &list = pListEntries;
	const int count = list.GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( lpeLangPackEntry* )list.GetItemAt( i )->GetData() )->GetSelected() ){
			list.SelectItem( i );
			
		}else{
			list.DeselectItem( i );
		}
	}
}

void lpeViewLangPack::UpdateEntry( lpeLangPackEntry *entry ){
	if( ! entry ){
		DETHROW( deeInvalidParam );
	}
	
	const int selection = pListEntries->IndexOfItemWithData( entry );
	if( selection == -1 ){
		return;
	}
	
	igdeListItem &item = *pListEntries->GetItemAt( selection );
	item.SetText( entry->GetName() );
	item.GetDetails().SetAt( 0, entry->GetText().ToUTF8() );
	
	pListEntries->ItemChangedAt( selection );
}
