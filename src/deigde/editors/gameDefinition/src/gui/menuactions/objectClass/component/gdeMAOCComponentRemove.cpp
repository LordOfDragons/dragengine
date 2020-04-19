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

#include "gdeMAOCComponentRemove.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../../../undosys/objectClass/component/gdeUOCRemoveComponent.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAOCComponentRemove
/////////////////////////////////

// Constructor
////////////////

gdeMAOCComponentRemove::gdeMAOCComponentRemove( gdeWindowMain &windowMain ) :
gdeBaseMAOCSubObject( windowMain, "Remove Object Class Component",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove object class component" )
{
}



// Management
///////////////

igdeUndo *gdeMAOCComponentRemove::OnActionSubObject(
gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass ){
	if( gameDefinition.GetSelectedObjectType() != gdeGameDefinition::eotOCComponent ){
		return NULL;
	}
	
	gdeOCComponent * const component = gameDefinition.GetActiveOCComponent();
	if( ! component ){
		return NULL;
	}
	
	return new gdeUOCRemoveComponent( &objectClass, component );
}

void gdeMAOCComponentRemove::Update(){
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	SetEnabled( gameDefinition
		&& gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCComponent
		&& gameDefinition->GetActiveOCComponent() != NULL );
}
