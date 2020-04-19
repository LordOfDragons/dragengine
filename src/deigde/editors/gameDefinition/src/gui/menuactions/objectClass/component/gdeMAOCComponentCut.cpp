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

#include "gdeMAOCComponentCut.h"
#include "../../../gdeWindowMain.h"
#include "../../../../clipboard/gdeClipboardDataOCComponent.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../undosys/objectClass/component/gdeUOCRemoveComponent.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCComponentCut
//////////////////////////////

// Constructor
////////////////

gdeMAOCComponentCut::gdeMAOCComponentCut( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Cut Object Class Component",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
	"Cut object class component" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCComponentCut::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCComponent ){
		return NULL;
	}
	
	gdeOCComponent * const component = gameDefinition.GetActiveOCComponent();
	if( ! component ){
		return NULL;
	}
	
	deObjectReference clipOCComponent;
	clipOCComponent.TakeOver( new gdeOCComponent( *component ) );
	
	igdeClipboardDataReference clipData;
	clipData.TakeOver( new gdeClipboardDataOCComponent( ( gdeOCComponent* )( deObject* )clipOCComponent ) );
	
	pWindowMain.GetClipboard().Set( clipData );
	
	return new gdeUOCRemoveComponent( &objectClass, component );
}

void gdeMAOCComponentCut::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCComponent
		&& gameDefinition->GetActiveOCComponent() != NULL );
}
