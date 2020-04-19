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

#include "igdeDialogBrowserParticleEmitter.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../dialog/igdeDialogReference.h"
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

igdeDialogBrowserParticleEmitter::igdeDialogBrowserParticleEmitter( igdeEnvironment &environment, const char *title ) :
igdeDialogBrowser( environment, title, true ){
	UpdateCategoryList();
	UpdateItemList();
}

igdeDialogBrowserParticleEmitter::~igdeDialogBrowserParticleEmitter(){
}



// Management
///////////////

igdeGDParticleEmitter *igdeDialogBrowserParticleEmitter::GetSelectedParticleEmitter() const{
	const igdeListItem * const selection = GetSelectedListItem();
	return selection ? ( igdeGDParticleEmitter* )selection->GetData() : NULL;
}

void igdeDialogBrowserParticleEmitter::SetSelectedParticleEmitter( igdeGDParticleEmitter *gdParticleEmitter ){
	if( ! gdParticleEmitter ){
		return;
	}
	
	igdeGDCategory * const category = GetRootCategory()->GetCategoryWithPath(
		decPath::CreatePathUnix( gdParticleEmitter->GetCategory() ) );
	
	SelectCategory( category );
	SelectListItemWithData( gdParticleEmitter );
}



bool igdeDialogBrowserParticleEmitter::SelectParticleEmitter( igdeWidget *owner,
igdeGDParticleEmitter* &particleEmitter, const char *title ){
	igdeDialogReference refDialog;
	refDialog.TakeOver( new igdeDialogBrowserParticleEmitter( owner->GetEnvironment(), title ) );
	igdeDialogBrowserParticleEmitter &dialog = ( igdeDialogBrowserParticleEmitter& )( igdeDialog& )refDialog;
	if( particleEmitter ){
		dialog.SetSelectedParticleEmitter( particleEmitter );
	}
	
	if( dialog.Run( owner ) && dialog.GetSelectedParticleEmitter() ){
		particleEmitter = dialog.GetSelectedParticleEmitter();
		return true;
		
	}else{
		return false;
	}
}

bool igdeDialogBrowserParticleEmitter::SelectParticleEmitter( igdeWidget *owner, decString &particleEmitter, const char *title ){
	const igdeGDParticleEmitterManager &particleEmitterManager = owner->GetGameDefinition()->GetParticleEmitterManager();
	igdeGDParticleEmitter *gdParticleEmitter = particleEmitterManager.GetEmitterList().GetWithPath( particleEmitter );
	if( SelectParticleEmitter( owner, gdParticleEmitter, title ) ){
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

void igdeDialogBrowserParticleEmitter::AddItemsToList( igdeGDAddToListVisitor &visitor ){
	const decString &filter = GetFilter();
	
	if( filter.IsEmpty() ){
		igdeGDCategory * const category = GetSelectedCategory();
		if( category ){
// 			GetGameDefinition()->GetParticleEmitterManager().VisitParticleEmittersMatchingCategory( visitor, category );
		}
		
	}else{
// 		GetGameDefinition()->GetParticleEmitterManager().VisitMatchingFilter( visitor, filter );
	}
}

void igdeDialogBrowserParticleEmitter::RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener ){
	igdeGDParticleEmitter * const gdParticleEmitter = GetSelectedParticleEmitter();
	if( gdParticleEmitter ){
// 		pvmgr.ClearPreviewParticleEmitter( gdParticleEmitter );
// 		pvmgr.CreatePreviewParticleEmitter( gdParticleEmitter, listener );
	}
}

void igdeDialogBrowserParticleEmitter::GetSelectedItemInfo( decString &info ){
	const igdeGDParticleEmitter * const gdParticleEmitter = GetSelectedParticleEmitter();
	if( gdParticleEmitter ){
		info.Format( "%s:\n%s", gdParticleEmitter->GetName().GetString(), gdParticleEmitter->GetDescription().GetString() );
	}
}
