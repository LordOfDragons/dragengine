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

#include "igdeDialogBrowserObjectClass.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialogReference.h"
#include "../model/igdeListItem.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../gamedefinition/class/igdeGDClassManager.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeDialogBrowserObjectClass
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowserObjectClass::igdeDialogBrowserObjectClass( igdeEnvironment &environment,
const char *title ) : igdeDialogBrowser( environment, title, true ){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserObjectClass::~igdeDialogBrowserObjectClass(){
}



// Management
///////////////

igdeGDClass *igdeDialogBrowserObjectClass::GetSelectedObjectClass() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? ( igdeGDClass* )selection->GetData() : NULL;
}

void igdeDialogBrowserObjectClass::SetSelectedObjectClass( igdeGDClass *gdClass ){
	if( ! gdClass ){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix( gdClass->GetCategory() ) );
	
	SelectCategory( category );
	SelectListItemWithData( gdClass );
}



bool igdeDialogBrowserObjectClass::SelectObjectClass( igdeWidget *owner, igdeGDClass* &objectClass, const char *title ){
	igdeDialogReference refDialog;
	refDialog.TakeOver( new igdeDialogBrowserObjectClass( owner->GetEnvironment(), title ) );
	igdeDialogBrowserObjectClass &dialog = ( igdeDialogBrowserObjectClass& )( igdeDialog& )refDialog;
	if( objectClass ){
		dialog.SetSelectedObjectClass( objectClass );
	}
	
	if( dialog.Run( owner ) && dialog.GetSelectedObjectClass() ){
		objectClass = dialog.GetSelectedObjectClass();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserObjectClass::SelectObjectClass( igdeWidget *owner, decString &objectClass, const char *title ){
	const igdeGDClassManager &classManager = *owner->GetGameDefinition()->GetClassManager();
	igdeGDClass *gdClass = classManager.GetNamed( objectClass );
	if( SelectObjectClass( owner, gdClass, title ) ){
		objectClass = gdClass->GetName();
		return true;
		
	}else{
		return false;
	}
}



// Protected Functions
////////////////////////

igdeGDCategory *igdeDialogBrowserObjectClass::GetRootCategory() const{
	return GetGameDefinition()->GetClassManager()->GetCategories();
}

void igdeDialogBrowserObjectClass::AddItemsToList( igdeGDAddToListVisitor &visitor ){
	const decString &filter = GetFilter();
	
	if( filter.IsEmpty() ){
		GetGameDefinition()->GetClassManager()->VisitClassesMatchingCategory( visitor, GetSelectedCategory() );
		
	}else{
		GetGameDefinition()->GetClassManager()->VisitMatchingFilter( visitor, filter );
	}
}

void igdeDialogBrowserObjectClass::RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener ){
	igdeGDClass * const gdClass = GetSelectedObjectClass();
	if( gdClass ){
		pvmgr.ClearPreviewObjectClass( gdClass );
		pvmgr.CreatePreviewObjectClass( gdClass, listener );
	}
}

void igdeDialogBrowserObjectClass::GetSelectedItemInfo( decString &info ){
	const igdeGDClass * const gdClass = GetSelectedObjectClass();
	if( gdClass ){
		info.Format( "%s:\n%s", gdClass->GetName().GetString(), gdClass->GetDescription().GetString() );
	}
}
