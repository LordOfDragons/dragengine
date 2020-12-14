/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPPropertyList.h"
#include "../../clipboard/meCDProperties.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/composed/igdeEditPropertyValueListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



static const decString vEmptyString;

// Actions
////////////

namespace{

class cComboKey : public igdeComboBoxListener {
	meWPPropertyList &pPanel;
	
public:
	cComboKey( meWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		pPanel.UpdateProperty();
		pPanel.UpdateInformation( comboBox->GetText() );
		pPanel.OnKeyChanged( comboBox->GetText() );
	}
};


class cListProperties : public igdeIconListBoxListener {
	meWPPropertyList &pPanel;
	
public:
	cListProperties( meWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeIconListBox *listBox ){
		pPanel.UpdateProperty();
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if( selection ){
			pPanel.OnPropertySelected( selection->GetText(), selection->GetDetails().GetAt( 0 ) );
			pPanel.SelectKey( selection->GetText() );
			
		}else{
			pPanel.OnPropertySelected( vEmptyString, vEmptyString );
		}
	}
	
	virtual void OnDoubleClickItem( igdeIconListBox*, int ){
		pPanel.EditPropertyValueInDialog();
	}
	
	virtual void AddContextMenuEntries( igdeIconListBox *listBox, igdeMenuCascade &menu ){
		igdeUIHelper &helper = listBox->GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, pPanel.GetActionPropertyAdd() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyRemove() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyClear() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyRename() );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, pPanel.GetActionPropertyCopy() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyCopyAll() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyCut() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyCutAll() );
		helper.MenuCommand( menu, pPanel.GetActionPropertyPaste() );
		
		pPanel.AddContextMenuEntries( helper, menu );
	}
};


class cActionPropertyAdd : public igdeAction {
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyAdd( meWPPropertyList &panel ) :
	igdeAction( "Add", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		const decString key( pPanel.GetKey() );
		if( ! pPanel.GetUndoSystem() || key.IsEmpty() || pPanel.GetProperties().Has( key ) ){
			return;
		}
		
		const decString value( pPanel.GetGDDefaultValue( key ) );
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoAddProperty( key, value ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
			pPanel.SelectProperty( key );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetUndoSystem() && ! pPanel.GetKey().IsEmpty()
			&& ! pPanel.GetProperties().Has( pPanel.GetKey() ) );
	}
};

class cActionPropertyRemove : public igdeAction {
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyRemove( meWPPropertyList &panel ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove selected property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoRemoveProperty( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetProperty().IsEmpty() && pPanel.GetUndoSystem() );
	}
};

class cActionPropertyClear : public igdeAction {
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyClear( meWPPropertyList &panel ) :
	igdeAction( "Clear", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove all properties" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		if( pPanel.GetProperties().GetCount() == 0 || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSetProperties( decStringDictionary() ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperties().GetCount() > 0  && pPanel.GetUndoSystem());
	}
};

class cActionPropertyRename : public igdeAction {
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyRename( meWPPropertyList &panel ) :
	igdeAction( "Rename...", NULL, "Rename property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decString name( property );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Property", "Name:", name ) || name == property ){
			return;
		}
		
		if( pPanel.GetProperties().Has( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Property", "Property exists already" );
			return;
		}
		
		decStringDictionary properties( pPanel.GetProperties() );
		properties.SetAt( name, properties.GetAt( property ) );
		properties.Remove( property );
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSetProperties( properties ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
			pPanel.SelectProperty( name );
		}
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetProperty().IsEmpty() && pPanel.GetUndoSystem() );
	}
};

class cActionPropertyCopy : public igdeAction {
protected:
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyCopy( meWPPropertyList &panel ) :
	igdeAction( "Copy", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy selected property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetClipboard() ){
			return;
		}
		
		decStringDictionary properties;
		properties.SetAt( property, pPanel.GetPropertyValue() );
		igdeClipboardDataReference clip;
		clip.TakeOver( new meCDProperties( properties ) );
		pPanel.GetClipboard()->Set( clip );
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetProperty().IsEmpty() && pPanel.GetClipboard() );
	}
};

class cActionPropertyCopyAll : public igdeAction {
protected:
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyCopyAll( meWPPropertyList &panel ) :
	igdeAction( "Copy All", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy all properties" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		if( pPanel.GetProperties().GetCount() == 0 || ! pPanel.GetClipboard() ){
			return;
		}
		
		igdeClipboardDataReference clip;
		clip.TakeOver( new meCDProperties( pPanel.GetProperties() ) );
		pPanel.GetClipboard()->Set( clip );
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetProperty().IsEmpty() && pPanel.GetClipboard() );
	}
};

class cActionPropertyCut : public cActionPropertyCopy {
public:
	cActionPropertyCut( meWPPropertyList &panel ) : cActionPropertyCopy( panel ){
		SetText( "Cut" );
		SetIcon( panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ) );
		SetDescription( "Cut selected property" );
	}
	
	virtual void OnAction(){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetUndoSystem() || ! pPanel.GetClipboard() ){
			return;
		}
		
		cActionPropertyCopy::OnAction();
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoRemoveProperty( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetProperty().IsEmpty() && pPanel.GetUndoSystem() && pPanel.GetClipboard() );
	}
};

class cActionPropertyCutAll : public cActionPropertyCopyAll {
public:
	cActionPropertyCutAll( meWPPropertyList &panel ) : cActionPropertyCopyAll( panel ){
		SetText( "Cut All" );
		SetIcon( panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ) );
		SetDescription( "Cut all properties" );
	}
	
	virtual void OnAction(){
		if( pPanel.GetProperties().GetCount() == 0 || ! pPanel.GetUndoSystem() || ! pPanel.GetClipboard() ){
			return;
		}
		
		cActionPropertyCopyAll::OnAction();
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSetProperties( decStringDictionary() ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperties().GetCount() > 0 && pPanel.GetUndoSystem() && pPanel.GetClipboard() );
	}
};

class cActionPropertyPaste : public igdeAction {
	meWPPropertyList &pPanel;
	
public:
	cActionPropertyPaste( meWPPropertyList &panel ) :
	igdeAction( "Paste", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetClipboard() ){
			return;
		}
		
		const meCDProperties * const clip = ( const meCDProperties * )
			pPanel.GetClipboard()->GetWithTypeName( meCDProperties::TYPE_NAME );
		if( ! clip || clip->GetProperties().GetCount() == 0 ){
			return;
		}
		
		decStringDictionary properties( pPanel.GetProperties() );
		properties += clip->GetProperties();
		if( pPanel.GetProperties() == properties ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoSetProperties( properties ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetClipboard() && pPanel.GetClipboard()->HasWithTypeName( meCDProperties::TYPE_NAME ) );
	}
};


class cEditPropertyValue : public igdeEditPropertyValueListener {
protected:
	meWPPropertyList &pPanel;
	igdeUndoReference pUndo;
	decString pOldValue;
	
public:
	cEditPropertyValue( meWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnPropertyValueChanged( igdeEditPropertyValue* ){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		const decString newValue( pPanel.GetEditPropertyValue() );
		if( ! pUndo ){
			pOldValue = pPanel.GetPropertyValue();
			if( newValue == pOldValue ){
				return;
			}
		}
		pUndo.TakeOver( pPanel.UndoSetProperty( property, pOldValue, newValue ) );
		if( pUndo ){
			pPanel.GetUndoSystem()->Add( pUndo );
			pUndo = NULL;
		}
	}
	
	virtual void OnPropertyValueChanging( igdeEditPropertyValue* ){
		const decString property( pPanel.GetProperty() );
		if( property.IsEmpty() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		const decString newValue( pPanel.GetEditPropertyValue() );
		if( ! pUndo ){
			pOldValue = pPanel.GetPropertyValue();
			if( newValue == pOldValue ){
				return;
			}
		}
		pUndo.TakeOver( pPanel.UndoSetProperty( property, pOldValue, newValue ) );
		if( pUndo ){
			pUndo->Redo();
		}
	}
};

}



// Class meWPPropertyList
////////////////////////////

// Constructor, destructor
////////////////////////////

meWPPropertyList::meWPPropertyList( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pUndoSystem( NULL ),
pClipboard( NULL ),
pEnabled( true )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference frameLine;
	
	
	pIconUnknownKey = env.GetStockIcon( igdeEnvironment::esiSmallWarning );
	pIconInvalidValue = env.GetStockIcon( igdeEnvironment::esiSmallWarning );
	
	
	pActionPropertyAdd.TakeOver( new cActionPropertyAdd( *this ) );
	pActionPropertyRemove.TakeOver( new cActionPropertyRemove( *this ) );
	pActionPropertyClear.TakeOver( new cActionPropertyClear( *this ) );
	pActionPropertyCopy.TakeOver( new cActionPropertyCopy( *this ) );
	pActionPropertyCopyAll.TakeOver( new cActionPropertyCopyAll( *this ) );
	pActionPropertyCut.TakeOver( new cActionPropertyCut( *this ) );
	pActionPropertyCutAll.TakeOver( new cActionPropertyCutAll( *this ) );
	pActionPropertyPaste.TakeOver( new cActionPropertyPaste( *this ) );
	pActionPropertyRename.TakeOver( new cActionPropertyRename( *this ) );
	
	
	helper.FormLineStretchFirst( *this, "Property:", "Property to add", frameLine );
	helper.ComboBox( frameLine, true, "Property to add", pCBKeys, new cComboKey( *this ) );
	pCBKeys->SetDefaultSorter();
	helper.Button( frameLine, pBtnKeyAdd, pActionPropertyAdd );
	
	const igdeUIHelper::sColumnHeader headers[] = { { "Key", NULL, 200 }, { "Value", NULL, 200 } };
	helper.IconListBox( *this, pListProperties, decPoint( 100, 150 ), headers, 2, "Properties",
		new cListProperties( *this ) );
	pListProperties->SetDefaultSorter();
	
	helper.EditPropertyValue( *this, pEditPropertyValue, new cEditPropertyValue( *this ) );
	
	helper.EditString( *this, "Information", pDisplayInfo, 5, NULL );
	pDisplayInfo->SetEditable( false );
}

meWPPropertyList::~meWPPropertyList(){
}



// Management
///////////////

void meWPPropertyList::SetUndoSystem( igdeUndoSystem *undoSystem ){
	pUndoSystem = undoSystem;
}

void meWPPropertyList::SetClipboard( igdeClipboard *clipboard ){
	pClipboard = clipboard;
}

void meWPPropertyList::SetProperties( const decStringDictionary &properties ){
	if( properties == pProperties ){
		return;
	}
	
	pProperties = properties;
	UpdateList();
}

void meWPPropertyList::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	UpdateEnabled();
}



const decString &meWPPropertyList::GetKey() const{
	return pCBKeys->GetText();
}

const decString &meWPPropertyList::GetProperty() const{
	const igdeListItem * const selection = pListProperties->GetSelectedItem();
	return selection ? selection->GetText() : vEmptyString;
}

const decString &meWPPropertyList::GetPropertyValue() const{
	const igdeListItem * const selection = pListProperties->GetSelectedItem();
	return selection ? pProperties.GetAt( selection->GetText(), vEmptyString ) : vEmptyString;
}

const decString &meWPPropertyList::GetEditPropertyValue() const{
	return pListProperties->GetSelectedItem() ? pEditPropertyValue->GetValue() : vEmptyString;
}

void meWPPropertyList::SetEditPropertyValue( const char *value ) const{
	const igdeListItem * const selection = pListProperties->GetSelectedItem();
	if( selection ){
		pEditPropertyValue->SetValue( value, pEditPropertyValue->GetGDProperty() );
	}
}



void meWPPropertyList::UpdateKeys(){
	const decStringSet keys( GetGDPropertyKeys() );
	const decString selection( pCBKeys->GetText() );
	
	pCBKeys->RemoveAllItems();
	
	const int count = keys.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pCBKeys->AddItem( keys.GetAt( i ) );
	}
	
	pCBKeys->SortItems();
	pCBKeys->SetText( selection );
}

void meWPPropertyList::UpdateList(){
	const decString selection( GetProperty() );
	
	const decStringList keys( pProperties.GetKeys() );
	const int count = keys.GetCount();
	decStringList details;
	int i;
	
	// remove no more existing items
	for( i=pListProperties->GetItemCount() - 1; i>=0; i-- ){
		if( ! keys.Has( pListProperties->GetItemAt( i )->GetText() ) ){
			pListProperties->RemoveItem( i );
		}
	}
	
	// add not existing properties and update existing ones
	for( i=0; i<count; i++ ){
		const decString &key = keys.GetAt( i );
		const int index = pListProperties->IndexOfItem( key );
		
		details.RemoveAll();
		details.Add( pProperties.GetAt( key ) );
		
		if( index == -1 ){
			pListProperties->AddItem( key, details );
			
		}else{
			igdeListItem &item = *pListProperties->GetItemAt( index );
			if( item.GetDetails() != details ){
				item.GetDetails() = details;
				pListProperties->ItemChangedAt( index );
			}
		}
	}
	
	pListProperties->SortItems();
	VerifyProperties();
	
	SelectProperty( selection );
	
	if( pListProperties->GetSelectedItem() ){
		UpdateProperty(); // required if selection is the same but value changed
		
	}else if( pListProperties->GetItemCount() > 0 ){
		pListProperties->SetSelection( 0 );
	}
}

void meWPPropertyList::UpdateProperty(){
	const igdeListItem * const selection = pListProperties->GetSelectedItem();
	
	if( selection ){
		pEditPropertyValue->SetValue( selection->GetDetails().GetAt( 0 ), GetGDProperty( selection->GetText() ) );
		pEditPropertyValue->SetEnabled( pEnabled );
		
	}else{
		pEditPropertyValue->ClearValue();
		pEditPropertyValue->SetEnabled( false );
	}
}

void meWPPropertyList::UpdateEnabled(){
	pCBKeys->SetEnabled( pEnabled );
	pListProperties->SetEnabled( pEnabled );
	pEditPropertyValue->SetEnabled( pEnabled && pListProperties->GetSelectedItem() );
	
	pActionPropertyAdd->Update();
}

void meWPPropertyList::SelectKey( const char *key ){
	pCBKeys->SetText( key );
}

void meWPPropertyList::SelectProperty( const char *property ){
	pListProperties->SetSelection( pListProperties->IndexOfItem( property ) );
}

void meWPPropertyList::EditPropertyValueInDialog(){
	const igdeListItem * const selection = pListProperties->GetSelectedItem();
	if( ! selection || ! pUndoSystem ){
		return;
	}
	
	const decString oldValue( selection->GetDetails().GetAt( 0 ) );
	decString newValue( oldValue );
	
	if( ! igdeCommonDialogs::GetMultilineString( this, "Edit Raw Property Value",
		"Raw property value. Values entered here can violate the\n"
		"property type so be careful what you enter", newValue ) ){
			return;
	}
	
	if( newValue == oldValue ){
		return;
	}
	
	igdeUndoReference undo;
	undo.TakeOver( UndoSetProperty( selection->GetText(), oldValue, newValue ) );
	if( undo ){
		pUndoSystem->Add( undo );
	}
}

void meWPPropertyList::VerifyProperties(){
	const int count = pListProperties->GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *pListProperties->GetItemAt( i );
		const igdeGDProperty * const gdProperty = GetGDProperty( item.GetText() );
		igdeIcon *icon = NULL;
		
		if( gdProperty ){
			// TODO verify if the value is correct according to property definition
			/*if( correct ){
				
			}else{
				icon = pIconInvalidValue;
			}*/
			
		}else{
			icon = pIconUnknownKey;
		}
		
		if( icon != item.GetIcon() ){
			item.SetIcon( icon );
			pListProperties->ItemChangedAt( i );
		}
	}
}

void meWPPropertyList::UpdateInformation( const char *key ){
	const igdeGDProperty * const gdProperty = GetGDProperty( key );
	const decString defaultValue( GetGDDefaultValue( key ) );
	decString text;
	
	if( gdProperty ){
		text.Format( "Property: %s\nDefault: '%s'\n\n%s", key, defaultValue.GetString(),
			gdProperty->GetDescription().GetString() );
		
	}else{
		text.Format( "Property: %s\nDefault: '%s'", key, defaultValue.GetString() );
	}
	
	pDisplayInfo->SetText( text );
}

void meWPPropertyList::SetIdentifiers( const decStringSet &identifiers ){
	pEditPropertyValue->SetIdentifiers( identifiers );
}

void meWPPropertyList::SetTriggerTargetList( igdeTriggerTargetList *list ){
	pEditPropertyValue->SetTriggerTargets( list );
}



void meWPPropertyList::AddContextMenuEntries( igdeUIHelper&, igdeMenuCascade& ){
}

void meWPPropertyList::OnKeyChanged( const decString & ){
}

void meWPPropertyList::OnPropertySelected( const decString &, const decString & ){
}
