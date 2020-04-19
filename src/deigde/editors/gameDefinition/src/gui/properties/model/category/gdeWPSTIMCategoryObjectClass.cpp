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

#include "gdeWPSTIMCategoryObjectClass.h"
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

gdeWPSTIMCategoryObjectClass::gdeWPSTIMCategoryObjectClass( gdeWPSTreeModel &tree, gdeCategory *category ) :
gdeWPSTIMCategory( tree, etCategoryObjectClass, category ){
}

gdeWPSTIMCategoryObjectClass::~gdeWPSTIMCategoryObjectClass(){
}



// Management
///////////////

gdeWPSTIMCategoryObjectClass *gdeWPSTIMCategoryObjectClass::GetChildWith(
gdeCategory* category, bool deep ) const{
	gdeWPSTIMCategoryObjectClass *child = ( gdeWPSTIMCategoryObjectClass* )GetFirstChild();
	
	while( child ){
		if( child->GetCategory() == category ){
			return child;
		}
		
		if( deep ){
			gdeWPSTIMCategoryObjectClass * const deepChild = child->GetChildWith( category, true );
			if( deepChild ){
				return deepChild;
			}
		}
		
		child = ( gdeWPSTIMCategoryObjectClass* )child->GetNext();
	}
	
	return NULL;
}



void gdeWPSTIMCategoryObjectClass::CategoriesChanged(){
	const gdeCategoryList &list = GetCategory()->GetCategories();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeCategory * const category = list.GetAt( i );
		gdeWPSTIMCategoryObjectClass * const modelCategory = GetChildWith( category, false );
		
		if( modelCategory ){
			modelCategory->CategoriesChanged();
			
		}else{
			item.TakeOver( new gdeWPSTIMCategoryObjectClass( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMCategoryObjectClass * const modelCategory = ( gdeWPSTIMCategoryObjectClass* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelCategory->GetCategory() ) ){
			RemoveModel( modelCategory );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMCategoryObjectClass::OnAddedToTree(){
	const gdeCategoryList &list = GetCategory()->GetCategories();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMCategoryObjectClass( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMCategoryObjectClass::OnSelected(){
	GetGameDefinition().SetActiveCategory( GetCategory() );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotCategoryObjectClass );
}

void gdeWPSTIMCategoryObjectClass::OnContextMenu( igdeMenuCascade &contextMenu ){
	gdeWPSTIMCategory::OnContextMenu( contextMenu );
}

void gdeWPSTIMCategoryObjectClass::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeCategory * const category = ( ( gdeWPSTIMCategoryObjectClass* )child )->GetCategory();
		child = child->GetNext();
		
		if( category->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveCategory( category );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotCategoryObjectClass );
		return;
	}
}
