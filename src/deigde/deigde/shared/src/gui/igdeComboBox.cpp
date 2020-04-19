/* 
 * Drag[en]gine IGDE
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
#include <string.h>

#include "igdeComboBox.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeComboBoxListener.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemReference.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeComboBox : public FXComboBox{
	FXDECLARE( cNativeIgdeComboBox )
	
protected:
	cNativeIgdeComboBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComboBox::ID_LAST,
	};
	
private:
	igdeComboBox *pOwner;
	igdeFontReference pFont;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	
public:
	cNativeIgdeComboBox( igdeComboBox &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeComboBox();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChanged( FXObject *sender, FXSelector selector, void *data );
	
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void SyncSelection( bool changing );
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void InsertItem( int index, const igdeListItem &item );
	virtual void Focus();
	virtual void UpdateRowCount();
	
	static int ComboBoxFlags( const igdeComboBox &owner );
	static igdeFont *ComboBoxFont( const igdeComboBox &owner, const igdeGuiTheme &guitheme );
	static int ComboBoxPadLeft( const igdeGuiTheme &guitheme );
	static int ComboBoxPadRight( const igdeGuiTheme &guitheme );
	static int ComboBoxPadTop( const igdeGuiTheme &guitheme );
	static int ComboBoxPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeComboBox ) cNativeIgdeComboBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeComboBox::ID_SELF, cNativeIgdeComboBox::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeComboBox::ID_SELF, cNativeIgdeComboBox::onChanged )
};


FXIMPLEMENT( cNativeIgdeComboBox, FXComboBox, cNativeIgdeComboBoxMap, ARRAYNUMBER( cNativeIgdeComboBoxMap ) )

cNativeIgdeComboBox::cNativeIgdeComboBox(){ }

cNativeIgdeComboBox::cNativeIgdeComboBox( igdeComboBox &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXComboBox( parent, owner.GetColumns(), this, ID_SELF, layoutFlags | ComboBoxFlags( owner ),
	0, 0, 0, 0,
	ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( ComboBoxFont( owner, guitheme ) ),
pOrgBackColor( field->getBackColor() ),
pInvalidBackColor( igdeUIFoxHelper::BlendColor( pOrgBackColor, FXRGB( 255, 0, 0 ), 0.25f ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setEditable( owner.GetEditable() );
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
	
	BuildList();
	setCurrentItem( owner.GetSelection() );
	setText( owner.GetText().GetString() );
	
	UpdateRowCount();
}

cNativeIgdeComboBox::~cNativeIgdeComboBox(){
}

long cNativeIgdeComboBox::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		SyncSelection( false );
		
		// SyncSelection calls SetText which does call NotifyTextChanged but only if the
		// text has not changed. Because onChanged is send right before onCommand the
		// text is already set and SyncSelection will not cause a notification to be send.
		// for this reason we have to do it manually here since onCommand will be only
		// send if user interaction changed the value.
		pOwner->NotifyTextChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeComboBox::onChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		SyncSelection( true );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

void cNativeIgdeComboBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	clearItems();
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pOwner->GetItemAt( i );
		
		appendItem( item.GetText().GetString() );
		
		if( item.GetIcon() ){
			list->setItemIcon( i, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		}
	}
}

void cNativeIgdeComboBox::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	
	list->setItemText( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void cNativeIgdeComboBox::SyncSelection( bool changing ){
	// keep in mind FXComboBox list selection can go out of synch with text. we need to
	// set the text, then update the selection index to match the text and eventually to
	// update the combo box list selection. since setting the combo box list selection
	// trashes the set text we can not use the simple solution. using SetText() is more
	// complex than required but it does work
	pOwner->SetText( getText().text(), changing );
}

void cNativeIgdeComboBox::OnInvalidValueChanged(){
	field->setBackColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void cNativeIgdeComboBox::UpdateText(){
	// documentation states setText() selects the matching list item. this is true if the
	// list item is present but not if it is absent. in this case the old selection is kept
	// causing the text in the combobox and the selected list item to be out of sync.
	// we have to ensure this situation to never happen. the igde combo box widget does set
	// selection to -1 if not matching the text.
	// 
	// unfortunately setCurrentItem() also changes the text although it is not stated in
	// the documentation that this side-effects is present. we thus have to do this in the
	// right order to avoid a total mess to break loose.
	setCurrentItem( pOwner->GetSelection() );
	setText( pOwner->GetText().GetString() );
}

void cNativeIgdeComboBox::InsertItem( int index, const igdeListItem &item ){
	list->insertItem( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void cNativeIgdeComboBox::Focus(){
	field->setFocus();
}

void cNativeIgdeComboBox::UpdateRowCount(){
	const int count = decMath::max( decMath::min( pOwner->GetRows(), getNumItems() ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
}


int cNativeIgdeComboBox::ComboBoxFlags( const igdeComboBox & ){
	return FRAME_SUNKEN | COMBOBOX_NORMAL;
}

igdeFont *cNativeIgdeComboBox::ComboBoxFont( const igdeComboBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::comboBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::comboBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::comboBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::comboBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeIgdeComboBox::ComboBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeComboBox::ComboBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeComboBox::ComboBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeComboBox::ComboBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, DEFAULT_PAD );
}



// Class igdeComboBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBox::igdeComboBox( igdeEnvironment &environment, int columns, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelection( -1 ),
pColumns( columns ),
pRows( 10 ),
pEditable( false ),
pDescription( description ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeComboBox::igdeComboBox( igdeEnvironment &environment, int columns, bool editable,
	const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelection( -1 ),
pColumns( columns ),
pRows( 10 ),
pEditable( editable ),
pDescription( description ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeComboBox::igdeComboBox( igdeEnvironment &environment, int columns, int rows,
	const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelection( -1 ),
pColumns( columns ),
pRows( rows ),
pEditable( false ),
pDescription( description ),
pInvalidValue( false )
{
	if( columns < 1 || rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeComboBox::igdeComboBox( igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelection( -1 ),
pColumns( columns ),
pRows( rows ),
pEditable( editable ),
pDescription( description ),
pInvalidValue( false )
{
	if( columns < 1 || rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeComboBox::~igdeComboBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeComboBox::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeComboBox::SetRows( int rows ){
	if( pRows == rows ){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeComboBox::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeComboBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeComboBox::SetInvalidValue( bool invalidValue ){
	if( pInvalidValue == invalidValue ){
		return;
	}
	
	pInvalidValue = invalidValue;
	OnInvalidValueChanged();
}

void igdeComboBox::Focus(){
	OnRequestFocus();
}



int igdeComboBox::GetItemCount() const{
	return pItems.GetCount();
}

igdeListItem *igdeComboBox::GetItemAt( int index ) const{
	return ( igdeListItem* )pItems.GetAt( index );
}

bool igdeComboBox::HasItem( igdeListItem *item ) const{
	return pItems.Has( item );
}

bool igdeComboBox::HasItem( const char *item ) const{
	return IndexOfItem( item ) != -1;
}

bool igdeComboBox::HasItemWithData( void *data ) const{
	return IndexOfItemWithData( data ) != -1;
}

int igdeComboBox::IndexOfItem( igdeListItem *item ) const{
	return pItems.IndexOf( item );
}

int igdeComboBox::IndexOfItem( const char *item ) const{
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetText() == item ){
			return i;
		}
	}
	
	return -1;
}

int igdeComboBox::IndexOfItemWithData( void *data ) const{
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetData() == data ){
			return i;
		}
	}
	
	return -1;
}

void igdeComboBox::AddItem( igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	pItems.Add( item );
	OnItemAdded( pItems.GetCount() - 1 );
	
	if( ! pEditable && pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

igdeListItem *igdeComboBox::AddItem( const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	item.TakeOver( new igdeListItem( text, icon, data ) );
	AddItem( item );
	return item;
}

void igdeComboBox::InsertItem( int index, igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	pItems.Insert( item, index );
	if( pSelection >= index ){
		pSelection++;
	}
	
	OnItemAdded( index );
	
	if( ! pEditable && pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

igdeListItem *igdeComboBox::InsertItem( int index, const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	item.TakeOver( new igdeListItem( text, icon, data ) );
	InsertItem( index, item );
	return item;
}

void igdeComboBox::MoveItem( int fromIndex, int toIndex ){
	pItems.Move( fromIndex, toIndex );
	
	if( pSelection != -1 ){
		if( fromIndex < toIndex ){
			if( pSelection == fromIndex ){
				pSelection = toIndex;
				
			}else if( pSelection > fromIndex && pSelection <= toIndex ){
				pSelection--;
			}
			
		}else{
			if( pSelection == fromIndex ){
				pSelection = toIndex;
				
			}else if( pSelection >= toIndex && pSelection < fromIndex ){
				pSelection++;
			}
		}
	}
	
	OnItemMoved( fromIndex, toIndex );
}

void igdeComboBox::RemoveItem( int index ){
	pItems.RemoveFrom( index );
	
	bool textChanged = false;
	
	if( pSelection == index ){
		if( pEditable ){
			pSelection = -1;
			
		}else{
			pSelection = decMath::min( pSelection, pItems.GetCount() - 1 );
			if( pSelection != -1 ){
				pText = ( ( igdeListItem* )pItems.GetAt( pSelection ) )->GetText();
				
			}else{
				pText.Empty();
			}
			textChanged = true;
		}
		
	}else if( pSelection > index ){
		pSelection--;
	}
	
	OnItemRemoved( index );
	
	if( textChanged ){
		NotifyTextChanged();
	}
}

void igdeComboBox::RemoveAllItems(){
	if( pItems.GetCount() == 0 ){
		return;
	}
	
	pItems.RemoveAll();
	pSelection = -1;
	if( ! pEditable ){
		pText.Empty();
	}
	
	OnAllItemsRemoved();
	
	if( ! pEditable ){
		NotifyTextChanged();
	}
}

void igdeComboBox::ItemChangedAt( int index ){
	if( index < 0 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	OnItemChanged( index );
}



void igdeComboBox::SetSorter( igdeListItemSorter *sorter ){
	pSorter = sorter;
}

void igdeComboBox::SetDefaultSorter(){
	pSorter.TakeOver( new igdeListItemSorter );
}

static void igdeComboBox_Sort( decObjectList &items, igdeListItemSorter &sorter, int left, int right ){
	igdeListItemReference pivot( ( igdeListItem* )items.GetAt( left ) );
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && sorter.Precedes( pivot, *( ( igdeListItem* )items.GetAt( right ) ) ) ){
			right--;
		}
		if( left != right ){
			items.SetAt( left, items.GetAt( right ) );
			left++;
		}
		while( left < right && sorter.Precedes( *( ( igdeListItem* )items.GetAt( left ) ), pivot ) ){
			left++;
		}
		if( left != right ){
			items.SetAt( right, items.GetAt( left ) );
			right--;
		}
	}
	
	items.SetAt( left, ( igdeListItem* )pivot );
	if( l_hold < left ){
		igdeComboBox_Sort( items, sorter, l_hold, left - 1 );
	}
	if( r_hold > left ){
		igdeComboBox_Sort( items, sorter, left + 1, r_hold );
	}
}

void igdeComboBox::SortItems(){
	if( ! pSorter ){
		return;
	}
	
	const int count = pItems.GetCount();
	if( count < 2 ){
		return;
	}
	
	deObject *selection = NULL;
	if( pSelection != -1 ){
		selection = pItems.GetAt( pSelection );
	}
	
	igdeComboBox_Sort( pItems, pSorter, 0, count - 1 );
	
	if( selection ){
		pSelection = pItems.IndexOf( selection );
	}
	
	OnItemsSorted();
}



igdeListItem *igdeComboBox::GetSelectedItem() const{
	if( pSelection != -1 ){
		return ( igdeListItem* )pItems.GetAt( pSelection );
		
	}else{
		return NULL;
	}
}

void igdeComboBox::SetSelection( int selection ){
	if( selection < -1 || selection >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( selection == pSelection ){
		return;
	}
	
	pSelection = selection;
	
	if( selection != -1 ){
		pText = ( ( igdeListItem* )pItems.GetAt( selection ) )->GetText();
		OnTextChanged();
		NotifyTextChanged();
		
	}else if( ! pEditable ){
		pText.Empty();
		OnTextChanged();
		NotifyTextChanged();
	}
}

void igdeComboBox::SetSelectionWithData( void *data ){
	SetSelection( IndexOfItemWithData( data ) );
}



void igdeComboBox::SetText( const char *text, bool changing ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	pSelection = IndexOfItem( text );
	
	OnTextChanged();
	
	if( changing ){
		NotifyTextChanging();
		
	}else{
		NotifyTextChanged();
	}
}

void igdeComboBox::ClearText(){
	SetText( "", false );
}



void igdeComboBox::AddListener( igdeComboBoxListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeComboBox::RemoveListener( igdeComboBoxListener *listener ){
	pListeners.Remove( listener );
}

void igdeComboBox::NotifyTextChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeComboBoxListener* )listeners.GetAt( i ) )->OnTextChanged( this );
	}
}

void igdeComboBox::NotifyTextChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeComboBoxListener* )listeners.GetAt( i ) )->OnTextChanging( this );
	}
}



void igdeComboBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeIgdeComboBox * const foxWidget = new cNativeIgdeComboBox(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeComboBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeComboBox* )GetNativeWidget();
	DropNativeWidget();
}



void igdeComboBox::OnItemAdded( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	const igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	native.InsertItem( index, item );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnItemRemoved( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	native.removeItem( index );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnAllItemsRemoved(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	native.clearItems();
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnItemChanged( int index ){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->UpdateItem( index );
	}
}

void igdeComboBox::OnItemMoved( int fromIndex, int toIndex ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	native.moveItem( toIndex, fromIndex );
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBox::OnItemsSorted(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	native.BuildList();
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBox::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->UpdateText();
	}
}

void igdeComboBox::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
	
	if( pEnabled ){
		native.enable();
		
	}else{
		native.disable();
	}
}

void igdeComboBox::OnRowsChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->UpdateRowCount();
	}
}

void igdeComboBox::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->setEditable( pEditable );
	}
}

void igdeComboBox::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		cNativeIgdeComboBox &native = *( ( cNativeIgdeComboBox* )GetNativeWidget() );
		native.setTipText( pDescription.GetString() );
		native.setHelpText( pDescription.GetString() );
	}
}

void igdeComboBox::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}

void igdeComboBox::OnRequestFocus(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBox* )GetNativeWidget() )->Focus();
	}
}
