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

#include "gdeWPSTreeItemModel.h"
#include "gdeWPSTreeModel.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <deigde/gui/igdeTreeList.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTreeItemModel::gdeWPSTreeItemModel( gdeWPSTreeModel &tree, eTypes type ) :
igdeTreeItem( "" ),
pTree( tree ),
pType( type ){
}

gdeWPSTreeItemModel::~gdeWPSTreeItemModel(){
}



// Management
///////////////

gdeWindowMain &gdeWPSTreeItemModel::GetWindowMain() const{
	return pTree.GetWindowMain();
}

gdeGameDefinition &gdeWPSTreeItemModel::GetGameDefinition() const{
	return *pTree.GetGameDefinition();
}



void gdeWPSTreeItemModel::SortChildren(){
	pTree.GetTreeList().SortItems( this );
}

void gdeWPSTreeItemModel::ParentSortItems(){
	if( GetParent() ){
		pTree.GetTreeList().SortItems( GetParent() );
	}
}

void gdeWPSTreeItemModel::SetAsCurrentItem(){
	pTree.GetTreeList().SetSelection( this );
}

void gdeWPSTreeItemModel::AppendModel( igdeTreeItem *item ){
	pTree.GetTreeList().AppendItem( this, item );
	( ( gdeWPSTreeItemModel* )item )->OnAddedToTree();
}

void gdeWPSTreeItemModel::RemoveModel( igdeTreeItem *item ){
	pTree.GetTreeList().RemoveItem( item );
}

void gdeWPSTreeItemModel::RemoveAllModels(){
	pTree.GetTreeList().RemoveAllItems( this );
}

void gdeWPSTreeItemModel::ItemChanged(){
	pTree.GetTreeList().ItemChanged( this );
}



void gdeWPSTreeItemModel::OnAddedToTree(){
}

int gdeWPSTreeItemModel::Compare( const gdeWPSTreeItemModel &item ) const{
	return 0;
}

void gdeWPSTreeItemModel::OnSelected(){
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotNoSelection );
}

void gdeWPSTreeItemModel::OnContextMenu( igdeMenuCascade& ){
}

void gdeWPSTreeItemModel::SelectBestMatching( const char* ){
}
