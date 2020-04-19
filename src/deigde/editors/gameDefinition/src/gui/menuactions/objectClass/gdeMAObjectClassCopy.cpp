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

#include "gdeMAObjectClassCopy.h"
#include "../../gdeWindowMain.h"
#include "../../../clipboard/gdeClipboardDataObjectClass.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassCopy
///////////////////////////////

// Constructor
////////////////

gdeMAObjectClassCopy::gdeMAObjectClassCopy( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Copy Object Class",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
	"Copy object class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassCopy::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const category = gameDefinition.GetActiveObjectClass();
	if( ! category || gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotObjectClass ){
		return NULL;
	}
	
	gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	deObjectReference clipObjectClass;
	clipObjectClass.TakeOver( new gdeObjectClass( *objectClass ) );
	
	igdeClipboardDataReference clipData;
	clipData.TakeOver( new gdeClipboardDataObjectClass( ( gdeObjectClass* )( deObject* )clipObjectClass ) );
	
	pWindowMain.GetClipboard().Set( clipData );
	return NULL;
}

void gdeMAObjectClassCopy::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != NULL );
}
