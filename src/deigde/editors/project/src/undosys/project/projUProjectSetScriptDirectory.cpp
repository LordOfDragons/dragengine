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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "projUProjectSetScriptDirectory.h"
#include "../../project/projProject.h"

#include <dragengine/common/exceptions.h>



// Class projUProjectSetScriptDirectory
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

projUProjectSetScriptDirectory::projUProjectSetScriptDirectory(
projProject *project, const char *newValue ) :
pProject( NULL ),
pNewValue( newValue )
{
	if( ! project ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Project set script directory" );
	
	pOldValue = project->GetScriptDirectory();
	
	pProject = project;
	project->AddReference();
}

projUProjectSetScriptDirectory::~projUProjectSetScriptDirectory(){
	if( pProject ){
		pProject->FreeReference();
	}
}



// Management
///////////////

void projUProjectSetScriptDirectory::Undo(){
	pProject->SetScriptDirectory( pOldValue );
}

void projUProjectSetScriptDirectory::Redo(){
	pProject->SetScriptDirectory( pNewValue );
}
