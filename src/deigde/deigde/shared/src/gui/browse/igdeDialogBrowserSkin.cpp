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

#include "igdeDialogBrowserSkin.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialog.h"
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
	igdeDialogBrowserSkin::Ref refDialog(igdeDialogBrowserSkin::Ref::New(
		new igdeDialogBrowserSkin( owner->GetEnvironment(), title ) ));
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
