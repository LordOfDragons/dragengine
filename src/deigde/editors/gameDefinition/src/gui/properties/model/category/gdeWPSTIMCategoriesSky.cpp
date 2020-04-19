/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPSTIMCategoriesSky.h"
#include "gdeWPSTIMCategorySky.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/category/gdeCategory.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMCategoriesSky::gdeWPSTIMCategoriesSky( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etCategoriesSky )
{
	SetText( "Skies" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMCategoriesSky::~gdeWPSTIMCategoriesSky(){
}



// Management
///////////////

gdeWPSTIMCategorySky *gdeWPSTIMCategoriesSky::GetChildWith( gdeCategory* category, bool deep ) const{
	gdeWPSTIMCategorySky *child = ( gdeWPSTIMCategorySky* )GetFirstChild();
	
	while( child ){
		if( child->GetCategory() == category ){
			return child;
		}
		
		if( deep ){
			gdeWPSTIMCategorySky * const deepChild = child->GetChildWith( category, true );
			if( deepChild ){
				return deepChild;
			}
		}
		
		child = ( gdeWPSTIMCategorySky* )child->GetNext();
	}
	
	return NULL;
}



void gdeWPSTIMCategoriesSky::CategoriesChanged(){
	const gdeCategoryList &list = GetGameDefinition().GetCategoriesSky();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeCategory * const category = list.GetAt( i );
		gdeWPSTIMCategorySky * const modelCategory = GetChildWith( category, false );
		
		if( modelCategory ){
			modelCategory->CategoriesChanged();
			
		}else{
			item.TakeOver( new gdeWPSTIMCategorySky( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMCategorySky * const modelCategory = ( gdeWPSTIMCategorySky* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelCategory->GetCategory() ) ){
			RemoveModel( modelCategory );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMCategoriesSky::OnAddedToTree(){
	const gdeCategoryList &list = GetGameDefinition().GetCategoriesSky();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMCategorySky( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMCategoriesSky::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionCategorySkyAdd() );
}

void gdeWPSTIMCategoriesSky::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	gdeGameDefinition &gameDefinition = GetGameDefinition();
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeCategory * const category = ( ( gdeWPSTIMCategorySky* )child )->GetCategory();
		child = child->GetNext();
		
		if( category->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		gameDefinition.SetActiveCategory( category );
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategorySky );
		return;
	}
}
