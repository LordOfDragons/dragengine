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

gdeWPSTIMSkin::gdeWPSTIMSkin( gdeWPSTreeModel &tree, gdeSkin *skin ) :
gdeWPSTreeItemModel( tree, etSkin ),
pSkin( NULL )
{
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( skin->GetName() );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pSkin = skin;
	skin->AddReference();
}

gdeWPSTIMSkin::~gdeWPSTIMSkin(){
	if( pSkin ){
		pSkin->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMSkin::NameChanged(){
	SetText( pSkin->GetName() );
	ParentSortItems();
}



int gdeWPSTIMSkin::Compare( const gdeWPSTreeItemModel &item ) const{
	const gdeWPSTIMSkin &other = ( const gdeWPSTIMSkin & )item;
	return pSkin->GetName().Compare( other.GetSkin()->GetName() );
}

void gdeWPSTIMSkin::OnSelected(){
	GetGameDefinition().SetActiveSkin( pSkin );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotSkin );
}

void gdeWPSTIMSkin::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionSkinAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionSkinRemove() );
}

void gdeWPSTIMSkin::SelectBestMatching( const char *string ){
	if( GetParent() ){
		( ( gdeWPSTreeItemModel* )GetParent() )->SelectBestMatching( string );
	}
}
