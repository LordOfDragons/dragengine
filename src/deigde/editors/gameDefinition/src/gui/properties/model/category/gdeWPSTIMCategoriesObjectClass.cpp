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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeWPSTIMCategoriesObjectClass.h"
#include "gdeWPSTIMCategoryObjectClass.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/category/gdeCategory.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMCategoriesObjectClass::gdeWPSTIMCategoriesObjectClass( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etCategoriesObjectClass )
{
	SetText( "Object Classes" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMCategoriesObjectClass::~gdeWPSTIMCategoriesObjectClass(){
}



// Management
///////////////

gdeWPSTIMCategoryObjectClass *gdeWPSTIMCategoriesObjectClass::GetChildWith(
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



void gdeWPSTIMCategoriesObjectClass::CategoriesChanged(){
	const gdeCategoryList &list = GetGameDefinition().GetCategoriesObjectClass();
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



void gdeWPSTIMCategoriesObjectClass::OnAddedToTree(){
	const gdeCategoryList &list = GetGameDefinition().GetCategoriesObjectClass();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMCategoryObjectClass( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMCategoriesObjectClass::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionCategoryObjectClassAdd() );
}

void gdeWPSTIMCategoriesObjectClass::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	gdeGameDefinition &gameDefinition = GetGameDefinition();
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeCategory * const category = ( ( gdeWPSTIMCategoryObjectClass* )child )->GetCategory();
		child = child->GetNext();
		
		if( category->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		gameDefinition.SetActiveCategory( category );
		gameDefinition.SetSelectedObjectType( gdeGameDefinition::eotCategoryObjectClass );
		return;
	}
}
