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

#include "gdeWPSTIMOCNavSpace.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
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

gdeWPSTIMOCNavSpace::gdeWPSTIMOCNavSpace( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navSpace, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassNavigationSpace, objectClass, index ),
pNavSpace( NULL )
{
	if( ! navSpace ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Navigation Space #%d", index + 1 );
	SetText( text );
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pNavSpace = navSpace;
	navSpace->AddReference();
}

gdeWPSTIMOCNavSpace::~gdeWPSTIMOCNavSpace(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCNavSpace::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCNavSpace::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if( ! pNavSpace->GetPath().IsEmpty()
	&& ! vfs.ExistsFile( decPath::CreatePathUnix( pNavSpace->GetPath() ) ) ){
		return false;
	}
	
	return true;
}



void gdeWPSTIMOCNavSpace::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCNavSpace::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCNavigationSpace( pNavSpace );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCNavigationSpace );
}

void gdeWPSTIMOCNavSpace::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCNavSpaceAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCNavSpaceRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCNavSpaceCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCNavSpaceCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCNavSpacePaste() );
}
