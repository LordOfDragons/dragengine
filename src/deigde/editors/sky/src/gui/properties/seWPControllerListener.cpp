/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seWPController.h"
#include "seWPControllerListener.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"

#include <dragengine/common/exceptions.h>



// Class seWPControllerListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPControllerListener::seWPControllerListener( seWPController &panel ) :
pPanel( panel ){
}

seWPControllerListener::~seWPControllerListener(){
}



// Management
///////////////

void seWPControllerListener::ControllerStructureChanged( seSky *sky ){
	pPanel.UpdateControllerList();
}

void seWPControllerListener::ControllerChanged( seSky *sky, seController *controller ){
	if( controller->GetActive() ){
		pPanel.UpdateController();
	}
}

void seWPControllerListener::ControllerNameChanged( seSky* sky, seController* controller ){
	pPanel.UpdateControllerList();
}

void seWPControllerListener::ControllerValueChanged( seSky *sky, seController *controller ){
	if( controller->GetActive() ){
		pPanel.UpdateControllerValue();
	}
}

void seWPControllerListener::ActiveControllerChanged( seSky *sky ){
	pPanel.SelectActiveController();
}
