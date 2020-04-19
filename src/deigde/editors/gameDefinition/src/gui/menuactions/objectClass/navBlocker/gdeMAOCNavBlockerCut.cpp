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

#include "gdeMAOCNavBlockerCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCNavBlocker.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../../../undosys/objectClass/navblocker/gdeUOCRemoveNavBlocker.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCNavBlockerCut
///////////////////////////////

// Constructor
////////////////

gdeMAOCNavBlockerCut::gdeMAOCNavBlockerCut( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Cut Object Class Navigation Blocker",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
	"Cut object class navigation blocker" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCNavBlockerCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCNavigationBlocker ){
		return NULL;
	}
	
	gdeOCNavigationBlocker * const navBlocker = gameDefinition.GetActiveOCNavigationBlocker();
	if( ! navBlocker ){
		return NULL;
	}
	
	deObjectReference clipOCNavigationBlocker;
	clipOCNavigationBlocker.TakeOver( new gdeOCNavigationBlocker( *navBlocker ) );
	
	igdeClipboardDataReference clipData;
	clipData.TakeOver( new gdeClipboardDataOCNavBlocker( ( gdeOCNavigationBlocker* )( deObject* )clipOCNavigationBlocker ) );
	
	pWindowMain.GetClipboard().Set( clipData );
	
	return new gdeUOCRemoveNavBlocker( &objectClass, navBlocker );
}

void gdeMAOCNavBlockerCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker
		&& gameDefinition->GetActiveOCNavigationBlocker() != NULL );
}
