/* 
 * Drag[en]gine IGDE Project Editor
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

#include "projPanelProfiles.h"
#include "projPanelProfilesListener.h"
#include "../project/projProject.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class projPanelProfilesListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelProfilesListener::projPanelProfilesListener( projPanelProfiles &panel ) :
pPanel( panel ){
}

projPanelProfilesListener::~projPanelProfilesListener(){
}



// Notifications
//////////////////

void projPanelProfilesListener::ProjectChanged( projProject *project ){
	if( pPanel.GetProject() != project ){
		return;
	}
	
	pPanel.UpdateProject();
}



void projPanelProfilesListener::ProfileStructureChanged( projProject *project ){
	if( pPanel.GetProject() != project ){
		return;
	}
	
	pPanel.UpdateProfiles();
}

void projPanelProfilesListener::ProfileChanged(
projProject *project, projProfile *profile ){
	if( pPanel.GetActiveProfile() != profile ){
		return;
	}
	
	pPanel.UpdateProfile();
}

void projPanelProfilesListener::ProfileNameChanged(
projProject *project, projProfile *profile ){
	if( pPanel.GetProject() != project ){
		return;
	}
	
	pPanel.UpdateProfiles();
}

void projPanelProfilesListener::ActiveProfileChanged( projProject *project ){
	if( pPanel.GetProject() != project ){
		return;
	}
	
	pPanel.SelectActiveProfile();
}
