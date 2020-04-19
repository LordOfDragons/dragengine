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

#include "gdeWPSOCNavigationSpace.h"
#include "gdeWPSOCNavigationSpaceListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCNavigationSpaceListener
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCNavigationSpaceListener::gdeWPSOCNavigationSpaceListener( gdeWPSOCNavigationSpace &panel ) :
pPanel( panel ){
}

gdeWPSOCNavigationSpaceListener::~gdeWPSOCNavigationSpaceListener(){
}



// Management
///////////////

void gdeWPSOCNavigationSpaceListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationSpaceListener::OCPropertiesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCNavigationSpaceListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCNavigationSpaceListener::OCNavigationSpacesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateNavigationSpace();
}

void gdeWPSOCNavigationSpaceListener::OCNavigationSpaceChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetNavigationSpace() != navspace ){
		return;
	}
	pPanel.UpdateNavigationSpace();
}

void gdeWPSOCNavigationSpaceListener::ActiveOCNavigationSpaceChanged( gdeGameDefinition* ){
	pPanel.UpdateNavigationSpace();
}
