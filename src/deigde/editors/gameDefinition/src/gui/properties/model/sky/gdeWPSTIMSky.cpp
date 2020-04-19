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

#include "gdeWPSTIMSky.h"
#include "../gdeWPSTreeModel.h"
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

gdeWPSTIMSky::gdeWPSTIMSky( gdeWPSTreeModel &tree, gdeSky *sky ) :
gdeWPSTreeItemModel( tree, etSky ),
pSky( NULL )
{
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( sky->GetName() );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pSky = sky;
	sky->AddReference();
}

gdeWPSTIMSky::~gdeWPSTIMSky(){
	if( pSky ){
		pSky->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMSky::NameChanged(){
	SetText( pSky->GetName() );
	ParentSortItems();
}



int gdeWPSTIMSky::Compare( const gdeWPSTreeItemModel &item ) const{
	const gdeWPSTIMSky &other = ( const gdeWPSTIMSky & )item;
	return pSky->GetName().Compare( other.GetSky()->GetName() );
}

void gdeWPSTIMSky::OnSelected(){
	GetGameDefinition().SetActiveSky( pSky );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotSky );
}

void gdeWPSTIMSky::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionSkyAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionSkyRemove() );
}

void gdeWPSTIMSky::SelectBestMatching( const char *string ){
	if( GetParent() ){
		( ( gdeWPSTreeItemModel* )GetParent() )->SelectBestMatching( string );
	}
}
