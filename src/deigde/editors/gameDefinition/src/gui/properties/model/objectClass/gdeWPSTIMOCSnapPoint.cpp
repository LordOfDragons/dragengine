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

#include "gdeWPSTIMOCSnapPoint.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCSnapPoint::gdeWPSTIMOCSnapPoint( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCSnapPoint *snapPoint, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassSnapPoint, objectClass, index ),
pSnapPoint( NULL ),
pIndex( index )
{
	if( ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Snap Point #%d: %s", index + 1, snapPoint->GetName().GetString() );
	SetText( text );
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pSnapPoint = snapPoint;
	snapPoint->AddReference();
}

gdeWPSTIMOCSnapPoint::~gdeWPSTIMOCSnapPoint(){
	if( pSnapPoint ){
		pSnapPoint->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCSnapPoint::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCSnapPoint::IsValid() const{
	return true;
}



void gdeWPSTIMOCSnapPoint::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCSnapPoint::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCSnapPoint( pSnapPoint );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCSnapPoint );
}

void gdeWPSTIMOCSnapPoint::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCSnapPointAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCSnapPointRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCSnapPointCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCSnapPointCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCSnapPointPaste() );
}

void gdeWPSTIMOCSnapPoint::UpdateName(){
	decString text;
	text.Format( "Snap Point #%d: %s", pIndex + 1, pSnapPoint->GetName().GetString() );
	SetText( text );
	ItemChanged();
}
