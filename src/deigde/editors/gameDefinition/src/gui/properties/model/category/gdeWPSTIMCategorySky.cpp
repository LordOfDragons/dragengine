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

gdeWPSTIMCategorySky::gdeWPSTIMCategorySky( gdeWPSTreeModel &tree, gdeCategory *category ) :
gdeWPSTIMCategory( tree, etCategorySky, category ){
}

gdeWPSTIMCategorySky::~gdeWPSTIMCategorySky(){
}



// Management
///////////////

gdeWPSTIMCategorySky *gdeWPSTIMCategorySky::GetChildWith( gdeCategory* category, bool deep ) const{
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



void gdeWPSTIMCategorySky::CategoriesChanged(){
	const gdeCategoryList &list = GetCategory()->GetCategories();
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



void gdeWPSTIMCategorySky::OnAddedToTree(){
	const gdeCategoryList &list = GetCategory()->GetCategories();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMCategorySky( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMCategorySky::OnSelected(){
	GetGameDefinition().SetActiveCategory( GetCategory() );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotCategorySky );
}

void gdeWPSTIMCategorySky::OnContextMenu( igdeMenuCascade &contextMenu ){
	gdeWPSTIMCategory::OnContextMenu( contextMenu );
}

void gdeWPSTIMCategorySky::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeCategory * const category = ( ( gdeWPSTIMCategorySky* )child )->GetCategory();
		child = child->GetNext();
		
		if( category->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveCategory( category );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotCategorySky );
		return;
	}
}
