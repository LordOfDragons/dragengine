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

#include "igdeComboBoxFilter.h"
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

class cNativeIgdeComboBoxFilter : public FXComboBox{
	FXDECLARE( cNativeIgdeComboBoxFilter )
	
protected:
	cNativeIgdeComboBoxFilter();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComboBox::ID_LAST,
		ID_EDIT_FILTER
	};
	
private:
	igdeComboBoxFilter *pOwner;
	igdeFontReference pFont;
	FXLabel *pLabelFilter;
	FXTextField *pEditFilter;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	
public:
	cNativeIgdeComboBoxFilter( igdeComboBoxFilter &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeComboBoxFilter();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChanged( FXObject *sender, FXSelector selector, void *data );
	long onEditFilterChanged( FXObject *sender, FXSelector selector, void *data );
	
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void UpdateFilterString();
	virtual void SyncSelection( bool changing );
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void InsertItem( int index, const igdeListItem &item );
	virtual void Focus();
	virtual void UpdateRowCount();
	
	static int ComboBoxFlags( const igdeComboBoxFilter &owner );
	static igdeFont *ComboBoxFont( const igdeComboBoxFilter &owner, const igdeGuiTheme &guitheme );
	static int ComboBoxPadLeft( const igdeGuiTheme &guitheme );
	static int ComboBoxPadRight( const igdeGuiTheme &guitheme );
	static int ComboBoxPadTop( const igdeGuiTheme &guitheme );
	static int ComboBoxPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeComboBoxFilter ) cNativeIgdeComboBoxFilterMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeComboBoxFilter::ID_SELF, cNativeIgdeComboBoxFilter::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeComboBoxFilter::ID_SELF, cNativeIgdeComboBoxFilter::onChanged ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeComboBoxFilter::ID_EDIT_FILTER, cNativeIgdeComboBoxFilter::onEditFilterChanged )
};


FXIMPLEMENT( cNativeIgdeComboBoxFilter, FXComboBox, cNativeIgdeComboBoxFilterMap, ARRAYNUMBER( cNativeIgdeComboBoxFilterMap ) )

cNativeIgdeComboBoxFilter::cNativeIgdeComboBoxFilter(){ }

cNativeIgdeComboBoxFilter::cNativeIgdeComboBoxFilter( igdeComboBoxFilter &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXComboBox( parent, owner.GetColumns(), this, ID_SELF, layoutFlags | ComboBoxFlags( owner ),
	0, 0, 0, 0,
	ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( ComboBoxFont( owner, guitheme ) ),
pLabelFilter( NULL ),
pEditFilter( NULL ),
pOrgBackColor( field->getBackColor() ),
pInvalidBackColor( igdeUIFoxHelper::BlendColor( pOrgBackColor, FXRGB( 255, 0, 0 ), 0.25f ) )
{
	FXHorizontalFrame * const lineFilter = new FXHorizontalFrame(
		pane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0 );
	pLabelFilter = new FXLabel( lineFilter, "Filter:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, 2 );
	pLabelFilter->setJustify( JUSTIFY_LEFT | JUSTIFY_CENTER_Y );
	pEditFilter = new FXTextField( lineFilter, 5, this, ID_EDIT_FILTER, FRAME_SUNKEN | LAYOUT_FILL_X );
	
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

cNativeIgdeComboBoxFilter::~cNativeIgdeComboBoxFilter(){
}

long cNativeIgdeComboBoxFilter::onCommand( FXObject*, FXSelector, void* ){
	// SEL_COMMAND will be send by FXComboBox if either the user clicked on a list box
	// item or if he finished entering text into the text field. Since we do not know
	// which event caused SEL_COMMAND to be fired we need to deal with the same problem
	// we are facing in UpdateText(). in particular we need to fix the list box selection
	// to not go out of sync with the text
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

long cNativeIgdeComboBoxFilter::onChanged( FXObject*, FXSelector, void* ){
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

long cNativeIgdeComboBoxFilter::onEditFilterChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetFilterString( pEditFilter->getText().text() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

void cNativeIgdeComboBoxFilter::BuildList(){
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

void cNativeIgdeComboBoxFilter::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	
	list->setItemText( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void cNativeIgdeComboBoxFilter::SyncSelection( bool changing ){
	// keep in mind FXComboBox list selection can go out of synch with text. we need to
	// set the text, then update the selection index to match the text and eventually to
	// update the combo box list selection. since setting the combo box list selection
	// trashes the set text we can not use the simple solution. using SetText() is more
	// complex than required but it does work
	pOwner->SetText( getText().text(), changing );
}

void cNativeIgdeComboBoxFilter::OnInvalidValueChanged(){
	field->setBackColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void cNativeIgdeComboBoxFilter::UpdateText(){
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

void cNativeIgdeComboBoxFilter::InsertItem( int index, const igdeListItem &item ){
	list->insertItem( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void cNativeIgdeComboBoxFilter::Focus(){
	field->setFocus();
}

void cNativeIgdeComboBoxFilter::UpdateRowCount(){
	const int count = decMath::max( decMath::min( pOwner->GetRows(),
		decMath::max( pOwner->GetItemCount(), pOwner->GetFilterItemCount() ) ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
}

void cNativeIgdeComboBoxFilter::UpdateFilterString(){
	pEditFilter->setText( pOwner->GetFilterString().GetString() );
}

int cNativeIgdeComboBoxFilter::ComboBoxFlags( const igdeComboBoxFilter & ){
	return FRAME_SUNKEN | COMBOBOX_NORMAL;
}

igdeFont *cNativeIgdeComboBoxFilter::ComboBoxFont( const igdeComboBoxFilter &owner, const igdeGuiTheme &guitheme ){
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

int cNativeIgdeComboBoxFilter::ComboBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeComboBoxFilter::ComboBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeComboBoxFilter::ComboBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeComboBoxFilter::ComboBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, DEFAULT_PAD );
}



// Class igdeComboBoxFilter
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns,
	const char *description ) :
igdeComboBox( environment, columns, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, bool editable,
	const char *description ) :
igdeComboBox( environment, columns, editable, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows,
	const char *description ) :
igdeComboBox( environment, columns, rows, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description ) :
igdeComboBox( environment, columns, rows, editable, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::~igdeComboBoxFilter(){
}



// Management
///////////////

void igdeComboBoxFilter::StoreFilterItems(){
	const int count = GetItemCount();
	int i;
	
	pFilterItems.RemoveAll();
	
	for( i=0; i<count; i++ ){
		pFilterItems.Add( GetItemAt( i ) );
	}
	
	FilterItems();
}

void igdeComboBoxFilter::ReleaseFilterItems(){
	pFilterItems.RemoveAll();
}

void igdeComboBoxFilter::FilterItems(){
	const bool noFilter = pFilterString.IsEmpty();
	const int count = pFilterItems.GetCount();
	const decString text( GetText() );
	int i;
	
	RemoveAllItems();
	
	if( pFilterCaseInsensitive ){
		const decString filter( pFilterString.GetLower() );
		
		for( i=0; i<count; i++ ){
			igdeListItem * const item = ( igdeListItem* )pFilterItems.GetAt( i );
			
			if( noFilter || item->GetText().GetLower().FindString( filter ) != -1 ){
				AddItem( item );
			}
		}
		
	}else{
		for( i=0; i<count; i++ ){
			igdeListItem * const item = ( igdeListItem* )pFilterItems.GetAt( i );
			
			if( noFilter || item->GetText().FindString( pFilterString ) != -1 ){
				AddItem( item );
			}
		}
	}
	
	SetText( text );
}

void igdeComboBoxFilter::SetFilterCaseInsentive( bool caseInsensitive ){
	if( caseInsensitive == pFilterCaseInsensitive ){
		return;
	}
	
	pFilterCaseInsensitive = caseInsensitive;
	FilterItems();
}

void igdeComboBoxFilter::SetFilterString( const char *filterString ){
	if( pFilterString == filterString ){
		return;
	}
	
	pFilterString = filterString;
	OnFilterStringChanged();
	
	FilterItems();
	
	// if the current selection is filtered out use the first one if available
	if( GetSelection() == -1 && GetItemCount() > 0 && ! GetEditable() ){
		SetSelection( 0 );
	}
}

int igdeComboBoxFilter::GetFilterItemCount() const{
	return pFilterItems.GetCount();
}

igdeListItem *igdeComboBoxFilter::GetFilterItemAt( int index ) const{
	return ( igdeListItem* )pFilterItems.GetAt( index );
}



void igdeComboBoxFilter::CreateNativeWidget(){
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
	cNativeIgdeComboBoxFilter * const foxWidget = new cNativeIgdeComboBoxFilter(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeComboBoxFilter::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeComboBoxFilter* )GetNativeWidget();
	DropNativeWidget();
}



void igdeComboBoxFilter::OnItemAdded( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	native.InsertItem( index, *GetItemAt( index ) );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnItemRemoved( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	native.removeItem( index );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnAllItemsRemoved(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	native.clearItems();
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnItemChanged( int index ){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->UpdateItem( index );
	}
}

void igdeComboBoxFilter::OnItemMoved( int fromIndex, int toIndex ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	native.moveItem( toIndex, fromIndex );
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBoxFilter::OnItemsSorted(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	native.BuildList();
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBoxFilter::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->UpdateText();
	}
}

void igdeComboBoxFilter::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
	
	if( GetEnabled() ){
		native.enable();
		
	}else{
		native.disable();
	}
}

void igdeComboBoxFilter::OnRowsChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->UpdateRowCount();
	}
}

void igdeComboBoxFilter::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->setEditable( GetEditable() );
	}
}

void igdeComboBoxFilter::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		cNativeIgdeComboBoxFilter &native = *( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() );
		native.setTipText( GetDescription().GetString() );
		native.setHelpText( GetDescription().GetString() );
	}
}

void igdeComboBoxFilter::OnFilterStringChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->UpdateFilterString();
	}
}

void igdeComboBoxFilter::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}

void igdeComboBoxFilter::OnRequestFocus(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeComboBoxFilter* )GetNativeWidget() )->Focus();
	}
}
