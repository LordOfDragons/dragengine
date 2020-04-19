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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "projPanelUndoHistory.h"
#include "projPanelUndoHistoryListener.h"
#include "../project/projProject.h"

#include <dragengine/common/exceptions.h>



// Class projPanelUndoHistory
//////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelUndoHistory::projPanelUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pProject( NULL ),
pListener( new projPanelUndoHistoryListener( *this ) ){
}

projPanelUndoHistory::~projPanelUndoHistory(){
	SetProject( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void projPanelUndoHistory::SetProject( projProject *project ){
	if( project == pProject ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pProject ){
		pProject->RemoveListener( pListener );
		pProject->FreeReference();
	}
	
	pProject = project;
	
	if( project ){
		project->AddReference();
		project->AddListener( pListener );
		
		SetUndoSystem( project->GetUndoSystem() );
	}
}
