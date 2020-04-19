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

#include "igdeDialogBrowserSkin.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialogReference.h"
#include "../model/igdeListItem.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/skin/igdeGDSkin.h"
#include "../../gamedefinition/skin/igdeGDSkinManager.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeDialogBrowserSkin
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowserSkin::igdeDialogBrowserSkin( igdeEnvironment &environment, const char *title ) :
igdeDialogBrowser( environment, title, true ){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserSkin::~igdeDialogBrowserSkin(){
}



// Management
///////////////

igdeGDSkin *igdeDialogBrowserSkin::GetSelectedSkin() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? ( igdeGDSkin* )selection->GetData() : NULL;
}

void igdeDialogBrowserSkin::SetSelectedSkin( igdeGDSkin *gdSkin ){
	if( ! gdSkin ){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix( gdSkin->GetCategory() ) );
	
	SelectCategory( category );
	SelectListItemWithData( gdSkin );
}



bool igdeDialogBrowserSkin::SelectSkin( igdeWidget *owner, igdeGDSkin* &skin, const char *title ){
	igdeDialogReference refDialog;
	refDialog.TakeOver( new igdeDialogBrowserSkin( owner->GetEnvironment(), title ) );
	igdeDialogBrowserSkin &dialog = ( igdeDialogBrowserSkin& )( igdeDialog& )refDialog;
	if( skin ){
		dialog.SetSelectedSkin( skin );
	}
	
	if( dialog.Run( owner ) && dialog.GetSelectedSkin() ){
		skin = dialog.GetSelectedSkin();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserSkin::SelectSkin( igdeWidget *owner, decString &skin, const char *title ){
	const igdeGDSkinManager &skinManager = *owner->GetGameDefinition()->GetSkinManager();
	igdeGDSkin *gdSkin = skinManager.GetSkinWithPath( skin );
	if( SelectSkin( owner, gdSkin, title ) ){
		skin = gdSkin->GetPath();
		return true;
		
	}else{
		return false;
	}
}



// Protected Functions
////////////////////////

igdeGDCategory *igdeDialogBrowserSkin::GetRootCategory() const{
	return GetGameDefinition()->GetSkinManager()->GetCategories();
}

void igdeDialogBrowserSkin::AddItemsToList( igdeGDAddToListVisitor &visitor ){
	const decString &filter = GetFilter();
	
	if( filter.IsEmpty() ){
		GetGameDefinition()->GetSkinManager()->VisitSkinsMatchingCategory( visitor, GetSelectedCategory() );
		
	}else{
		GetGameDefinition()->GetSkinManager()->VisitMatchingFilter( visitor, filter );
	}
}

void igdeDialogBrowserSkin::RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener ){
	igdeGDSkin * const gdSkin = GetSelectedSkin();
	if( gdSkin ){
		pvmgr.ClearPreviewSkin( gdSkin );
		pvmgr.CreatePreviewSkin( gdSkin, listener );
	}
}

void igdeDialogBrowserSkin::GetSelectedItemInfo( decString &info ){
	const igdeGDSkin * const gdSkin = GetSelectedSkin();
	if( gdSkin ){
		info.Format( "%s:\n%s", gdSkin->GetName().GetString(), gdSkin->GetDescription().GetString() );
	}
}
