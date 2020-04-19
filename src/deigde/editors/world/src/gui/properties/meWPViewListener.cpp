/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdlib.h>
#include <string.h>

#include "meWPView.h"
#include "meWPViewListener.h"
#include "../../world/meWorld.h"
#include "../../world/meCamera.h"
#include "../../world/object/meObject.h"

#include "dragengine/common/exceptions.h"



// Class meWPViewListener
///////////////////////////

// Constructor, destructor
////////////////////////////

meWPViewListener::meWPViewListener( meWPView &panel ) :
pPanel( panel ){
}

meWPViewListener::~meWPViewListener(){
}



// Notifications
//////////////////

void meWPViewListener::SkyChanged( meWorld* ){
	pPanel.UpdateSky();
}

void meWPViewListener::EditingChanged( meWorld* ){
	pPanel.UpdateView();
}

void meWPViewListener::TriggerTableChanged( meWorld* ){
	pPanel.UpdateTriggerTable();
}

void meWPViewListener::ClassHideTagsChanged( meWorld* ) {
    pPanel.UpdateClassHideTagLists();
}



void meWPViewListener::ObjectChanged( meWorld*, meObject* ){
	pPanel.UpdateCameraList();
}

void meWPViewListener::ObjectPropertiesChanged( meWorld*, meObject* ){
	pPanel.UpdateCameraList();
}



void meWPViewListener::ActiveCameraChanged( meWorld* ){
	pPanel.UpdateCamera();
}

void meWPViewListener::CameraChanged( meWorld*, meCamera* ){
	pPanel.UpdateCamera();
}

