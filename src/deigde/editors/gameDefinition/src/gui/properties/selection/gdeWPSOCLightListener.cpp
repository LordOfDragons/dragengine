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

#include "gdeWPSOCLight.h"
#include "gdeWPSOCLightListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/light/gdeOCLight.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCLightListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCLightListener::gdeWPSOCLightListener( gdeWPSOCLight &panel ) :
pPanel( panel ){
}

gdeWPSOCLightListener::~gdeWPSOCLightListener(){
}



// Management
///////////////

void gdeWPSOCLightListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCLightListener::OCPropertiesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCLightListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCLightListener::OCLightsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateLight();
}

void gdeWPSOCLightListener::OCLightChanged( gdeGameDefinition*, gdeObjectClass *objectClass,
gdeOCLight *light ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetLight() != light ){
		return;
	}
	pPanel.UpdateLight();
}

void gdeWPSOCLightListener::ActiveOCLightChanged( gdeGameDefinition* ){
	pPanel.UpdateLight();
}
