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

#include "gdeWPSOCCamera.h"
#include "gdeWPSOCCameraListener.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSOCCameraListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCCameraListener::gdeWPSOCCameraListener( gdeWPSOCCamera &panel ) :
pPanel( panel ){
}

gdeWPSOCCameraListener::~gdeWPSOCCameraListener(){
}



// Management
///////////////

void gdeWPSOCCameraListener::OCPropertyNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeProperty* ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCCameraListener::OCPropertiesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdatePropertyList();
}

void gdeWPSOCCameraListener::ActiveObjectClassChanged( gdeGameDefinition* ){
	pPanel.UpdatePropertyList();
}



void gdeWPSOCCameraListener::OCCamerasChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	pPanel.UpdateCamera();
}

void gdeWPSOCCameraListener::OCCameraChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCCamera *camera ){
	if( pPanel.GetObjectClass() != objectClass ){
		return;
	}
	if( pPanel.GetCamera() != camera ){
		return;
	}
	pPanel.UpdateCamera();
}

void gdeWPSOCCameraListener::ActiveOCCameraChanged( gdeGameDefinition* ){
	pPanel.UpdateCamera();
}
