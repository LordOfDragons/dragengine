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

#include "igdeGDSky.h"
#include "igdeGDSkyController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSky::igdeGDSky( const char *path, const char *name ) :
pPath( path ),
pName( name ){
}

igdeGDSky::igdeGDSky( const igdeGDSky &sky ) :
pPath( sky.pPath ),
pName( sky.pName ),
pDescription( sky.pDescription ),
pCategory( sky.pCategory )
{
	const int controllerCount = sky.GetControllerCount();
	igdeGDSkyController *controller = NULL;
	int i;
	
	try{
		for( i=0; i<controllerCount; i++ ){
			controller = new igdeGDSkyController( *sky.GetControllerAt( i ) );
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

igdeGDSky::~igdeGDSky(){
}



// Management
///////////////

void igdeGDSky::SetPath( const char *path ){
	pPath = path;
}

void igdeGDSky::SetName( const char *name ){
	pName = name;
}

void igdeGDSky::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDSky::SetCategory( const char *category ){
	pCategory = category;
}

void igdeGDSky::SetPreviewImage( deImage *image ){
	pPreviewImage = image;
}



// Controllers
////////////////

int igdeGDSky::GetControllerCount() const{
	return pControllers.GetCount();
}

igdeGDSkyController *igdeGDSky::GetControllerAt( int index ) const{
	return ( igdeGDSkyController* )pControllers.GetAt( index );
}

void igdeGDSky::AddController( igdeGDSkyController *controller ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void igdeGDSky::RemoveAllControllers(){
	pControllers.RemoveAll();
}
