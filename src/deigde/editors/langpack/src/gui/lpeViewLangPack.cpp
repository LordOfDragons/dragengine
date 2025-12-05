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
#include "../undosys/entry/lpeULangPackEntryAdd.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/model/igdeListItemSorter.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



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
		if( ! entry || entry->GetLangPack() != pView.GetLangPack() ){
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
		
		igdeUndo::Ref undo;
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
		if( entry->GetText() == text && entry->GetLangPack() == pView.GetLangPack() ){
			return;
		}
		
		igdeUndo::Ref undo;
		
		if( entry->GetLangPack() == pView.GetLangPack() ){
			undo.TakeOver( new lpeULangPackEntrySetText( entry, text ) );
			
		}else{
			lpeLangPackEntry::Ref newEntry( lpeLangPackEntry::Ref::New( new lpeLangPackEntry ) );
			newEntry->SetName( entry->GetName() );
			newEntry->SetText( text );
			undo.TakeOver( new lpeULangPackEntryAdd( pView.GetLangPack(), newEntry, entry ) );
		}
		
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
	cListEntries( lpeViewLangPack &view ) : pView( view ), pSorting( esNameAscending ){}
	
	virtual void OnSelectionChanged( igdeIconListBox *listBox ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack || pView.preventUpdate ){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		pView.SelectEntry( selection ? ( lpeLangPackEntry* )selection->GetData() : nullptr );
	}
	
	// TODO add
	// - SEL_RIGHTBUTTONPRESS, SEL_RIGHTBUTTONRELEASE
	//   something like ListBoxClicked, ItemClicked
	virtual void OnItemSelected( igdeIconListBox *listBox, int index ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack || pView.preventUpdate ){
			return;
		}
		
		langpack->GetEntrySelection().Add( ( lpeLangPackEntry* )listBox->GetItemAt( index )->GetData() );
		langpack->NotifyEntrySelectionChanged();
	}
	
	virtual void OnItemDeselected( igdeIconListBox *listBox, int index ){
		lpeLangPack * const langpack = pView.GetLangPack();
		if( ! langpack || pView.preventUpdate ){
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
		igdeListItemSorter::Ref sorter;
		
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
		igdeIconListBoxListener::Ref listener;
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
pListener( lpeLangPackListener::Ref::New( new lpeViewLangPackListener( *this ) ) ),
preventUpdate( false )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	// filter line on top
	igdeContainer::Ref topLine;
	topLine.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( topLine, "Filter:", "Filter entries by identifier.",
		pEditFilter, new cTextFilter( *this ) );
	AddChild( topLine, igdeContainerBorder::eaTop );
	
	// content split between list and bottom line
	igdeContainerSplitted::Ref splitted;
	splitted.TakeOver(new igdeContainerSplitted(env, igdeContainerSplitted::espBottom,
		igdeApplication::app().DisplayScaled(100)));
	AddChild( splitted, igdeContainerBorder::eaCenter );
	
	igdeContainer::Ref sidePanel;
	sidePanel.TakeOver( new igdeContainerForm( env, igdeContainerForm::esLast ) );
	helper.EditString( sidePanel, "Identifier:", "Unique identifier name of the entry.",
		pEditEntryName, new cTextName( *this ) );
	helper.EditString( sidePanel, "Reference Text:", "Reference text of the reference language pack entry.",
		pEditRefText, nullptr );
	helper.EditString( sidePanel, "Text:", "Text of the selected entry.",
		pEditEntryText, 3, new cTextText( *this ) );
	pEditRefText->SetEditable( false );
	splitted->AddChild( sidePanel, igdeContainerSplitted::eaSide );
	
	const igdeUIHelper::sColumnHeader listHeaders[3] = {
		{"Identifier", nullptr, igdeApplication::app().DisplayScaled(250)},
		{"Text", nullptr, igdeApplication::app().DisplayScaled(650)},
		{"Reference", nullptr, igdeApplication::app().DisplayScaled(400)}
	};
	helper.IconListBox(
		igdeApplication::app().DisplayScaled(decPoint(100, 150)),
		listHeaders, 3, "Entries", pListEntries, nullptr);
	pListEntries->SetSelectionMode( igdeIconListBox::esmMultiple );
	cListEntries::AddToListBox( *this, pListEntries );
	splitted->AddChild( pListEntries, igdeContainerSplitted::eaCenter );
}

lpeViewLangPack::~lpeViewLangPack(){
	SetLangPack( nullptr );
}



// Management
///////////////

void lpeViewLangPack::SetLangPack( lpeLangPack *langpack ){
	if( pLangPack == langpack ){
		return;
	}
	
	if( pLangPack ){
		pLangPack->RemoveListener( pListener );
	}
	
	pLangPack = langpack;
	pRefLangPack = nullptr;
	
	if( langpack ){
		langpack->AddListener( pListener );
	}
	
	UpdateEntries();
	SelectActiveEntry();
}

void lpeViewLangPack::SetReferenceLangPack( lpeLangPack *langpack ){
	if( pRefLangPack == langpack ){
		return;
	}
	
	pRefLangPack = langpack;
	
	UpdateEntries();
	SelectActiveEntry();
}



lpeLangPackEntry *lpeViewLangPack::GetActiveEntry() const{
	if( pLangPack ){
		return pLangPack->GetEntrySelection().GetActive();
	}
	return nullptr;
}



void lpeViewLangPack::UpdateEntries(){
	preventUpdate = true;
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
				
				const lpeLangPackEntry *refEntry = nullptr;
				if( pRefLangPack ){
					refEntry = pRefLangPack->GetEntryList().GetNamed( entry->GetName() );
				}
				
				details.Add( refEntry ? refEntry->GetText().ToUTF8().GetString() : "" );
				
				pListEntries->AddItem( entry->GetName(), details, nullptr, entry );
			}
		}
		
		if( pRefLangPack ){
			const lpeLangPackEntryList &refList = pRefLangPack->GetEntryList();
			const int refEntryCount = refList.GetCount();
			igdeIcon * const icon = GetEnvironment().GetStockIcon( igdeEnvironment::esiWarning );
			
			for( i=0; i<refEntryCount; i++ ){
				lpeLangPackEntry * const refEntry = refList.GetAt( i );
				if( list.HasNamed( refEntry->GetName() ) ){
					continue;
				}
				
				if( filter.IsEmpty() || refEntry->GetName().GetLower().FindString( filter ) != -1 ){
				//if( filter.IsEmpty() || refEntry->GetName().MatchesPattern( filter ) ){
					decStringList details;
					details.Add( "" );
					details.Add( refEntry->GetText().ToUTF8() );
					
					pListEntries->AddItem( refEntry->GetName(), details, icon, refEntry );
				}
			}
		}
	}
	
	SortEntries();
	UpdateEntrySelection();
	preventUpdate = false;
	SelectActiveEntry();
}

void lpeViewLangPack::SortEntries() {
	pListEntries->SortItems();
}

void lpeViewLangPack::SelectActiveEntry(){
	pListEntries->SetSelectionWithData( GetActiveEntry() );
	pListEntries->EnsureSelectedItemVisible();
	UpdateActiveEntry();
}

void lpeViewLangPack::SelectEntryNamed( const char *name ){
	if( ! pLangPack ){
		return;
	}
	
	lpeLangPackEntry *entry = pLangPack->GetEntryList().GetNamed( name );
	if( ! entry && pRefLangPack ){
		entry = pRefLangPack->GetEntryList().GetNamed( name );
	}
	SelectEntry( entry );
}

void lpeViewLangPack::SelectEntry( lpeLangPackEntry *entry ){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	lpes.RemoveAll();
	if( entry ){
		lpes.Add( entry );
	}
	lpes.SetActive( entry );
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

void lpeViewLangPack::SelectNextMissingEntry(){
	if( ! pLangPack || ! pRefLangPack ){
		return;
	}
	
	const igdeIconListBox &list = pListEntries;
	const int startIndex = list.GetSelection() + 1;
	const int count = list.GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		lpeLangPackEntry * const entry = ( lpeLangPackEntry* )
			list.GetItemAt( ( startIndex + i ) % count )->GetData();
		if( entry->GetLangPack() != pLangPack ){
			SelectEntry( entry );
			return;
		}
	}
	
	igdeCommonDialogs::Information( &pWindowMain, "Select next missing entry",
		"No more missing language pack entries" );
}

void lpeViewLangPack::UpdateActiveEntry(){
	lpeLangPackEntry * const entry = GetActiveEntry();
	
	if( entry ){
		UpdateEntry( entry );
		pEditEntryName->SetText( entry->GetName() );
		
		if( entry->GetLangPack() == pLangPack ){
			pEditEntryText->SetText( entry->GetText().ToUTF8() );
			
			const lpeLangPackEntry *refEntry = nullptr;
			if( pRefLangPack ){
				refEntry = pRefLangPack->GetEntryList().GetNamed( entry->GetName() );
			}
			
			pEditRefText->SetText( refEntry ? refEntry->GetText().ToUTF8() : decString() );
			
		}else{
			pEditEntryText->ClearText();
			pEditRefText->SetText( entry->GetText().ToUTF8() );
		}
		
	}else{
		pEditEntryName->ClearText();
		pEditEntryText->ClearText();
		pEditRefText->ClearText();
	}
	
	pEditEntryName->SetEnabled( entry && entry->GetLangPack() == pLangPack );
	pEditEntryText->SetEnabled( entry != nullptr );
	pEditRefText->SetEnabled( entry != nullptr );
}

void lpeViewLangPack::UpdateEntrySelection(){
	if( ! pLangPack ){
		return;
	}
	
	igdeIconListBox &list = pListEntries;
	const int count = list.GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const lpeLangPackEntry &entry = *( ( lpeLangPackEntry* )list.GetItemAt( i )->GetData() );
		if( entry.GetSelected() ){
			list.SelectItem( i );
			
		}else{
			list.DeselectItem( i );
		}
	}
}

void lpeViewLangPack::UpdateEntry( lpeLangPackEntry *entry ){
	DEASSERT_NOTNULL( entry )
	
	const int selection = pListEntries->IndexOfItemWithData( entry );
	if( selection == -1 ){
		return;
	}
	
	igdeListItem &item = *pListEntries->GetItemAt( selection );
	decStringList &details = item.GetDetails();
	
	item.SetText( entry->GetName() );
	
	if( entry->GetLangPack() == pLangPack ){
		details.SetAt( 0, entry->GetText().ToUTF8() );
		
		const lpeLangPackEntry *refEntry = nullptr;
		if( pRefLangPack ){
			refEntry = pRefLangPack->GetEntryList().GetNamed( entry->GetName() );
		}
		
		details.SetAt( 1, refEntry ? refEntry->GetText().ToUTF8().GetString() : "" );
		item.SetIcon( nullptr );
		
	}else{
		details.SetAt( 0, "" );
		details.SetAt( 1, entry->GetText().ToUTF8() );
		item.SetIcon( GetEnvironment().GetStockIcon( igdeEnvironment::esiWarning ) );
	}
	
	pListEntries->ItemChangedAt( selection );
}
