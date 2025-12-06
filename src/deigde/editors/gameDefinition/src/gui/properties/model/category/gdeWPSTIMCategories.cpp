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

#include "gdeWPSTIMCategories.h"
#include "gdeWPSTIMCategoriesObjectClass.h"
#include "gdeWPSTIMCategoriesParticleEmitter.h"
#include "gdeWPSTIMCategoriesSkin.h"
#include "gdeWPSTIMCategoriesSky.h"
#include "../gdeWPSTreeModel.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMCategories::gdeWPSTIMCategories( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etCategories ),
pObjectClass( NULL ),
pSkin( NULL ),
pSky( NULL ),
pParticleEmitter( NULL )
{
	SetText( "Categories" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMCategories::~gdeWPSTIMCategories(){
}



// Management
///////////////

void gdeWPSTIMCategories::OnAddedToTree(){
	gdeWPSTreeModel &treeModel = GetTree();
	igdeTreeItem::Ref item;
	
	item.TakeOver( new gdeWPSTIMCategoriesObjectClass( treeModel ) );
	AppendModel( item );
	pObjectClass = ( gdeWPSTIMCategoriesObjectClass* )( igdeTreeItem* )item;
	
	item.TakeOver( new gdeWPSTIMCategoriesParticleEmitter( treeModel ) );
	AppendModel( item );
	pParticleEmitter = ( gdeWPSTIMCategoriesParticleEmitter* )( igdeTreeItem* )item;
	
	item.TakeOver( new gdeWPSTIMCategoriesSkin( treeModel ) );
	AppendModel( item );
	pSkin = ( gdeWPSTIMCategoriesSkin* )( igdeTreeItem* )item;
	
	item.TakeOver( new gdeWPSTIMCategoriesSky( treeModel ) );
	AppendModel( item );
	pSky = ( gdeWPSTIMCategoriesSky* )( igdeTreeItem* )item;
}

void gdeWPSTIMCategories::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionCategoryObjectClassAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionCategoryParticleEmitterAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionCategorySkinAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionCategorySkyAdd() );
}
