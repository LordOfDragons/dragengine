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

#include "gdeWPSTIMOCForceField.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/forceField/gdeOCForceField.h"
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

gdeWPSTIMOCForceField::gdeWPSTIMOCForceField( gdeWPSTreeModel &tree,
	gdeObjectClass *objectClass, gdeOCForceField *forceField, int index ) :
gdeWPSTIMOCSubObject( tree, etObjectClassForceField, objectClass, index ),
pForceField( NULL )
{
	if( ! forceField ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	text.Format( "Force Field #%d", index + 1 );
	SetText( text );
	
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ) );
	
	pForceField = forceField;
	forceField->AddReference();
}

gdeWPSTIMOCForceField::~gdeWPSTIMOCForceField(){
	if( pForceField ){
		pForceField->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCForceField::Validate(){
	SetIcon( GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning ) );
	ItemChanged();
}

bool gdeWPSTIMOCForceField::IsValid() const{
	return true;
}



void gdeWPSTIMOCForceField::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCForceField::OnSelected(){
	GetGameDefinition().SetActiveObjectClass( GetObjectClass() );
	GetGameDefinition().SetActiveOCForceField( pForceField );
	GetGameDefinition().SetSelectedObjectType( gdeGameDefinition::eotOCForceField );
}

void gdeWPSTIMOCForceField::OnContextMenu( igdeMenuCascade &contextMenu ){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCForceFieldAdd() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCForceFieldRemove() );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, windowMain.GetActionOCForceFieldCopy() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCForceFieldCut() );
	helper.MenuCommand( contextMenu, windowMain.GetActionOCForceFieldPaste() );
}
