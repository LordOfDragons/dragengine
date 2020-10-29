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

#include "igdeDialogBrowser.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeButton.h"
#include "../igdeContainerReference.h"
#include "../igdeComboBox.h"
#include "../igdeGroupBox.h"
#include "../igdeIconListBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeTreeList.h"
#include "../igdeSwitcher.h"
#include "../event/igdeComboBoxListener.h"
#include "../event/igdeIconListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../event/igdeTreeListListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionContextMenu.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerSplitted.h"
#include "../layout/igdeContainerSplittedReference.h"
#include "../menu/igdeMenuCascade.h"
#include "../model/igdeListItem.h"
#include "../model/igdeTreeItem.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeIconReference.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeDialogBrowser_TreeCategories : public igdeTreeListListener{
	igdeDialogBrowser &pDialog;
	
public:
	igdeDialogBrowser_TreeCategories( igdeDialogBrowser &dialog ) : pDialog( dialog ){}
	
	virtual void OnSelectionChanged( igdeTreeList* ){
		pDialog.UpdateItemList();
	}
};


class igdeDialogBrowser_TextFilter : public igdeTextFieldListener{
	igdeDialogBrowser &pDialog;
	
public:
	igdeDialogBrowser_TextFilter( igdeDialogBrowser &dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeTextField* ){
		pDialog.UpdateItemList();
	}
	
	virtual void OnTextChanging( igdeTextField* ){
		pDialog.UpdateItemList();
	}
};


class igdeDialogBrowser_ActionPreviewSize : public igdeAction{
	igdeDialogBrowser &pDialog;
	const igdeDialogBrowser::ePreviewSize pSize;
	
public:
	igdeDialogBrowser_ActionPreviewSize( igdeDialogBrowser &dialog, igdeDialogBrowser::ePreviewSize size,
	const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pDialog( dialog ), pSize( size ){ }
	
	virtual void OnAction(){
		pDialog.SetPreviewSize( pSize );
	}
	
	virtual void Update(){
		SetSelected( pDialog.GetPreviewSize() == pSize );
	}
};

class igdeDialogBrowser_ActionViewMode : public igdeAction{
	igdeDialogBrowser &pDialog;
	const igdeDialogBrowser::eViewModes pMode;
	
public:
	igdeDialogBrowser_ActionViewMode( igdeDialogBrowser &dialog, igdeDialogBrowser::eViewModes mode,
	const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pDialog( dialog ), pMode( mode ){ }
	
	virtual void OnAction(){
		pDialog.SetViewMode( pMode );
	}
	
	virtual void Update(){
		SetSelected( pDialog.GetViewMode() == pMode );
	}
};

class igdeDialogBrowser_ActionPIRebuild : public igdeAction{
	igdeDialogBrowser &pDialog;
	
public:
	igdeDialogBrowser_ActionPIRebuild( igdeDialogBrowser &dialog ) : igdeAction(
		"Rebuild Preview", NULL, "Rebuild Preview" ), pDialog( dialog ){ }
	
	virtual void OnAction(){
		pDialog.RebuildPISelectedItem();
	}
	
	virtual void Update(){
		SetEnabled( pDialog.GetSelectedListItem() );
	}
};

class igdeDialogBrowser_ListItems : public igdeIconListBoxListener{
	igdeDialogBrowser &pDialog;
	
public:
	igdeDialogBrowser_ListItems( igdeDialogBrowser &dialog ) : pDialog( dialog ){}
	
	virtual void OnSelectionChanged( igdeIconListBox* ){
		pDialog.CurrentItemChanged();
	}
	
	virtual void OnDoubleClickItem( igdeIconListBox*, int ){
		pDialog.Accept();
	}
	
	virtual void AddContextMenuEntries( igdeIconListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuOption( menu, pDialog.GetActionPISizeLarge() );
		helper.MenuOption( menu, pDialog.GetActionPISizeMedium() );
		helper.MenuOption( menu, pDialog.GetActionPISizeSmall() );
		
		helper.MenuSeparator( menu );
		helper.MenuOption( menu, pDialog.GetActionPIViewPreview() );
		helper.MenuOption( menu, pDialog.GetActionPIViewList() );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new igdeDialogBrowser_ActionPIRebuild( pDialog ), true );
	}
};




// Class igdeDialogBrowser
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowser::igdeDialogBrowser( igdeEnvironment &environment, const char *title, bool canResize ) :
igdeDialog( environment, title, NULL, canResize )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	SetInitialSize( decPoint( 1000, 600 ) );
	
	pActionPISizeSmall.TakeOver( new igdeDialogBrowser_ActionPreviewSize( *this, epsSmall,
		"Small Preview Image", NULL, "Small Preview Image" ) );
	pActionPISizeMedium.TakeOver( new igdeDialogBrowser_ActionPreviewSize( *this, epsMedium,
		"Medium Preview Image", NULL, "Medium Preview Image" ) );
	pActionPISizeLarge.TakeOver( new igdeDialogBrowser_ActionPreviewSize( *this, epsLarge,
		"Large Preview Image", NULL, "Large Preview Image" ) );
	
	pActionPIViewList.TakeOver( new igdeDialogBrowser_ActionViewMode( *this, evmList,
		"List Mode", NULL, "List Mode" ) );
	pActionPIViewPreview.TakeOver( new igdeDialogBrowser_ActionViewMode( *this, evmPreview,
		"Preview Mode", NULL, "Preview Mode" ) );
	
	
	igdeContainerSplittedReference content;
	content.TakeOver( new igdeContainerSplitted( environment, igdeContainerSplitted::espLeft, 250 ) );
	
	// left side: category list with filter
	igdeContainerReference panelCategory;
	panelCategory.TakeOver( new igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 3 ) );
	
	igdeContainerReference filterLine;
	filterLine.TakeOver( new igdeContainerForm( environment ) );
	helper.EditString( filterLine, "Filter:", "Show items containing filter case insensitive",
		pEditFilter, new igdeDialogBrowser_TextFilter( *this ) );
	panelCategory->AddChild( filterLine );
	
	helper.TreeList( panelCategory, pTreeCategories, 10, "Categories", new igdeDialogBrowser_TreeCategories( *this ) );
	pTreeCategories->SetDefaultSorter();
	
	content->AddChild( panelCategory, igdeContainerSplitted::eaSide );
	
	// right side: item list with information
	igdeContainerReference panelItems;
	panelItems.TakeOver( new igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 3 ) );
	
	const igdeUIHelper::sColumnHeader headers[] = {
		igdeUIHelper::sColumnHeader( "Name", NULL, 200 )
	};
	helper.IconListBox( panelItems, pListItems, decPoint( 100, 200 ), headers, 1, "Items",
		new igdeDialogBrowser_ListItems( *this ) );
	pListItems->SetDefaultSorter();
	pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
	
	helper.EditString( panelItems, "Item information", pEditInfos, 50, 5, NULL );
	pEditInfos->SetEditable( false );
	
	content->AddChild( panelItems, igdeContainerSplitted::eaCenter );
	
	
	// buttons at bottom
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Discard" );
	
	AddContent( content, buttonBar );
}

igdeDialogBrowser::~igdeDialogBrowser(){
}



// Management
///////////////

void igdeDialogBrowser::UpdateCategoryList(){
	igdeGDCategory * const selection = GetSelectedCategory();
	
	pTreeCategories->RemoveAllItems();
	UpdateCategoryListWith( GetRootCategory() );
	pTreeCategories->SortAllItems();
	
	SelectCategory( selection );
}

void igdeDialogBrowser::UpdateCategoryListWith( igdeGDCategory *category ){
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	const int categoryCount = category->GetCategoryCount();
	int i;
	
	pTreeCategories->AppendItem( NULL, "< No Category >" );
	
	for( i=0; i<categoryCount; i++ ){
		igdeGDCategory * const child = category->GetCategoryAt( i );
		if( ! child->GetHidden() ){
			AddCategoryToList( child, NULL );
		}
	}
}

void igdeDialogBrowser::AddCategoryToList( igdeGDCategory *category, igdeTreeItem *parent ){
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	const int categoryCount = category->GetCategoryCount();
	int i;
	
	igdeTreeItemReference item;
	item.TakeOver( new igdeTreeItem( category->GetName(), category ) );
	pTreeCategories->AppendItem( parent, item );
	
	for( i=0; i<categoryCount; i++ ){
		igdeGDCategory * const child = category->GetCategoryAt( i );
		if( ! child->GetHidden() ){
			AddCategoryToList( child, item );
		}
	}
}

void igdeDialogBrowser::UpdateItemList(){
	void * const selection = pListItems->GetSelectedItem() ? pListItems->GetSelectedItem()->GetData() : NULL;
	
	pListItems->RemoveAllItems();
	
	igdeGDAddToListVisitor visitor( GetEnvironment(), pListItems, GetPreviewIconSize() );
	AddItemsToList( visitor );
	pListItems->SortItems();
	
	pListItems->SetSelectionWithData( selection );
	if( ! pListItems->GetSelectedItem() && pListItems->GetItemCount() > 0 ){
		pListItems->SetSelection( 0 );
	}
}

void igdeDialogBrowser::RebuildPISelectedItem(){
	igdeListItem * const item = GetSelectedListItem();
	if( ! item ){
		return;
	}
	
	igdeGDPreviewManager &pvmgr = *GetEnvironment().GetGDPreviewManager();
	
	igdeIconReference icon;
	const int iconSize = GetPreviewIconSize();
	icon.TakeOver( new igdeIcon( *pvmgr.GetImageCreating(), iconSize, iconSize ) );
	
	deObjectReference listener;
	listener.TakeOver( new igdeBrowseItemGDPreviewListener( pListItems, item, iconSize ) );
	
	item->SetIcon( icon );
	pListItems->ItemChangedAt( pListItems->GetSelection() );
	RebuildItemPreview( pvmgr, ( igdeGDPreviewListener* )( deObject* )listener );
}

void igdeDialogBrowser::CurrentItemChanged(){
	decString info;
	GetSelectedItemInfo( info );
	pEditInfos->SetText( info );
}

igdeGDCategory *igdeDialogBrowser::GetSelectedCategory() const{
	const igdeTreeItem * const selection = pTreeCategories->GetSelection();
	return selection ? ( igdeGDCategory* )selection->GetData() : NULL;
}

void igdeDialogBrowser::SelectCategory( igdeGDCategory *category ){
	pTreeCategories->SetSelectionWithData( category );
	if( ! pTreeCategories->GetSelection() && pTreeCategories->GetFirstChild() ){
		pTreeCategories->SetSelection( pTreeCategories->GetFirstChild() );
	}
}

igdeListItem *igdeDialogBrowser::GetSelectedListItem() const{
	return pListItems->GetSelectedItem();
}

void igdeDialogBrowser::SelectListItemWithData( void *data ){
	pListItems->SetSelectionWithData( data );
}

void igdeDialogBrowser::SetPreviewSize( ePreviewSize size ){
	if( size == pPreviewSize ){
		return;
	}
	
	pPreviewSize = size;
	UpdateItemList();
}

void igdeDialogBrowser::SetViewMode( eViewModes viewMode ){
	if( viewMode == pViewMode ){
		return;
	}
	
	pViewMode = viewMode;
	
	switch( viewMode ){
	case evmList:
		pListItems->SetViewMode( igdeIconListBox::evmList );
		break;
		
	case evmPreview:
		pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
		break;
		
	default:
		pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
	}
}

int igdeDialogBrowser::GetPreviewIconSize() const{
	switch( pPreviewSize ){
	case epsLarge:
		return 128;
		
	case epsMedium:
		return 96;
		
	case epsSmall:
		return 64;
	}
	
	return 128;
}

const decString &igdeDialogBrowser::GetFilter() const{
	return pEditFilter->GetText();
}

void igdeDialogBrowser::SetFilter( const char *filter ){
	pEditFilter->SetText( filter );
}
