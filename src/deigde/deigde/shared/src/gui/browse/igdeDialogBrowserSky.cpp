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

#include "igdeDialogBrowserSky.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialog.h"
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

igdeDialogBrowserSky::igdeDialogBrowserSky(igdeEnvironment &environment,
const char *title, bool canResize) : igdeDialogBrowser(environment, title, canResize){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserSky::~igdeDialogBrowserSky(){
}



// Management
///////////////

igdeGDSky *igdeDialogBrowserSky::GetSelectedSky() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? (igdeGDSky*)selection->GetData() : nullptr;
}

void igdeDialogBrowserSky::SetSelectedSky(igdeGDSky *gdSky){
	if(!gdSky){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix(gdSky->GetCategory()));
	
	SelectCategory(category);
	SelectListItemWithData(gdSky);
}



bool igdeDialogBrowserSky::SelectSky(igdeWidget *owner, igdeGDSky* &sky, const char *title){
	igdeDialogBrowserSky::Ref dialog(igdeDialogBrowserSky::Ref::New(
		owner->GetEnvironment(), title));
	if(sky){
		dialog->SetSelectedSky(sky);
	}
	
	if(dialog->Run(owner) && dialog->GetSelectedSky()){
		sky = dialog->GetSelectedSky();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserSky::SelectSky(igdeWidget *owner, decString &sky, const char *title){
	const igdeGDSkyManager &skyManager = *owner->GetGameDefinition()->GetSkyManager();
	igdeGDSky *gdSky = skyManager.GetSkyList().GetWithPath(sky);
	if(SelectSky(owner, gdSky, title)){
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

void igdeDialogBrowserSky::AddItemsToList(igdeGDAddToListVisitor &visitor){
	const decString &filter = GetFilter();
	
	if(filter.IsEmpty()){
		GetGameDefinition()->GetSkyManager()->VisitSkiesMatchingCategory(visitor, GetSelectedCategory());
		
	}else{
		GetGameDefinition()->GetSkyManager()->VisitMatchingFilter(visitor, filter);
	}
}

void igdeDialogBrowserSky::RebuildItemPreview(igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener){
	igdeGDSky * const gdSky = GetSelectedSky();
	if(gdSky){
		pvmgr.ClearPreviewSky(gdSky);
		pvmgr.CreatePreviewSky(gdSky, listener);
	}
}

void igdeDialogBrowserSky::GetSelectedItemInfo(decString &info){
	const igdeGDSky * const gdSky = GetSelectedSky();
	if(gdSky){
		info.Format("%s:\n%s", gdSky->GetName().GetString(), gdSky->GetDescription().GetString());
	}
}
