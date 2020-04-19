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

#include "gdeWPSOCBillboard.h"
#include "gdeWPSOCBillboardListener.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCBillboardListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCBillboardListener::gdeWPSOCBillboardListener( gdeWPSOCBillboard &panel ) :
pPanel( panel ){
}

gdeWPSOCBillboardListener::~gdeWPSOCBillboardListener(){
}



// Management
///////////////

void gdeWPSOCBillboardListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCBillboardListener::OCPropertiesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCBillboardListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCBillboardListener::OCBillboardsChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	
	pPanel.UpdateBillboard();
}

void gdeWPSOCBillboardListener::OCBillboardChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCBillboard *billboard ){
	if( pPanel.GetObjectClass() != objectClass || pPanel.GetBillboard() != billboard ){
		return;
	}
	
	pPanel.UpdateBillboard();
}

void gdeWPSOCBillboardListener::ActiveOCBillboardChanged( gdeGameDefinition* ){
	pPanel.UpdateBillboard();
}
