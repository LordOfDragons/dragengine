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

#include "gdeWPSTIMOCBillboard.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCBillboard::gdeWPSTIMOCBillboard( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassBillboard, objectClass, index ),
pBillboard( NULL )
{
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Billboard #%d", index + 1 );
	SetText( text );
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pBillboard = billboard;
	billboard->AddReference();
}

gdeWPSTIMOCBillboard::~gdeWPSTIMOCBillboard(){
	if( pBillboard ){
		pBillboard->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCBillboard::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCBillboard::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if( ! pBillboard->GetSkinPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pBillboard->GetSkinPath() ) ) ){
		return false;
	}
	
	return true;
}



void gdeWPSTIMOCBillboard::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCBillboard::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCBillboard( pBillboard );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCBillboard );
}

void gdeWPSTIMOCBillboard::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCBillboardPaste() );
}
