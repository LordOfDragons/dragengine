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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeSky.h"
#include "gdeSkyController.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeSky
/////////////////

// Constructor, destructor
////////////////////////////

gdeSky::gdeSky() :
pGameDefinition( NULL ){
}

gdeSky::gdeSky( const char *path, const char *name ) :
pGameDefinition( NULL ),
pPath( path ),
pName( name ){
}

gdeSky::gdeSky( const gdeSky &sky ) :
pGameDefinition( NULL ),

pPath( sky.pPath ),
pName( sky.pName ),
pDescription( sky.pDescription ),
pCategory( sky.pCategory ),
pTags( sky.pTags )
{
	const int controllerCount = sky.pControllers.GetCount();
	gdeSkyController *controller = NULL;
	int i;
	
	try{
		for( i=0; i<controllerCount; i++ ){
			controller = new gdeSkyController( *sky.pControllers.GetAt( i ) );
			pControllers.Add( controller );
			controller->FreeReference();
			controller = NULL;
		}
		
	}catch( const deException & ){
		if( controller ){
			controller->FreeReference();
		}
		throw;
	}
}

gdeSky::~gdeSky(){
	SetGameDefinition( NULL );
}



// Management
///////////////

void gdeSky::SetGameDefinition( gdeGameDefinition *gamedef ){
	pGameDefinition = gamedef;
}



void gdeSky::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pPath = path;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyChanged( this );
	}
}

void gdeSky::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyNameChanged( this );
	}
}

void gdeSky::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyChanged( this );
	}
}

void gdeSky::SetCategory( const char *category ){
	if( pCategory == category ){
		return;
	}
	
	pCategory = category;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyChanged( this );
	}
}

void gdeSky::SetTags( const decStringSet &tags ){
	if( pTags == tags ){
		return;
	}
	
	pTags = tags;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyChanged( this );
	}
}



// Controllers
////////////////

void gdeSky::AddController( gdeSkyController *controller ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyControllerStructureChanged( this );
	}
}

void gdeSky::RemoveController( gdeSkyController *controller ){
	pControllers.Remove( controller );
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkyControllerStructureChanged( this );
	}
}

void gdeSky::RemoveAllControllers(){
	pControllers.RemoveAll();
}

void gdeSky::NotifyControllerChanged( gdeSkyController *controller ){
	if( pGameDefinition ){
		pGameDefinition->NotifySkyControllerChanged( this, controller );
	}
}
