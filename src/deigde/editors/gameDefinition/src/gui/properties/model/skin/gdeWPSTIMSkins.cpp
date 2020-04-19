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

#include "gdeWPSTIMSkin.h"
#include "gdeWPSTIMSkins.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/skin/gdeSkin.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMSkins::gdeWPSTIMSkins( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etSkins )
{
	SetText( "Skins" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMSkins::~gdeWPSTIMSkins(){
}



// Management
///////////////

gdeWPSTIMSkin *gdeWPSTIMSkins::GetChildWith( gdeSkin *skin ) const{
	gdeWPSTIMSkin *child = ( gdeWPSTIMSkin* )GetFirstChild();
	
	while( child ){
		if( child->GetSkin() == skin ){
			return child;
		}
		child = ( gdeWPSTIMSkin* )child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMSkins::StructureChanged(){
	const gdeSkinList &list = GetGameDefinition().GetSkins();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeSkin * const skin = list.GetAt( i );
		gdeWPSTIMSkin * const modelSkin = GetChildWith( skin );
		
		if( ! modelSkin ){
			item.TakeOver( new gdeWPSTIMSkin( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMSkin * const modelSkin = ( gdeWPSTIMSkin* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelSkin->GetSkin() ) ){
			RemoveModel( modelSkin );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMSkins::OnAddedToTree(){
	const gdeSkinList &list = GetGameDefinition().GetSkins();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMSkin( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMSkins::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionSkinAdd() );
}

void gdeWPSTIMSkins::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeSkin * const skin = ( ( gdeWPSTIMSkin* )child )->GetSkin();
		child = child->GetNext();
		
		if( skin->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveSkin( skin );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotSkin );
		return;
	}
}
