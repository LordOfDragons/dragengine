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

#include "gdeWPSOCNavigationBlocker.h"
#include "gdeWPSOCNavigationBlockerListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCNavigationBlockerListener
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCNavigationBlockerListener::gdeWPSOCNavigationBlockerListener( gdeWPSOCNavigationBlocker &panel ) :
pPanel( panel ){
}

gdeWPSOCNavigationBlockerListener::~gdeWPSOCNavigationBlockerListener(){
}



// Management
///////////////

void gdeWPSOCNavigationBlockerListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	if( gameDefinition->GetIsProjectGameDef() ){
		return;
	}
	
	pPanel.UpdatePropertyList();
	pPanel.UpdateNavigationBlocker();
}

void gdeWPSOCNavigationBlockerListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	if( pPanel.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	pPanel.UpdatePropertyList();
	pPanel.UpdateNavigationBlocker();
}



void gdeWPSOCNavigationBlockerListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationBlockerListener::OCPropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationBlockerListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCNavigationBlockerListener::OCNavigationBlockersChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateNavigationBlocker();
}

void gdeWPSOCNavigationBlockerListener::OCNavigationBlockerChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetNavigationBlocker() != navblocker ){
		return;
	}
	pPanel.UpdateNavigationBlocker();
}

void gdeWPSOCNavigationBlockerListener::ActiveOCNavigationBlockerChanged( gdeGameDefinition* ){
	pPanel.UpdateNavigationBlocker();
}
