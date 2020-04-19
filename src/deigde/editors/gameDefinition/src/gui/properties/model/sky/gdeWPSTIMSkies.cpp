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

#include "gdeWPSTIMSkies.h"
#include "gdeWPSTIMSky.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/sky/gdeSky.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMSkies::gdeWPSTIMSkies( gdeWPSTreeModel &tree ) :
gdeWPSTreeItemModel( tree, etSkies )
{
	SetText( "Skies" );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
}

gdeWPSTIMSkies::~gdeWPSTIMSkies(){
}



// Management
///////////////

gdeWPSTIMSky *gdeWPSTIMSkies::GetChildWith( gdeSky *sky ) const{
	gdeWPSTIMSky *child = ( gdeWPSTIMSky* )GetFirstChild();
	
	while( child ){
		if( child->GetSky() == sky ){
			return child;
		}
		child = ( gdeWPSTIMSky* )child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMSkies::StructureChanged(){
	const gdeSkyList &list = GetGameDefinition().GetSkies();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	// update existing and add new categories
	for( i=0; i<count; i++ ){
		gdeSky * const sky = list.GetAt( i );
		gdeWPSTIMSky * const modelSky = GetChildWith( sky );
		
		if( ! modelSky ){
			item.TakeOver( new gdeWPSTIMSky( GetTree(), list.GetAt( i ) ) );
			AppendModel( item );
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while( child ){
		gdeWPSTIMSky * const modelSky = ( gdeWPSTIMSky* )child;
		child = child->GetNext();
		
		if( ! list.Has( modelSky->GetSky() ) ){
			RemoveModel( modelSky );
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMSkies::OnAddedToTree(){
	const gdeSkyList &list = GetGameDefinition().GetSkies();
	const int count = list.GetCount();
	igdeTreeItemReference item;
	int i;
	
	for( i=0; i<count; i++ ){
		item.TakeOver( new gdeWPSTIMSky( GetTree(), list.GetAt( i ) ) );
		AppendModel( item );
	}
	
	SortChildren();
}

void gdeWPSTIMSkies::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionSkyAdd() );
}

void gdeWPSTIMSkies::SelectBestMatching( const char *string ){
	if( ! string ){
		return;
	}
	
	const decString searchString( decString( string ).GetLower() );
	igdeTreeItem *child = GetFirstChild();
	
	while( child ){
		gdeSky * const sky = ( ( gdeWPSTIMSky* )child )->GetSky();
		child = child->GetNext();
		
		if( sky->GetName().GetLower().FindString( searchString ) == -1 ){
			continue;
		}
		
		GetGameDefinition().SetActiveSky( sky );
		GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotSky );
		return;
	}
}
