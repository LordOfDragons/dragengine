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

#include "gdeSkin.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeSkin
//////////////////

// Constructor, destructor
////////////////////////////

gdeSkin::gdeSkin() :
pGameDefinition( NULL ){
}

gdeSkin::gdeSkin( const char *path, const char *name ) :
pGameDefinition( NULL ),
pPath( path ),
pName( name ){
}

gdeSkin::gdeSkin( const gdeSkin &skin ) :
pGameDefinition( NULL ),

pPath( skin.pPath ),
pName( skin.pName ),
pDescription( skin.pDescription ),
pCategory( skin.pCategory ),
pTags( skin.pTags ){
}

gdeSkin::~gdeSkin(){
	pCleanUp();
}



// Management
///////////////

void gdeSkin::SetGameDefinition( gdeGameDefinition *gamedef ){
	pGameDefinition = gamedef;
}



void gdeSkin::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pPath = path;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinNameChanged( this );
	}
}

void gdeSkin::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetCategory( const char *category ){
	if( pCategory == category ){
		return;
	}
	
	pCategory = category;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}

void gdeSkin::SetTags( const decStringSet &tags ){
	if( pTags == tags ){
		return;
	}
	
	pTags = tags;
	
	if( pGameDefinition ){
		pGameDefinition->NotifySkinChanged( this );
	}
}



// Private functions
//////////////////////

void gdeSkin::pCleanUp(){
	SetGameDefinition( NULL );
}
