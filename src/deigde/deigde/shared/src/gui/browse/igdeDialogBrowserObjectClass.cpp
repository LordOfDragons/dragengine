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

#include "igdeDialogBrowserObjectClass.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialog.h"
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
	igdeDialogBrowserObjectClass::Ref dialog(igdeDialogBrowserObjectClass::Ref::NewWith(
		owner->GetEnvironment(), title));
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
