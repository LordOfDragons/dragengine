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

#include "igdeDialogBrowserSky.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialogReference.h"
#include "../model/igdeListItem.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeDialogBrowserSky
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowserSky::igdeDialogBrowserSky( igdeEnvironment &environment,
const char *title, bool canResize ) : igdeDialogBrowser( environment, title, canResize ){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserSky::~igdeDialogBrowserSky(){
}



// Management
///////////////

igdeGDSky *igdeDialogBrowserSky::GetSelectedSky() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? ( igdeGDSky* )selection->GetData() : NULL;
}

void igdeDialogBrowserSky::SetSelectedSky( igdeGDSky *gdSky ){
	if( ! gdSky ){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix( gdSky->GetCategory() ) );
	
	SelectCategory( category );
	SelectListItemWithData( gdSky );
}



bool igdeDialogBrowserSky::SelectSky( igdeWidget *owner, igdeGDSky* &sky, const char *title ){
	igdeDialogReference refDialog;
	refDialog.TakeOver( new igdeDialogBrowserSky( owner->GetEnvironment(), title ) );
	igdeDialogBrowserSky &dialog = ( igdeDialogBrowserSky& )( igdeDialog& )refDialog;
	if( sky ){
		dialog.SetSelectedSky( sky );
	}
	
	if( dialog.Run( owner ) && dialog.GetSelectedSky() ){
		sky = dialog.GetSelectedSky();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserSky::SelectSky( igdeWidget *owner, decString &sky, const char *title ){
	const igdeGDSkyManager &skyManager = *owner->GetGameDefinition()->GetSkyManager();
	igdeGDSky *gdSky = skyManager.GetSkyList().GetWithPath( sky );
	if( SelectSky( owner, gdSky, title ) ){
		sky = gdSky->GetPath();
		return true;
		
	}else{
		return false;
	}
}



// Protected Functions
////////////////////////

igdeGDCategory *igdeDialogBrowserSky::GetRootCategory() const{
	return GetGameDefinition()->GetSkyManager()->GetCategories();
}

void igdeDialogBrowserSky::AddItemsToList( igdeGDAddToListVisitor &visitor ){
	const decString &filter = GetFilter();
	
	if( filter.IsEmpty() ){
		GetGameDefinition()->GetSkyManager()->VisitSkiesMatchingCategory( visitor, GetSelectedCategory() );
		
	}else{
		GetGameDefinition()->GetSkyManager()->VisitMatchingFilter( visitor, filter );
	}
}

void igdeDialogBrowserSky::RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener ){
	igdeGDSky * const gdSky = GetSelectedSky();
	if( gdSky ){
		pvmgr.ClearPreviewSky( gdSky );
		pvmgr.CreatePreviewSky( gdSky, listener );
	}
}

void igdeDialogBrowserSky::GetSelectedItemInfo( decString &info ){
	const igdeGDSky * const gdSky = GetSelectedSky();
	if( gdSky ){
		info.Format( "%s:\n%s", gdSky->GetName().GetString(), gdSky->GetDescription().GetString() );
	}
}
