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
#include <deigde/gui/model/igdeTreeItemReference.h>

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
	igdeTreeItemReference item;
	
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
