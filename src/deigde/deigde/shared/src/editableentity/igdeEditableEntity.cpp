/* 
 * Drag[en]gine IGDE
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

#include "igdeEditableEntity.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gamedefinition/igdeGameDefinition.h"
#include "../gameproject/igdeGameProject.h"
#include "../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeEditableEntity
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditableEntity::igdeEditableEntity( igdeEnvironment *environment ){
	if( ! environment ){
		DETHROW( deeInvalidParam );
	}
	
	pEnvironment = environment;
	
	pChanged = false;
	pSaved = false;
	
	pUndoSystem = NULL;
	
	try{
		pUndoSystem = new igdeUndoSystem( this );
		
	}catch( const deException & ){
		if( pUndoSystem ){
			delete pUndoSystem;
		}
		throw;
	}
}

igdeEditableEntity::~igdeEditableEntity(){
	if( pUndoSystem ){
		delete pUndoSystem;
	}
}



// Management
///////////////

deEngine *igdeEditableEntity::GetEngine() const{
	return pEnvironment->GetEngineController()->GetEngine();
}

igdeGameDefinition *igdeEditableEntity::GetGameDefinition() const{
	return pEnvironment->GetGameProject()->GetGameDefinition();
}

deLogger *igdeEditableEntity::GetLogger() const{
	return pEnvironment->GetLogger();
}

void igdeEditableEntity::SetFilePath( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pPathFile.Equals( path ) ){
		decPath pathDirectory;
		
		pPathFile = path;
		
		pathDirectory.SetFromUnix( path );
		if( pathDirectory.GetComponentCount() > 1 ){
			pathDirectory.RemoveLastComponent();
			
		}else{
			pathDirectory.SetFromUnix( "/" );
		}
		
		pPathDirectory = pathDirectory.GetPathUnix();
	}
}

void igdeEditableEntity::SetChanged( bool changed ){
	if( changed != pChanged ){
		pChanged = changed;
		NotifyStateChanged();
	}
}

void igdeEditableEntity::SetSaved( bool saved ){
	if( saved != pSaved ){
		pSaved = saved;
		NotifyStateChanged();
	}
}

void igdeEditableEntity::NotifyStateChanged(){
}

void igdeEditableEntity::NotifyUndoChanged(){
}
