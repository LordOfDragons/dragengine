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

#include "seWPLink.h"
#include "seWPLinkListener.h"
#include "../../sky/seSky.h"
#include "../../sky/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seWPLinkListener
///////////////////////////

// Constructor, destructor
////////////////////////////

seWPLinkListener::seWPLinkListener( seWPLink &panel ) :
pPanel( panel ){
}

seWPLinkListener::~seWPLinkListener(){
}



// Management
///////////////

void seWPLinkListener::ControllerStructureChanged( seSky *sky ){
	pPanel.UpdateControllerList();
}

void seWPLinkListener::ControllerNameChanged( seSky *sky, seController *controller ){
	pPanel.UpdateControllerList();
}



void seWPLinkListener::LinkStructureChanged( seSky *sky ){
	pPanel.UpdateLinkList();
}

void seWPLinkListener::LinkChanged( seSky *sky, seLink *link ){
	if( link->GetActive() ){
		pPanel.UpdateLink();
	}
}

void seWPLinkListener::LinkNameChanged( seSky* sky, seLink* link ){
	pPanel.UpdateLinkList();
}

void seWPLinkListener::ActiveLinkChanged( seSky *sky ){
	pPanel.SelectActiveLink();
}
