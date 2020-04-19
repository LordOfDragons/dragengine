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

#include "gdeWPSTIMCategory.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/category/gdeCategory.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMCategory::gdeWPSTIMCategory( gdeWPSTreeModel &tree, eTypes type, gdeCategory *category ) :
gdeWPSTreeItemModel( tree, type ),
pCategory( NULL )
{
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	SetText( category->GetName() );
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pCategory = category;
	category->AddReference();
}

gdeWPSTIMCategory::~gdeWPSTIMCategory(){
	if( pCategory ){
		pCategory->FreeReference();
	}
}



// Management
///////////////

int gdeWPSTIMCategory::Compare( const gdeWPSTreeItemModel &item ) const{
	const gdeWPSTIMCategory &other = ( const gdeWPSTIMCategory & )item;
	return pCategory->GetName().Compare( other.GetCategory()->GetName() );
}

void gdeWPSTIMCategory::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionCategoryAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionCategoryRemove() );
	
	// TODO Copy Category Decal
	// TODO Cut Category Decal
	// TODO Paste Category Decal
}
