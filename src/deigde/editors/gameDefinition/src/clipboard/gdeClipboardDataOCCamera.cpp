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
#include <stdlib.h>

#include "gdeClipboardDataOCCamera.h"
#include "../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeClipboardDataOCCamera
///////////////////////////////////

const char * const gdeClipboardDataOCCamera::TYPE_NAME = "OCCamera";

// Constructor, destructor
////////////////////////////

gdeClipboardDataOCCamera::gdeClipboardDataOCCamera( gdeOCCamera *camera ) :
igdeClipboardData( TYPE_NAME ),
pCamera( NULL )
{
	if( ! camera ){
		DETHROW( deeInvalidParam );
	}
	
	pCamera = camera;
	camera->AddReference();
}

gdeClipboardDataOCCamera::~gdeClipboardDataOCCamera(){
	if( pCamera ){
		pCamera->FreeReference();
	}
}
