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

#include "igdeDialogBrowserParticleEmitter.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialog.h"
#include "../model/igdeListItem.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/particleemitter/igdeGDParticleEmitter.h"
#include "../../gamedefinition/particleemitter/igdeGDParticleEmitterManager.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeDialogBrowserParticleEmitter
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowserParticleEmitter::igdeDialogBrowserParticleEmitter(igdeEnvironment &environment, const char *title) :
igdeDialogBrowser(environment, title, true){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserParticleEmitter::~igdeDialogBrowserParticleEmitter(){
}



// Management
///////////////

igdeGDParticleEmitter *igdeDialogBrowserParticleEmitter::GetSelectedParticleEmitter() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? (igdeGDParticleEmitter*)selection->GetData() : NULL;
}

void igdeDialogBrowserParticleEmitter::SetSelectedParticleEmitter(igdeGDParticleEmitter *gdParticleEmitter){
	if(!gdParticleEmitter){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix(gdParticleEmitter->GetCategory()));
	
	SelectCategory(category);
	SelectListItemWithData(gdParticleEmitter);
}



bool igdeDialogBrowserParticleEmitter::SelectParticleEmitter(igdeWidget *owner,
igdeGDParticleEmitter* &particleEmitter, const char *title){
	igdeDialogBrowserParticleEmitter::Ref dialog(
		igdeDialogBrowserParticleEmitter::Ref::New(owner->GetEnvironment(), title));
	if(particleEmitter){
		dialog->SetSelectedParticleEmitter(particleEmitter);
	}
	
	if(dialog->Run(owner) && dialog->GetSelectedParticleEmitter()){
		particleEmitter = dialog->GetSelectedParticleEmitter();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserParticleEmitter::SelectParticleEmitter(igdeWidget *owner, decString &particleEmitter, const char *title){
	const igdeGDParticleEmitterManager &particleEmitterManager = owner->GetGameDefinition()->GetParticleEmitterManager();
	igdeGDParticleEmitter *gdParticleEmitter = particleEmitterManager.GetEmitterList().GetWithPath(particleEmitter);
	if(SelectParticleEmitter(owner, gdParticleEmitter, title)){
		particleEmitter = gdParticleEmitter->GetPath();
		return true;
		
	}else{
		return false;
	}
}



// Protected Functions
////////////////////////

igdeGDCategory *igdeDialogBrowserParticleEmitter::GetRootCategory() const{
	return GetGameDefinition()->GetParticleEmitterManager().GetCategories();
}

void igdeDialogBrowserParticleEmitter::AddItemsToList(igdeGDAddToListVisitor &visitor){
	const decString &filter = GetFilter();
	
	if(filter.IsEmpty()){
		igdeGDCategory * const category = GetSelectedCategory();
		if(category){
// 			GetGameDefinition()->GetParticleEmitterManager().VisitParticleEmittersMatchingCategory( visitor, category );
		}
		
	}else{
// 		GetGameDefinition()->GetParticleEmitterManager().VisitMatchingFilter( visitor, filter );
	}
}

void igdeDialogBrowserParticleEmitter::RebuildItemPreview(igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener){
	igdeGDParticleEmitter * const gdParticleEmitter = GetSelectedParticleEmitter();
	if(gdParticleEmitter){
// 		pvmgr.ClearPreviewParticleEmitter( gdParticleEmitter );
// 		pvmgr.CreatePreviewParticleEmitter( gdParticleEmitter, listener );
	}
}

void igdeDialogBrowserParticleEmitter::GetSelectedItemInfo(decString &info){
	const igdeGDParticleEmitter * const gdParticleEmitter = GetSelectedParticleEmitter();
	if(gdParticleEmitter){
		info.Format("%s:\n%s", gdParticleEmitter->GetName().GetString(), gdParticleEmitter->GetDescription().GetString());
	}
}
